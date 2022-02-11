#include "ble_uart.h"

#include "board.h"
#include "common.h"

static void uart_init(void)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };
  LL_USART_InitTypeDef uart_config = { 0 };

  BLE_UART_CLK_EN();
  BLE_UART_GPIO_CLK_EN();

  gpio_config.Pin = BLE_UART_RX_PIN | BLE_UART_TX_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = BLE_UART_GPIO_AF;
  LL_GPIO_Init(BLE_UART_GPIO_PORT, &gpio_config);

  uart_config.BaudRate = 28800;
  uart_config.DataWidth = LL_USART_DATAWIDTH_8B;
  uart_config.StopBits = LL_USART_STOPBITS_1;
  uart_config.Parity = LL_USART_PARITY_NONE;
  uart_config.TransferDirection = LL_USART_DIRECTION_TX_RX;
  uart_config.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  uart_config.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(BLE_UART, &uart_config);
  LL_USART_ConfigAsyncMode(BLE_UART);
  LL_USART_Enable(BLE_UART);
}

void ble_uart_init(void)
{
  uart_init();
}
