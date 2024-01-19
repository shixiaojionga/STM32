#include "stm32f10x.h"                  // Device header


void Timer_Init(void)//配置定时中断
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//TIM_InternalClockConfig(TIM2);//使用内部时钟，如果要使用内部时钟的话可以不写
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);//配置外部时钟
	//第一个参数使用通过ETR引脚的外部时钟2
	//第二个参数选择指定的外部触发预分频器
	//第三个参数是外部触发的极性，Inverted即下降沿或低电平有效，NonInverted即上升或高电平沿有效
	//最后一个参数为滤波次数,如果用0x00时计数器会乱跳，则要调为0x0F
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;//ARR，自动重装，范围在0-65535
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;//PSC，预分频，范围在0-65535
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器，只有高级定时器才有
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//配置时基单元
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//手动清除更新中断标志位，否则计数时会直接从1开始，而不是从0开始
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//开启更新中断到NVIC的通路
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//定时器2（或其他）在NVIC中的通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);//启动定时器
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}

/*中断函数可以直接放到别的文件里去使用
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}*/
