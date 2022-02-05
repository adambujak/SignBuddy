#include "dma.h"

#include "board.h"
#include "common.h"
#include "comms.h"
#include "flex.h"

static void dma_transfer_error_cb(void)
{
  error_handler();
}

void dma_adc_init(uint32_t destination, uint32_t num_readings)
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

  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_0);

  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_CHANNEL_1,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                         destination,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, num_readings);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

void DMA1_Channel1_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC1(DMA1) == 1) {
    LL_DMA_ClearFlag_TC1(DMA1);

    adc_data_ready_cb();
  }

  if (LL_DMA_IsActiveFlag_TE1(DMA1) == 1) {
    LL_DMA_ClearFlag_TE1(DMA1);

    dma_transfer_error_cb();
  }
}

void dma_uart_init(uint32_t tx_dest, uint32_t tx_size, uint32_t rx_dest, uint32_t rx_size)
{
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, DMA_PRIORITY);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* Configure TX DMA */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_2,
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                        LL_DMA_MODE_CIRCULAR |
                        LL_DMA_PERIPH_NOINCREMENT |
                        LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_BYTE |
                        LL_DMA_MDATAALIGN_BYTE |
                        LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMA_REQUEST_5);

  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2,
                         tx_dest,
                         LL_LPUART_DMA_GetRegAddr(BLE_UART, LL_LPUART_DMA_REG_DATA_TRANSMIT),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2));

  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, tx_size);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);

  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);

  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);

  /* Configure RX DMA */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_3,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_MODE_CIRCULAR |
                        LL_DMA_PERIPH_NOINCREMENT |
                        LL_DMA_MEMORY_INCREMENT |
                        LL_DMA_PDATAALIGN_BYTE |
                        LL_DMA_MDATAALIGN_BYTE |
                        LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_5);

  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3,
                         LL_LPUART_DMA_GetRegAddr(BLE_UART, LL_LPUART_DMA_REG_DATA_RECEIVE),
                         rx_dest,
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3));

  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, rx_size);

  LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_3);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);

  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);

  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

void DMA1_Channel2_3_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC2(DMA1) == 1) {
    LL_DMA_ClearFlag_TC2(DMA1);

    comms_tx_data_clear_cb();
  }

  if (LL_DMA_IsActiveFlag_TE2(DMA1) == 1) {
    LL_DMA_ClearFlag_TE2(DMA1);

    dma_transfer_error_cb();
  }

  if (LL_DMA_IsActiveFlag_TC3(DMA1) == 1) {
    LL_DMA_ClearFlag_TC3(DMA1);

    comms_rx_data_ready_cb();
  }

  if (LL_DMA_IsActiveFlag_HT3(DMA1) == 1) {
    LL_DMA_ClearFlag_HT3(DMA1);

    comms_rx_data_ready_cb();
  }

  if (LL_DMA_IsActiveFlag_TE3(DMA1) == 1) {
    LL_DMA_ClearFlag_TE3(DMA1);

    dma_transfer_error_cb();
  }
}
