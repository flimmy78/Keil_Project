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
		// LED PB10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);
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
	/*void SYSTICK_Configuration(){
		//ʱ��
			SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //SysTick_CLKSource_HCLK_Div8
		//����
			SysTick_CounterCmd(SysTick_Counter_Clear);
		SysTick_SetReload(72000);   //9000/9Mhz
		//ʹ��
		
		//SysTick_CounterCmd(SysTick_Counter_Disable);
		SysTick_ITConfig(DISABLE); 
		
		
		//SysTick_ITConfig(ENABLE); 
		//SysTick_CounterCmd(SysTick_Counter_Enable);
	}*/
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
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			USART_Cmd(USART1, ENABLE);
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
		//ʱ��
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�������2	 
	 /*
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�����ⲿ�ж�1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
			*/
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�����ⲿ�ж�1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
		//GPIO����
		//��������
		

		//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);	//PA1��Ϊ�ⲿ�ж���1����
		/*EXTI_ClearITPendingBit(EXTI_Line0);	//���1�߱�־λ
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//���ش���
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 //�������ϲ�����ʼ���ṹ��
		*/
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);	//PA1��Ϊ�ⲿ�ж���1����
		EXTI_ClearITPendingBit(EXTI_Line5);	//���1�߱�־λ
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//���ش���
		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 //�������ϲ�����ʼ���ṹ��
		//ʹ��
		;
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
		;
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
		//ʱ��
		//GPIO����
		//��������
		//ʹ��
		;
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
			;
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
			/*SPI_Init(SPI1, &SPI_InitStructure);
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI����Ϊ˫��˫��ȫ˫�� ��
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// ����Ϊ��SPI ��
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI���ͽ���8λ֡�ṹ ��
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// ʱ�����յ� ��
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// ���ݲ����ڵ�һ��ʱ���� ��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// �ڲ�NSS�ź���SSIλ���ƣ�������ƣ�
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;// ������Ԥ��ƵֵΪ2 ��
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ���ݴ����MSBλ��ʼ ��
			SPI_InitStructure.SPI_CRCPolynomial = 7;//��������CRCֵ����Ķ���ʽ��*/
			//SPI_I2S_DeInit(SPI1);
			/*SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPIΪ2��ȫ˫��
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI��ģʽ
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //SPI���ͽ���8λ֡�ṹ
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //ʱ�ӿ���ʱΪ0
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					  //���ݲ����ڵ�һ��ʱ����
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //�ڲ�NSS�ź����������
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//������Ԥ��ƵֵΪ4
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݴ���Ӹ�λ��ʼ
			SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRCֵ����Ķ���ʽ���Ϊ7��*/
			//SPI_Init(SPI1, &SPI_InitStructure);
		//ʹ��
			//SPI_Cmd(SPI1, ENABLE);
			
			
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
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// ����Ϊ��SPI ��
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI���ͽ���8λ֡�ṹ ��
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// ʱ�����յ� ��
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// ���ݲ����ڵ�һ��ʱ���� ��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// �ڲ�NSS�ź���SSIλ���ƣ�������ƣ�
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;// ������Ԥ��ƵֵΪ2 ��
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ���ݴ����MSBλ��ʼ ��
			SPI_InitStructure.SPI_CRCPolynomial = 7;//��������CRCֵ����Ķ���ʽ��
			
			SPI_Init(SPI2, &SPI_InitStructure);
		//ʹ��
			SPI_Cmd(SPI2, ENABLE);

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
		;
	}
#endif
	
/********************************************************************************************************
*  Function Name  : TIM_Configuration					                                                           
*  Object					: TIM����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��Ң 2015��11��7��12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_WWDG_H
	void WWDG_Configuration(){
		;
	}
#endif	
	

void NVIC_Configuration(){
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�������2	 
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�����ⲿ�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//ռ��ʽ���ȼ�����Ϊ0
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//�����ⲿ�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
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
  //����ö�����ͱ���
	ErrorStatus HSEStartUpStatus;
	//��λϵͳʱ������
	RCC_DeInit();
	//����HSE
	RCC_HSEConfig(RCC_HSE_ON); 
	//�ȴ�HSE�����ȶ�
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	//�ж�HSE�Ƿ�����ɹ����������if�ڲ�
	while(HSEStartUpStatus == ERROR);
	//ѡ��HCLK(AHB)ʱ��ԴΪSYSCLK 1��Ƶ
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//ѡ��PCLK1ʱ��ԴΪ 	HCLK(AHB)	1��Ƶ
	RCC_PCLK1Config(RCC_HCLK_Div1);
	//ѡ��PCLK2ʱ��ԴΪ 	HCLK(AHB)	1��Ƶ
	RCC_PCLK2Config(RCC_HCLK_Div1);
// 	//����FLASH��ʱ������Ϊ2
//   FLASH_SetLatency(FLASH_Latency_2);
// 	//ʹ��FLASHԤȡ����
// 	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	//ѡ�����໷(PPL)ʱ��ԴΪHSE 1��Ƶ����Ƶ��Ϊ9����PPL���Ƶ��Ϊ8MHz * 9 = 72MHz
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	//ʹ��PPL
	RCC_PLLCmd(ENABLE); 
	//�ȴ�PLL����ȶ�
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//ѡ��SYSCLKʱ��ԴΪPLL
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//�ȴ�PLL��ΪSYSCLKʱ��Դ
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
	//2-level interrupt 
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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
	#endif
	
	#ifdef __STM32F10x_EXTI_H
		EXTI_Configuration();
	#endif
	
	#ifdef __STM32F10x_DMA_H
		DMA_Configuration();
	#endif
	
	#ifdef __STM32F10x_ADC_H
		ADC_Configuration();
	#endif
	
	#ifdef __STM32F10x_I2C_H
		I2C_Configuration();
	#endif
	
	#ifdef __STM32F10x_SPI_H
		SPI_Configuration();
	#endif
	
	#ifdef __STM32F10x_TIM_H
		TIM_Configuration();
	#endif	
	
	#ifdef __STM32F10x_WWDG_H
		WWDG_Configuration();
	#endif	
}	
