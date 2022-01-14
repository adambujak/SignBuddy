#include "adc.h"

#include "board.h"
#include "common.h"

#define VREF                                 ((uint32_t) 3300)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES    (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define VAR_CONVERTED_DATA_INIT_VALUE        (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

void adc_init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  FLEX_ADC_GPIO_CLK_EN();

  GPIO_InitStruct.Pin = FLEX_ADC_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(FLEX_ADC_PORT, &GPIO_InitStruct);

  NVIC_SetPriority(FLEX_ADC_IRQn, FLEX_ADC_PRIORITY);
  NVIC_EnableIRQ(FLEX_ADC_IRQn);

  FLEX_ADC_CLK_EN();

  if (LL_ADC_IsEnabled(FLEX_ADC) == 0) {
    LL_ADC_SetClock(FLEX_ADC, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
    LL_ADC_SetSamplingTimeCommonChannels(FLEX_ADC, LL_ADC_SAMPLINGTIME_39CYCLES_5);
  }

  if ((LL_ADC_IsEnabled(FLEX_ADC) == 0) || (LL_ADC_REG_IsConversionOngoing(FLEX_ADC) == 0)) {
    LL_ADC_REG_SetTriggerSource(FLEX_ADC, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetContinuousMode(FLEX_ADC, LL_ADC_REG_CONV_SINGLE);
    LL_ADC_REG_SetOverrun(FLEX_ADC, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
    LL_ADC_REG_SetSequencerChannels(FLEX_ADC, FLEX_ADC_CHANNEL);
  }

  LL_ADC_EnableIT_OVR(FLEX_ADC);
}

void adc_enable(void)
{
  uint32_t wait_loop_index = 0;

  if (LL_ADC_IsEnabled(FLEX_ADC) == 0) {
    LL_ADC_StartCalibration(FLEX_ADC);

    while (LL_ADC_IsCalibrationOnGoing(FLEX_ADC) != 0);

    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while (wait_loop_index != 0)
      wait_loop_index--;
  }
  else {
    error_handler();
  }

  LL_ADC_Enable(FLEX_ADC);

  while (LL_ADC_IsActiveFlag_ADRDY(FLEX_ADC) == 0);
}

void adc_disable(void)
{
  if (LL_ADC_IsEnabled(FLEX_ADC) == 1) {
    while (LL_ADC_REG_IsConversionOngoing(ADC1) != 0);
    LL_ADC_Disable(FLEX_ADC);
  }
  else {
    error_handler();
  }
}

void FLEX_ADC_IRQHandler(void)
{}

uint16_t adc_read(void)
{
  uint16_t adc_data_raw = VAR_CONVERTED_DATA_INIT_VALUE;
  uint16_t adc_data_mvolt = 0;

  if ((LL_ADC_IsEnabled(ADC1) == 1) &&
      (LL_ADC_IsDisableOngoing(ADC1) == 0) &&
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)) {
    LL_ADC_REG_StartConversion(FLEX_ADC);
  }
  else {
    error_handler();
  }

  while (LL_ADC_IsActiveFlag_EOC(FLEX_ADC) == 0);

  LL_ADC_ClearFlag_EOS(FLEX_ADC);

  adc_data_raw = LL_ADC_REG_ReadConversionData12(FLEX_ADC);

  adc_data_mvolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VREF, adc_data_raw, LL_ADC_RESOLUTION_12B);

  return adc_data_mvolt;
}
