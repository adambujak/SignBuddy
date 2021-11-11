#include "gpio.h"

#include "board.h"

#define PIN_WRITE(pin, port, value)        \
  if ((value) == 0) {                      \
    LL_GPIO_ResetOutputPin((port), (pin)); \
  }                                        \
  else {                                   \
    LL_GPIO_SetOutputPin((port), (pin));   \
  }                                        \

void gpio_init(void)
{
  GPIO_LED_CLK_EN();

  LL_GPIO_InitTypeDef config = { 0 };

  config.Pin = LED1_PIN;
  config.Mode = LL_GPIO_MODE_OUTPUT;
  config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  config.Pull = LL_GPIO_PULL_UP;

  LL_GPIO_Init(LED1_PORT, &config);
}

void gpio_led_set(uint8_t val)
{
  PIN_WRITE(LED1_PIN, LED1_PORT, val);
}
