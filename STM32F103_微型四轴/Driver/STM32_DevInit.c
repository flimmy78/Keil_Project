#include "STM32_DevInit.h"
#include <stdio.h>
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++  平台驱动 +++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/********************************************************************************************************
*  Function Name  : GPIO_Configuration					                                                           
*  Object					: GPIO配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                                 
********************************************************************************************************/
#ifdef __STM32F10x_GPIO_H
	void GPIO_Configuration(){
		//时钟
		//GPIO配置
		//功能配置
		//使能
		GPIO_InitTypeDef GPIO_InitStructure;
		// LED PB10 || PD2
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//		GPIO_Init(GPIOD, &GPIO_InitStructure);
//		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	}
#endif

/********************************************************************************************************
*  Function Name  : SYSTICK_Configuration					                                                           
*  Object					: SysTick配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                                 
********************************************************************************************************/
#ifdef __STM32F10x_SYSTICK_H
	void SYSTICK_Configuration(){
		/*//时钟
			SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //SysTick_CLKSource_HCLK_Div8
		//配置
			SysTick_CounterCmd(SysTick_Counter_Clear);
		SysTick_SetReload(72000);   //9000/9Mhz
		//使能
		
		//SysTick_CounterCmd(SysTick_Counter_Disable);
		SysTick_ITConfig(DISABLE); 
		
		
		//SysTick_ITConfig(ENABLE); 
		//SysTick_CounterCmd(SysTick_Counter_Enable);*/
	}
#endif
	
/********************************************************************************************************
*  Function Name  : USART1_Configuration					                                                           
*  Object					: USART1配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                                
********************************************************************************************************/
#ifdef __STM32F10x_USART_H
	void USART_Configuration()
	{
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure; 
		//时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//功能配置
			USART_DeInit(USART1);
			USART_InitStructure.USART_BaudRate = 115200; 
			USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
			USART_InitStructure.USART_StopBits = USART_StopBits_1; 
			USART_InitStructure.USART_Parity = USART_Parity_No; 
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
			USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
			USART_Init(USART1, &USART_InitStructure);
		//使能
			USART_Cmd(USART1, ENABLE);
			MPU6050_USART_Configuration();
	}
#endif

/********************************************************************************************************
*  Function Name  : EXTI_Configuration					                                                           
*  Object					: EXTI配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                                   
********************************************************************************************************/
#ifdef __STM32F10x_EXTI_H
	void EXTI_Configuration(){
		EXTI_InitTypeDef EXTI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		//时钟
		//GPIO配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		//功能配置
		
		EXTI_ClearITPendingBit(EXTI_Line5);	//清除线标志位
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);	//PA1作为外部中断线引脚
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//边沿触发
		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 //根据以上参数初始化结构体
		;
	}
#endif
	
/********************************************************************************************************
*  Function Name  : ADC_Configuration					                                                           
*  Object					: ADC配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/

#ifdef __STM32F10x_ADC_H
	void ADC_Configuration(){
		ADC_InitTypeDef ADC_InitStructure;
		//时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
		//功能配置
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

		//使能
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
*  Object					: DMA配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_DMA_H
	void DMA_Configuration(){
		DMA_InitTypeDef DMA_InitStructure;//定义DMA初始化结构体
		//时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	    
		//功能配置
	 	DMA_DeInit(DMA1_Channel3);//复位DMA通道1
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR; //定义外设地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)_Usart3_Buff; //定义缓冲区地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//地址为外设地址非内部
		DMA_InitStructure.DMA_BufferSize = RECV_BUFSIZE_USART3;//缓冲区大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不变
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//当前缓冲区地址递增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//双字节
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //双字节
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//非内存到内存
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);//初始化DMA通道
		DMA_ClearFlag(DMA1_FLAG_TC3);
		DMA_Cmd(DMA1_Channel3, ENABLE); //使能DMA通道
		//DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);            // ????DMA????
	}
#endif
	
/********************************************************************************************************
*  Function Name  : I2C_Configuration					                                                           
*  Object					: I2C配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_I2C_H
	void I2C_Configuration(){
		//时钟
		//GPIO配置
		//功能配置
		//使能
			;
		}
#endif
		
/********************************************************************************************************
*  Function Name  : SPI_Configuration					                                                           
*  Object					: SPI配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_SPI_H
	void SPI_Configuration(){
			GPIO_InitTypeDef GPIO_InitStructure;
			SPI_InitTypeDef SPI_InitStructure;
		//时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//功能配置
			SPI_Init(SPI1, &SPI_InitStructure);
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI设置为双线双向全双工 ；
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// 设置为主SPI ；
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI发送接收8位帧结构 ；
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// 时钟悬空低 ；
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// 数据捕获于第一个时钟沿 ；
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 内部NSS信号有SSI位控制（软件控制）
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;// 波特率预分频值为2 ；
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 数据传输从MSB位开始 ；
			SPI_InitStructure.SPI_CRCPolynomial = 7;//定义用于CRC值计算的多项式。
			//SPI_I2S_DeInit(SPI1);
			SPI_Init(SPI1, &SPI_InitStructure);
		//使能
			SPI_Cmd(SPI1, ENABLE);
			
		/*	
		//时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		//功能配置
			SPI_I2S_DeInit(SPI2);
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI设置为双线双向全双工 ；
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;// 设置为主SPI ；
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI发送接收8位帧结构 ；
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;// 时钟悬空低 ；
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// 数据捕获于第一个时钟沿 ；
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 内部NSS信号有SSI位控制（软件控制）
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// 波特率预分频值为2 ；
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 数据传输从MSB位开始 ；
			//SPI_InitStructure.SPI_CRCPolynomial = 7;//定义用于CRC值计算的多项式。
			
			SPI_Init(SPI2, &SPI_InitStructure);
		//使能
			SPI_Cmd(SPI2, ENABLE);*/
		printf("SPI_Configuration DONE!\r\n");
	}
#endif
		
/********************************************************************************************************
*  Function Name  : TIM_Configuration					                                                           
*  Object					: TIM配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_TIM_H
	void TIM_Configuration(){
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;
//-------------------------电机PWM-------------------------
		//??
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//
		//GPIO??
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//????
			TIM_DeInit(TIM2);
			TIM_InternalClockConfig(TIM2);//72000000
			TIM_TimeBaseStructure.TIM_Period = PWM_MAX;
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			 
			TIM_OCInitStructure.TIM_Pulse = PWM_MAX;
			
			TIM_OC1Init(TIM2, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
			TIM_OC3Init(TIM2, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
			TIM_OC4Init(TIM2, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
			
			TIM_SetCompare1(TIM2, 0);
			TIM_SetCompare2(TIM2, 0);
			TIM_SetCompare3(TIM2, 0);
			TIM_SetCompare4(TIM2, 0);
		//使能
			TIM_ARRPreloadConfig(TIM2, ENABLE); 
			TIM_Cmd(TIM2, ENABLE);		 
	}
#endif
	
/********************************************************************************************************
*  Function Name  : TIM_Configuration					                                                           
*  Object					: TIM配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
#ifdef __STM32F10x_WWDG_H
	void WWDG_Configuration(){
		;
	}
#endif	
	
void NVIC_Configuration(){
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //采用组别2	
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//配置外部中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始化 
	
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//配置外部中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	
	
//		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;//配置外部中断
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//占先式优先级设置为0
//		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
//		NVIC_Init(&NVIC_InitStructure);//中断初始化
}

/********************************************************************************************************
*  Function: Driver_MCU_Init						                                                           
*  Object: MCU初始化               
*  输入： 无
*  输出： 无								                         	                                     
*  备注： 时钟配置 MCU启动的一些配置                                      
********************************************************************************************************/
void Driver_MCU_Init(void)
{
  //定义枚举类型变量
	ErrorStatus HSEStartUpStatus;
	//复位系统时钟设置
	RCC_DeInit();
	//开启HSE
	RCC_HSEConfig(RCC_HSE_ON); 
	//等待HSE起振并稳定
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	//判断HSE是否起振成功，是则进入if内部
	while(HSEStartUpStatus == ERROR);
	//选择HCLK(AHB)时钟源为SYSCLK 1分频
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//选择PCLK1时钟源为 	HCLK(AHB)	1分频
	RCC_PCLK1Config(RCC_HCLK_Div1);
	//选择PCLK2时钟源为 	HCLK(AHB)	1分频
	RCC_PCLK2Config(RCC_HCLK_Div1);
// 	//设置FLASH延时周期数为2
//   FLASH_SetLatency(FLASH_Latency_2);
// 	//使能FLASH预取缓存
// 	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	//选择锁相环(PPL)时钟源为HSE 1分频，倍频数为9，则PPL输出频率为8MHz * 9 = 72MHz
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	//使能PPL
	RCC_PLLCmd(ENABLE); 
	//等待PLL输出稳定
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//选择SYSCLK时钟源为PLL
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//等待PLL成为SYSCLK时钟源
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
		printf("GPIO_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_SYSTICK_H
		SYSTICK_Configuration();
		printf("SYSTICK_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_DMA_H
		DMA_Configuration();
		printf("DMA_Configuration DONE!\r\n");
	#endif
	
	#ifdef __STM32F10x_USART_H
		USART_Configuration();
		printf("USART_Configuration DONE!\r\n");
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
	
	DelayMS(100);
}	
