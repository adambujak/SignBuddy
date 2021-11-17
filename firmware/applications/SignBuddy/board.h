/******************************************************************************
* @file     board.h
* @brief    SignBuddy board file
* @version  1.0
* @date     2021-10-23
* @author   Adam Bujak
******************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#if defined(BOARD_DISCO_L4)
#include "disco_l4_board.h"
#elif defined(BOARD_NUCLEO_L4)
#include "nucleo_l4_board.h"
#elif defined(BOARD_NUCLEO_L0)
#include "nucleo_l0_board.h"
#endif // BOARD_DEF

#endif // BOARD_H
