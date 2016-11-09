/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      ���ſƼ� http://euse.taobao.com  --------------------------
 *
 */
#include "Prohead.h"
#include "Driver.h"
//#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
/**********************************************************
** ������: EXTI_Config
** ��������:  �ⲿ�ж�����
** �������: ��
** �������: ��
***********************************************************/
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);	//PA1��Ϊ�ⲿ�ж���1����
	EXTI_ClearITPendingBit(EXTI_Line1);	//���1�߱�־λ
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//���ش���
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 //�������ϲ�����ʼ���ṹ��

// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);	//PA8��Ϊ�ⲿ�ж���8����
// 	EXTI_ClearITPendingBit(EXTI_Line8);	//���8�߱�־λ
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�����ش���
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);

// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);	//PA11��Ϊ�ⲿ�ж���11����
// 	EXTI_ClearITPendingBit(EXTI_Line11);	//���11�߱�־λ
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�����ش���
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
}
/**********************************************************
** ������: NVIC_Config
** ��������: �ж����ȼ�����������
** �������: ��
** �������: ��
***********************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�������2	 
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�����ⲿ�ж�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
// 	//�ж���5��9����һ���ж�EXTI9_5_IRQn
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�����ⲿ�ж�9_5
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�����Ϊ1
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
// 	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��

// 	////�ж���10��15����һ���ж�EXTI15_10_IRQn											
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�����ⲿ�ж�15_10
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ1
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	  //�����ȼ�����Ϊ0
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);  

}

void EXTI_config(void){
	
}
/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
main()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	Driver_MCU_Init();
	SysTick_Configuration();
	USART1_Configuration(9600);
	EXTI_Config();
	NVIC_Config();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	while(1){
		
		//GPIO_WriteBit(GPIOB, GPIO_Pin_10,!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10));
		
		//GPIO_SetBits();	
		//printf("\n"+!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10));
		
		//GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		//printf("\n0");
		
		EXTI_GenerateSWInterrupt(EXTI_Line1);//�������һ���жϣ��ж���8
		Delay_MS(1000);
		//Delay_MS(1000);
	}
		//��ʼ��ϵͳʱ��
	/*
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}*/
}





