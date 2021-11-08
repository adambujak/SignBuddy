/******************************************************************************
* @file     board.h
* @brief    SignBuddy board file
* @version  1.0
* @date     2021-10-23
* @author   Adam Bujak
******************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#if defined(BOARD_DISCO)
#include "disco_board.h"
#elif defined(BOARD_NUCLEO)
#include "nucleo_board.h"
#endif // BOARD_DEF

#endif // BOARD_H

