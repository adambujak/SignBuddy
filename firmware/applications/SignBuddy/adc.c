#include "adc.h"

#include "board.h"

#define ADC_PREEMPT_PRIO                     0
#define ADC_SUB_PRIO                         0

#define VREF                                 ((uint32_t)5000)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES    (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define VAR_CONVERTED_DATA_INIT_VALUE        (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

void adc_init(void)
{
  LL_ADC_InitTypeDef adc_config = { 0 };
  LL_ADC_REG_InitTypeDef adc_reg_config = { 0 };
  LL_ADC_CommonInitTypeDef adc_common_config = { 0 };

  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_ADC_CLK_EN();

  GPIO_InitStruct.Pin = ADC_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ADC_PORT, &GPIO_InitStruct);

  LL_GPIO_EnablePinAnalogControl(ADC_PORT, ADC_PIN);

  NVIC_SetPriority(ADC1_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), ADC_PREEMPT_PRIO, ADC_SUB_PRIO));
  NVIC_EnableIRQ(ADC1_2_IRQn);

  ADC_CLK_EN();

  adc_config.Resolution = LL_ADC_RESOLUTION_12B;
  adc_config.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  adc_config.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADC1, &adc_config);
  adc_reg_config.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  adc_reg_config.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  adc_reg_config.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  adc_reg_config.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  adc_reg_config.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  adc_reg_config.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(ADC1, &adc_reg_config);

  adc_common_config.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &adc_common_config);

  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_47CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SINGLE_ENDED);

  LL_ADC_EnableIT_OVR(ADC1);

  LL_ADC_DisableDeepPowerDown(ADC1);
  LL_ADC_EnableInternalRegulator(ADC1);

  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while (wait_loop_index != 0)
    wait_loop_index--;

  LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);

  while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0);

  wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
  while (wait_loop_index != 0)
    wait_loop_index--;

  LL_ADC_Enable(ADC1);

  while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0);
}

void ADC1_2_IRQHandler(void)
{}

uint16_t adc_read(void)
{
  uint16_t adc_data_raw = VAR_CONVERTED_DATA_INIT_VALUE;
  uint16_t adc_data_mvolt = 0;

  LL_ADC_REG_StartConversion(ADC1);

  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0);

  LL_ADC_ClearFlag_EOS(ADC1);

  adc_data_raw = LL_ADC_REG_ReadConversionData12(ADC1);

  adc_data_mvolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VREF, adc_data_raw, LL_ADC_RESOLUTION_12B);

  return adc_data_mvolt;
}
