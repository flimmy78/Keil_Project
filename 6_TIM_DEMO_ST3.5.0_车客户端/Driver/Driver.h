#ifndef __Driver__
#define __Driver__
/*--------------------------------------------------------------------------------------
*  @file     Driver.h
*  @author   ���ſƼ� lin
*  @version  base on stm32f10x   
*  @date     2013.11.08
*  @brief    none
---------------------------------------------------------------------------------------*/
#include <stm32f10x.h> 
/*-------------------------------------------------------------------------------------------------------
*  ��������					 
-------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------
*  ����ӿ�	ΪӦ�ò��ṩ�Ľӿ�										 
-------------------------------------------------------------------------------------------------------*/
extern char printString[1];
extern void Delay_MS(u32 dly);
extern void Delay_US(u32 dly);
extern void  USART1_SendData(u8 SendData);
extern void  USART1_SendString(char* SendData,u8 length);
extern u8 equals(u8* source,u8* target,u8 length);
extern void SPI1_Configuration(void);
extern u8 SPIx_Send(SPI_TypeDef* SPIx,u8 Buff);
extern u8 SPIx_Receive(SPI_TypeDef* SPIx);
extern void asyncPrint(char* str);
#endif



