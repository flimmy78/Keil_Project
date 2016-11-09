#include "stm32f0xx.h"
extern void I2C_GPIO_Config(void);
extern void DATA_printf(uint8_t *s,short temp_data);
extern void I2C_delay(uint16_t count);
extern uint8_t I2C_Start(void);
extern void I2C_Stop(void); 
extern void I2C_Ack(void); 
extern void I2C_NoAck(void);
extern uint8_t I2C_WaitAck(void); 	 //����Ϊ:=1��ACK,=0��ACK
extern void I2C_SendByte(uint8_t SendByte); //���ݴӸ�λ����λ//
extern uint8_t I2C_RadeByte(void);  //���ݴӸ�λ����λ//
//���ֽ�д��*******************************************
uint8_t Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
//���ֽ�д��*******************************************
uint8_t Mult_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t len,uint8_t* REG_data);
//���ֽڶ�ȡ*****************************************
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
//���ֽڶ�ȡ*****************************************
uint8_t Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t len,uint8_t* buff);
