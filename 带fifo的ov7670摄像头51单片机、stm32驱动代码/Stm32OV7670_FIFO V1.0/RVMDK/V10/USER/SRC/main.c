/*******************************************************************************
*  Copyright (C) 2010 - All Rights Reserved
*		
* �������:	��ɼ���
* ��Ȩ����: ��ɵ���	
* ��������:	2012��8��18�� 
* �����ʷ:	2012��8��18���װ�
* Version:  1.0 
* Demo �Ա���ַ��http://store.taobao.com/shop/view_shop.htm?asker=wangwang&shop_nick=qifeidianzi
**********************************************************************************************************************************************
����������������������������������������������������������������������������������������������������������������������������������������������
����������������������һ������������������������������������������������������������һ����������������������������һһһһһһһ��������������
������һһһһ��һһһһһһ������������һһһһһһһһ�������������������ą�����һһ������һ������������������һһһһ����һһһ������������
������������һ������һ��һ������������һһһһ�ą���һ����һһ������������һһһһһһһһһһһ������������������������һһһһ��������������
�������ą���һ��������������������������������������һ��һһ��������������һһ����һһ������һһ������������������������һһ������������������
������һ����һһһһһһһһһһһ������������������һһ������������������һһһһһһһһһһ������������һһһһһһһһһһһһһһһ������
������һһһһһ����һһ��һ����������������������һһһһһ��������������һһ����һһ������һ������������һһ����������һһ������һһ��������
��������������һ��һ��һ��һ������������������������һ����һһ������������һһһһһһһһһһ��������������������������һһ������������������
����һһһ��������һһһ��һ������������������������һһ����������������������������һ��������������һ������������������һһ������������������
������������һ����������һһ��������������������������һһһһһһ������������������һһһһһһһһһ������������������һһ������������������
��������һһһ������һһһһ����������������������������һһһһһ��������������������һһһһһһһ��������������һһһһ��������������������
����������������������������������������������������������������������������������������������������������������������������������������������
**********************************************************************************************************************************************/

#include <stm32f10x.h>
#include <stdio.h>
#include <Nvic_Exit.h>
#include <delay.h>
#include <tft.h>
#include <ov7670.h>
#include<I2C.h>

typedef enum {TRUE = 0, FALSE = !TRUE} bool; 

void RCC_Configuration(void);
void IO_Init(void);
void CLK_generate(void);

main()
{ 
	RCC_Configuration();;//ϵͳʱ������
	delay_init(72);		//��ʱ��ʼ��
	GPIOA->ODR ^= (1 <<3);						//LED1��˸һ��	   
	LCD_Init();
	IO_Init(); 
	LCD_Clear(BLUE);       //���� 
	POINT_COLOR=YELLOW;
	BACK_COLOR=BLUE;
   	LCD_ShowString(20,50,"QF Welcom");
	LCD_ShowString(20,100,"STM32 Camera");
	LCD_ShowString(20,150,"OV7670 Initing......");
	InitI2C0();
	while(1!=Cmos7670_init());   //CMOS��ʼ��	
	Exit_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_IPU, EXTI_Trigger_Falling, 2, 5);	//VSYNC	 	

  	while(1) 
	{		  		

	}
}

/*******************************************************************************
* Function Name  : RCC_Configuration 
* Description    : RCC select(external 8MHz)
* Input          : NO
* Output         : NO
* Return         : NO
*******************************************************************************/
void RCC_Configuration(void)
{ 

  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);   
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)	  
  {
  
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
    RCC_PCLK2Config(RCC_HCLK_Div1);		   
	RCC_PCLK1Config(RCC_HCLK_Div2);	 	  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	
    RCC_PLLCmd(ENABLE); 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	  
       {
       }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
    while(RCC_GetSYSCLKSource() != 0x08)	  
       { 
       }
     }

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 
}

void CLK_generate(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;
 	RCC_ClocksTypeDef RCC_Clocks;   	
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP ; 
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure1);  	
	RCC_GetClocksFreq(&RCC_Clocks);  
	RCC_MCOConfig(RCC_MCO_HSE);   

}

void IO_Init(void)
{

  RCC->APB2ENR|=1<<0; 
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<3;
 	RCC->APB2ENR|=1<<4;

  GPIOA->CRH=0X33333333;
	GPIOA->CRL=0X33333333;
	GPIOA->ODR=0XFFFF;	 

	GPIOB->CRL=0X88888888;
	GPIOB->CRH=0X33333333;
	GPIOA->ODR=0XFFFF;

  GPIOC->CRH=0X33333333;
	GPIOC->CRL=0X33333333; 	
	GPIOC->ODR=0XFFFF;	
		    
   	JTAG_Set(JTAG_SWD_DISABLE);	 //JTAG���ܽ�ֹ������JTAG�˿�
}
