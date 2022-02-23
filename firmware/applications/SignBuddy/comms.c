#include "comms.h"

#include "ble_uart.h"
#include "board.h"
#include "common.h"
#include "crc.h"
#include "fifo.h"
#include "logger.h"
#include "pb_encode.h"
#include "sensors.h"

#include <stdlib.h>

#define COMMS_TX_BUFFER_SIZE    128

#define CMD_SAMPLE_STATIC       0x01
#define CMD_SAMPLE_DYNAMIC      0x02

#define SYNC                    0x16

#define MID_SAMPLE              0x01
#define MID_STATUS              0xFF

#define PACKET_SIZE             sizeof(packet_header_t) + s.packet.header.length

#define MAX_MSG_SIZE            SBPMessage_size

typedef struct __attribute__((__packed__)) {
  uint8_t sync;
  uint8_t length;
  uint32_t crc;
} packet_header_t;

typedef struct __attribute__((__packed__)) {
  packet_header_t header;
  uint8_t msg[MAX_MSG_SIZE];
} packet_t;

typedef struct {
  TaskHandle_t      task_handle;
  SBPMessage        msg;
  volatile uint8_t  msg_ready;
  packet_t          packet;
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
  uint8_t cmd = LL_USART_ReceiveData8(BLE_UART);
  ENABLE_IRQ();

  switch (cmd) {
  case CMD_SAMPLE_STATIC:
    LOG_DEBUG("comms: sample static\r\n");
    sensors_sample(SAMPLE_STATIC);
    break;

  case CMD_SAMPLE_DYNAMIC:
    LOG_DEBUG("comms: sample dynamic\r\n");
    sensors_sample(SAMPLE_DYNAMIC);
    break;

  default:
    LOG_DEBUG("comms: unknown cmd\r\n");
    break;
  }
}

static void ingest_packet()
{
  if (fifo_bytes_unused_cnt_get(&s.tx_fifo) >= PACKET_SIZE) {
    fifo_push(&s.tx_fifo, (uint8_t *) &s.packet.header, sizeof(packet_header_t));
    fifo_push(&s.tx_fifo, s.packet.msg, s.packet.header.length);
    s.packet_ready = 0;
  }
}

static void packetize_sample()
{
  pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *) s.packet.msg, MAX_MSG_SIZE);
  pb_encode(&stream, &SBPMessage_msg, &s.msg);

  s.msg_ready = 0;
  s.packet.header.length = stream.bytes_written;
  s.packet.header.crc = 0xA5A5A5A5;
  s.packet_ready = 1;
  ingest_packet();
}

static void tx(void)
{
  DISABLE_IRQ();
  uint8_t data;
  fifo_pop(&s.tx_fifo, &data, 1);
  LL_USART_TransmitData8(BLE_UART, data);
  ENABLE_IRQ();
}

static void comms_task(void *arg)
{
  LOG_INFO("comms: task started\r\n");

  s.sample_mutex = xSemaphoreCreateMutex();

  while (1) {
    /* Create packet if new sample ready and last packet ingested */
    xSemaphoreTake(s.sample_mutex, portMAX_DELAY);
    if ((s.msg_ready == 1) && (s.packet_ready == 0)) {
      packetize_sample();
    }
    xSemaphoreGive(s.sample_mutex);

    /* Ingest packet into tx buffer if packet ready*/
    if (s.packet_ready == 1) {
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

void comms_tx_sample(SBPSample *sample)
{
  xSemaphoreTake(s.sample_mutex, portMAX_DELAY);
  s.msg.id = MID_SAMPLE;
  s.msg.payload.sample = *sample;
  s.msg.which_payload = SBPMessage_sample_tag;
  s.msg_ready = 1;
  xSemaphoreGive(s.sample_mutex);
}

void comms_tx_status(SBPStatus *status)
{
  s.msg.id = MID_STATUS;
  s.msg.payload.status = *status;
  s.msg.which_payload = SBPMessage_status_tag;
  s.msg_ready = 1;
}

void comms_task_setup(void)
{
  hw_init();
  crc_init();
  fifo_init(&s.tx_fifo, s.tx_buffer, COMMS_TX_BUFFER_SIZE);
  s.packet.header.sync = SYNC;
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
