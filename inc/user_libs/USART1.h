#ifndef USART1_LIBRARY
#define USART1_LIBRARY

//These are the Includes
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include <stdbool.h>

#define SEND_SERIAL_MSG Usart1_SendString
#define SEND_SERIAL_BYTE Usart1_Send

#define BAUD_9600 9600
#define BAUD_4800 4800
#define BAUD_115200 115200

//These are the prototypes for the routines
void Usart1_Init(int baudrate);
void Usart1_Send(uint8_t data);
uint16_t Usart1_Recieve(void);
void ConfigureUsart1Interrupt(void);
void Usart1_SendString(char* string);
char* Usart1_RecieveString(char* String);

#endif
