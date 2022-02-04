#include "tsc.h"

#include "board.h"
#include "common.h"

#define CHANNEL_IOS     TSC_ELECTRODE_IO
#define SAMPLING_IOS    TSC_SAMPLER_IO

typedef struct {
  TSC_HandleTypeDef tsc;
} state_t;

static state_t s;

static void hw_init(void)
{
  TSC_CLK_EN();
  TSC_GPIO_CLK_EN();

  LL_GPIO_InitTypeDef gpio_config = { 0 };

  gpio_config.Pin = TSC_ELECTRODE_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = TSC_ELECTRODE_AF;
  LL_GPIO_Init(TSC_ELECTRODE_PORT, &gpio_config);

  gpio_config.Pin = TSC_SAMPLER_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = TSC_SAMPLER_AF;
  LL_GPIO_Init(TSC_SAMPLER_PORT, &gpio_config);
}

void tsc_init(void)
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

uint32_t tsc_get_value(void)
{
  HAL_TSC_IODischarge(&s.tsc, ENABLE);
  delay_ms(1);

  if (HAL_TSC_Start(&s.tsc) != HAL_OK) {
    error_handler();
  }

  while (HAL_TSC_GetState(&s.tsc) == HAL_TSC_STATE_BUSY);

  __HAL_TSC_CLEAR_FLAG(&s.tsc, (TSC_FLAG_EOA | TSC_FLAG_MCE));

  uint32_t val;
  if (HAL_TSC_GroupGetStatus(&s.tsc, TSC_GROUP3_IDX) == TSC_GROUP_COMPLETED) {
    val = HAL_TSC_GroupGetValue(&s.tsc, TSC_GROUP3_IDX);
  }
  return val;
}

void tsc_config(void)
{
  TSC_IOConfigTypeDef io_config;

  io_config.ChannelIOs = CHANNEL_IOS;
  io_config.SamplingIOs = SAMPLING_IOS;
  HAL_TSC_IOConfig(&s.tsc, &io_config);
}
