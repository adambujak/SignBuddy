#include "board.h"
#include "ble_uart.h"
#include "common.h"
#include "comms.h"
#include "flex.h"
#include "gpio.h"
#include "imu.h"
#include "log_uart.h"
#include "logger.h"
#include "sensors.h"
#include "tsc.h"

static uint8_t os_started = 0;

static void os_start(void)
{
  os_started = 1;
  vTaskStartScheduler();
}

void rtos_delay_ms(uint32_t ms)
{
  if (os_started) {
    vTaskDelay(ms);
  }
  else {
    HAL_Delay(ms);
  }
}

static void sysclk_init(void)
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

  HAL_Init();

  gpio_init();
}

int main(void)
{
  board_bringup();
  // init early
  log_uart_init();

  flex_task_setup();
  tsc_task_setup();
  imu_task_setup();
  sensors_task_setup();
  comms_task_setup();

  LOG_INFO("App started\r\n");

  comms_task_start();
  sensors_task_start();

  tsc_task_start();

  flex_task_start();

  imu_task_start();

  os_start();
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

void SysTick_Handler(void)
{
  HAL_IncTick();
  if (os_started) {
    OSSysTick_Handler();
  }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  __asm__ ("BKPT");
  LOG_ERROR("Stack Overflow");
  error_handler();
}

void assert_failed(uint8_t *file, uint32_t line)
{
  LOG_ERROR("\r\nassert_failed(). file: %s, line: %ld\r\n", file, line);
}
