#include "utility_tim.h"

void TIM_Init(uint16_t arr, uint16_t psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 启用TIM2时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;           // 设置自动重装载寄存器值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;        // 设置预分频器值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       // 初始化TIM2
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);            // 使能TIM2更新中断
    TIM_Cmd(TIM2, ENABLE);                                // 使能TIM2
}

void TIM_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   // 配置优先级分组

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   // 设置TIM2中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // 使能TIM2中断通道
    NVIC_Init(&NVIC_InitStructure);                   // 初始化NVIC
}





