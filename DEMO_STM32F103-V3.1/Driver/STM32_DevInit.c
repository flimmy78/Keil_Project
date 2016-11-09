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
		GPIO_InitTypeDef GPIO_InitStructure;
		// LED 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOA, &GPIO_InitStructure);   
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
		//GPIO����
		//��������
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
		ADC_InitTypeDef ADC_InitStructure;
		//ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
		//��������
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //???????
	    ADC_InitStructure.ADC_ScanConvMode = ENABLE;          //??????
	    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //????????
	    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ADC????,????
	    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //????,ADC?12??,?????
	    ADC_InitStructure.ADC_NbrOfChannel = ADC_ENB_NUM;    //?????,1?
	    ADC_Init(ADC1, &ADC_InitStructure);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_0 , 1 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1 , 2 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4 , 3 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5 , 4 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6 , 5 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_7 , 6 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8 , 7 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 8 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 9 , ADC_SampleTime_55Cycles5);  
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 10, ADC_SampleTime_55Cycles5);  

		//ʹ��
		ADC_DMACmd(ADC1, ENABLE);     //ADC??,??
	    ADC_Cmd(ADC1, ENABLE);  //??ADC1
	    ADC_ResetCalibration(ADC1);   //????
	    while(ADC_GetResetCalibrationStatus(ADC1));  //????????
	    ADC_StartCalibration(ADC1);     //????
	    while(ADC_GetCalibrationStatus(ADC1));     //??????
	    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
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
		DMA_InitTypeDef DMA_InitStructure;//����DMA��ʼ���ṹ��
		//ʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	    
		//��������
	 	DMA_DeInit(DMA1_Channel1);//��λDMAͨ��1
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //���������ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_TempBuff; //���建������ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//��ַΪ�����ַ���ڲ�
		DMA_InitStructure.DMA_BufferSize = ADC_SAMPLING * ADC_CHANNEL;//��������С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ǰ��������ַ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//˫�ֽ�
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //˫�ֽ�
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ�
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��DMAͨ��
		DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMAͨ��
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
			/*GPIO_InitTypeDef GPIO_InitStructure;
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
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;// ʱ�����յ� ��
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// ���ݲ����ڵ�һ��ʱ���� ��
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// �ڲ�NSS�ź���SSIλ���ƣ�������ƣ�
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// ������Ԥ��ƵֵΪ2 ��
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ���ݴ����MSBλ��ʼ ��
			SPI_InitStructure.SPI_CRCPolynomial = 7;//��������CRCֵ����Ķ���ʽ��
			//SPI_I2S_DeInit(SPI1);
			SPI_Init(SPI1, &SPI_InitStructure);
		//ʹ��
			SPI_Cmd(SPI1, DISABLE);
			
			
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
		/*//��������׽ģʽ	
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_ICInitTypeDef TIM_ICInitStructure; 
		//ʱ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		//GPIO����
			GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//��������
			TIM_DeInit(TIM2);
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_Prescaler = 160;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
		
			TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		
			//TIM_ICStructInit(&TIM_ICInitStructure);
			TIM_ICInitStructure.TIM_ICFilter=0x03;
			TIM_ICInit(TIM3,&TIM_ICInitStructure);
		//ʹ��
			TIM_Cmd(TIM2, ENABLE);*/
		
		printf("TIM_Configuration DONE!\r\n");
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
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�������4
	
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//�����ⲿ�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռʽ���ȼ�
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��
		NVIC_Init(&NVIC_InitStructure);//��ʼ��
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
	
	#ifdef __STM32F10x_SYSTICK_H
		SYSTICK_Configuration();
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
