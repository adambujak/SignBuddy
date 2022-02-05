#include "comms.h"

#include "common.h"
#include "logger.h"
#include "pb_encode.h"
#include "SignBuddy.pb.h"

#include <stdlib.h>

typedef struct __attribute__((__packed__)) {
  uint8_t length;
  uint8_t protobuf[Sample_size];
  uint32_t crc;
} packet_t;

typedef struct {
  TaskHandle_t task_handle;
  uint8_t      tx_dr;
  Sample       sample;
  packet_t     packet;
} state_t;

static state_t s;

static inline void set_tx_dr(void)
{
  s.tx_dr = 1;
}

static inline void clear_tx_dr(void)
{
  s.tx_dr = 0;
}

static void comms_task(void *arg)
{
  LOG_INFO("comms: task started\r\n");

  while (1) {
    // TODO Check for message received

    if (s.tx_dr == 1) {
      pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *) &s.packet.protobuf, Sample_size);
      pb_encode(&stream, &Sample_msg, &s.sample);
      pb_get_encoded_size((size_t *) &s.packet.length, &Sample_msg, &s.sample);
      clear_tx_dr();
      LOG_INFO("Encoded packet length: %d\r\n", s.packet.length);
    }
  }
}

void comms_tx_data(Sample *sample)
{
  s.sample = *sample;
  set_tx_dr();
}

void comms_task_setup(void)
{}

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
