#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"                  // Device header
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <math.h>

//static int Util_PWM_Find_Solution_Simular(UTIL_TIM_PWM_Handler* hdl,uint32_t pwm_freq,float duty_deadzone);
//static int Util_PWM_Find_Solution_FreqFirst(UTIL_TIM_PWM_Handler* hdl,uint32_t pwm_freq,float duty_deadzone);
//static int Util_PWM_Find_Solution_DutyFirst(UTIL_TIM_PWM_Handler* hdl,uint32_t pwm_freq,float duty_deadzone);
//static int Util_Isr_Find_Solution(UTIL_TIM_Isr_Handler* hdl,uint32_t timer_us);
//static void Util_RCC_Enable_TIM(Util_TIM_Handler TIMx);

void TIM_Init(uint16_t arr, uint16_t psc);

void TIM_NVIC_Init(void);

#endif
