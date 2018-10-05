#include "SysTickDelay.h"


void initialiseSysTick(void){
	RCC_ClocksTypeDef RCC_clocks;
	RCC_GetClocksFreq(&RCC_clocks);
	/* For 1 us delay, we need to divide with 4M */
	SysTick_Config(RCC_clocks.HCLK_Frequency/1000);
}

void delayMs(int ms){
	sysTickCounter = ms;
	while (sysTickCounter != 0)
	{
	}
}

void SysTick_Handler(void)
{
	if(sysTickCounter != 0)
	sysTickCounter--;
}
