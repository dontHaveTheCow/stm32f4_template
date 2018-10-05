#include "my_eeprom.h"

void I2C_EE_ByteWrite(uint8_t val, uint16_t WriteAddr)
{


    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((WriteAddr/256) << 1), I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData(I2C_EE, (uint8_t)(WriteAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while(! I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C_EE, val);

        /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C_EE, ENABLE);

    //delay between write and read...not less 4ms
    delayMs(5);
}

void I2C_WritePage(uint16_t WriteAddr, uint8_t* buff){

    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((WriteAddr/256) << 1), I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the EEPROM's internal address to write to : MSB of the address first */
    I2C_SendData(I2C_EE, (uint8_t)WriteAddr & 0xFF);

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    uint8_t loopCounter = 0;
    while(loopCounter < 16){
        I2C_SendData(I2C_EE, *(buff+loopCounter));
            /* Test on EV8 and clear it */
        while (!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        loopCounter++;
    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C_EE, ENABLE);

    //delay between write and read...not less 4ms
    delayMs(5);
}
//*********************************************************************************
uint8_t I2C_EE_ByteRead( uint16_t ReadAddr)
{
    uint8_t tmp;

        /* While the bus is busy ~*/
    while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((ReadAddr/256) << 1), I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C_EE, (uint8_t)(ReadAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((ReadAddr / 256) << 1), I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE,I2C_EVENT_MASTER_BYTE_RECEIVED));//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    tmp=I2C_ReceiveData(I2C_EE);


    I2C_AcknowledgeConfig(I2C_EE, DISABLE);

    /* Send STOP Condition */
    I2C_GenerateSTOP(I2C_EE, ENABLE);

    return tmp;
}

void I2C_ReadPage(uint16_t ReadAddr, uint8_t* buff){

    /* While the bus is busy ~*/
    while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((ReadAddr / 256) << 1), I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));


    // /* Send the EEPROM's internal address to read from: MSB of the address first */
    // I2C_SendData(I2C_EE, (uint8_t)((ReadAddr & 0xFF00) >> 8));

    // /* Test on EV8 and clear it */
    // while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send the EEPROM's internal address to read from: LSB of the address */
    I2C_SendData(I2C_EE, (uint8_t)(ReadAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    // Is this EEPROM_HW_ADDRESS + 1 necessary
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((ReadAddr/256) << 1), I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE,I2C_EVENT_MASTER_BYTE_RECEIVED));//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    uint8_t loopCounter = 0;
    while(loopCounter < 16){
        *(buff+loopCounter)=I2C_ReceiveData(I2C_EE);
        while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_RECEIVED));
        loopCounter++;
    }

    I2C_AcknowledgeConfig(I2C_EE, DISABLE);
    /* Send STOP Condition */
    I2C_GenerateSTOP(I2C_EE, ENABLE);
}

void I2C_ReadEE(uint16_t StartReadAddr, uint8_t* buff){

     /* While the bus is busy ~*/
    while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((StartReadAddr / 256) << 1), I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the EEPROM's internal address to read from: LSB of the address */
    I2C_SendData(I2C_EE, (uint8_t)(StartReadAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    // Is this EEPROM_HW_ADDRESS + 1 necessary
    I2C_Send7bitAddress(I2C_EE, EEPROM_HW_ADDRESS | ((StartReadAddr/256) << 1), I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_EE,I2C_EVENT_MASTER_BYTE_RECEIVED));//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    uint16_t loopCounter = 0;

    while(loopCounter < 2048){
        // if(loopCounter % 16 == 0){
        //     Usart1_SendString("\r\n");
        //     Usart1_Send((loopCounter/16)/100 + 0x30);
        //     Usart1_Send(((loopCounter/16)%100)/10 + 0x30);
        //     Usart1_Send((loopCounter/16)%10 + 0x30);
        //     Usart1_Send(' ');
        // }
        //*(buff+loopCounter)=I2C_ReceiveData(I2C_EE);
        Usart1_Send(I2C_ReceiveData(I2C_EE));
        //Usart1_Send(' ');
        while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_RECEIVED));
        loopCounter++;
    }

    I2C_AcknowledgeConfig(I2C_EE, DISABLE);
    /* Send STOP Condition */
    I2C_GenerateSTOP(I2C_EE, ENABLE);
}

void eeprom_WP_init(){
		
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable clock on APB2 pripheral bus where button and LEDs are connected
	RCC_AHB1PeriphClockCmd(WPCLK, ENABLE);
	//select pins to initialize LED
	GPIO_InitStructure.GPIO_Pin = WPPIN;
	//select output push-pull mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	//highest speed available
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WPPORT, &GPIO_InitStructure);
}

