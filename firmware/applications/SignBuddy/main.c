#include "board.h"
#include "ble_uart.h"
#include "common.h"
#include "gpio.h"
#include "log_uart.h"
#include "logger.h"
#include "sensors.h"
#include "system_time.h"
#include "adc.h"
#include "imu.h"
#include "ble_uart.h"
#include "sensors.h"

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1);

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();

  while (LL_RCC_HSI_IsReady() != 1);

  LL_RCC_HSI_SetCalibTrimming(16);

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_3, LL_RCC_PLL_DIV_2);
  LL_RCC_PLL_Enable();

  while (LL_RCC_PLL_IsReady() != 1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(SYSCLK_FREQ);

  LL_SetSystemCoreClock(SYSCLK_FREQ);

  BLE_UART_CLK_SRC();
  LOG_UART_CLK_SRC();
  IMU_I2C_CLK_SRC();
}

static void board_bringup(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  sysclk_init();

  gpio_init();
}

static void led_process(void)
{
  static uint32_t last_ticks = 0;
  static uint32_t led_state = 0;

  uint32_t time = system_time_get();

  if (system_time_cmp_ms(last_ticks, time) < 1000) {
    return;
  }
  last_ticks = time;
  led_state = (led_state + 1) % 2;
  gpio_led_set(led_state);

  LOG_INFO("LED process\r\n");
}

int main(void)
{
  board_bringup();
  // init early
  system_time_init();
  log_uart_init();

  sensors_init();
  ble_uart_init();
  adc_init();
  imu_init();

  LOG_INFO("App started\r\n");

  while (1) {
    led_process();
    sensors_process();
    imu_process();
  }
}

void error_handler(void)
{
  __disable_irq();
  while (1) {
    gpio_led_set(0);
    for (uint32_t i = 0; i < 1000000; i++);
    gpio_led_set(1);
    for (uint32_t i = 0; i < 1000000; i++);
  }
}
