#include "sensors.h"

#include "adc.h"
#include "ble_uart.h"
#include "common.h"
#include "comms.h"
#include "flex.h"
#include "imu.h"
#include "logger.h"
#include "sign_buddy.pb.h"
#include "tsc.h"

#include <stdlib.h>

// DATA READY EVENTS
#define TSC_DR_EVENT       (1 << 0)
#define FLEX_DR_EVENT      (1 << 1)
#define IMU_DR_EVENT       (1 << 2)

#define ALL_DR_EVENTS      (TSC_DR_EVENT | FLEX_DR_EVENT | IMU_DR_EVENT)

#define TIMEOUT_MS         40
#define TIMEOUT_TICKS      pdMS_TO_TICKS(TIMEOUT_MS)

#define SAMPLING_PERIOD    50
#define MAX_SAMPLES        40

typedef struct {
  TaskHandle_t       sensors_task_handle;
  TimerHandle_t      sampling_timer;
  EventGroupHandle_t data_ready_event_group;
  SBPSample          sample;
  uint8_t            max_samples;
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

static void imu_data_ready_cb(void)
{
  xEventGroupSetBits(s.data_ready_event_group, IMU_DR_EVENT);
}

static void sampling_timer_cb(TimerHandle_t xTimer)
{
  xTaskNotifyGive(s.sensors_task_handle);
}

static void sensors_task(void *arg)
{
  LOG_INFO("sens: task started\r\n");
  s.data_ready_event_group = xEventGroupCreate();
  if (s.data_ready_event_group == NULL) {
    LOG_ERROR("Event Group Creation Failed\r\n");
  }

  s.sampling_timer = xTimerCreate("sampling_timer", pdMS_TO_TICKS(SAMPLING_PERIOD), pdTRUE, NULL, sampling_timer_cb);
  if (s.sampling_timer == NULL) {
    LOG_ERROR("Sampling timer creation failed\r\n");
  }

  while (1) {
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

    imu_start_read();
    tsc_start_read();
    flex_start_read();

    EventBits_t event_bits;
    event_bits = xEventGroupWaitBits(s.data_ready_event_group,
                                     ALL_DR_EVENTS,
                                     pdTRUE,
                                     pdTRUE,
                                     TIMEOUT_TICKS);

    if (event_bits != (ALL_DR_EVENTS)) {
      LOG_ERROR("TIMEOUT waiting for sensor data\r\n");
    }
    else {
      LOG_DEBUG("Sample_id: %lu\r\n", s.sample.sample_id);

      tsc_data_get(&s.sample.touch_data);
      flex_data_get(&s.sample.flex_data);
      imu_data_get(&s.sample.imu_data);
    }

    comms_tx_sample(&s.sample);
    s.sample.sample_id++;
    if (s.sample.sample_id > s.max_samples) {
      RTOS_ERR_CHECK(xTimerStop(s.sampling_timer, 0));
      s.sample.sample_id = 1;
    }
  }
}

void sensors_task_setup(void)
{
  LOG_DEBUG("Sensors Initialized\r\n");
  flex_callback_register(flex_data_ready_cb);
  tsc_callback_register(tsc_data_ready_cb);
  imu_callback_register(imu_data_ready_cb);
  s.sample.sample_id = 1;
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

void sensors_sampling_timer_start(void)
{
  RTOS_ERR_CHECK(xTimerStart(s.sampling_timer, 0));
}

void sensors_sample_periodic(uint8_t periodic)
{
  if (periodic) {
    s.max_samples = MAX_SAMPLES;
  }
  else {
    s.max_samples = 1;
  }
}
