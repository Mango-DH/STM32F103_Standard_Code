#include "drive_led.h"
/**
* @name:  Drv_LED_Init
* @brief: 初始化指定的LED引脚，配置GPIO为推挽输出模式并将其设为低电平
* @param: led_handle - LED控制结构体的指针，用于保存GPIO配置
* @param: GPIOx - 指定LED连接的GPIO端口（例如GPIOB）
* @param: GPIO_Pin_x - 指定LED连接的GPIO引脚（例如GPIO_Pin_6）
* @reval: None
*/
My_Err_Type Drv_LED_Init(Drv_LED_Handler* led_handler, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	  Util_GPIO_Create(&(led_handler->GPIO_LED_Handler), GPIOx, GPIO_Pin_x, Util_GPIO_Mode_PP);
	return MY_OK;
}



/**
 * @name: Drv_LED_Proc
 * @brief:控制GPIO对象的输出电平
 * @param:Util_GPIO_Handler* hdl UTIL_GPIO对象指针
 * @param:output_level           1高电平,0低电平
 **/
My_Err_Type Drv_LED_Proc(Drv_LED_Handler* led_handler,uint8_t output_level)
{
	  Util_GPIO_Output(&(led_handler->GPIO_LED_Handler), output_level);
		return MY_OK;
}



/**
* @name:  Drv_LED_Toggle
* @brief: 切换指定的LED引脚的状态（若当前亮则熄灭，若当前灭则点亮）
* @param: led_handle - LED控制结构体的指针，用于保存GPIO配置
* @reval: None
*/
My_Err_Type Drv_LED_Toggle(Drv_LED_Handler* led_handler)
{
	 Util_GPIO_Toggle(&(led_handler->GPIO_LED_Handler));	
	return MY_OK;
}
