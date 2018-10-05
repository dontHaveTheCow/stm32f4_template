#include "USART1.h"

void Usart1_Init(int baudrate){


// Enable clock for GPIOA
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

GPIO_InitTypeDef     GPIO_InitStruct; 
/**
* Tell pins PA9 and PA10 which alternating function you will use
* @important Make sure, these lines are before pins configuration!
*/
GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
// Initialize pins as alternating function
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_Init(GPIOB, &GPIO_InitStruct);
/**
 * Enable clock for USART1 peripheral
 */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

USART_InitTypeDef USART_InitStruct;
 
USART_InitStruct.USART_BaudRate = baudrate;
USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
USART_InitStruct.USART_Parity = USART_Parity_No;
USART_InitStruct.USART_StopBits = USART_StopBits_1;
USART_InitStruct.USART_WordLength = USART_WordLength_8b;
USART_Init(USART1, &USART_InitStruct);
USART_Cmd(USART1, ENABLE);

}

void Usart1_Send(uint8_t data)
{
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC ) == RESET)
	{
	}
}

void Usart1_SendString(char* string){
	while(*string != 0){
		Usart1_Send(*string++);
	}
}

char* Usart1_RecieveString(char* String){
	while(*String != '\r'){
		while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
		*String++ = USART_ReceiveData(USART1);
	}
	return String;
}

uint16_t Usart1_Recieve(void)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART1);
}

void ConfigureUsart1Interrupt(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;

	  /* Enable the USART1 Interrupt */
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

