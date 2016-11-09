/*--------------------------------------------------------------------------------------
*  @file     Driver.c
*  @author   ���ſƼ� lin
*  @version  base on stm32f10x   
*  @date     2013.11.08
*  @brief    none
---------------------------------------------------------------------------------------*/
/* ��׼C����� */
#include <string.h>
#include <stdio.h>
#include <stddef.h> 
/* ��չ�� */
#include <stm32f10x_systick.h> 
#include <stm32f10x_spi.h> 
#include <stm32f10x_gpio.h> 
#include <Driver.h>
#include <stm32f10x_rcc.h>
//����һ��BITΪ1    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x0001,  SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	 SETBIT3 = 0x0008,
	SETBIT4 = 0x0010,	 SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	 SETBIT7 = 0x0080,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//����һ��BIT   Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//ѡ��һ��BIT  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

/* INCLUDES */


/*-------------------------------------------------------------------------------------------------------
*  �ڲ�����								 
-------------------------------------------------------------------------------------------------------*/
#define	PRINTF_USE_USART 			USART1

/*-------------------------------------------------------------------------------------------------------
*  ��Դ����											 
-------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------
*  ��������												 
-------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------
*  ִ�д���													 
-------------------------------------------------------------------------------------------------------*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++  ƽ̨���� +++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/********************************************************************************************************
*  Function Name  : USART1_Configuration					                                                           
*  Object					: USART1����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��                                     
********************************************************************************************************/
// void USART1_Configuration(u32 BaudRate)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	USART_InitTypeDef USART_InitStructure; 
//   /* Configure USARTx_Tx as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//   /* Configure USARTx_Rx as input floating */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
// 	
// 	USART_InitStructure.USART_BaudRate = BaudRate; 
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
// 	USART_InitStructure.USART_Parity = USART_Parity_No; 
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
// 	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
// 	USART_Init(USART1, &USART_InitStructure);
// 	USART_Cmd(USART1, ENABLE);
// }
/********************************************************************************************************
*  Function Name  : fputc						                                                           
*  Object					: �ض������C�⣨stdio��printf����  �ļ�������������USART1 ��PRINTF_USE_USART�������ƣ�              
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��                                     
********************************************************************************************************/
int fputc(int ch,FILE *f)
{
	//����ǰ������־λ������USART_FLAG_TC��
	USART_ClearFlag(PRINTF_USE_USART, USART_FLAG_TC);
	//ch�͸�USARTx
	USART_SendData(USART1, ch);
	//�ȴ��������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	//����ch
	return(ch);	   
}
/*
*   fputc end
*******************************************************************************/ 
/********************************************************************************************************
*  Function Name  : equals						                                                           
*  Object					: �ַ����Ƚ�             
*  ����						�� Դ�ַ�����Ŀ���ַ���������
*  ���						�� ��								                         	                                     
*  ��ע						�� ��                                     
********************************************************************************************************/
u8 equals(u8* source,u8* target,u8 length){
	while(length--){
		if(!(*source++==*target++))
			return 0;
	}
	return 1;
}
/*
*   equals end
*******************************************************************************/ 
/********************************************************************************************************
*  Function Name  : Delay_Ms						                                                           
*  Object					: ��ʱ����            
*  ����						�� xMS
*  ���						�� ��								                         	                                     
*  ��ע						�� ������ʱ                                    
********************************************************************************************************/
void Delay_MS(u32 dly)
{
	FlagStatus Status;
	SysTick_CounterCmd(SysTick_Counter_Clear);
	SysTick_SetReload(9000);   //9000/9Mhz
	SysTick_CounterCmd(SysTick_Counter_Enable);
	while(dly--){
		Status = RESET;
    while(Status == RESET){
			Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
		}
	}
	SysTick_CounterCmd(SysTick_Counter_Disable);
}
/********************************************************************************************************
*  Function Name  : Delay_US						                                                           
*  Object					: ��ʱ����            
*  ����						�� xUS
*  ���						�� ��								                         	                                     
*  ��ע						�� ������ʱ                                    
********************************************************************************************************/
void Delay_US(u32 dly)
{
	FlagStatus Status;
	SysTick_CounterCmd(SysTick_Counter_Clear);
	SysTick_SetReload(9);   //9000/9Mhz
	SysTick_CounterCmd(SysTick_Counter_Enable);
	while(dly--){
		Status = RESET;
    while(Status == RESET){
			Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
		}
	}
	SysTick_CounterCmd(SysTick_Counter_Disable);
}
/*
*  Delay_Ms	end                                  
********************************************************************************************************/
/********************************************************************************************************
*  Function Name  : USART1_SendData						                                                           
*  Object					: USART1����            
*  ����						�� ���͵�����
*  ���						�� ��								                         	                                     
*  ��ע						��                                  
********************************************************************************************************/
void  USART1_SendData(u8 SendData)
{
	USART_SendData(USART1, SendData);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
}
/*
*  USART1_SendData	end                                  
********************************************************************************************************/
/********************************************************************************************************
*  Function Name  : SPI1_Configuration						                                                           
*  Object					: ��ʼ��SPI����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� PA5- SPISCK;  PA6- SPIMISO; PA7- SPIMOSI;                                    
********************************************************************************************************/
// void SPI1_Configuration(void)
// {
// 		GPIO_InitTypeDef GPIO_InitStructure;
// 		SPI_InitTypeDef SPI_InitStructure;
// 		//��ʼ��SPI IO ʹ��PA5 PA6 PA7
// 		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
// 		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
// 		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 		GPIO_Init(GPIOA, &GPIO_InitStructure);
// 		//����SPIʱ��
// 		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
// 		SPI_Cmd(SPI2, DISABLE);
// 		//SPIģʽ����
// 		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI����Ϊ˫��˫��ȫ˫�� ��
// 		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// ����Ϊ��SPI ��
// 		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI���ͽ���8λ֡�ṹ ��
// 		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// ʱ�����յ� ��
// 		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// ���ݲ����ڵ�һ��ʱ���� ��
// 		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// �ڲ�NSS�ź���SSIλ���ƣ�������ƣ�
// 		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;// ������Ԥ��ƵֵΪ2 ��
// 		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ���ݴ����MSBλ��ʼ ��
// 		SPI_InitStructure.SPI_CRCPolynomial = 7;//��������CRCֵ����Ķ���ʽ��
// 		//SPI_I2S_DeInit(SPI1);
// 		SPI_Init(SPI1, &SPI_InitStructure);
// 		
// 		//SPI_SSOutputCmd(SPI1,ENABLE);
// 		//ʹ��SPI
// 		SPI_Cmd(SPI1, ENABLE);
// }

//��SPIx����1���ֽ�
u8 SPIx_Send(SPI_TypeDef* SPIx,u8 Buff)
{
		//�ȴ�SPI���У������ͻ������ձ�־λ��
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		//����1���ֽ�
		SPI_I2S_SendData(SPIx, Buff);
		//�����ջ������ǿձ�־λ
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		return (u8)SPI_I2S_ReceiveData(SPIx);
}
//��SPI����1���ֽ�
u8 SPIx_Receive(SPI_TypeDef* SPIx)
{
		//�ȴ�SPI���У������ͻ������ձ�־λ��
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); 
		//���Ϳ������ƶ����豸��������
		SPI_I2S_SendData(SPIx, 0x00); 
		//�����ջ������ǿձ�־λ
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		//������
		return (u8)SPI_I2S_ReceiveData(SPIx);
}
/*
* 	SPI end
*******************************************************************************/
