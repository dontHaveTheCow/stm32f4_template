#ifndef MY_EEPROM_
#define MY_EEPROM_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_i2c.h"
#include "SysTickDelay.h"
#include "USART1.h"

#define WPPORT GPIOB
#define WPPIN GPIO_Pin_15
#define WPCLK RCC_AHB1ENR_GPIOBEN

#define I2C_EE I2C2
#define EEPROM_HW_ADDRESS 0xA0
#define EEPROM_PAGE_SIZE 16

//#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

void eeprom_WP_init(void);

void I2C_EE_ByteWrite(uint8_t val, uint16_t WriteAddr);
void I2C_WritePage(uint16_t ReadAddr, uint8_t* buff);

uint8_t I2C_EE_ByteRead(uint16_t ReadAddr);
void I2C_ReadPage(uint16_t ReadAddr, uint8_t* buff);
void I2C_ReadEE(uint16_t StartReadAddr, uint8_t* buff);


#endif