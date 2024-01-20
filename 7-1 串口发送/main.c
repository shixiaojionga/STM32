#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "stdio.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	//Serial_SendByte('A');
//	uint8_t My_Array[] = {0x42, 0x43, 0x44, 0x45};
//	Serial_SendArray(My_Array, 4);
	//Serial_SendString("Hello World!");
	//Serial_SendNumber(12345, 5);
	//printf("Num=%d\r\n", 666);
	Serial_Printf("aaa");
	while(1)
	{
		
	}
}
