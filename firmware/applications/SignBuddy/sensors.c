#include "sensors.h"

#include "adc.h"
#include "ble_uart.h"
#include "common.h"
#include "imu.h"
#include "logger.h"
#include "tsc.h"

#include <stdlib.h>

#define FLEX_SENSOR_CNT      1
#define PROCESS_PERIOD_MS    1000

// DATA READY EVENTS
#define TSC_DR_EVENT         (1 << 0)
#define FLEX_DR_EVENT        (1 << 1)
#define IMU_DR_EVENT         (1 << 2)

#define ALL_DR_EVENTS        (TSC_DR_EVENT | FLEX_DR_EVENT | IMU_DR_EVENT)

#define TIMEOUT_MS           40
#define TIMEOUT_TICKS        pdMS_TO_TICKS(TIMEOUT_MS)

typedef struct {
  uint16_t sensor_data[FLEX_SENSOR_CNT];
} flex_data_t;

typedef struct {
  TaskHandle_t       sensors_task_handle;
  EventGroupHandle_t data_ready_event_group;
} state_t;

static state_t s;

static void tsc_data_ready_cb(void)
{
  xEventGroupSetBits(s.data_ready_event_group, TSC_DR_EVENT);
}

static int get_flex_data(flex_data_t *data)
{
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    adc_enable();
    data->sensor_data[i] = adc_read();
    adc_disable();
  }
  return RET_OK;
}

static void tx_flex_data(flex_data_t *data)
{
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    LOG_INFO("Reading: %hu\n\r", data->sensor_data[i]);
  }
}

static void sensors_task(void *arg)
{
  s.data_ready_event_group = xEventGroupCreate();
  if (s.data_ready_event_group == NULL) {
    LOG_ERROR("Event Group Creation Failed\r\n");
  }

  while (1) {
    flex_data_t flex_data;
    ERR_CHECK(get_flex_data(&flex_data));

    tsc_start_read();

    imu_process();

    // TODO Change bits to wait for to all when added
    EventBits_t event_bits;
    event_bits = xEventGroupWaitBits(s.data_ready_event_group, TSC_DR_EVENT, pdTRUE, pdTRUE, TIMEOUT_TICKS);

    // TODO change bits here too, and add proper handling of this scenario
    if (event_bits != TSC_DR_EVENT) {
      LOG_ERROR("TIMEOUT waiting for sensor data\r\n");
    }
    else {
      LOG_DEBUG("Data ready\r\n");

      int8_t tsc_val;
      tsc_get_value(&tsc_val);
      LOG_DEBUG("touch sensor val: %d\r\n", (int) tsc_val);
    }
    rtos_delay_ms(500);
  }
}

void sensors_task_setup(void)
{
  adc_init();
  imu_init();

  LOG_DEBUG("Sensors Initialized\r\n");
  tsc_callback_register(tsc_data_ready_cb);
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
