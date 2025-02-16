#include "drive_key.h"
/**
* @name:  Drv_Key_Init
* @brief: 初始化指定的按键引脚，配置GPIO为浮空输入模式
* @param: key_handle    按键控制结构体的指针，用于保存GPIO配置
* @param: GPIOx 	      指定按键连接的GPIO端口（例如GPIOA
* @param: GPIO_Pin  		指定按键连接的GPIO引脚（例如GPIO_Pin_4）
* @reval: MY_OK 				操作成功
*         MY_ERR_INVALID_PARAM 	 无效参数错误
*/
My_Err_Type Drv_Key_Init(Drv_KEY_Handler* key_handler, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
    if (key_handler == NULL || GPIOx == NULL) 
		{
        return MY_ERR_INVALID_PARAM;
    }
		Util_GPIO_Create(&(key_handler->GPIO_Key_Handler), GPIOx, GPIO_Pin_x, Util_GPIO_Mode_IN_FLOATING);
    return MY_OK;
}


/**
* @name:  Drv_Key_Get_State
* @brief: 获取按键的当前状态并保存到按键句柄中
* @param: key_handle - 按键控制结构体的指针，用于保存GPIO配置
* @reval: MY_OK 				操作成功
*         MY_ERR_INVALID_PARAM 	 无效参数错误
*/
My_Err_Type Drv_Key_Get_State(Drv_KEY_Handler* key_handler)
{
    if (key_handler == NULL || key_handler->GPIO_Key_Handler.GPIOx == NULL) {
        return MY_ERR_INVALID_PARAM;
    }
		key_handler->state = (bool)Util_GPIO_Input(&(key_handler->GPIO_Key_Handler));
    return MY_OK;
}


/**
* @name:  Drv_Key_Proc
* @brief: 处理按键状态变化，检测到按键按下时，触发LED状态切换
* @param: key_handle - 按键控制结构体的指针，用于保存GPIO配置
* @reval: MY_OK 				操作成功
*         MY_ERR_INVALID_PARAM 	 无效参数错误
*/
My_Err_Type Drv_Key_Proc(Drv_KEY_Handler* key_handler)
{
    if (key_handler == NULL) {
        return MY_ERR_INVALID_PARAM;
    }

    if (key_handler->state && !key_handler->last_state) 
    {  
        key_handler->led_is_toggle = 1;
    }
    key_handler->last_state = key_handler->state; // 更新上一次的状态
    return MY_OK;
}



