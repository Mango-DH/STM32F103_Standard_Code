/**
 * @filename:RCS_ANSI.h
 * @brief:提供小黑窗命令行的排版支持+串口重定向
 * @contribute: CYK-Dot 2024-5-29
 * --------------------------------------------------------
 **/
#ifndef __RCS_ANSI_H
#define __RCS_ANSI_H

#include "stdio.h"
#include <stm32f10x_usart.h>


/*-----------导出枚举------------------------------------*/
typedef enum{
	ANSI_SHADE_DARK=3,
	ANSI_SHADE_NONE=9,
}ANSI_SHADE;

typedef enum{
	ANSI_COLOR_BLACK=0,
	ANSI_COLOR_RED=1,
	ANSI_COLOR_GREEN=2,
	ANSI_COLOR_YELLOW=3,
	ANSI_COLOR_BLUE=4,
	ANSI_COLOR_PURPLE=5,
	ANSI_COLOR_WHITE=7,
}ANSI_COLOR;

/*-----------导出函数------------------------------------*/
void ANSI_Set_OutPort(USART_TypeDef* USARTx);
void ANSI_Set_InPort(USART_TypeDef* USARTx);
void ANSI_CHANGE_COLOR(int ANSI_SHADE,int ANSI_COLOR);
void ANSI_CLEAR_ALL(void);
void ANSI_MOVE_CURSOR(int Line,int Colomn);
void ANSI_MOVE_CURSOR_COLOMN(int Colomn);
void ANSI_CLEAR_CURSOR(void);
uint8_t ANSI_Assert(void);

#endif
