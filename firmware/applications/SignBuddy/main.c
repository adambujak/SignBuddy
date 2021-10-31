#include "board.h"
#include "gpio.h"
#include "system_time.h"
#include "ble_uart.h"

#define PRIORITYGROUP    ((uint32_t)0x00000003)

void delay_us(uint32_t us)
{
  uint32_t start_time = system_time_get();

  while (system_time_cmp_us(start_time, system_time_get()) < us);
}

void delay_ms(uint32_t ms)
{
  uint32_t start_time = system_time_get();

  while (system_time_cmp_ms(start_time, system_time_get()) < ms);
}

void sysclk_init(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
  while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3);

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();

  while (LL_RCC_HSI_IsReady() != 1);

  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

  while (LL_RCC_PLL_IsReady() != 1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(SYSCLK_FREQ);

  LL_SetSystemCoreClock(SYSCLK_FREQ);
  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_HSI);
}

static void board_bringup(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(PRIORITYGROUP);

  sysclk_init();

  gpio_init();
  system_time_init();
  ble_uart_init();
}

int main(void)
{
  board_bringup();

  while (1) {
    gpio_led_set(1);
    ble_uart_tx((uint8_t)0x94);
    delay_ms(2000);
    gpio_led_set(0);
    ble_uart_tx((uint8_t)0x95);
    delay_ms(1000);
  }
}

void error_handler(void)
{
  __disable_irq();
  while (1);
}
