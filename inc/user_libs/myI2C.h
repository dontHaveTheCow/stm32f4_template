#ifndef MYI2C_H_
#define MYI2C_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_i2c.h"

//These are the prototypes for the routines
void init_I2C2(void);
void I2C_init(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);

#endif /* MYI2C_H_ */
