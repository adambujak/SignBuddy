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

typedef struct {
  uint16_t sensor_data[FLEX_SENSOR_CNT];
} flex_data_t;

typedef struct {
  TaskHandle_t sensors_task_handle;
} state_t;

static state_t s;

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
  while (1) {
    flex_data_t flex_data;
    ERR_CHECK(get_flex_data(&flex_data));

    tx_flex_data(&flex_data);
    uint32_t touch_val = tsc_get_value();
    LOG_INFO("Touch sense: %lu\r\n", touch_val);

    imu_process();
    rtos_delay_ms(500);
  }
}

void sensors_task_setup(void)
{
  adc_init();
  imu_init();
  tsc_init();

  tsc_config();

  LOG_DEBUG("Sensors Initialized\r\n");
}

void sensors_task_start(void)
{
  BaseType_t task_status = xTaskCreate(sensors_task,
                                       "sensors_task",
                                       SENSORS_STACK_SIZE,
                                       NULL,
                                       sensors_TASK_PRIORITY,
                                       &s.sensors_task_handle);

  RTOS_ERR_CHECK(task_status);
}
