#include "comms.h"

#include "ble_uart.h"
#include "board.h"
#include "common.h"
#include "crc.h"
#include "fifo.h"
#include "logger.h"
#include "pb_encode.h"
#include "sensors.h"
#include "SignBuddy.pb.h"

#include <stdlib.h>

#define COMMS_TX_BUFFER_SIZE    512

#define SYNC_BEGIN              0x16
#define SYNC_END                0x27

#define MID_SAMPLE              0x01

#define PACKET_SIZE             sizeof(packet_header_t) + s.packet_header.length * sizeof(uint8_t) + \
  sizeof(packet_footer_t)

typedef struct __attribute__((__packed__)) {
  uint8_t sync_begin;
  uint8_t mid;
  uint8_t length;
} packet_header_t;

typedef struct __attribute__((__packed__)) {
  uint32_t crc;
  uint8_t sync_end;
} packet_footer_t;

typedef struct {
  TaskHandle_t      task_handle;
  packet_header_t   packet_header;
  packet_footer_t   packet_footer;
  Sample            sample;
  uint8_t           sample_buffer[Sample_size];
  volatile uint8_t  sample_ready;
  volatile uint8_t  packet_ready;
  uint8_t           tx_buffer[COMMS_TX_BUFFER_SIZE];
  fifo_t            tx_fifo;
  SemaphoreHandle_t sample_mutex;
} state_t;

static state_t s;

static void hw_init(void)
{
  ble_uart_init();
}

static void rx()
{
  DISABLE_IRQ();
  uint8_t cmd = LL_LPUART_ReceiveData8(BLE_UART);
  ENABLE_IRQ();

  switch (cmd) {
  case 's':
    LOG_DEBUG("comms: start sampling\r\n");
    RTOS_ERR_CHECK(xTimerStart(sampling_timer, 0));
    break;

  default:
    LOG_DEBUG("comms: unknown cmd\r\n");
    break;
  }
}

static void ingest_packet()
{
  fifo_push(&s.tx_fifo, (uint8_t *) &s.packet_header, sizeof(packet_header_t));
  fifo_push(&s.tx_fifo, s.sample_buffer, s.packet_header.length);
  fifo_push(&s.tx_fifo, (uint8_t *) &s.packet_footer, sizeof(packet_footer_t));
  s.packet_ready = 0;
}

static void packetize_sample()
{
  pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *) s.sample_buffer, Sample_size);

  pb_encode(&stream, &Sample_msg, &s.sample);
  s.sample_ready = 0;
  s.packet_header.length = stream.bytes_written;
  s.packet_header.mid = MID_SAMPLE;
  s.packet_footer.crc = compute_crc(s.sample_buffer, s.packet_header.length);
  s.packet_ready = 1;
  /* If tx buffer has room, ingest the packet immediately */
  if (fifo_bytes_unused_cnt_get(&s.tx_fifo) >= PACKET_SIZE) {
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
      packetize_sample();
    }
    xSemaphoreGive(s.sample_mutex);

    /* Ingest packet into tx buffer if packet ready and tx buffer has enough space */
    if ((s.packet_ready == 1) && (fifo_bytes_unused_cnt_get(&s.tx_fifo) >= PACKET_SIZE)) {
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
  s.sample_ready = 1;
  xSemaphoreGive(s.sample_mutex);
}

void comms_task_setup(void)
{
  hw_init();
  fifo_init(&s.tx_fifo, s.tx_buffer, COMMS_TX_BUFFER_SIZE);
  s.packet_header.sync_begin = SYNC_BEGIN;
  s.packet_footer.sync_end = SYNC_END;
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
