#include "sensors.h"

#include "adc.h"
#include "ble_uart.h"
#include "common.h"
#include "flex.h"
#include "imu.h"
#include "logger.h"
#include "SignBuddy.pb.h"
#include "tsc.h"

#include <stdlib.h>

#define PROCESS_PERIOD_MS    1000

// DATA READY EVENTS
#define TSC_DR_EVENT         (1 << 0)
#define FLEX_DR_EVENT        (1 << 1)
#define IMU_DR_EVENT         (1 << 2)

#define ALL_DR_EVENTS        (TSC_DR_EVENT | FLEX_DR_EVENT | IMU_DR_EVENT)

#define TIMEOUT_MS           40
#define TIMEOUT_TICKS        pdMS_TO_TICKS(TIMEOUT_MS)

#define SAMPLING_PERIOD      1000
#define MAX_SAMPLES          40

typedef struct {
  TaskHandle_t       sensors_task_handle;
  EventGroupHandle_t data_ready_event_group;
  TimerHandle_t      sampling_timer;
  Sample             sample;
  void (*callback)(void);
} state_t;

static state_t s;

static void tsc_data_ready_cb(void)
{
  xEventGroupSetBits(s.data_ready_event_group, TSC_DR_EVENT);
}

static void flex_data_ready_cb(void)
{
  xEventGroupSetBits(s.data_ready_event_group, FLEX_DR_EVENT);
}

void sampling_timer_cb(TimerHandle_t xTimer)
{
  xTaskNotifyGive(s.sensors_task_handle);
}

static void sensors_task(void *arg)
{
  s.data_ready_event_group = xEventGroupCreate();
  if (s.data_ready_event_group == NULL) {
    LOG_ERROR("Event Group Creation Failed\r\n");
  }

  s.sampling_timer = xTimerCreate("sampling_timer", pdMS_TO_TICKS(SAMPLING_PERIOD), pdTRUE, NULL, sampling_timer_cb);
  if (s.sampling_timer == NULL) {
    LOG_ERROR("Sampling timer creation failed\r\n");
  }

  s.sample.sample_id = 1;

  // TODO Timer start should be triggered by message received from BLE
  RTOS_ERR_CHECK(xTimerStart(s.sampling_timer, 0));

  while (1) {
    if (s.sample.sample_id > MAX_SAMPLES) {
      RTOS_ERR_CHECK(xTimerStop(s.sampling_timer, 0));
    }
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    flex_start_read();

    tsc_start_read();

    //imu_process();

    // TODO Change bits to wait for to all when added
    EventBits_t event_bits;
    event_bits = xEventGroupWaitBits(s.data_ready_event_group,
                                     TSC_DR_EVENT | FLEX_DR_EVENT,
                                     pdTRUE,
                                     pdTRUE,
                                     TIMEOUT_TICKS);

    // TODO change bits here too, and add proper handling of this scenario
    if (event_bits != (TSC_DR_EVENT | FLEX_DR_EVENT)) {
      LOG_ERROR("TIMEOUT waiting for sensor data\r\n");
    }
    else {
      LOG_DEBUG("Sample_id: %lu\r\n", s.sample.sample_id);
      int8_t tsc_val;
      tsc_get_value(&tsc_val);
      s.sample.touchData.touch1 = tsc_val;
      LOG_DEBUG("touch sensor val: %d\r\n", s.sample.touchData.touch1);
      uint16_t flex_val;
      uint32_t *flex_Ptr = (uint32_t *) &s.sample.flexData;
      for (uint8_t i = 0; i < FLEX_SENSOR_CNT; i++) {
        flex_get_value(&flex_val, i);
        *flex_Ptr = flex_val;
        LOG_DEBUG("Flex_%hu val: %lu\r\n", i + 1, *flex_Ptr);
        flex_Ptr++;
      }
    }
    s.callback();
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    s.sample.sample_id++;
  }
}

static void sensors_callback_register(void (*callback)(void))
{
  s.callback = callback;
}

void comms_data_recvd_cb(void)
{
  xTaskNotifyGive(s.sensors_task_handle);
}

void get_sensor_data(Sample *sample)
{
  *sample = s.sample;
}

void sensors_task_setup(void)
{
  imu_init();

  LOG_DEBUG("Sensors Initialized\r\n");
  flex_callback_register(flex_data_ready_cb);
  tsc_callback_register(tsc_data_ready_cb);
  sensors_callback_register(sensors_data_ready_cb);
}

void sensors_task_start(void)
{
  BaseType_t task_status = xTaskCreate(sensors_task,
                                       "sens",
                                       SENSORS_STACK_SIZE,
                                       NULL,
                                       SENSORS_TASK_PRIORITY,
                                       &s.sensors_task_handle);

  RTOS_ERR_CHECK(task_status);
}
