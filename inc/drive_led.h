#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "utility_gpio.h"

typedef struct 
{
	 Util_GPIO_Handler  GPIO_LED_Handler;    //对应的GPIO对象
}Drv_LED_Handler;


My_Err_Type Drv_LED_Init(Drv_LED_Handler* led_handler, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
My_Err_Type Drv_LED_Proc(Drv_LED_Handler* led_handler,uint8_t output_level);
My_Err_Type Drv_LED_Toggle(Drv_LED_Handler* led_handler);


#endif
