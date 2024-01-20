#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//使用模拟信号输入的专属模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//把通道0（复用到PA0）接到序列1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);//把通道1（复用到PA1）接到序列2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);//把通道2（复用到PA2）接到序列3
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);//把通道3（复用到PA3）接到序列4
	
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//外设寄存器起始地址，这里为ADC_DR的地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设寄存器数据宽度，要比DMA1的少16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设寄存器不自增，因为读的是ADC_DR的地址，它只有一个
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//存储器起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器是否自增
	DMA_InitStructure.DMA_BufferSize = 4;//传输计数器
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//硬件触发或软件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//传输模式，是否使用自动重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	

	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//单次转换或者连续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//是否用扫描模式
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//DMA一定要在ADC之前开启使能
	DMA_Cmd(DMA1_Channel1, ENABLE);//若用ENABLE，则初始化之后立刻开始转运
	ADC_DMACmd(ADC1, ENABLE);//将ADC与DMA1连接起来
	ADC_Cmd(ADC1, ENABLE);
	
	//初始化ADC
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET/*获取复位校准的状态*/);
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET/*获取校准的状态*/);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//触发ADC1转换
}


