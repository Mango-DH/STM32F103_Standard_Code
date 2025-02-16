#ifndef __EXIT_H
#define __EXIT_H

#include "stm32f10x.h"                  // Device header
#include "utility_error.h"
#include "utility_gpio.h"


My_Err_Type Util_GPIO_Exit_Init(Util_GPIO_Handler* exit_handler,
															Util_GPIOx GPIOx, Util_GPIO_Pin_x GPIO_Pin_x);

uint16_t Util_GPIO_Recv(Util_GPIO_Handler* control_handler,Util_GPIOx clGPIOx,Util_GPIO_Pin_x clGPIO_Pin_x);

#endif
