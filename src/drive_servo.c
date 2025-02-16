#include "drive_servo.h"
/**
 * @brief 初始化伺服电机驱动程序并配置PWM用于伺服控制。
 *
 * 该函数通过配置指定的定时器和GPIO以产生PWM（脉宽调制）信号，从而初始化伺服电机驱动程序。
 * 它使用给定的自动重载寄存器（ARR）和预分频器（PSC）值初始化定时器，设置PWM通道，
 * 并配置GPIO引脚以输出PWM信号。同时，它还设置伺服电机的初始角度。
 *
 * @param handler       指向 Drv_Servo_Handler 结构体的指针，该结构体包含伺服驱动程序的配置信息
 *                      在初始化过程中，该结构体将被填充必要的数据
 * @param TIMx          表示用于生成PWM信号的定时器外设
 * @param TIM_Channel_x 用于PWM输出的定时器通道。应对应于指定定时器的可用通道之一（如 TIM_CHANNEL_1）。
 * @param GPIOx         表示用于PWM输出的GPIO端口。
 * @param GPIO_Pin      配置为PWM输出的GPIO引脚
 * @param max_angle      电机转角的最大值
 * @param angle         伺服电机的初始角度。该值应在伺服电机的操作范围内（例如，典型伺服电机的范围为0到180度）
 *
 * @return Drv_Servo_Err 返回一个错误代码，指示初始化的结果。可能的错误代码在 Drv_Servo_Err 枚举中定义。
 */
Drv_Servo_Err Drv_Servo_Init(Drv_Servo_Handler* handler, TIM_TypeDef* TIMx, uint32_t TIM_Channel_x,
GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t max_angle, float angle)
{
    if (handler == NULL || TIMx == NULL || GPIOx == NULL)
    {
        return SERVO_ERR_INPUT;
    }

    // 初始化舵机控制结构体的基本参数
    handler->TIMx = TIMx;
    handler->TIM_Channel_x = TIM_Channel_x;
    handler->GPIOx = GPIOx;
    handler->GPIO_Pin = GPIO_Pin;
		handler->max_angle = max_angle;  
		handler->angle = angle;

		 if (GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
		
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);

    // 定时器配置
		 if (TIMx == TIM1) {
			 RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);       //TIM1是高级定时器挂载在APB2总线上
    } else if (TIMx == TIM2) {
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    } else if (TIMx == TIM3) {
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	
    } else if (TIMx == TIM4) {
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
    }				
		
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;           // PWM周期,20ms (50Hz)
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;           // 预分频器，72MHz主频，分频到1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

		
    // PWM通道配置
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    //选择的是PWM1模式一
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;     
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    switch (TIM_Channel_x)
    {
        case TIM_Channel_1:
            TIM_OC1Init(TIMx, &TIM_OCInitStructure);
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_2:
            TIM_OC2Init(TIMx, &TIM_OCInitStructure);
            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_3:
            TIM_OC3Init(TIMx, &TIM_OCInitStructure);
            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_4:
            TIM_OC4Init(TIMx, &TIM_OCInitStructure);
            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        default:
            return SERVO_ERR_INPUT;
    }

    // 启动定时器
    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE);

    return SERVO_OK;
}

/**
* @name:  Drv_Servo_Set_Pos
* @brief: 设置舵机的目标角度，并通过PWM信号控制舵机转动到该角度。
* @param: handler - 舵机控制结构体的指针，用于保存配置和状态。
* @param: angle - 目标角度，范围应在 min_angle 和 max_angle 之间。
* @reval: 返回舵机角度设置的状态，成功时返回 SERVO_OK，否则返回错误代码。
*/
Drv_Servo_Err Drv_Servo_Set_Pos(Drv_Servo_Handler* handler, TIM_TypeDef* TIMx,uint32_t TIM_Channel_x, float angle)
{
	switch (TIM_Channel_x)   //180度舵机，最常用，设置位置
    {
			case TIM_Channel_1:
			TIM_SetCompare1(TIMx, angle / 180 * 2000 + 500);
			case TIM_Channel_2:
			TIM_SetCompare1(TIMx, angle / 180 * 2000 + 500);
			case TIM_Channel_3:
			TIM_SetCompare1(TIMx, angle / 180 * 2000 + 500);			
			case TIM_Channel_4:
			TIM_SetCompare1(TIMx, angle / 180 * 2000 + 500);			
		}
    return SERVO_OK;		
}

//195----------------正向最大转速；
//185----------------速度为0；
//175----------------反向最大转速；

/**
* @name:  Drv_Servo_Set_Spd
* @brief: 设置舵机的旋转速度，影响舵机从当前位置到目标位置的移动时间。
* @param: handler - 舵机控制结构体的指针，用于保存配置和状态。
* @param: spd    -100 ~ 100
* @reval: 返回舵机速度设置的状态，成功时返回 SERVO_OK，否则返回错误代码。
*/
Drv_Servo_Err Drv_Servo_Set_Spd(Drv_Servo_Handler* handler, TIM_TypeDef* TIMx,uint32_t TIM_Channel_x, uint16_t spd)
{
	switch (TIM_Channel_x)   //180度舵机，最常用，设置位置
    {
			case TIM_Channel_1:
			TIM_SetCompare1(TIMx, (int)spd/10.0+185);
			case TIM_Channel_2:
			TIM_SetCompare1(TIMx, (int)spd/10.0+185);
			case TIM_Channel_3:
			TIM_SetCompare1(TIMx, (int)spd/10.0+185);			
			case TIM_Channel_4:
			TIM_SetCompare1(TIMx, (int)spd/10.0+185);			
		}
    return SERVO_OK;		
}
