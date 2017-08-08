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
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_flash.h"
#include "iap.h"
#include "FirmwareUpdate.h"
#include "tool.h"
int fputc(int ch,FILE *f)
{
		
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	return(ch);	   
}

void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void USART_Configuration()
	{
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure; 
		//ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,	ENABLE); 
			RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_GPIOB, 	ENABLE);  
		//GPIO����
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		//��������

			USART_InitStructure.USART_BaudRate = 115200;				 			//9600������
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//����λΪ8λ
			USART_InitStructure.USART_StopBits = USART_StopBits_1;				//ֹͣλΪ1
			USART_InitStructure.USART_Parity = USART_Parity_No;					//û����żУ��λ
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//Ӳ��������ʧ�� ???
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//����ʹ�ܽ���ʹ��
			USART_Init(USART1, &USART_InitStructure);				//��ʼ��USART1
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
uint8_t plan = 0;
char Program_Buffer[4096] = {0};
u8 isJump = 0;
int main(void) 
{	
	BaseClock_Init();
	NVIC_Configuration();
	USART_Configuration();
	pro_queue = Queue_Init(Program_Buffer,sizeof(char),4096);
	basetime = microsecond;
	#ifdef PRINT_ERR
		printf("bootloader start.......\r\n");
	#else
		printf("ok");
	#endif
	while(1){
		if(isJump || (Firmware_Info->State < 2 && Elapse_Us(microsecond,basetime)>5000000)){ 
			printf("jump"); 
			DelayMS(10);
			iap_load_app(FLASH_APP_ADDR);
		}
		if(Firmware_Info->State == 7){//���ݹ̼���С����flashҳ
			basetime = microsecond;	
			Page_Count = Firmware_Info->Firmware_Size/0x400+1;
			
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);
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
	
		if(Queue_Get(pro_queue,&data) == 0)//�Ӷ�����ȡ������д��flash
		{
			Firmware_Info->Firmware_Real_Sum += data;//У����ۼӼ���
			tmpData[bytecount%4] = data;
			bytecount++;
			if(bytecount%4==0 && bytecount!=0){//4�ֽ�д��
				FLASH_ProgramWord(FLASH_APP_ADDR+(bytecount-4),
				tmpData[0]|tmpData[1]<<8|tmpData[2]<<16|tmpData[3]<<24);
			}
			
			
				if(bytecount>=(Firmware_Info->Firmware_Size*plan/100)){
					#ifdef PRINT_ERR
					printf("%d%%\r\n",plan);
					
					#endif
					plan += 10;
				}
//			if(plan> 80){
//					#ifdef PRINT_ERR
//						printf("%d\r\n",bytecount); 
//					#endif
//			}  
			if(bytecount == Firmware_Info->Firmware_Size){
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("���ݽ������,��һ��׼��У���\r\n");
				#endif
			}
		}
		if(Firmware_Info->State == 9){ 
			FLASH_Lock();
			/*У���*/
			if(Firmware_Info->Firmware_Real_Sum == Firmware_Info->Firmware_Check_Sum){
				#ifdef PRINT_ERR
				printf("У��ͳɹ�׼����ת��%d\r\n",bytecount);
				#endif
			}else{
				#ifdef PRINT_ERR
				printf("У���ʧ��\r\n");
				#else
				printf("error3");
				#endif
				return 0;
			}
			DelayMS(10);
			printf("ok");
			iap_load_app(FLASH_APP_ADDR);
		}
	}

}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

