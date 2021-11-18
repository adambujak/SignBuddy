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

  GPIO_ADC_CLK_EN();

  GPIO_InitStruct.Pin = ADC_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ADC_PORT, &GPIO_InitStruct);

  #if defined(BOARD_DISCO_L4) || defined(BOARD_NUCLEO_L4)
  LL_GPIO_EnablePinAnalogControl(ADC_PORT, ADC_PIN);
  #endif
  NVIC_SetPriority(ADC_IRQn, ADC_PRIORITY);
  NVIC_EnableIRQ(ADC_IRQn);

  ADC_CLK_EN();

  adc_config.Resolution = LL_ADC_RESOLUTION_12B;
  adc_config.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  adc_config.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADC_PERIPH, &adc_config);
  adc_reg_config.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  #if defined(BOARD_DISCO_L4) || defined(BOARD_NUCLEO_L4)
  adc_reg_config.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  #endif
  adc_reg_config.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  adc_reg_config.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  adc_reg_config.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  adc_reg_config.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  LL_ADC_REG_Init(ADC_PERIPH, &adc_reg_config);

  adc_common_config.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC_PERIPH), &adc_common_config);

  #if defined(BOARD_DISCO_L4) || defined(BOARD_NUCLEO_L4)
  LL_ADC_REG_SetSequencerRanks(ADC_PERIPH, LL_ADC_REG_RANK_1, ADC_CHANNEL);
  LL_ADC_SetChannelSamplingTime(ADC_PERIPH, ADC_CHANNEL, LL_ADC_SAMPLINGTIME_47CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADC_PERIPH, ADC_CHANNEL, LL_ADC_SINGLE_ENDED);
  LL_ADC_DisableDeepPowerDown(ADC_PERIPH);
  #elif defined(BOARD_NUCLEO_L0)
  LL_ADC_SetSamplingTimeCommonChannels(ADC_PERIPH, LL_ADC_SAMPLINGTIME_1CYCLE_5);
  LL_ADC_SetOverSamplingScope(ADC_PERIPH, LL_ADC_OVS_DISABLE);
  LL_ADC_REG_SetSequencerScanDirection(ADC_PERIPH, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
  LL_ADC_SetCommonFrequencyMode(__LL_ADC_COMMON_INSTANCE(ADC_PERIPH), LL_ADC_CLOCK_FREQ_MODE_HIGH);
  #endif

  LL_ADC_EnableIT_OVR(ADC_PERIPH);

  LL_ADC_EnableInternalRegulator(ADC_PERIPH);

  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while (wait_loop_index != 0)
    wait_loop_index--;
  #if defined(BOARD_DISCO_L4) || defined(BOARD_NUCLEO_L4)
  LL_ADC_StartCalibration(ADC_PERIPH, LL_ADC_SINGLE_ENDED);
  #elif defined(BOARD_NUCLEO_L0)
  LL_ADC_StartCalibration(ADC_PERIPH);
  #endif

  while (LL_ADC_IsCalibrationOnGoing(ADC_PERIPH) != 0);

  wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
  while (wait_loop_index != 0)
    wait_loop_index--;

  LL_ADC_Enable(ADC_PERIPH);

  while (LL_ADC_IsActiveFlag_ADRDY(ADC_PERIPH) == 0);
}

void ADC_IRQHandler(void)
{}

uint16_t adc_read(void)
{
  uint16_t adc_data_raw = VAR_CONVERTED_DATA_INIT_VALUE;
  uint16_t adc_data_mvolt = 0;

  LL_ADC_REG_StartConversion(ADC_PERIPH);

  while (LL_ADC_IsActiveFlag_EOC(ADC_PERIPH) == 0);

  LL_ADC_ClearFlag_EOS(ADC_PERIPH);

  adc_data_raw = LL_ADC_REG_ReadConversionData12(ADC_PERIPH);

  adc_data_mvolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VREF, adc_data_raw, LL_ADC_RESOLUTION_12B);

  return adc_data_mvolt;
}
