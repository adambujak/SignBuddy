/******************************************************************************
* @file     board.h
* @brief    SignBuddy board file
* @version  1.0
* @date     2021-10-23
* @author   Adam Bujak
******************************************************************************/

#ifndef BOARD_H
#define BOARD_H

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
#include "stm32l4xx_ll_utils.h"

#define SYSCLK_FREQ    64000000

#if defined(BOARD_DISCO)

/* LED */
#define LED1_PORT               GPIOB
#define LED1_PIN                LL_GPIO_PIN_2

#define GPIO_LED_CLK_EN()       LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)

/* System Time */
#define SYSTEM_TIME_TIMER       TIM2
#define SYSTEM_TIME_CLK_EN()    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2)

/* Bluetooth UART */
#define LPUART_PORT             GPIOC
#define LPUART_RX_PIN           LL_GPIO_PIN_0
#define LPUART_TX_PIN           LL_GPIO_PIN_1
#define GPIO_LPUART_CLK_EN()    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC)

#define LPUART_CLK_EN()         LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1)

#endif // defined(BOARD_DISCO)

#endif // BOARD_H
