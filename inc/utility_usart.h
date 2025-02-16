#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "utility_queue.h"
#include "utility_error.h"
#include "utility_bit.h"


typedef enum {
    WAIT_FOR_HEADER,     // 等待包头0xEE
    WAIT_FOR_LENGTH,     // 等待数据包长度
    WAIT_FOR_DATA,       // 等待接收有效数据
    WAIT_FOR_CHECKSUM    // 等待接收校验和
} RcvState;


void Serial_Init(uint16_t bound);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);


void UART_Queue_Init(Queue* uartQueue, uint8_t* uartBuffer,uint8_t buffer_size) ;
size_t UART_Get_Received_Bytes_Count(Queue* uartQueue) ;
My_Err_Type UART_Read_Bytes(Queue* uartQueue); 
void Protocol_Rcv(uint8_t input, uint8_t* output, uint8_t* Len) ;
void Protocol_Rcv_Test();
void Vofa_Wave_Print(float* data_arr, uint8_t channel);


/*    //示例代码
	    ANSI_CHANGE_COLOR(9, 2);     //使用ANSI Escape转义码控制光标的位置、字符的颜色
			
		  Serial_SendByte(0x41);	
			uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
			Serial_SendArray(MyArray, 4);	
			Serial_SendString("\r\nNum1=");	
			Serial_SendNumber(111, 3);	
			printf("\r\nNum2=%d", 222);			
			char String[100];
			sprintf(String, "\r\nNum3=%d", 333);
			Serial_SendString(String);
	
			Serial_Printf("\r\nNum4=%d", 444);
			Serial_Printf("\r\n");
*/

#endif
