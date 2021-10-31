#include "ble_uart.h"

#include "board.h"

#define LPUART_PREEMPT_PRIO    0
#define LPUART_SUB_PRIO        0

static void lpuart_init(void)
{
  LL_GPIO_InitTypeDef   config      = { 0 };
  LL_LPUART_InitTypeDef uart_config = { 0 };

  LPUART_CLK_EN();
  GPIO_LPUART_CLK_EN();

  config.Pin        = LPUART_RX_PIN | LPUART_TX_PIN;
  config.Mode       = LL_GPIO_MODE_ALTERNATE;
  config.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  config.Pull       = LL_GPIO_PULL_NO;
  config.Alternate  = LL_GPIO_AF_8;
  LL_GPIO_Init(LPUART_PORT, &config);

  NVIC_SetPriority(LPUART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), LPUART_PREEMPT_PRIO, LPUART_SUB_PRIO));
  NVIC_EnableIRQ(LPUART1_IRQn);

  uart_config.BaudRate            = 9600;
  uart_config.DataWidth           = LL_LPUART_DATAWIDTH_7B;
  uart_config.StopBits            = LL_LPUART_STOPBITS_1;
  uart_config.Parity              = LL_LPUART_PARITY_NONE;
  uart_config.TransferDirection   = LL_LPUART_DIRECTION_TX_RX;
  uart_config.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;
  LL_LPUART_Init(LPUART1, &uart_config);
  LL_LPUART_Enable(LPUART1);
}

void LPUART1_IRQHandler(void)
{}

uint8_t ble_uart_rx(void)
{
  return LL_LPUART_ReceiveData8(LPUART1);
}

void ble_uart_tx(uint8_t data)
{
  LL_LPUART_TransmitData8(LPUART1, data);
}

void ble_uart_init(void)
{
  lpuart_init();
}
