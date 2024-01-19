#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)//将舵机角度转变函数的参数由CRR转变为角度，即输入角度就可以直接得到角度
{
	PWM_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle / 180 *2000 + 500);
}
