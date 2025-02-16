#ifndef __MY_GPIO_H
#define __MY_GPIO_H

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stdbool.h>

#include "utility_error.h"
#include "utility_delay.h"

typedef GPIO_TypeDef*  Util_GPIOx;       /** @arg:GPIOx,其中x可以是A~G   **/
typedef uint16_t       Util_GPIO_Pin_x;  /** @arg:GPIO_Pin_x,其中x可以是0~15 **/


typedef enum util_gpio_mode{
	Util_GPIO_Mode_PP,
	Util_GPIO_Mode_OD,
	Util_GPIO_Mode_IPU,
	Util_GPIO_Mode_IN_FLOATING,
}Util_GPIO_Mode;

typedef struct util_gpio_handler{
	GPIO_TypeDef* GPIOx;
	uint16_t      GPIO_Pin_x;
	uint8_t       GPIO_Mode;
}Util_GPIO_Handler;


/*============导出函数==============================================================*/

My_Err_Type Util_GPIO_Create(Util_GPIO_Handler* gpio_handler, Util_GPIOx GPIOx,Util_GPIO_Pin_x GPIO_Pin_x, Util_GPIO_Mode Util_GPIO_Mode);
My_Err_Type Util_GPIO_Output(Util_GPIO_Handler* gpio_handler, uint8_t output_level);
bool Util_GPIO_Input(Util_GPIO_Handler* gpio_handler);
My_Err_Type Util_GPIO_Toggle(Util_GPIO_Handler* gpio_handler);
My_Err_Type Util_GPIO_Send_Square(Util_GPIO_Handler* control_handler,Util_GPIOx clGPIOx,Util_GPIO_Pin_x clGPIO_Pin_x,
						Util_GPIO_Handler* signal_handler,Util_GPIOx sgGPIOx,Util_GPIO_Pin_x sgGPIO_Pin_x,
						uint8_t num,uint16_t high_time,uint16_t low_time);

#endif
