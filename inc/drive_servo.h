#ifndef __SERVO_H
#define __SERVO_H

#define SERVO_DEADBAND 2

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

typedef struct 
{
	TIM_TypeDef* TIMx;           //控制舵机的定时器
	uint32_t TIM_Channel_x;             //定时器的通道
	GPIO_TypeDef* GPIOx;      //舵机连接的GPIO端口
	uint16_t GPIO_Pin;            //舵机连接的GPIO引脚
	uint16_t max_angle;           //舵机的最大转角
	float angle;                  //舵机转动的角度
}Drv_Servo_Handler; 


typedef enum servo_err
{
	SERVO_OK=0,                   //正常
	SERVO_ERR_INPUT =1,           //输入参数有误
}Drv_Servo_Err;



Drv_Servo_Err Drv_Servo_Init(Drv_Servo_Handler* handler, TIM_TypeDef* TIMx, uint32_t TIM_Channel_x,
GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t max_angle, float angle);

Drv_Servo_Err Drv_Servo_Set_Pos(Drv_Servo_Handler* handler, TIM_TypeDef* TIMx,uint32_t TIM_Channel_x, float angle);
Drv_Servo_Err Drv_Servo_Set_Spd(Drv_Servo_Handler* handler, TIM_TypeDef* TIMx,uint32_t TIM_Channel_x, uint16_t spd);

#endif
