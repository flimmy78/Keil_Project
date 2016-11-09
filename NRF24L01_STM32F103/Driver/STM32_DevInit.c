#include "STM32_DevInit.h"
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
		// LED PB10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);
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
	/*void SYSTICK_Configuration(){
		//时钟
			SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //SysTick_CLKSource_HCLK_Div8
		//配置
			SysTick_CounterCmd(SysTick_Counter_Clear);
		SysTick_SetReload(72000);   //9000/9Mhz
		//使能
		
		//SysTick_CounterCmd(SysTick_Counter_Disable);
		SysTick_ITConfig(DISABLE); 
		
		
		//SysTick_ITConfig(ENABLE); 
		//SysTick_CounterCmd(SysTick_Counter_Enable);
	}*/
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
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			USART_Cmd(USART1, ENABLE);
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
		//时钟
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //采用组别2	 
	 /*
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//配置外部中断1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始化
			*/
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//配置外部中断1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始化
		//GPIO配置
		//功能配置
		

		//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);	//PA1作为外部中断线1引脚
		/*EXTI_ClearITPendingBit(EXTI_Line0);	//清除1线标志位
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//边沿触发
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 //根据以上参数初始化结构体
		*/
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);	//PA1作为外部中断线1引脚
		EXTI_ClearITPendingBit(EXTI_Line5);	//清除1线标志位
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//边沿触发
		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 //根据以上参数初始化结构体
		//使能
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
		//时钟
		//GPIO配置
		//功能配置
		//使能
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
		//时钟
		//GPIO配置
		//功能配置
		//使能
		;
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
			/*SPI_Init(SPI1, &SPI_InitStructure);
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI设置为双线双向全双工 ；
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// 设置为主SPI ；
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI发送接收8位帧结构 ；
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// 时钟悬空低 ；
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// 数据捕获于第一个时钟沿 ；
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 内部NSS信号有SSI位控制（软件控制）
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;// 波特率预分频值为2 ；
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 数据传输从MSB位开始 ；
			SPI_InitStructure.SPI_CRCPolynomial = 7;//定义用于CRC值计算的多项式。*/
			//SPI_I2S_DeInit(SPI1);
			/*SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI为2线全双工
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI主模式
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //SPI发送接收8位帧结构
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //时钟空闲时为0
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					  //数据捕获于第一个时钟沿
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //内部NSS信号由软件控制
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//波特率预分频值为4
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据传输从高位开始
			SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRC值计算的多项式最高为7次*/
			//SPI_Init(SPI1, &SPI_InitStructure);
		//使能
			//SPI_Cmd(SPI1, ENABLE);
			
			
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
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// 设置为主SPI ；
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI发送接收8位帧结构 ；
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// 时钟悬空低 ；
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// 数据捕获于第一个时钟沿 ；
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 内部NSS信号有SSI位控制（软件控制）
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;// 波特率预分频值为2 ；
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 数据传输从MSB位开始 ；
			SPI_InitStructure.SPI_CRCPolynomial = 7;//定义用于CRC值计算的多项式。
			
			SPI_Init(SPI2, &SPI_InitStructure);
		//使能
			SPI_Cmd(SPI2, ENABLE);

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
		;
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
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//占先式优先级设置为0
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始化
	
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//配置外部中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
		NVIC_Init(&NVIC_InitStructure);//中断初始化
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
