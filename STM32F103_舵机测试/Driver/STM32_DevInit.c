#include <stdio.h>
#include "STM32_DevInit.h"
#include "RudderControl.h"
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
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		
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
		//GPIO配置
		//功能配置
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
			/*GPIO_InitTypeDef GPIO_InitStructure;
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
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;// 时钟悬空低 ；
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;// 数据捕获于第一个时钟沿 ；
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 内部NSS信号有SSI位控制（软件控制）
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;// 波特率预分频值为2 ；
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 数据传输从MSB位开始 ；
			SPI_InitStructure.SPI_CRCPolynomial = 7;//定义用于CRC值计算的多项式。
			//SPI_I2S_DeInit(SPI1);
			SPI_Init(SPI1, &SPI_InitStructure);
		//使能
			SPI_Cmd(SPI1, DISABLE);
			
			
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
		//时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		//功能配置
			TIM_DeInit(TIM4);
			TIM_TimeBaseStructure.TIM_Period = 20000;
			TIM_TimeBaseStructure.TIM_Prescaler = RUDDER_TIM_Prescaler;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);
			
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			 
			TIM_OCInitStructure.TIM_Pulse = 20000;
			TIM_OC1Init(TIM4, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
			
			TIM_OCInitStructure.TIM_Pulse = 20000;
			TIM_OC2Init(TIM4, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
			
		//使能
			TIM_Cmd(TIM4, ENABLE);
		
		printf("TIM_Configuration DONE!\r\n");	
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
/********************************************************************************************************
*  Function: Driver_MCU_Init						                                                           
*  Object: MCU初始化               
*  输入： 无
*  输出： 无								                         	                                     
*  备注： 时钟配置 MCU启动的一些配置                                      
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
	//2-level interrupt 
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   	
	//---------------------------- JTAG CONFIG ---------------------------
	//JTAG/SWD disable
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
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
