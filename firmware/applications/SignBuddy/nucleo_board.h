#ifndef NUCLEO_BOARD_H
#define NUCLEO_BOARD_H

#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_utils.h"

#define SYSCLK_FREQ                     24000000

#define SYSTEM_TIME_PRIORITY            4
#define FLEX_ADC_PRIORITY               5
#define BLE_UART_PRIORITY               9
#define LOG_UART_PRIORITY               10

/* LED */
#define LED1_PORT                       GPIOA
#define LED1_PIN                        LL_GPIO_PIN_5
#define GPIO_LED_CLK_EN()               LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)

/* System Time */
#define SYSTEM_TIME_TIMER               TIM2
#define SYSTEM_TIME_TIMER_IRQn          TIM2_IRQn
#define SYSTEM_TIME_TIMER_IRQHandler    TIM2_IRQHandler
#define SYSTEM_TIME_CLK_EN()            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2)

/* BLE UART */
#define BLE_UART                        LPUART1
#define BLE_UART_CONFIG                 LL_LPUART_InitTypeDef
#define BLE_UART_IRQn                   LPUART1_IRQn
#define BLE_UART_IRQHandler             LPUART1_IRQHandler
#define BLE_UART_RX_PIN                 LL_GPIO_PIN_5
#define BLE_UART_TX_PIN                 LL_GPIO_PIN_4
#define BLE_UART_GPIO_PORT              GPIOC
#define BLE_UART_GPIO_AF                LL_GPIO_AF_2
#define BLE_UART_DATAWIDTH              LL_LPUART_DATAWIDTH_8B
#define BLE_UART_STOPBITS               LL_LPUART_STOPBITS_1
#define BLE_UART_PARITY                 LL_LPUART_PARITY_NONE
#define BLE_UART_DIRECTION              LL_LPUART_DIRECTION_TX_RX
#define BLE_UART_FLOWCTRL               LL_LPUART_HWCONTROL_NONE
#define BLE_UART_Init                   LL_LPUART_Init
#define BLE_UART_Enable                 LL_LPUART_Enable
#define BLE_UART_RX                     LL_LPUART_ReceiveData8
#define BLE_UART_TX                     LL_LPUART_TransmitData8
#define BLE_UART_EnableIT_RXNE          LL_LPUART_EnableIT_RXNE
#define BLE_UART_EnableIT_TC            LL_LPUART_EnableIT_TC
#define BLE_UART_IsActiveFlag_RXNE      LL_LPUART_IsActiveFlag_RXNE
#define BLE_UART_ClearFlag_NE           LL_LPUART_ClearFlag_NE
#define BLE_UART_IsActiveFlag_TC        LL_LPUART_IsActiveFlag_TC
#define BLE_UART_ClearFlag_TC           LL_LPUART_ClearFlag_TC
#define BLE_UART_CLK_SRC()              LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1)
#define BLE_UART_CLK_EN()               LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1)
#define BLE_UART_GPIO_CLK_EN()          LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)


/* LOG UART */
#define LOG_UART                  USART2
#define LOG_UART_IRQn             USART2_IRQn
#define LOG_UART_IRQHandler       USART2_IRQHandler
#define LOG_UART_RX_PIN           LL_GPIO_PIN_3
#define LOG_UART_TX_PIN           LL_GPIO_PIN_2
#define LOG_UART_GPIO_PORT        GPIOA
#define LOG_UART_GPIO_AF          LL_GPIO_AF_4
#define LOG_UART_BAUDRATE         115200
#define LOG_UART_CLK_SRC()        LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define LOG_UART_CLK_EN()         LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define LOG_UART_GPIO_CLK_EN()    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)

/* ADC */
#define FLEX_ADC                  ADC1
#define FLEX_ADC_IRQn             ADC1_COMP_IRQn
#define FLEX_ADC_IRQHandler       ADC1_COMP_IRQHandler
#define FLEX_ADC_CHANNEL          LL_ADC_CHANNEL_4
#define FLEX_ADC_PORT             GPIOA
#define FLEX_ADC_PIN              LL_GPIO_PIN_4
#define FLEX_ADC_GPIO_CLK_EN()    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)
#define FLEX_ADC_CLK_EN()         LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1)

#endif // NUCLEO_BOARD_H
