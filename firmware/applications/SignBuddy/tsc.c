#include "tsc.h"

#include "board.h"
#include "common.h"
#include "logger.h"

#define CHANNEL_IOS     TSC_ELECTRODE_IO
#define SAMPLING_IOS    TSC_SAMPLER_IO

typedef struct {
  TSC_HandleTypeDef tsc;
  // TODO add these
  // uint32_t electrode_measurement[TSC_ELECTRODE_CNT];
  // uint32_t calibration_value[TSC_ELECTRODE_CNT];
  int8_t            touch_value;
  TaskHandle_t      task_handle;
  void (*callback)(void);
} state_t;

static state_t s;

static inline void sampler_pin_init(uint32_t pin, GPIO_TypeDef *port)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };

  gpio_config.Pin = pin;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = TSC_SAMPLER_AF;
  LL_GPIO_Init(port, &gpio_config);
}

static inline void electrode_pin_init(uint32_t pin, GPIO_TypeDef *port)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };

  gpio_config.Pin = pin;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = TSC_ELECTRODE_AF;
  LL_GPIO_Init(port, &gpio_config);
}

static void hw_init(void)
{
  TSC_CLK_EN();
  TSC_GPIO_CLK_EN();

  sampler_pin_init(TSC_SAMPLER_PIN, TSC_SAMPLER_PORT);
  electrode_pin_init(TSC_ELECTRODE_PIN, TSC_ELECTRODE_PORT);
}

static void io_config(void)
{
  TSC_IOConfigTypeDef io_config;

  io_config.ChannelIOs = CHANNEL_IOS;
  io_config.SamplingIOs = SAMPLING_IOS;
  HAL_TSC_IOConfig(&s.tsc, &io_config);
}

static inline int convert_value_to_touch(uint32_t value)
{
  if (value < 6000) {
    return 1;
  }

  return 0;
}

static int read_value(uint32_t *value)
{
  if (HAL_TSC_Start(&s.tsc) != HAL_OK) {
    error_handler();
  }

  while (HAL_TSC_GetState(&s.tsc) == HAL_TSC_STATE_BUSY);

  // if max cnt err occured return fail
  uint32_t isr_val = TSC->ISR;

  if (isr_val & TSC_ISR_MCEF) {
    return RET_ERR;
  }

  __HAL_TSC_CLEAR_FLAG(&s.tsc, (TSC_FLAG_EOA | TSC_FLAG_MCE));


  if (HAL_TSC_GroupGetStatus(&s.tsc, TSC_GROUP3_IDX) == TSC_GROUP_COMPLETED) {
    *value = HAL_TSC_GroupGetValue(&s.tsc, TSC_GROUP3_IDX);
  }
  return RET_OK;
}

static inline void sample_data(void)
{
  uint32_t value;

  if (read_value(&value) == RET_OK) {
    s.touch_value = convert_value_to_touch(value);
  }
  else {
    s.touch_value = 0;
  }

  io_config();
  HAL_TSC_IODischarge(&s.tsc, ENABLE);
}

static void tsc_task(void *arg)
{
  while (1) {
    // wait to be notified
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    sample_data();
    s.callback();
  }
}

void tsc_task_setup(void)
{
  hw_init();

  s.tsc.Instance = TSC;
  s.tsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  s.tsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  s.tsc.Init.SpreadSpectrum = DISABLE;
  s.tsc.Init.SpreadSpectrumDeviation = 1;
  s.tsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  s.tsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  s.tsc.Init.MaxCountValue = TSC_MCV_8191;
  s.tsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  s.tsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  s.tsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  s.tsc.Init.MaxCountInterrupt = DISABLE;
  s.tsc.Init.ChannelIOs = TSC_ELECTRODE_IO;
  s.tsc.Init.ShieldIOs = 0;
  s.tsc.Init.SamplingIOs = TSC_SAMPLER_IO;

  if (HAL_TSC_Init(&s.tsc) != HAL_OK) {
    error_handler();
  }
}

void tsc_task_start(void)
{
  BaseType_t task_status = xTaskCreate(tsc_task, "tsc", TSC_STACK_SIZE, NULL, TSC_TASK_PRIORITY, &s.task_handle);

  RTOS_ERR_CHECK(task_status);
}

void tsc_start_read(void)
{
  xTaskNotifyGive(s.task_handle);
}

void tsc_get_value(int8_t *measurement)
{
  *measurement = s.touch_value;
}

void tsc_callback_register(void (*callback)(void))
{
  s.callback = callback;
}
