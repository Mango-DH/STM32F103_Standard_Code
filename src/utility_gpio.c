#include "utility_gpio.h"
static inline uint32_t My_GPIO_Get_RCC(GPIO_TypeDef* GPIOx);

/**
 * @name: Util_GPIO_Create
 * @brief:完成UTIL_GPIO对象成员的初始化+完成GPIO外设初始化
 * @param:Util_GPIO_Handler* gpio_handler         UTIL_GPIO对象指针
 * @param:Util_GPIOx GPIOx                        该对象使用哪一个GPIO Port, @def:UTIL_GPIO_Port
 * @param:Util_GPIO_Pin_x GPIO_Pin_x              该对象使用哪一个GPIO Pin , @def:UTIL_GPIO_Pin
 * @param:Util_GPIO_Mode Util_GPIO_Mode           该对象使用哪一种GPIO模式,   @def:Util_GPIO_Mode
 * @reval:MY_OK
 **/
My_Err_Type Util_GPIO_Create(Util_GPIO_Handler* gpio_handler, Util_GPIOx GPIOx,Util_GPIO_Pin_x GPIO_Pin_x, Util_GPIO_Mode Util_GPIO_Mode)
{
	GPIO_InitTypeDef conf;
	
	gpio_handler->GPIOx      = GPIOx;
	gpio_handler->GPIO_Pin_x = GPIO_Pin_x;
	gpio_handler->GPIO_Mode  = Util_GPIO_Mode;
	
	switch (Util_GPIO_Mode)
	{
		case Util_GPIO_Mode_PP:
			RCC_APB2PeriphClockCmd(My_GPIO_Get_RCC(GPIOx),ENABLE);
			conf.GPIO_Mode = GPIO_Mode_Out_PP;
			conf.GPIO_Pin  = GPIO_Pin_x;
			conf.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIOx,&conf);
		break;
		
		case Util_GPIO_Mode_OD:
			RCC_APB2PeriphClockCmd(My_GPIO_Get_RCC(GPIOx),ENABLE);
			conf.GPIO_Mode = GPIO_Mode_Out_OD;
			conf.GPIO_Pin  = GPIO_Pin_x;
			conf.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIOx,&conf);
		break;
		
		case Util_GPIO_Mode_IPU:
			RCC_APB2PeriphClockCmd(My_GPIO_Get_RCC(GPIOx),ENABLE);
			conf.GPIO_Mode = GPIO_Mode_IPU;
			conf.GPIO_Pin  = GPIO_Pin_x;
			conf.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIOx,&conf);
		break;
		
		case Util_GPIO_Mode_IN_FLOATING:
			RCC_APB2PeriphClockCmd(My_GPIO_Get_RCC(GPIOx),ENABLE);
			conf.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			conf.GPIO_Pin  = GPIO_Pin_x;
			conf.GPIO_Speed= GPIO_Speed_50MHz;
			GPIO_Init(GPIOx,&conf);
		break;
	}
	return MY_OK;
}

/**
 * @name: Util_GPIO_Output
 * @brief:控制GPIO对象的输出电平
 * @param:Util_GPIO_Handler* gpio_handler   对象指针
 * @param:output_level                      1高电平,0低电平
 * @reval:MY_OK
 **/
My_Err_Type Util_GPIO_Output(Util_GPIO_Handler* gpio_handler, uint8_t output_level)
{
	if(output_level)GPIO_SetBits(gpio_handler->GPIOx, gpio_handler->GPIO_Pin_x);
	else GPIO_ResetBits(gpio_handler->GPIOx, gpio_handler->GPIO_Pin_x);
	return MY_OK;
}


/**
 * @name: Util_GPIO_Input
 * @brief:读取GPIO对象的输出电平
 * @param:Util_GPIO_Handler* gpio_handler
 * @reval:1高电平,0低电平
 **/
bool Util_GPIO_Input(Util_GPIO_Handler* gpio_handler)
{
	return GPIO_ReadInputDataBit(gpio_handler->GPIOx,gpio_handler->GPIO_Pin_x);
}


/**
 * @name: Util_GPIO_Toggle
 * @brief:翻转GPIO对象的输出电平
 * @param:Util_GPIO_Handler* hdl UTIL_GPIO对象指针
 * @reval:MY_OK
 **/
My_Err_Type Util_GPIO_Toggle(Util_GPIO_Handler* gpio_handler)
{
	if (Util_GPIO_Input(gpio_handler)) 
	{
		Util_GPIO_Output(gpio_handler,0);
	}
	else
	{
		Util_GPIO_Output(gpio_handler,1);
	}
	return MY_OK;
}

/**
 * @name:  Util_GPIO_Send
 * @brief: 发送指定数量，指定高低电平的方波（延时实现）
 * @param: Util_GPIO_Handler* control_handler   控制发送信号的GPIO句柄
 * @param: Util_GPIOx clGPIOx                   control_handler句柄的端口     
 * @param: Util_GPIO_Pin_x clGPIO_Pin_x         control_handler句柄的号数  
 * @param: Util_GPIO_Handler* signal_handler    发送信号的GPIO句柄
 * @param: Util_GPIOx sgGPIOx                   signal_handler句柄的端口
 * @param: Util_GPIO_Pin_x sgGPIO_Pin_x         signal_handler句柄的端口号数
 * @param: uint8_t num    方波数量
 * @param: uint16_t high_time   高电平时间(ms)
 * @param: uint16_t low_time    低电平时间(ms)

 * @reval:MY_OK
 **/
My_Err_Type Util_GPIO_Send_Square(Util_GPIO_Handler* control_handler,Util_GPIOx clGPIOx,Util_GPIO_Pin_x clGPIO_Pin_x,
						Util_GPIO_Handler* signal_handler,Util_GPIOx sgGPIOx,Util_GPIO_Pin_x sgGPIO_Pin_x,
						uint8_t num,uint16_t high_time,uint16_t low_time)
{
	 	Util_GPIO_Create(control_handler,clGPIOx,clGPIO_Pin_x,Util_GPIO_Mode_PP);
		Util_GPIO_Create(signal_handler,sgGPIOx,sgGPIO_Pin_x,Util_GPIO_Mode_PP);		
	 	
	
   Util_GPIO_Output(control_handler, 1);    //开始发送
   for (uint8_t i = 0; i < num; i++) 
		{
		Util_GPIO_Output(signal_handler, 1);
     Delay_ms(high_time);        
		Util_GPIO_Output(signal_handler, 0);
     Delay_ms(low_time);   //发送结束后持续低电平
   }	
   Util_GPIO_Output(control_handler, 0);   //发送完毕，停止发送
	 return MY_OK;
}


/**
 * @name:  Util_GPIO_Recv
 * @brief: 在中断里接收方波信号并计数
 * @param: Util_GPIO_Handler* rx_control_handler   
 * @param: Util_GPIO_Handler* rx_signal_handler    控制接收信号的GPIO句柄
 * @reval: 接收到方波的数量
 **/

/*============静态函数定义==================*/
static inline uint32_t My_GPIO_Get_RCC(GPIO_TypeDef* GPIOx)
{
	if (GPIOx==GPIOA)
		return RCC_APB2Periph_GPIOA;
	else if (GPIOx==GPIOB)
		return RCC_APB2Periph_GPIOB;
	else if (GPIOx==GPIOC)
		return RCC_APB2Periph_GPIOC;
	else if (GPIOx==GPIOD)
		return RCC_APB2Periph_GPIOD;
	else if (GPIOx==GPIOE)
		return RCC_APB2Periph_GPIOE;
	else if (GPIOx==GPIOF)
		return RCC_APB2Periph_GPIOF;
	else if (GPIOx==GPIOG)
		return RCC_APB2Periph_GPIOG;
	else return 0;
}
