#include "adc.h"

#include "board.h"
#include "common.h"
#include "logger.h"

#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES    (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

static void adc_overrun_cb(void)
{
  error_handler();
}

void adc_init(void)
{
  NVIC_SetPriority(ADC1_COMP_IRQn, ADC_PRIORITY);
  NVIC_EnableIRQ(ADC1_COMP_IRQn);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  if (LL_ADC_IsEnabled(ADC1) == 0) {
    LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_39CYCLES_5);
  }

  if ((LL_ADC_IsEnabled(ADC1) == 0) || (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)) {
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
    LL_ADC_REG_SetSequencerChannels(ADC1,
                                    FLEX_THUMB_CHANNEL | FLEX_INDEX_CHANNEL | FLEX_MIDDLE_CHANNEL | FLEX_RING_CHANNEL |
                                    FLEX_LITTLE_CHANNEL);
  }

  LL_ADC_EnableIT_OVR(ADC1);
}

void adc_enable(void)
{
  uint32_t wait_loop_index = 0;

  if (LL_ADC_IsEnabled(ADC1) == 0) {
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
    LL_ADC_StartCalibration(ADC1);

    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0);

    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while (wait_loop_index != 0)
      wait_loop_index--;
  }
  else {
    error_handler();
  }

  LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

  LL_ADC_Enable(ADC1);

  while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0);
}

void adc_disable(void)
{
  if (LL_ADC_IsEnabled(ADC1) == 1) {
    while (LL_ADC_REG_IsConversionOngoing(ADC1) != 0);
    LL_ADC_Disable(ADC1);
  }
  else {
    error_handler();
  }
}

void ADC1_COMP_IRQHandler(void)
{
  if (LL_ADC_IsActiveFlag_OVR(ADC1) != 0) {
    LL_ADC_ClearFlag_OVR(ADC1);

    adc_overrun_cb();
  }
}

void adc_start_conversion(void)
{
  if ((LL_ADC_IsEnabled(ADC1) == 1) &&
      (LL_ADC_IsDisableOngoing(ADC1) == 0) &&
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)) {
    LL_ADC_REG_StartConversion(ADC1);
  }
  else {
    LOG_ERROR("ADC: Couldn't start conversion\r\n");
    error_handler();
  }
}
