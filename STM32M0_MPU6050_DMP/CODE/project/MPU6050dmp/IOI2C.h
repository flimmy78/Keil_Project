#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f0xx.h"
//#include "sys.h"
#define   uint8_t  uint8_t

#define SCL_H         GPIOB->BSRR = GPIO_Pin_3
#define SCL_L         GPIOB->BRR  = GPIO_Pin_3 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_4
#define SDA_L         GPIOB->BRR  = GPIO_Pin_4

#define READ_SDA      GPIOB->IDR  & GPIO_Pin_4
   	   		   
//IO��������
#define SDA_IN()  {;}//GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {;}//GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}

//IO��������	 
#define IIC_SCL    //PBout(3) //SCL
#define IIC_SDA    //PBout(4) //SDA	 
//#define READ_SDA   //PBin(4)  //����SDA 
extern int liyao;
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
int IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(uint8_t ack);//IIC��ȡһ���ֽ�
int IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 
uint8_t I2C_Readkey(uint8_t I2C_Addr);

uint8_t I2C_ReadOneByte(uint8_t I2C_Addr,uint8_t addr);
uint8_t IICwriteByte(uint8_t dev, uint8_t reg, uint8_t data);
uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
uint8_t IICwriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


void get_ms(unsigned long *count);
#endif

//------------------End of File----------------------------
