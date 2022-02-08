#include "log_uart.h"

#include "board.h"
#include "common.h"
#include "fifo.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static void hw_init(void)
{
  LL_USART_InitTypeDef uart_config = { 0 };
  LL_GPIO_InitTypeDef gpio_config = { 0 };

  LOG_UART_CLK_EN();
  LOG_UART_GPIO_CLK_EN();

  gpio_config.Pin = LOG_UART_RX_PIN | LOG_UART_TX_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = LOG_UART_GPIO_AF;
  LL_GPIO_Init(LOG_UART_GPIO_PORT, &gpio_config);

  uart_config.BaudRate = LOG_UART_BAUDRATE;
  uart_config.DataWidth = LL_USART_DATAWIDTH_8B;
  uart_config.StopBits = LL_USART_STOPBITS_1;
  uart_config.Parity = LL_USART_PARITY_NONE;
  uart_config.TransferDirection = LL_USART_DIRECTION_TX_RX;
  uart_config.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  uart_config.OverSampling = LL_USART_OVERSAMPLING_16;

  LL_USART_Init(LOG_UART, &uart_config);

  LL_USART_ConfigAsyncMode(LOG_UART);
  LL_USART_Enable(LOG_UART);
}

static inline void tx(uint8_t write_byte)
{
  LL_USART_TransmitData8(LOG_UART, write_byte);
  while (!LL_USART_IsActiveFlag_TC(LOG_UART));
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
