#include "adc.h"

#include "board.h"

#define VREF                                 ((uint32_t)3300)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES    (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define VAR_CONVERTED_DATA_INIT_VALUE        (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

void adc_init(void)
{
  LL_ADC_InitTypeDef adc_config = { 0 };
  LL_ADC_REG_InitTypeDef adc_reg_config = { 0 };
  LL_ADC_CommonInitTypeDef adc_common_config = { 0 };

  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  FLEX_ADC_GPIO_CLK_EN();

  GPIO_InitStruct.Pin = FLEX_ADC_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(FLEX_ADC_PORT, &GPIO_InitStruct);

  NVIC_SetPriority(FLEX_ADC_IRQn, FLEX_ADC_PRIORITY);
  NVIC_EnableIRQ(FLEX_ADC_IRQn);

  FLEX_ADC_CLK_EN();

  adc_config.Resolution = LL_ADC_RESOLUTION_12B;
  adc_config.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  adc_config.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(FLEX_ADC, &adc_config);
  adc_reg_config.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  adc_reg_config.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  adc_reg_config.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  adc_reg_config.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  adc_reg_config.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(FLEX_ADC, &adc_reg_config);

  adc_common_config.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(FLEX_ADC), &adc_common_config);

  LL_ADC_SetSamplingTimeCommonChannels(FLEX_ADC, LL_ADC_SAMPLINGTIME_1CYCLE_5);
  LL_ADC_SetOverSamplingScope(FLEX_ADC, LL_ADC_OVS_DISABLE);
  LL_ADC_REG_SetSequencerScanDirection(FLEX_ADC, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
  LL_ADC_SetCommonFrequencyMode(__LL_ADC_COMMON_INSTANCE(FLEX_ADC), LL_ADC_CLOCK_FREQ_MODE_HIGH);

  LL_ADC_EnableIT_OVR(FLEX_ADC);

  LL_ADC_EnableInternalRegulator(FLEX_ADC);

  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while (wait_loop_index != 0)
    wait_loop_index--;

  LL_ADC_StartCalibration(FLEX_ADC);

  while (LL_ADC_IsCalibrationOnGoing(FLEX_ADC) != 0);

  wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
  while (wait_loop_index != 0)
    wait_loop_index--;

  LL_ADC_Enable(FLEX_ADC);

  while (LL_ADC_IsActiveFlag_ADRDY(FLEX_ADC) == 0);
}

void FLEX_ADC_IRQHandler(void)
{}

uint16_t adc_read(void)
{
  uint16_t adc_data_raw = VAR_CONVERTED_DATA_INIT_VALUE;
  uint16_t adc_data_mvolt = 0;

  LL_ADC_REG_StartConversion(FLEX_ADC);

  while (LL_ADC_IsActiveFlag_EOC(FLEX_ADC) == 0);

  LL_ADC_ClearFlag_EOS(FLEX_ADC);

  adc_data_raw = LL_ADC_REG_ReadConversionData12(FLEX_ADC);

  adc_data_mvolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VREF, adc_data_raw, LL_ADC_RESOLUTION_12B);

  return adc_data_mvolt;
}
