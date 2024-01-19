#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "Wang Shenglong,");
	OLED_ShowString(2, 1, "good good study,");
	OLED_ShowString(3, 1, "day day up!");
	while(1)
	{
		
	}
}
