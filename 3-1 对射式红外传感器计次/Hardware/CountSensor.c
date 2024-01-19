#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//打开GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//打开AFIO的时钟
	//NVIC和EXTI时钟默认开启
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//配置GPIOB8端口
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);  //AFIO外部中断引脚选择，选择AFIO8端口
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);//配置AFIO8端口接到EXTI
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置NVIC的优先级
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);//配置EXTI9-5端口接到NVIC
	
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8) == SET )//因为9-5通道都可以进来，故要判断是不是想要的8通道进来
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 1)//下降沿时为0，上升沿时为1，双边沿时为0|1
		{
			CountSensor_Count++;
			//中断程序结束后，一定要再调用一下清除中断标志位的函数，
			//只要中断标志位置1，程序就会跳转到中断函数
			//如果不清除中断标志位，就会一直申请中断，
			//这样程序就会不断响应中断，执行中断函数，程序就会卡死在中断函数中
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
	}
}

