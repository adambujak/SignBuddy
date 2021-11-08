#include "adc.h"

#include "board.h"

#define ADC_PREEMPT_PRIO                     0
#define ADC_SUB_PRIO                         0

#define VREF                                 ((uint32_t)5000)
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES    (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define VAR_CONVERTED_DATA_INIT_VALUE        (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

uint16_t uhADCxConvertedData_Voltage_mVolt = 0;               /* Value of voltage calculated from ADC conversion data (unit: mV) */
uint8_t  ubAdcGrpRegularUnitaryConvStatus  = 2;               /* Variable set into ADC interruption callback */

void adc_init(void)
{
  LL_ADC_InitTypeDef       ADC_InitStruct       = { 0 };
  LL_ADC_REG_InitTypeDef   ADC_REG_InitStruct   = { 0 };
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = { 0 };

  LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_ADC_CLK_EN();

  GPIO_InitStruct.Pin  = ADC_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ADC_PORT, &GPIO_InitStruct);

  LL_GPIO_EnablePinAnalogControl(ADC_PORT, ADC_PIN);

  NVIC_SetPriority(ADC1_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), ADC_PREEMPT_PRIO, ADC_SUB_PRIO));
  NVIC_EnableIRQ(ADC1_2_IRQn);

  ADC_CLK_EN();

  ADC_InitStruct.Resolution    = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.LowPowerMode  = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength  = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer      = LL_ADC_REG_DMA_TRANSFER_NONE;
  ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);

  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
  LL_ADC_SetChannelSamplingTime(ADC3, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_47CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADC3, LL_ADC_CHANNEL_9, LL_ADC_SINGLE_ENDED);

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
  uint16_t ADC_Data_Raw   = VAR_CONVERTED_DATA_INIT_VALUE;
  uint16_t ADC_Data_mVolt = 0;

  LL_ADC_REG_StartConversion(ADC1);

  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0);

  LL_ADC_ClearFlag_EOS(ADC1);

  ADC_Data_Raw = LL_ADC_REG_ReadConversionData12(ADC1);

  ADC_Data_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VREF, ADC_Data_Raw, LL_ADC_RESOLUTION_12B);

  return ADC_Data_mVolt;
}
