#ifndef NUCLEO_BOARD_H
#define NUCLEO_BOARD_H

#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_utils.h"

#define SYSCLK_FREQ               64000000

/* LED */
#define LED1_PORT                 GPIOA
#define LED1_PIN                  LL_GPIO_PIN_5
#define GPIO_LED_CLK_EN()         LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)

/* System Time */
#define SYSTEM_TIME_TIMER         TIM2
#define SYSTEM_TIME_CLK_EN()      LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2)

/* BLE UART */
#define LPUART_PORT               GPIOC
#define LPUART_RX_PIN             LL_GPIO_PIN_0
#define LPUART_TX_PIN             LL_GPIO_PIN_1
#define GPIO_LPUART_CLK_EN()      LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)
#define LPUART_CLK_EN()           LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1)

/* LOG UART */
#define LOG_UART                  USART3
#define LOG_UART_IRQn             USART3_IRQn
#define LOG_UART_IRQHandler       USART3_IRQHandler
#define LOG_UART_RX_PIN           LL_GPIO_PIN_8
#define LOG_UART_TX_PIN           LL_GPIO_PIN_9
#define LOG_UART_GPIO_PORT        GPIOD
#define LOG_UART_GPIO_AF          LL_GPIO_AF_7
#define LOG_UART_BAUDRATE         115200
#define LOG_UART_CLK_EN()         LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define LOG_UART_GPIO_CLK_EN()    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD)

/* ADC */
#define ADC                       ADC1
#define ADC_IRQ                   ADC1_2_IRQn
#define ADC_IRQ_Callback          ADC1_2_IRQHandler
#define ADC_CHANNEL               LL_ADC_CHANNEL_9
#define ADC_PORT                  GPIOA
#define ADC_PIN                   LL_GPIO_PIN_4
#define GPIO_ADC_CLK_EN()         LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define ADC_CLK_EN()              LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC)

#endif // NUCLEO_BOARD_H
