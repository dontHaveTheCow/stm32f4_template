#include "gpioInOut.h"

void initializeLeds(){

GPIO_InitTypeDef GPIO_InitStructure;
//Enable clock on APB2 pripheral bus where button and LEDs are connected
RCC_AHB1PeriphClockCmd(LEDPORTCLK, ENABLE);
//select pins to initialize LED
GPIO_InitStructure.GPIO_Pin = LEDG|LEDB|LEDR|LEDO;
//select output push-pull mode
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//highest speed available
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(LEDPORT, &GPIO_InitStructure);

}

void initializeButton(void){

GPIO_InitTypeDef GPIO_InitStructure;
//Enable clock on APB2 pripheral bus where button and LEDs are connected
RCC_AHB1PeriphClockCmd(BUTTONPORTCLK, ENABLE);
//select pins to initialize LED
GPIO_InitStructure.GPIO_Pin = BUTTON;
//select output push-pull mode
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//highest speed available
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(BUTTONPORT, &GPIO_InitStructure);

RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE);
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

EXTI_InitTypeDef EXTI_InitStructure;
EXTI_InitStructure.EXTI_Line= EXTI_Line0;
EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd=ENABLE;
EXTI_Init(&EXTI_InitStructure);

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
NVIC_Init(&NVIC_InitStructure);

}

void init_i2s_gpio_clocks(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA
							 | RCC_AHB1Periph_GPIOB
							 | RCC_AHB1Periph_GPIOC
							 | RCC_AHB1Periph_GPIOD, ENABLE);

}
