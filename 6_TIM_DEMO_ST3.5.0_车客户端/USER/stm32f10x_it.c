/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h> 
#include <Driver.h> 
#include <stm32f10x_it.h>
#include <stm32f10x_tim.h>
#include <mpu6050_driver.h> 
//#include <stm32f10x_wwdg.h>
#include <stm32f10x_exti.h>
#include <NRF24L01_Driver.h>
#include <Control_Driver.h>
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}
/*
********************************************************************************
** �������� �� WWDG_IRQHandler(void)
** �������� �� ������ǰ�����ж�
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/ 
// void WWDG_IRQHandler(void)
// {
//   /* Update WWDG counter */
//   WWDG_SetCounter(0x50);
// 	
//   /* Clear EWI flag */
//   WWDG_ClearFlag(); 
// }
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
		/**********************************************************
** ������: EXTI1_IRQHandler
** ���ܣ��ⲿ�ж���1�жϷ������
** ���룺��
** �������
** ���أ���
***********************************************************/
void EXTI4_IRQHandler(void)
{ 
	extern char* runState;
	u8 status;
	u8 Rx_Buf[32];
	//printf("\nnrf�����ж�");
	if(EXTI_GetITStatus(EXTI_Line4) != RESET )
	{
		status=nRF24L01_RxPacket(Rx_Buf);//�õ�״̬����������
			//printf(" status=%x\n",status);
		if(equals("ʵʱͬ��",Rx_Buf,8)){
			runState = "ʵʱͬ��";
		}
		if(status!=0xff)// ���յ�����
		{ 
			if(status==0x48){
				RF_Write_Reg(NRF_WRITE_REG+STATUS,0XFF); //�����־ 010010000 Rx_DR���������ж�Ϊ1 û�б����
			}
			if(status==Ch4)//�ж�ͨ��4�Ƿ�������
			{
				//printf(" ch4=%s",Rx_Buf);
				//printf("����\n");
// 				TX_Mode();
// 				nRF24L01_TxPacket("synchronization");
// 				RX_Mode(4);
				if(Rx_Buf[4]==0){
					USART1_Handle(Rx_Buf);	
				}
				
				
			}
		}
		//printf("inEXT"):
		
		//NVIC_DisableIRQ(EXTI3_IRQn); //�ر�EXTI_Line3�ж�
		//irq_flag=1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);//����жϱ�־λ
	
}
/**********************************************************
** ������: TIM2_IRQHandler
** ��������: ��ʱ��2�ĸ����жϷ������
** �������: ��
** �������: ��
***********************************************************/
void TIM2_IRQHandler(void)
{
	//count+=1000;
	//USART_SendBreak(USART1);
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		//if(count>=9000){
		//	count=0;
		//}
//		TIM_SetCompare1(TIM2, count);
		//���жϱ�ʶ
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//��������жϱ�־λ
		//if(count>=1){
		//	count = 0;
		//printf("\n����TIM2�ж�");
		//}
	//	printf("%s",data);
		//printf("\nUSART1�жϽ��������%d",sum);
		//GPIOB->ODR^=GPIO_Pin_0;	//��PB0��ƽ����	
	}
}
/**********************************************************
** ������: TIM6_IRQHandler
** ��������: ��ʱ��6�ĸ����жϷ������
** �������: ��
** �������: ��
***********************************************************/
u8 checkFlag = 0;
extern char* runState;
void TIM6_IRQHandler(void)
{
	if(!(checkFlag++<100)){//ÿ100��ִ��һ�� ÿ��3��
		checkFlag=0;
		if(equals((u8*)"ͬ��",(u8*)runState,4)){
			//USART1_SendString("���չ���������",14);
			//asyncPrint("���չ���������");
			runState="";
		}else{
			//EXTI_ClearITPendingBit(EXTI_Line4);//����жϱ�־λ
			//USART1_SendData(0x11);
			//USART1_SendData(RF_Read_Reg(STATUS)); //��ȡ״̬�Ĵ�����ֵ
			RF_Write_Reg(NRF_WRITE_REG+STATUS,0XFF); //�����־ 010010000 Rx_DR���������ж�Ϊ1 û�б����
//			RX_Mode(4);
			//USART1_SendString(,14);
			//asyncPrint("���չ���ʧ�ܣ�");
		}
	}

	Build_Data();
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//��������жϱ�־λ
}
u8 tim3_flag;
void TIM3_IRQHandler(void)
{
	USART1_SendData('%');
	if(tim3_flag == 1){
		tim3_flag = 0;
		TIM_SetCompare1(TIM2,0);
  	TIM_SetCompare2(TIM2,0);
	}
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//��������жϱ�־λ
}
// u16 usart1_str=0;
// u8 is16 = 0;
// u8 RxData = 0;
u8 data[32];
int i=0;
u16 resData=0;
void USART1_IRQHandler(void){
	u16 temData = 0;
	//USART_ReceiveData(USART1);
	//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
	//if(USART1->SR & USART_FLAG_RXNE){
		
		temData =USART_ReceiveData(USART1) ;
		//USART1_SendData(temData);
	
		i++;
		if(i==4){
			data[i-1]=temData;
			//nRF24L01_TxPacket_send(data);
			USART1_Handle(data);
			i=0;
		}else{
			data[i-1]=temData;
		}

	USART_ClearFlag(USART1,USART_FLAG_RXNE); 
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//USART1_SendData(sum);
	if(USART_GetFlagStatus(USART1,USART_FLAG_CTS)==SET){
		printf("CTS��־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_LBD  )==SET){
		printf("LIN�жϼ���־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==SET){
		//printf("�������ݼĴ����ձ�־λ");
		//USART_ClearFlag(USART1,USART_FLAG_TXE); 
		//USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_TC)==SET){
		printf("������ɱ�־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE)==SET){
		printf("�������߱�־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET){
		printf("��������־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_NE)==SET){
		printf("���������־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_FE)==SET){
		printf("֡�����־λ");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_PE)==SET){
		printf("��ż�����־λ");
	}
	
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/




		   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
