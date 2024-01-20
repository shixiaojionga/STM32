#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//外设寄存器起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设寄存器数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//外设寄存器是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//存储器起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器是否自增
	DMA_InitStructure.DMA_BufferSize = Size;//传输计数器
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//硬件触发或软件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//传输模式，是否使用自动重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, DISABLE);//若用ENABLE，则初始化之后立刻开始转运
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//判断转运有没有完成，如果没有完成则一直锁在循环中不能进行下一步
	
	DMA_ClearFlag(DMA1_FLAG_TC1);//清除标志位
}
	
