/**
 * @filename:RCS_ANSI.c
 * @brief:提供小黑窗命令行的排版支持+串口重定向
 * @contribute: CYK-Dot 2024-5-29
 * --------------------------------------------------------
 **/

#include "RCS_ANSI.h"

/*==============全局变量=================================*/

USART_TypeDef* printf_usart = NULL;  //选择哪一路串口作为printf输出
USART_TypeDef* scanf_usart  = NULL;  //选择哪一路串口作为scanf输入

/*==============指定printf输出口==========================*/

void ANSI_Set_OutPort(USART_TypeDef* USARTx)
{
	printf_usart=USARTx;
}

void ANSI_Set_InPort(USART_TypeDef* USARTx)
{
	scanf_usart=USARTx;
}

/*==============ANSI排版=================================*/
/**
 * @name:ANSI_CHANGE_COLOR
 * @brief:通过\033指令更改终端颜色
 * @param:ANSI_SHADE 字体是否带有阴影 @ref enum ANSI_SHADE
 * @param:ANSI_COLOR 字体颜色        @ref enum ANSI_COLOR
*/
void ANSI_CHANGE_COLOR(int ANSI_SHADE,int ANSI_COLOR)
{
	printf("\033[%d%dm",ANSI_SHADE,ANSI_COLOR);
}

/**
 * @name:ANSI_CLEAR_ALL
 * @brief:通过\033指令清屏
*/
void ANSI_CLEAR_ALL(void)
{
	printf("%c[2J\n\r",27);
}
/**
 * @name:ANSI_MOVE_CURSOR
 * @brief:通过\033指令更改光标位置,同时更改X和Y
*/
void ANSI_MOVE_CURSOR(int Line,int Colomn)
{
	printf("%c[%d;%dH",27,Line,Colomn);
}
/**
 * @name:ANSI_MOVE_CURSOR_COLOMN
 * @brief:通过\033指令更改光标位置,不更改Y,只更改X
*/
void ANSI_MOVE_CURSOR_COLOMN(int Colomn)
{
	printf("\033[%dG",Colomn);
}
/**
 * @name:ANSI_CLEAR_ALL
 * @brief:通过\033指令从光标位置开始清空屏幕
*/
void ANSI_CLEAR_CURSOR(void)
{
	printf("\033[K");
}

/*==============安全性检查=================================*/

uint8_t ANSI_Assert(void)
{
	if ((printf_usart!=NULL)&&(scanf_usart!=NULL)) return 1;
	else                                           return 0;
}

/*==============printf重定向===============================*/
//#ifdef  __CC_ARM
//#pragma import(__use_no_semihosting)
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;        
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
//#elif defined ( __GNUC__ ) || defined (__clang__)
//__asm (".global __use_no_semihosting\n\t");   
//#endif

//int fputc(int ch, FILE *f)
//{ 	
//	printf_usart->DR = (u8) ch;
//	while((printf_usart->SR&0X40)==0);
//	return ch;
//}

