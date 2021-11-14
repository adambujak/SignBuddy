#include "ble_uart.h"

#include "board.h"

static void uart_init(void)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };
  BLE_UART_CONFIG uart_config = { 0 };

  BLE_UART_CLK_EN();
  BLE_UART_GPIO_CLK_EN();

  gpio_config.Pin = BLE_UART_RX_PIN | BLE_UART_TX_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = BLE_UART_GPIO_AF;
  LL_GPIO_Init(BLE_UART_GPIO_PORT, &gpio_config);

  NVIC_SetPriority(BLE_UART_IRQn, 0);
  NVIC_EnableIRQ(BLE_UART_IRQn);

  uart_config.BaudRate = 9600;
  uart_config.DataWidth = BLE_UART_DATAWIDTH;
  uart_config.StopBits = BLE_UART_STOPBITS;
  uart_config.Parity = BLE_UART_PARITY;
  uart_config.TransferDirection = BLE_UART_DIRECTION;
  uart_config.HardwareFlowControl = BLE_UART_FLOWCTRL;
  BLE_UART_Init(BLE_UART, &uart_config);
  BLE_UART_Enable(BLE_UART);
}

void BLE_UART_IRQHandler(void)
{}

uint8_t ble_uart_rx(void)
{
  return BLE_UART_RX(BLE_UART);
}

void ble_uart_tx(uint8_t data)
{
  BLE_UART_TX(BLE_UART, data);
}

void ble_uart_init(void)
{
  uart_init();
}
