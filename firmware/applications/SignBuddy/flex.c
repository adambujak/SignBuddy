#include "flex.h"

#include "adc.h"
#include "board.h"
#include "dma.h"
#include "common.h"
#include "logger.h"

#define VREF                  ((uint32_t) 3300)

#define FLEX_SENSOR_CNT       5

#define FLEX_SENSOR_THUMB     0
#define FLEX_SENSOR_INDEX     1
#define FLEX_SENSOR_MIDDLE    2
#define FLEX_SENSOR_RING      3
#define FLEX_SENSOR_LITTLE    4

typedef struct {
  uint16_t     flex_values[FLEX_SENSOR_CNT];
  float        unsaturated_data[FLEX_SENSOR_CNT];
  TaskHandle_t task_handle;
  void (*callback)(void);
} state_t;

static state_t s;

static inline void flex_pin_init(uint32_t pin, GPIO_TypeDef *port)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(port, &GPIO_InitStruct);
}

static void hw_init(void)
{
  flex_pin_init(FLEX_THUMB_PIN, FLEX_THUMB_PORT);
  flex_pin_init(FLEX_INDEX_PIN, FLEX_INDEX_PORT);
  flex_pin_init(FLEX_MIDDLE_PIN, FLEX_MIDDLE_PORT);
  flex_pin_init(FLEX_RING_PIN, FLEX_RING_PORT);
  flex_pin_init(FLEX_LITTLE_PIN, FLEX_LITTLE_PORT);

  dma_init((uint32_t) &s.flex_values, FLEX_SENSOR_CNT);
  adc_init();
}

static inline void calibrate_data(void)
{
  s.unsaturated_data[FLEX_SENSOR_THUMB] = s.flex_values[FLEX_SENSOR_THUMB] * 0.1978 - 438.58;
  s.unsaturated_data[FLEX_SENSOR_INDEX] = s.flex_values[FLEX_SENSOR_INDEX] * 0.1359 - 234.34;
  s.unsaturated_data[FLEX_SENSOR_MIDDLE] = s.flex_values[FLEX_SENSOR_MIDDLE] * 0.2611 - 677.55;
  s.unsaturated_data[FLEX_SENSOR_RING] = s.flex_values[FLEX_SENSOR_RING] * 0.2823 - 610.97;
  s.unsaturated_data[FLEX_SENSOR_LITTLE] = s.flex_values[FLEX_SENSOR_LITTLE] * 0.4321 - 675.7;

  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    if (s.unsaturated_data[i] > 90) {
      s.flex_values[i] = 90;
    }
    else if (s.unsaturated_data[i] < 0) {
      s.flex_values[i] = 0;
    }
    else {
      s.flex_values[i] = (uint16_t) s.unsaturated_data[i];
    }
  }
}

static inline void sample_data(void)
{
  adc_enable();
  adc_start_conversion();
  ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
  adc_disable();
  for (int i = 0; i < FLEX_SENSOR_CNT; i++) {
    s.flex_values[i] = __LL_ADC_CALC_DATA_TO_VOLTAGE(VREF, s.flex_values[i], LL_ADC_RESOLUTION_12B);
  }
  calibrate_data();
}

static void flex_task(void *arg)
{
  LOG_INFO("flex: task started\r\n");
  while (1) {
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    sample_data();
    s.callback();
  }
}

void adc_data_ready_cb(void)
{
  vTaskNotifyGiveFromISR(s.task_handle, 0);
}

void flex_task_setup(void)
{
  hw_init();
}

void flex_task_start(void)
{
  BaseType_t task_status = xTaskCreate(flex_task, "flex", FLEX_STACK_SIZE, NULL, FLEX_TASK_PRIORITY, &s.task_handle);

  RTOS_ERR_CHECK(task_status);
}

void flex_start_read(void)
{
  xTaskNotifyGive(s.task_handle);
}

void flex_data_get(SBPSample_FlexData *data)
{
  data->flex_thumb = s.flex_values[FLEX_SENSOR_THUMB];
  data->flex_index = s.flex_values[FLEX_SENSOR_INDEX];
  data->flex_middle = s.flex_values[FLEX_SENSOR_MIDDLE];
  data->flex_ring = s.flex_values[FLEX_SENSOR_RING];
  data->flex_little = s.flex_values[FLEX_SENSOR_LITTLE];
}

void flex_callback_register(void (*callback)(void))
{
  s.callback = callback;
}
