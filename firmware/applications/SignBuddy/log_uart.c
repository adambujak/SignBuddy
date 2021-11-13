#include "log_uart.h"

#include "board.h"
#include "common.h"
#include "fifo.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t rx_buffer[LOG_UART_RX_BUFFER_SIZE];
static uint8_t tx_buffer[LOG_UART_TX_BUFFER_SIZE];

static fifo_t rx_fifo;
static fifo_t tx_fifo;

static bool writing = false;

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

  LL_USART_EnableIT_RXNE(LOG_UART);
  LL_USART_EnableIT_TC(LOG_UART);

  NVIC_SetPriority(LOG_UART_IRQn, 0);
  NVIC_EnableIRQ(LOG_UART_IRQn);
}

static inline void tx(void)
{
  uint8_t write_byte;

  if (fifo_pop(&tx_fifo, &write_byte, 1) == 1) {
    LL_USART_TransmitData8(LOG_UART, write_byte);
    writing = true;
  }
}

bool log_uart_is_writing(void)
{
  return writing;
}

void log_uart_write(uint8_t *data, uint32_t length)
{
  DISABLE_IRQ();
  fifo_push(&tx_fifo, data, length);
  if (!writing) {
    tx();
  }
  ENABLE_IRQ();
}

int log_uart_read(uint8_t *data, uint32_t length)
{
  DISABLE_IRQ();
  int ret = fifo_pop(&rx_fifo, data, length);
  ENABLE_IRQ();
  return ret;
}

void log_uart_init(void)
{
  hw_init();
  fifo_init(&rx_fifo, rx_buffer, LOG_UART_RX_BUFFER_SIZE);
  fifo_init(&tx_fifo, tx_buffer, LOG_UART_TX_BUFFER_SIZE);
}

void LOG_UART_IRQHandler(void)
{
  DISABLE_IRQ();
  uint8_t data;
  if (LL_USART_IsActiveFlag_RXNE(LOG_UART)) {
    data = LL_USART_ReceiveData8(LOG_UART);
    // TODO: make sure this is correct flag to clear
    LL_USART_ClearFlag_NE(LOG_UART);
    fifo_push(&rx_fifo, &data, 1);
  }
  if (LL_USART_IsActiveFlag_TC(LOG_UART)) {
    writing = false;
    LL_USART_ClearFlag_TC(LOG_UART);
    tx();
  }
  ENABLE_IRQ();
}
