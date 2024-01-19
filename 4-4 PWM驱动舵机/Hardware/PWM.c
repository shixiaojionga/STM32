#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//要用复用推挽输出才能把引脚的控制权交给片上外设
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//初始化内部时钟TIM2_CH2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//这里需要查找引脚定义，哪个引脚可以复用到另一个内置外设
	
	TIM_InternalClockConfig(TIM2);//调用内部时钟，不过默认就是内部时钟，所以可以不写
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//初始化时基单元
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);//自动给其他几个用不到的成员配上默认初始值
	//TIM_OCStructInit用法即先给所有成员赋默认值，再手动更改自己想改的成员的值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//通用计时器只需要配置这几个，其他的在用高级计时器时才能用到
	//PWM频率   f=CK_PSC/(PSC+1)/(ARR+1)
	//PWM占空比 d=CCR/(ARR+1)
	//PWM分辨率 r=1/(ARR+1)
	TIM_Cmd(TIM2, ENABLE);//外设的新状态
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);//设置CCR的值
}
