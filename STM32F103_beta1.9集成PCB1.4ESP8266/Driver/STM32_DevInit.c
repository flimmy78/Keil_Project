#include <stdio.h>
#include "STM32_DevInit.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++  ƽ̨���� +++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/********************************************************************************************************
*  Function Name  : GPIO_Configuration					                                                           
*  Object					: GPIO����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                                 
********************************************************************************************************/
#ifdef __STM32F10x_GPIO_H
	void GPIO_Configuration(){
		//ʱ��
		//GPIO����
		//��������
		//ʹ��
		GPIO_InitTypeDef GPIO_InitStructure;
		// LED PB10 || PD2
		/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);*/
		
		//LED1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//LED2
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOA, &GPIO_InitStructure); 
		    
		Motor_GPIO_Configuration();//�������GPIO��ʼ��
		//printf("GPIO_Configuration DONE!\r\n");
		
		OLED_GPIO_Configuration(); 
#ifdef __ESP8266_H__		
		ESP8266_GPIO_Configuration();
#endif		
	}
#endif

/********************************************************************************************************
*  Function Name  : SYSTICK_Configuration					                                                           
*  Object					: SysTick����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                                 
********************************************************************************************************/
#ifdef __STM32F10x_SYSTICK_H
	void SYSTICK_Configuration(){
		/*//ʱ��
			SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //SysTick_CLKSource_HCLK_Div8
		//����
			SysTick_CounterCmd(SysTick_Counter_Clear);
		SysTick_SetReload(72000);   //9000/9Mhz
		//ʹ��
		
		//SysTick_CounterCmd(SysTick_Counter_Disable);
		SysTick_ITConfig(DISABLE); 
		
		
		//SysTick_ITConfig(ENABLE); 
		//SysTick_CounterCmd(SysTick_Counter_Enable);*/
		//printf("SYSTICK_Configuration DONE!\r\n");	
	}
#endif
	
/********************************************************************************************************
*  Function Name  : USART1_Configuration					                                                           
*  Object					: USART1����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                                
********************************************************************************************************/
#ifdef __STM32F10x_USART_H
	void USART_Configuration()
	{
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure; 
		//ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
		
		//GPIO����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin =		GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_11;
      GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			GPIO_InitStructure.GPIO_Pin =		GPIO_Pin_12;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_2;
      GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOD, &GPIO_InitStructure);

		//�������� 
			USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
			USART_InitStructure.USART_StopBits = USART_StopBits_1; 
			USART_InitStructure.USART_Parity = USART_Parity_No; 
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
			USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
			
			USART_InitStructure.USART_BaudRate = 115200; 
			USART_DeInit(USART1);
			USART_Init(USART1, &USART_InitStructure);
			
			USART_InitStructure.USART_BaudRate = 9600; 
			USART_DeInit(USART2);
			USART_Init(USART2, &USART_InitStructure);
			
			USART_InitStructure.USART_BaudRate = 115200; 
			USART_DeInit(USART3);
			USART_Init(USART3, &USART_InitStructure);
			
			USART_InitStructure.USART_BaudRate = 115200; 
			USART_DeInit(UART5);
			USART_Init(UART5, &USART_InitStructure);
			
		//ʹ��
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			USART_ClearITPendingBit(USART3, USART_IT_RXNE);
			USART_ClearITPendingBit(UART5, USART_IT_RXNE);
			
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
			USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);			

			USART_Cmd(USART1, ENABLE);
			USART_Cmd(USART2, ENABLE);
			USART_Cmd(USART3, ENABLE);
			USART_Cmd(UART5, ENABLE);
			#ifdef __MPU6050_H_
				MPU6050_USART_Configuration();
			#endif
	}	
#endif

/********************************************************************************************************
*  Function Name  : EXTI_Configuration					                                                           
*  Object					: EXTI����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                                   
********************************************************************************************************/
#ifdef __STM32F10x_EXTI_H
	void EXTI_Configuration(){
		EXTI_InitTypeDef EXTI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		//ʱ��
		//GPIO����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		//��������
		
		//NRF_IRQ
		EXTI_ClearITPendingBit(EXTI_Line8);	//����߱�־λ
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);	//PA1��Ϊ�ⲿ�ж�������
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//���ش���
		EXTI_InitStructure.EXTI_Line = EXTI_Line8;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 //�������ϲ�����ʼ���ṹ��
		/*GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		//ʱ��
		
		//GPIO����
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
		//��������
			EXTI_DeInit();
			EXTI_InitStructure.EXTI_Line = EXTI_Line4;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
			EXTI_InitStructure.EXTI_Line = EXTI_Line5;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);
		//ʹ��*/

			

	}
#endif
	
/********************************************************************************************************
*  Function Name  : ADC_Configuration					                                                           
*  Object					: ADC����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/

#ifdef __STM32F10x_ADC_H
	void ADC_Configuration(){
		//ʱ��
		//GPIO����
		//��������
		//ʹ��
		printf("ADC_Configuration DONE!\r\n");	

	}
#endif
	
/********************************************************************************************************
*  Function Name  : DMA_Configuration					                                                           
*  Object					: DMA����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_DMA_H
	void DMA_Configuration(){
		DMA_InitTypeDef DMA_InitStructure;//����DMA��ʼ���ṹ��
		//ʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	    
		//��������
	 	DMA_DeInit(DMA2_Channel3);//��λDMAͨ��1
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART4->DR; //���������ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)_Uart4_Buff; //���建������ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//��ַΪ�����ַ���ڲ�
		DMA_InitStructure.DMA_BufferSize = RECV_BUFSIZE_UART4;//��������С 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ǰ��������ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//˫�ֽ�
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //˫�ֽ�
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ�
		DMA_Init(DMA2_Channel3, &DMA_InitStructure);//��ʼ��DMAͨ��
		DMA_ClearFlag(DMA2_FLAG_TC3);
		DMA_Cmd(DMA2_Channel3, ENABLE); //ʹ��DMAͨ��
		DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);            // ????DMA????
	}
#endif
	
/********************************************************************************************************
*  Function Name  : I2C_Configuration					                                                           
*  Object					: I2C����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_I2C_H
	void I2C_Configuration(){
		//ʱ��
		//GPIO����
		//��������
		//ʹ��
		printf("I2C_Configuration DONE!\r\n");	
	}
#endif
		
/********************************************************************************************************
*  Function Name  : SPI_Configuration					                                                           
*  Object					: SPI����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_SPI_H
	void SPI_Configuration(){
		//OLED_SPI_Configuration();
			GPIO_InitTypeDef GPIO_InitStructure;
			SPI_InitTypeDef SPI_InitStructure;
		//ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		//GPIO����
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//��������
			SPI_Init(SPI1, &SPI_InitStructure);
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI����Ϊ˫��˫��ȫ˫�� ��
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// ����Ϊ��SPI ��
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI���ͽ���8λ֡�ṹ ��
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// ʱ�����յ� ��
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// ���ݲ����ڵ�һ��ʱ���� ��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// �ڲ�NSS�ź���SSIλ���ƣ�������ƣ�
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;// ������Ԥ��ƵֵΪ2 ��
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ���ݴ����MSBλ��ʼ ��
			SPI_InitStructure.SPI_CRCPolynomial = 7;//��������CRCֵ����Ķ���ʽ��
			SPI_Init(SPI1, &SPI_InitStructure);
		//ʹ��
			SPI_Cmd(SPI1, ENABLE);
			
			/*
		//ʱ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		//GPIO����
			GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		//��������
			SPI_I2S_DeInit(SPI2);
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI����Ϊ˫��˫��ȫ˫�� ��
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;// ����Ϊ��SPI ��
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI���ͽ���8λ֡�ṹ ��
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;// ʱ�����յ� ��
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// ���ݲ����ڵ�һ��ʱ���� ��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// �ڲ�NSS�ź���SSIλ���ƣ�������ƣ�
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// ������Ԥ��ƵֵΪ2 ��
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ���ݴ����MSBλ��ʼ ��
			//SPI_InitStructure.SPI_CRCPolynomial = 7;//��������CRCֵ����Ķ���ʽ��
			
			SPI_Init(SPI2, &SPI_InitStructure);
		//ʹ��
			SPI_Cmd(SPI2, ENABLE);*/
	}
#endif
		
/********************************************************************************************************
*  Function Name  : TIM_Configuration					                                                           
*  Object					: TIM����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_TIM_H
	void TIM_Configuration(){ 
		Rudder_TIM_Configuration();
		Motor_TIM_Configuration();
	}
#endif
	
/********************************************************************************************************
*  Function Name  : WWDG_Configuration					                                                           
*  Object					: WWDG����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_WWDG_H
	void WWDG_Configuration(){
	}
#endif	
	
/********************************************************************************************************
*  Function Name  : NVIC_Configuration					                                                           
*  Object					: NVIC����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�������0
	
	NVIC_InitStructure.NVIC_IRQChannel =  TIM3_IRQn;//�������ֱ���������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel =  TIM4_IRQn;//�������ֱ���������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//���õ��Դ����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//���ó����������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel =  USART3_IRQn;//������������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
#ifdef STM32F10X_HD
	NVIC_InitStructure.NVIC_IRQChannel =  UART5_IRQn;//���ó����������ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
#endif	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�����ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ�� 
#ifdef STM32F10X_HD	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;//�����ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;//ռ��ʽ���ȼ�����Ϊ0
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
#endif	
}
/********************************************************************************************************
*  Function: Driver_MCU_Init						                                                           
*  Object: MCU��ʼ��               
*  ���룺 ��
*  ����� ��								                         	                                     
*  ��ע�� ʱ������ MCU������һЩ����                                      
********************************************************************************************************/
void Driver_MCU_Init(void)
{ 
  //--------------------------- CLK INIT, HSE PLL ----------------------------
	ErrorStatus HSEStartUpStatus;
	//RCC reset
	RCC_DeInit();
	//Enable HSE
	RCC_HSEConfig(RCC_HSE_ON); 
	//Wait HSE is ready
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	//If HSE start fail, wail and wail.
	while(HSEStartUpStatus == ERROR);
	//Set bus clock
   	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	//HSE8M PLL-> 72M
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//Select PLL as system clock source
   	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);

	//----------------------------- CLOSE HSI ---------------------------
	RCC_HSICmd(DISABLE);

	//--------------------------- OPEN GPIO CLK -------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//---------------------------- INT CONFIG ---------------------------
	EXTI_IT_DISABLE();
	NVIC_Configuration();
   	
	
	//---------------------------- JTAG CONFIG ---------------------------
	//JTAG/SWD disable
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	
	#ifdef __STM32F10x_GPIO_H
		GPIO_Configuration();
	#endif
	
	#ifdef __STM32F10x_USART_H
		USART_Configuration();
		printf("USART_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_SYSTICK_H
		SYSTICK_Configuration();
		printf("SYSTICK_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_DMA_H
		#ifndef STM32F10X_R8
		DMA_Configuration();
		printf("DMA_Configuration DONE!\r\n");
		#endif
	#endif 
	
	#ifdef __STM32F10x_EXTI_H
		EXTI_Configuration();
		printf("EXTI_Configuration DONE!\r\n");
	#endif 

	#ifdef __STM32F10x_ADC_H
		ADC_Configuration();
		printf("ADC_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_I2C_H
		I2C_Configuration();
		printf("I2C_Configuration DONE!\r\n");
	#endif

	#ifdef __STM32F10x_SPI_H
		SPI_Configuration();
		printf("SPI_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_TIM_H
		TIM_Configuration();
		printf("TIM_Configuration DONE!\r\n");
	#endif	

	#ifdef __STM32F10x_WWDG_H
		WWDG_Configuration();
		printf("WWDG_Configuration DONE!\r\n");
	#endif	
	
	EXTI_IT_ENABLE();
	DelayMS(100);
}	
