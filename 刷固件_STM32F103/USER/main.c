/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/ 
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_flash.h"
#include "iap.h"
#include "FirmwareUpdate.h"
int fputc(int ch,FILE *f)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, ch);
	return(ch);	   
}

void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�������0
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//���ô����ж� �������ݽ����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��

}

void USART_Configuration()
	{
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure; 
		//ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		//GPIO����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//��������
			USART_DeInit(USART1);
			USART_InitStructure.USART_BaudRate = 115200; 
			USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
			USART_InitStructure.USART_StopBits = USART_StopBits_1; 
			USART_InitStructure.USART_Parity = USART_Parity_No; 
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
			USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
			USART_Init(USART1, &USART_InitStructure);
		//ʹ��
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			USART_Cmd(USART1, ENABLE);	
	}

/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/


int32_t bytecount = 0,basetime = 0;
char tmpData[4] = {0};
uint8_t Page_Count = 0,i = 0,data;
QUEUE_T* pro_queue;
char Program_Buffer[5120] = {0};
int main(void)
{	
	NVIC_Configuration();
	USART_Configuration();
		
	#ifdef PRINT_ERR
		printf("bootloader start.......\r\n");
	#else
		printf("ok");
	#endif
	while(1){
		if(Firmware_Info->State == 0 ){
			printf("jump");
			iap_load_app(FLASH_APP_ADDR);
		}
		if(Firmware_Info->State == 5){//���ݹ̼���С����flashҳ
		
			Page_Count = Firmware_Info->Firmware_Size/0x400+1;
			
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
			for(i = 0;i < Page_Count;i++){
				FLASH_ErasePage(FLASH_APP_ADDR+(i*0x400));
			}
			Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("ҳ�������:%d,��һ����������\r\n",i);
				#else
				printf("ok");
				#endif
		}
	
		if(1)//�Ӷ�����ȡ������д��flash
		{
			Firmware_Info->Firmware_Real_Sum += data;//У����ۼӼ���
			tmpData[bytecount%4] = data;
			bytecount++;
			if(bytecount%4==0 && bytecount!=0){//4�ֽ�д��
				FLASH_ProgramWord(FLASH_APP_ADDR+(bytecount-4),
				tmpData[0]|tmpData[1]<<8|tmpData[2]<<16|tmpData[3]<<24);
			}
			
			/*if(bytecount>8450)
				printf("data:%d\r\n",bytecount);*/
			if(bytecount == Firmware_Info->Firmware_Size){
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("���ݽ������,��һ��׼��У���\r\n");
				#endif
			}
		}
		if(Firmware_Info->State == 7){
			
			FLASH_Lock();
			/*У���*/
			/*if(Firmware_Info->Firmware_Real_Sum == Firmware_Info->Firmware_Check_Sum){
				#ifdef PRINT_ERR
				printf("У��ͳɹ�׼����ת��%d\r\n",bytecount);
				#endif
				iap_load_app(FLASH_APP_ADDR);
			}else{
				#ifdef PRINT_ERR
				printf("У���ʧ��\r\n");
				#else
				printf("error3");
				#endif
			}*/
			printf("jump");
			iap_load_app(FLASH_APP_ADDR);
				
			
		}
	}

}





