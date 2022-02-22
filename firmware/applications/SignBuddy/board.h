/******************************************************************************
* @file     board.h
* @brief    SignBuddy board file
* @version  1.0
* @date     2021-10-23
* @author   Adam Bujak
******************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#if defined(BOARD_NUCLEO)
#include "nucleo_board.h"
#endif // BOARD_NUCLEO

#if defined(BOARD_BUDDY)
#include "buddy_board.h"
#endif // BOARD_BUDDY

#endif // BOARD_H
