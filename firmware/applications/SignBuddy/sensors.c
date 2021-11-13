#include "sensors.h"

#include "adc.h"
#include "ble_uart.h"
#include "common.h"

#include <stdlib.h>

#define FLEX_SENSOR_CNT      1
#define PROCESS_PERIOD_MS    1000

typedef struct {
  uint16_t sensor_data[FLEX_SENSOR_CNT];
} flex_data_t;

typedef struct {
  uint32_t last_ticks;
} state_t;

static state_t state;

int get_flex_data(flex_data_t *data)
{
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    data->sensor_data[i] = adc_read();
  }
  return RET_OK;
}

void tx_flex_data(flex_data_t *data)
{
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    uint8_t tx_data[2] = { data->sensor_data[i] & 0xff, data->sensor_data[i] >> 8 };
    for (int j = 0; j < 2; j++) {
      ble_uart_tx(tx_data[j]);
      uint32_t start_time = system_time_get();
      while (system_time_cmp_ms(start_time, system_time_get()) < 100);
    }
  }
}

void sensors_init(void)
{
  state.last_ticks = system_time_get();
}

void sensors_process(void)
{
  uint32_t time = system_time_get();

  if (system_time_cmp_ms(state.last_ticks, time) < PROCESS_PERIOD_MS) {
    return;
  }
  state.last_ticks = time;

  flex_data_t flex_data;
  ERR_CHECK(get_flex_data(&flex_data));

  tx_flex_data(&flex_data);
}
