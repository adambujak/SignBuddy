#include "ble_uart.h"

#include "board.h"
#include "common.h"
#include "comms.h"

static void uart_init(void)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };
  LL_LPUART_InitTypeDef uart_config = { 0 };

  BLE_UART_CLK_EN();
  BLE_UART_GPIO_CLK_EN();

  gpio_config.Pin = BLE_UART_RX_PIN | BLE_UART_TX_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = BLE_UART_GPIO_AF;
  LL_GPIO_Init(BLE_UART_GPIO_PORT, &gpio_config);

  NVIC_SetPriority(BLE_UART_IRQn, BLE_UART_PRIORITY);
  NVIC_EnableIRQ(BLE_UART_IRQn);

  uart_config.BaudRate = 9600;
  uart_config.DataWidth = LL_LPUART_DATAWIDTH_8B;
  uart_config.StopBits = LL_LPUART_STOPBITS_1;
  uart_config.Parity = LL_LPUART_PARITY_NONE;
  uart_config.TransferDirection = LL_LPUART_DIRECTION_TX_RX;
  uart_config.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;
  LL_LPUART_Init(BLE_UART, &uart_config);
  LL_LPUART_Enable(BLE_UART);
}

void ble_uart_enable_it(void)
{
  LL_LPUART_EnableIT_RXNE(BLE_UART);
  LL_LPUART_EnableIT_TXE(BLE_UART);
  LL_LPUART_EnableIT_TC(BLE_UART);
}

void ble_uart_init(void)
{
  uart_init();
}

void BLE_UART_IRQHandler(void)
{
  DISABLE_IRQ();
  if (LL_LPUART_IsActiveFlag_RXNE(BLE_UART)) {
    comms_rx_data_ready_cb();
  }
  if (LL_LPUART_IsActiveFlag_TXE(BLE_UART)) {
    comms_tx_empty_cb();
  }
  if (LL_LPUART_IsActiveFlag_TC(BLE_UART)) {
    LL_LPUART_ClearFlag_TC(BLE_UART);
    comms_tx_data_clear_cb();
  }
  ENABLE_IRQ();
}
