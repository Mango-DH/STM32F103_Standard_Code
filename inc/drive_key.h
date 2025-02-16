#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include <stdio.h>
#include "utility_gpio.h"

typedef struct 
{
	  Util_GPIO_Handler  GPIO_Key_Handler;    //按键对应的GPIO对象
    bool state;         // 当前按键状态
    bool last_state;    // 上一次按键状态
    bool led_is_toggle; // 是否触发LED切换
}Drv_KEY_Handler;


My_Err_Type Drv_Key_Init(Drv_KEY_Handler* key_handler, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
My_Err_Type Drv_Key_Get_State(Drv_KEY_Handler* key_handler);
My_Err_Type Drv_Key_Proc(Drv_KEY_Handler* key_handler);


/*      //实例代码
				Drv_Key_Get_State(&KEY_PA4); // 持续获取按键状态
				Drv_Key_Get_State(&KEY_PA5); // 持续获取按键状态
			
				Drv_Key_Proc(&KEY_PA4); 
				Drv_Key_Proc(&KEY_PA5); 

				if(KEY_PA4.led_is_toggle)
				{
					Drv_LED_Toggle(&LED_PB6);
					KEY_PA4.led_is_toggle = 0;
				}
			
				if(KEY_PA5.led_is_toggle)
				{
					Drv_LED_Toggle(&LED_PB15);
					KEY_PA5.led_is_toggle = 0;
				}
			Drv_LED_Toggle(&LED_PB6);
			Drv_LED_Toggle(&LED_PB15);
*/

#endif
