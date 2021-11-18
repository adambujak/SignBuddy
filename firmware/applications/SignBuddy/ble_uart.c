#include "ble_uart.h"

#include "board.h"
#include "common.h"
#include "fifo.h"

static uint8_t rx_buffer[BLE_UART_RX_BUFFER_SIZE];
static uint8_t tx_buffer[BLE_UART_TX_BUFFER_SIZE];

static fifo_t rx_fifo;
static fifo_t tx_fifo;

static bool writing = false;

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

  NVIC_SetPriority(BLE_UART_IRQn, BLE_UART_PRIORITY);
  NVIC_EnableIRQ(BLE_UART_IRQn);

  uart_config.BaudRate = 9600;
  uart_config.DataWidth = BLE_UART_DATAWIDTH;
  uart_config.StopBits = BLE_UART_STOPBITS;
  uart_config.Parity = BLE_UART_PARITY;
  uart_config.TransferDirection = BLE_UART_DIRECTION;
  uart_config.HardwareFlowControl = BLE_UART_FLOWCTRL;
  BLE_UART_Init(BLE_UART, &uart_config);
  BLE_UART_Enable(BLE_UART);

  BLE_UART_EnableIT_RXNE(BLE_UART);
  BLE_UART_EnableIT_TC(BLE_UART);
}

static inline void tx(void)
{
  uint8_t write_byte;

  if (fifo_pop(&tx_fifo, &write_byte, 1) == 1) {
    BLE_UART_TX(BLE_UART, write_byte);
    writing = true;
  }
}

bool ble_uart_is_writing(void)
{
  return writing;
}

void ble_uart_write(uint8_t *data, uint32_t length)
{
  BLE_UART_TX(BLE_UART, 0xf);
  DISABLE_IRQ();
  fifo_push(&tx_fifo, data, length);
  if (!writing) {
    tx();
  }
  ENABLE_IRQ();
}

int ble_uart_read(uint8_t *data, uint32_t length)
{
  DISABLE_IRQ();
  int ret = fifo_pop(&rx_fifo, data, length);
  ENABLE_IRQ();
  return ret;
}

void ble_uart_init(void)
{
  uart_init();
  fifo_init(&rx_fifo, rx_buffer, BLE_UART_RX_BUFFER_SIZE);
  fifo_init(&tx_fifo, tx_buffer, BLE_UART_TX_BUFFER_SIZE);
}

void BLE_UART_IRQHandler(void)
{
  DISABLE_IRQ();
  uint8_t data;
  if (BLE_UART_IsActiveFlag_RXNE(BLE_UART)) {
    data = BLE_UART_RX(BLE_UART);
    // TODO: make sure this is correct flag to clear
    BLE_UART_ClearFlag_NE(BLE_UART);
    fifo_push(&rx_fifo, &data, 1);
  }
  if (BLE_UART_IsActiveFlag_TC(BLE_UART)) {
    writing = false;
    BLE_UART_ClearFlag_TC(BLE_UART);
    tx();
  }
  ENABLE_IRQ();
}
