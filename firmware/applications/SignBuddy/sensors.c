#include "sensors.h"

#include "adc.h"
#include "ble_uart.h"
#include "common.h"
#include "logger.h"
#include "tsc.h"

#include <stdlib.h>

#define FLEX_SENSOR_CNT      1
#define PROCESS_PERIOD_MS    1000

typedef struct {
  uint16_t sensor_data[FLEX_SENSOR_CNT];
} flex_data_t;

typedef struct {
  uint32_t last_ticks;
} state_t;

static state_t s;

int get_flex_data(flex_data_t *data)
{
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    adc_enable();
    data->sensor_data[i] = adc_read();
    adc_disable();
  }
  return RET_OK;
}

void tx_flex_data(flex_data_t *data)
{
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    LOG_INFO("Reading: %hu\n\r", data->sensor_data[i]);
  }
}

void sensors_init(void)
{
  s.last_ticks = system_time_get();
  tsc_config();
}

void sensors_process(void)
{
  uint32_t time = system_time_get();

  if (system_time_cmp_ms(s.last_ticks, time) < PROCESS_PERIOD_MS) {
    return;
  }
  s.last_ticks = time;

  flex_data_t flex_data;
  ERR_CHECK(get_flex_data(&flex_data));

  tx_flex_data(&flex_data);
  uint32_t touch_val = tsc_get_value();
  LOG_INFO("Touch sense: %u\r\n", touch_val);
}
