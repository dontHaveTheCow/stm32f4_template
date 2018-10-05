#ifndef GPIOINOUT_H_
#define GPIOINOUT_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"

#define LEDG GPIO_Pin_12
#define LEDO GPIO_Pin_13
#define LEDR GPIO_Pin_14
#define LEDB GPIO_Pin_15
#define LEDPORT GPIOD
#define LEDPORTCLK RCC_AHB1ENR_GPIODEN
#define BUTTON GPIO_Pin_0
#define BUTTONPORT GPIOA
#define BUTTONPORTCLK RCC_AHB1ENR_GPIOAEN
#define DELAY 900000

//These are the prototypes for the routines
void initializeLeds(void);
void initializeButton(void);
void init_i2s_gpio_clocks(void);

#endif /* GPIOINOUT_H_ */
