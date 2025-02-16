#include "utility_exit.h"

/**
* @name:  Util_GPIO_Exit_Init 
* @brief: 为某GPIO端口配置exit中断
* @param: None           
* @reval: MY_OK
*/
My_Err_Type Util_GPIO_Exit_Init(Util_GPIO_Handler* exit_handler,
															Util_GPIOx GPIOx, Util_GPIO_Pin_x GPIO_Pin_x)
{


//	Util_GPIO_Create(exit_handler,GPIOx,GPIO_Pin_x,Util_GPIO_Mode_IPU);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   //中断的引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	return MY_OK;

}



//在中断里调用函数
uint16_t Util_GPIO_Recv(Util_GPIO_Handler* control_handler,Util_GPIOx clGPIOx,Util_GPIO_Pin_x clGPIO_Pin_x)
{
	uint16_t square_num;
	Util_GPIO_Create(control_handler,clGPIOx,clGPIO_Pin_x,Util_GPIO_Mode_IN_FLOATING);
	if(Util_GPIO_Input(control_handler))
	{
		square_num++;
	}
	return square_num;
}
