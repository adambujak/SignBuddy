#ifndef NUCLEO_BOARD_H
#define NUCLEO_BOARD_H

#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_i2c.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_hal.h"

#define SYSCLK_FREQ                     24000000

#define SYSTEM_TIME_PRIORITY            0
#define ADC_PRIORITY                    1
#define DMA_PRIORITY                    2
#define BLE_UART_PRIORITY               3
#define LOG_UART_PRIORITY               3

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
#define BLE_UART_IRQn                   RNG_LPUART1_IRQn
#define BLE_UART_IRQHandler             RNG_LPUART1_IRQHandler
#define BLE_UART_GPIO_PORT              GPIOC
#define BLE_UART_TX_PIN                 LL_GPIO_PIN_4
#define BLE_UART_RX_PIN                 LL_GPIO_PIN_5
#define BLE_UART_GPIO_AF                LL_GPIO_AF_2
#define BLE_UART_CLK_SRC()              LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1)
#define BLE_UART_CLK_EN()               LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1)
#define BLE_UART_GPIO_CLK_EN()          LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)


/* LOG UART */
#define LOG_UART                  USART2
#define LOG_UART_IRQn             USART2_IRQn
#define LOG_UART_IRQHandler       USART2_IRQHandler
#define LOG_UART_GPIO_PORT        GPIOA
#define LOG_UART_TX_PIN           LL_GPIO_PIN_2
#define LOG_UART_RX_PIN           LL_GPIO_PIN_3
#define LOG_UART_GPIO_AF          LL_GPIO_AF_4
#define LOG_UART_BAUDRATE         115200
#define LOG_UART_CLK_SRC()        LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define LOG_UART_CLK_EN()         LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define LOG_UART_GPIO_CLK_EN()    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)

/* FLEX */
#define FLEX_THUMB_PORT           GPIOA
#define FLEX_THUMB_PIN            LL_GPIO_PIN_0
#define FLEX_THUMB_CHANNEL        LL_ADC_CHANNEL_0
#define FLEX_INDEX_PORT           GPIOA
#define FLEX_INDEX_PIN            LL_GPIO_PIN_1
#define FLEX_INDEX_CHANNEL        LL_ADC_CHANNEL_1
#define FLEX_MIDDLE_PORT          GPIOA
#define FLEX_MIDDLE_PIN           LL_GPIO_PIN_6
#define FLEX_MIDDLE_CHANNEL       LL_ADC_CHANNEL_6
#define FLEX_RING_PORT            GPIOA
#define FLEX_RING_PIN             LL_GPIO_PIN_7
#define FLEX_RING_CHANNEL         LL_ADC_CHANNEL_7
#define FLEX_LITTLE_PORT          GPIOB
#define FLEX_LITTLE_PIN           LL_GPIO_PIN_0
#define FLEX_LITTLE_CHANNEL       LL_ADC_CHANNEL_8
#define FLEX_GPIO_CLK_EN()        do { LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA); LL_IOP_GRP1_EnableClock( \
                                         LL_IOP_GRP1_PERIPH_GPIOB); } while (0)

/* I2C */
#define IMU_I2C                   I2C1
#define IMU_I2C_SCL_PIN           LL_GPIO_PIN_6
#define IMU_I2C_SDA_PIN           LL_GPIO_PIN_7
#define IMU_I2C_GPIO_AF           LL_GPIO_AF_1
#define IMU_I2C_GPIO_PORT         GPIOB
#define IMU_I2C_CLK_SRC()         LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);
#define IMU_I2C_CLK_EN()          LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
#define IMU_I2C_GPIO_CLK_EN()     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

/* TSC */
#define TSC_ELECTRODE_PORT        GPIOC
#define TSC_ELECTRODE_PIN         LL_GPIO_PIN_5
#define TSC_ELECTRODE_AF          LL_GPIO_AF_3
#define TSC_ELECTRODE_IO          TSC_GROUP3_IO1

#define TSC_SAMPLER_PORT          GPIOB
#define TSC_SAMPLER_PIN           LL_GPIO_PIN_2
#define TSC_SAMPLER_AF            LL_GPIO_AF_3
#define TSC_SAMPLER_IO            TSC_GROUP3_IO4

#define TSC_CLK_EN()              LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_TSC)
#define TSC_GPIO_CLK_EN()         LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB); \
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)

#endif // NUCLEO_BOARD_H
