#include "stm32f10x.h"                  // Device header


void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//使用模拟信号输入的专属模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//配置规则组的输入通道
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	//初始化ADC
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET/*获取复位校准的状态*/);
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET/*获取校准的状态*/);
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//触发ADC1转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//获取标志位状态
	return ADC_GetConversionValue(ADC1);//获取转换的结果
}
