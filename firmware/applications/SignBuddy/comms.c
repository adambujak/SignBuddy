#include "comms.h"

#include "ble_uart.h"
#include "board.h"
#include "common.h"
#include "crc.h"
#include "fifo.h"
#include "logger.h"
#include "pb_encode.h"
#include "SignBuddy.pb.h"

#include <stdlib.h>

#define COMMS_TX_BUFFER_SIZE    512

typedef struct __attribute__((__packed__)) {
  uint8_t length;
  uint8_t sample[Sample_size];
  uint32_t crc;
} packet_t;

typedef struct {
  TaskHandle_t      task_handle;
  Sample            sample;
  packet_t          packet;
  uint8_t           sample_ready;
  uint8_t           packet_ready;
  uint8_t           tx_buffer[COMMS_TX_BUFFER_SIZE];
  fifo_t            tx_fifo;
  SemaphoreHandle_t sample_mutex;
} state_t;

static state_t s;

static inline void set_sample_ready(void)
{
  s.sample_ready = 1;
}

static inline void clear_sample_ready(void)
{
  s.sample_ready = 0;
}

static inline void set_packet_ready(void)
{
  s.packet_ready = 1;
}

static inline void clear_packet_ready(void)
{
  s.packet_ready = 0;
}

static void hw_init(void)
{
  ble_uart_init();
}

static void rx()
{
  LOG_INFO("comms: process rx\r\n");
  DISABLE_IRQ();
  uint8_t cmd = LL_LPUART_ReceiveData8(BLE_UART);
  ENABLE_IRQ();

  switch (cmd) {
  case 0:
    LOG_DEBUG("comms: cmd 1\r\n");
    break;

  case 1:
    LOG_DEBUG("comms: cmd 2\r\n");
    break;

  case 2:
    LOG_DEBUG("comms: cmd 3\r\n");
    break;

  default:
    LOG_DEBUG("comms: unknown cmd\r\n");
    break;
  }
}

static void ingest_packet()
{
  fifo_push(&s.tx_fifo, (uint8_t *) &s.packet, sizeof(s.packet));
  clear_packet_ready();
}

static void create_packet()
{
  pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *) s.packet.sample, Sample_size);

  pb_encode(&stream, &Sample_msg, &s.sample);
  pb_get_encoded_size((size_t *) &s.packet.length, &Sample_msg, &s.sample);
  clear_sample_ready();
  s.packet.crc = compute_crc(s.packet.sample, s.packet.length);
  set_packet_ready();
  /* If tx buffer has room, ingest the packet immediately */
  if (fifo_bytes_unused_cnt_get(&s.tx_fifo) >= sizeof(s.packet)) {
    ingest_packet();
  }
}

static void tx(void)
{
  DISABLE_IRQ();
  uint8_t data;
  fifo_pop(&s.tx_fifo, &data, 1);
  LL_LPUART_TransmitData8(BLE_UART, data);
  ENABLE_IRQ();
}

static void comms_task(void *arg)
{
  LOG_INFO("comms: task started\r\n");

  s.sample_mutex = xSemaphoreCreateMutex();

  while (1) {
    /* Create packet if new sample ready and last packet ingested */
    xSemaphoreTake(s.sample_mutex, portMAX_DELAY);
    if ((s.sample_ready == 1) && (s.packet_ready == 0)) {
      create_packet();
    }
    xSemaphoreGive(s.sample_mutex);

    /* Ingest packet into tx buffer if packet ready and tx buffer has enough space */
    if ((s.packet_ready == 1) && (fifo_bytes_unused_cnt_get(&s.tx_fifo) >= sizeof(s.packet))) {
      ingest_packet();
    }

    /* Process RX from UART */
    if (BLE_UART_RXNE() == 1) {
      rx();
    }

    /* If there are bytes in the tx buffer, transmit using UART if UART ready */
    if (BLE_UART_TXE() == 1 && fifo_bytes_used_cnt_get(&s.tx_fifo) != 0) {
      tx();
    }
  }
}

void comms_tx_data(Sample *sample)
{
  xSemaphoreTake(s.sample_mutex, portMAX_DELAY);
  s.sample = *sample;
  set_sample_ready();
  xSemaphoreGive(s.sample_mutex);
}

void comms_task_setup(void)
{
  hw_init();
  fifo_init(&s.tx_fifo, s.tx_buffer, COMMS_TX_BUFFER_SIZE);
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
