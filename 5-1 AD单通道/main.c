#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t AD_Value;
float Voltage;

int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowChar(2, 2, '.');
	while(1)
	{
		AD_Value = AD_GetValue();
		OLED_ShowNum(1, 1, AD_Value, 4);
		Voltage = (float) AD_Value * 3.3 / 4095; 
		OLED_ShowNum(2, 1, Voltage, 1);
		OLED_ShowNum(2, 3, (uint16_t)(Voltage * 100) % 100, 2);
		Delay_ms(100);
	}
}
