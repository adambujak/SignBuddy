#include "log_uart.h"

#include "board.h"
#include "common.h"
#include "fifo.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static void hw_init(void)
{
  LL_LPUART_InitTypeDef uart_config = { 0 };
  LL_GPIO_InitTypeDef gpio_config = { 0 };

  LOG_UART_CLK_EN();
  LOG_UART_GPIO_CLK_EN();

  gpio_config.Pin = LOG_UART_TX_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_UP;
  gpio_config.Alternate = LOG_UART_GPIO_AF;
  LL_GPIO_Init(LOG_UART_GPIO_PORT, &gpio_config);

  uart_config.BaudRate = LOG_UART_BAUDRATE;
  uart_config.DataWidth = LL_LPUART_DATAWIDTH_8B;
  uart_config.StopBits = LL_LPUART_STOPBITS_1;
  uart_config.Parity = LL_LPUART_PARITY_NONE;
  uart_config.TransferDirection = LL_LPUART_DIRECTION_TX_RX;

  LL_LPUART_Init(LOG_UART, &uart_config);
  LL_LPUART_EnableHalfDuplex(LOG_UART);

  LL_LPUART_Enable(LOG_UART);
}

static inline void tx(uint8_t write_byte)
{
  LL_LPUART_TransmitData8(LOG_UART, write_byte);
  while (!LL_LPUART_IsActiveFlag_TC(LOG_UART));
}

void log_uart_write(uint8_t *data, uint32_t length)
{
  for (uint32_t i = 0; i < length; i++) {
    tx(data[i]);
  }
}

void log_uart_init(void)
{
  hw_init();
}
