#include "comms.h"

#include "ble_uart.h"
#include "board.h"
#include "common.h"
#include "crc.h"
#include "dma.h"
#include "fifo.h"
#include "logger.h"
#include "pb_encode.h"
#include "SignBuddy.pb.h"

#include <stdlib.h>

#define COMMS_TX_BUFFER_SIZE    256
#define COMMS_RX_BUFFER_SIZE    64

typedef struct __attribute__((__packed__)) {
  uint8_t length;
  uint8_t sample[Sample_size];
  uint32_t crc;
} packet_t;

typedef struct {
  TaskHandle_t task_handle;
  Sample       sample;
  packet_t     packet;
  uint8_t      packet_dr;
  uint8_t      rx_dr;
  uint8_t      tx_dr;
  uint8_t      tx_e;
  uint8_t      rx_buffer[COMMS_RX_BUFFER_SIZE];
  fifo_t       rx_fifo;
  uint8_t      tx_buffer[COMMS_TX_BUFFER_SIZE];
  fifo_t       tx_fifo;
} state_t;

static state_t s;

static inline void set_packet_dr(void)
{
  s.packet_dr = 1;
}

static inline void clear_packet_dr(void)
{
  s.packet_dr = 0;
}

static inline void set_rx_dr(void)
{
  s.rx_dr = 1;
}

static inline void clear_rx_dr(void)
{
  s.rx_dr = 0;
}

static inline void set_tx_dr(void)
{
  s.tx_dr = 1;
}

static inline void clear_tx_dr(void)
{
  s.tx_dr = 0;
}

static inline void set_tx_e(void)
{
  s.tx_e = 1;
}

static inline void clear_tx_e(void)
{
  s.tx_e = 0;
}

static void hw_init(void)
{
  ble_uart_init();
}

static void process_rx()
{
  uint8_t cmd;

  fifo_pop(&s.rx_fifo, &cmd, 1);
  if (fifo_bytes_used_cnt_get(&s.rx_fifo) == 0) {
    clear_rx_dr();
  }
  switch (cmd) {
  case 0:
    LOG_INFO("comms: cmd 1\r\n");
    break;

  case 1:
    LOG_INFO("comms: cmd 2\r\n");
    break;

  case 2:
    LOG_INFO("comms: cmd 3\r\n");
    break;

  default:
    LOG_INFO("comms: unknown cmd\r\n");
    break;
  }
}

static void create_packet()
{
  LOG_INFO("comms: create packet\r\n");
  pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *) s.packet.sample, Sample_size);

  pb_encode(&stream, &Sample_msg, &s.sample);
  pb_get_encoded_size((size_t *) &s.packet.length, &Sample_msg, &s.sample);
  clear_tx_dr();
  s.packet.crc = compute_crc(s.packet.sample, s.packet.length);
  set_packet_dr();
}

static void ingest_packet()
{
  LOG_INFO("comms: ingest packet\r\n");
  fifo_push(&s.tx_fifo, (uint8_t *) &s.packet, sizeof(s.packet));
  clear_packet_dr();
}

static void tx(void)
{
  LOG_INFO("comms: tx\r\n");
  DISABLE_IRQ();
  uint8_t data;
  fifo_pop(&s.tx_fifo, &data, 1);
  LL_LPUART_TransmitData8(BLE_UART, data);
  ENABLE_IRQ();
}

static void comms_task(void *arg)
{
  LOG_INFO("comms: task started\r\n");

  ble_uart_enable_it();

  while (1) {
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    /* Process RX */
    if (s.rx_dr == 1) {
      process_rx();
    }
    /* Process sample */
    if (s.tx_dr == 1) {
      create_packet();
    }
    uint8_t in_tx_buffer = fifo_bytes_used_cnt_get(&s.tx_fifo);
    /* TX packet */
    if ((s.packet_dr == 1) && (COMMS_TX_BUFFER_SIZE - in_tx_buffer >= sizeof(s.packet))) {
      ingest_packet();
    }

    if (in_tx_buffer == 0) {
      set_tx_e();
    }

    if (s.tx_e != 0) {
      tx();
    }
  }
}

void comms_rx_data_ready_cb(void)
{
  uint8_t data = LL_LPUART_ReceiveData8(BLE_UART);

  fifo_push(&s.rx_fifo, &data, 1);
  set_rx_dr();
  vTaskNotifyGiveFromISR(s.task_handle, 0);
}

void comms_tx_empty_cb(void)
{
  set_tx_e();
  vTaskNotifyGiveFromISR(s.task_handle, 0);
}

void comms_tx_data_clear_cb(void)
{
  vTaskNotifyGiveFromISR(s.task_handle, 0);
}

void comms_tx_data(Sample *sample)
{
  s.sample = *sample;
  set_tx_dr();
  xTaskNotifyGive(s.task_handle);
}

void comms_task_setup(void)
{
  hw_init();
  fifo_init(&s.rx_fifo, s.rx_buffer, COMMS_RX_BUFFER_SIZE);
  fifo_init(&s.tx_fifo, s.tx_buffer, COMMS_TX_BUFFER_SIZE);
  set_tx_e();
}

void comms_task_start(void)
{
  BaseType_t task_status = xTaskCreate(comms_task,
                                       "comms",
                                       COMMS_STACK_SIZE,
                                       NULL,
                                       COMMS_TASK_PRIORITY,
                                       &s.task_handle);

  RTOS_ERR_CHECK(task_status);
}
