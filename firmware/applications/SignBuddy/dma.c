#include "dma.h"

#include "board.h"
#include "common.h"
#include "flex.h"

static void dma_transfer_error_cb(void)
{
  error_handler();
}

void dma_init(uint32_t destination, uint32_t num_readings)
{
  NVIC_SetPriority(DMA1_Channel1_IRQn, DMA_PRIORITY);
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_1,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_MODE_CIRCULAR |
                        LL_DMA_PERIPH_NOINCREMENT |
                        LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_HALFWORD |
                        LL_DMA_MDATAALIGN_HALFWORD |
                        LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetPeriphRequest(DMA1,
                          LL_DMA_CHANNEL_1,
                          LL_DMA_REQUEST_0);

  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_CHANNEL_1,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                         destination,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_CHANNEL_1,
                       num_readings);

  LL_DMA_EnableIT_TC(DMA1,
                     LL_DMA_CHANNEL_1);

  LL_DMA_EnableIT_TE(DMA1,
                     LL_DMA_CHANNEL_1);

  LL_DMA_EnableChannel(DMA1,
                       LL_DMA_CHANNEL_1);
}

void DMA1_Channel1_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC1(DMA1) == 1) {
    LL_DMA_ClearFlag_TC1(DMA1);

    dma_data_ready_cb();
  }

  if (LL_DMA_IsActiveFlag_TE1(DMA1) == 1) {
    LL_DMA_ClearFlag_TE1(DMA1);

    dma_transfer_error_cb();
  }
}
