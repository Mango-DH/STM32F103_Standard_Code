/*=========================头文件===============================*/
#include "stm32f10x.h"           // Device header
#include <cmsis_compiler.h>
#include <stdio.h>
#include "RCS_ANSI.h"
#include "utility_bit.h"
#include "utility_linked_list.h"
#include "utility_stack.h"
#include "utility_queue.h"
#include "utility_gpio.h"
#include "utility_delay.h"
#include "utility_exit.h"
#include "utility_tim.h"
#include "utility_usart.h"
#include "drive_led.h"
#include "drive_key.h"

/*=========================宏定义===============================*/

#define DELAY_TIME   10
#define BUFFER_SIZE  128 

/*=========================全局变量=============================*/

uint16_t square_num = 0; //外部中断产生方波的个数
uint16_t tim2; 
uint8_t led_freq_change[10];
uint8_t len = 0;
uint8_t element;

extern uint8_t Serial_RxData;
extern uint8_t Serial_RxFlag;
extern uint8_t data_to_send[64];

Queue uartQueue;
uint8_t uartBuffer[BUFFER_SIZE];

Drv_LED_Handler LED_PB6;
Drv_LED_Handler LED_PB15;

Drv_KEY_Handler KEY_PA4;
Drv_KEY_Handler KEY_PA5;


Util_GPIO_Handler TX_Control_PA6;    //控制发送端
Util_GPIO_Handler TX_Signal_PB11;    //发送端数据
Util_GPIO_Handler RX_Signal_PB10;    //接收端数据，中断引脚
Util_GPIO_Handler RX_Control_PA7;    //控制接收端


float data[4] = {1.23, 4.56, 7.89, 0.12};
unsigned char tail[4] = {0x00,0x00,0x80,0x7f}; 

/*=========================业务逻辑=============================*/
int main() 
{	
    /*--------------------初始化----------------------------------------*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
		Drv_LED_Init(&LED_PB6,GPIOB,GPIO_Pin_6);
		Drv_LED_Init(&LED_PB15,GPIOB,GPIO_Pin_15);
		Drv_Key_Init(&KEY_PA4,GPIOA,GPIO_Pin_4);
		Drv_Key_Init(&KEY_PA5,GPIOA,GPIO_Pin_5);
	
		TIM_Init(10000,7200);   //定时1s
		TIM_NVIC_Init();
		
	  Serial_Init(9600);
		Queue_Init(&uartQueue, uartBuffer, sizeof(uint8_t), BUFFER_SIZE);

	
//		Util_GPIO_Send_Square(&TX_Control_PA6,GPIOA,GPIO_Pin_6,
//													&TX_Signal_PB11,GPIOB,GPIO_Pin_11,10,500,500); //阻塞发送方波，测试用	
//    My_Err_Type num = Util_GPIO_Exit_Init(&RX_Signal_PB10,GPIOB,GPIO_Pin_10); //开启中断，中断还没调好就是了

	
		/*----------------单次代码测试区-------------------------------------*/
		//Protocol_Rcv_Test(); //接收数据包解析测试
	
    /*--------------------轮询-------------------------------------------*/		
		while (1) 
		{	
			/*----------------循环代码测试区-----------------------------------*/
			
			//UART_Read_Bytes(&uartQueue); //队列接收发送数据		
				
			
			//定义的通信协议：包头0xEF【1】 数据长度【1】 数据【1】 校验和【1】	
			//加入传入的数据是1Hz     【EF 01 01 02】  Hex格式发送
			if (Serial_GetRxFlag())
			{		
				while(Queue_Dequeue(&uartQueue, &element))	
				{				
					Protocol_Rcv(element, led_freq_change, &len);
					//printf("0x%02X\n", element);  //Dequeued value
				}					
			}
			if(len>0)
			{
				len = 0;  //重置len的值方便下次接收 
        for (int i = 0; i < len; i++)printf("0x%02X ", led_freq_change[i]);   

				
				if(led_freq_change[0] && (led_freq_change[0]<10))
				{   //对要传入的参数限幅，计算得到的arr的值是期望频率*10
						TIM_SetAutoreload(TIM2, 10000/led_freq_change[0]);  // 设置新的ARR值
				}
				    
			}

	

// 发送第1通道的数据
Vofa_Wave_Print(data, 0);

// 发送第2通道的数据
Vofa_Wave_Print(data, 1);

// 发送第3通道的数据
Vofa_Wave_Print(data, 2);

// 发送第4通道的数据
Vofa_Wave_Print(data, 3);
    Serial_SendArray(tail, 4);
	
			
				Delay_ms(DELAY_TIME);					
    }
    return 0;
}


void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//在中断处理函数中，只需将接收到的数据存入队列，不要进行复杂的操作
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		Queue_Enqueue(&uartQueue, &Serial_RxData);  // 将数据放入队列
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  // 检查更新中断标志
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    // 清除更新中断标志
        tim2++;  //查看定时器是否工作
				Drv_LED_Toggle(&LED_PB6);
		}
}

//void EXTI15_10_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line10) == SET)
//	{  
//		square_num++;	
//		//square_num = Util_GPIO_Recv(&RX_Control_PA7,GPIOA ,GPIO_Pin_7);
//		EXTI_ClearITPendingBit(EXTI_Line10);
//	}
//}
