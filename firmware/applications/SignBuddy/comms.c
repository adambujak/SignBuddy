#include "comms.h"

#include "common.h"
#include "logger.h"
#include "pb_encode.h"
#include "sensors.h"
#include "SignBuddy.pb.h"

#include <stdlib.h>

typedef struct __attribute__((__packed__)) {
  uint8_t length;
  uint8_t protobuf[Sample_size];
  uint32_t crc;
} packet_t;

typedef struct {
  TaskHandle_t task_handle;
  Sample       sample;
  packet_t     packet;
  void (*callback)(void);
} state_t;

static state_t s;

void sensors_data_ready_cb(void)
{
  xTaskNotifyGive(s.task_handle);
}

static void comms_task(void *arg)
{
  LOG_INFO("Comms task started\r\n");

  while (1) {
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    get_sensor_data(&s.sample);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *) &s.packet.protobuf, Sample_size);
    pb_encode(&stream, &Sample_msg, &s.sample);
    pb_get_encoded_size((size_t *) &s.packet.length, &Sample_msg, &s.sample);
    LOG_INFO("Encoded packet length: %d\r\n", s.packet.length);
    s.callback();
  }
}

static void comms_callback_register(void (*callback)(void))
{
  s.callback = callback;
}

void comms_task_setup(void)
{
  comms_callback_register(comms_data_recvd_cb);
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
