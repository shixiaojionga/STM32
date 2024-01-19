#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	while(1)
	{
		GPIO_Write(GPIOA, ~GPIO_Pin_0);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_1);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_2);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_3);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_4);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_5);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_6);
		Delay_ms(500);

		GPIO_Write(GPIOA, ~GPIO_Pin_7);
		Delay_ms(500);

	}
}