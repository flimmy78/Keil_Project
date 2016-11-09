#include <stdio.h> 
#include <Type.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_systick.h> 
#include <stm32f10x_i2c.h> 
#include <I2C_driver.h> 
#include <stm32f10x_exti.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_wwdg.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include <NRF24L01_Driver.h>
#include <NRF24L01_Driver_send.h>
#include <mpu6050_driver.h>
ErrorStatus HSEStartUpStatus;

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
	//----------------------------��ʱ��CLK------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʹ��TIM6ʱ��
	//----------------------------SPI------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //ʹ��SPI1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //ʹ��SPI2ʱ��
	//----------------------------USART1------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	//---------------------------- INT CONFIG ---------------------------
	//2-level interrupt 
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   	
	//---------------------------- JTAG CONFIG ---------------------------
	//JTAG/SWD disable
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
}		 
/********************************************************************************************************
*  Function Name  : SysTick_Configuration						                                                           
*  Object					: SysTick����            
*  ����						�� ��
*  ���						�� ��								                         	                                     
*  ��ע						�� ��                                     
********************************************************************************************************/
void SysTick_Configuration(void)
{
	
	SysTick_ITConfig(DISABLE);
	//SysTick_CounterCmd(SysTick_Counter_Disable);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  
  SysTick_CounterCmd(SysTick_Counter_Clear);

	 
  //SysTick_CounterCmd(SysTick_Counter_Enable);
	
}
/*
*  SysTick_Configuration	end                                  
********************************************************************************************************/
/*******************************************************************************
** �������� �� USART1_Configuration(void)
** �������� �� ����1��ʼ��
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************/
void USART1_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
//	USART_ClockInitTypeDef  USART_ClockInitStructure;
  /* Configure USARTx_Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTx_Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	

// 	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// ʱ�ӵ͵�ƽ�
// 	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// ʱ�ӵ͵�ƽ
// 	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// ʱ�ӵڶ������ؽ������ݲ���
// 	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// ���һλ���ݵ�ʱ�����岻��SCLK���
// 	/* Configure the USART1 synchronous paramters */
// 	USART_ClockInit(USART1, &USART_ClockInitStructure);					// ʱ�Ӳ�����ʼ������

	USART_InitStructure.USART_BaudRate =BaudRate;						  // ������Ϊ��115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // ��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;				  // ��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // ����ʹ��+����ʹ��
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART1 */
	USART_ClearFlag(USART1,USART_FLAG_RXNE); //���жϣ�����һ�����жϺ����������ж�
	USART_ClearITPendingBit(USART1, USART_IT_RXNE); 			
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//ʹ��USART1�ж�Դ
	USART_ITConfig(USART1,USART_IT_TXE, DISABLE);		
	USART_Cmd(USART1, ENABLE);							//USART1�ܿ��أ����� 
	

}
/**********************************************************
** ������: EXTI_Config
** ��������:  �ⲿ�ж�����
** �������: ��
** �������: ��
***********************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);	//PA4��Ϊ�ⲿ�ж���1����
	EXTI_ClearITPendingBit(EXTI_Line4);	//���1�߱�־λ
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//���ش���
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 //�������ϲ�����ʼ���ṹ��
}
/*******************************************************************************
** �������� �� NVIC_Configuration(void)
** �������� �� �жϳ�ʼ��
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
*******************************************************************************/
void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�������0
	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//���ô����ж� �������ݽ����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;//TIM2�ж�    PWM���
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ�� 
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;//TIM6�ж�		��̬����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//ռ��ʽ���ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ�� 
 
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI4_IRQn;//����NRF24L01�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	
	


}
/*
********************************************************************************
** �������� �� WWDG_Configuration(void)
** �������� �� ���Ź���ʼ��
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
void WWDG_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
  WWDG_SetPrescaler(WWDG_Prescaler_8);	              //  WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  
  WWDG_SetWindowValue(0x41);		                 // Set Window value to 0x41
  WWDG_Enable(0x50);		       // Enable WWDG and set counter value to 0x7F, WWDG timeout = ~4 ms * 64 = 262 ms 
  WWDG_ClearFlag();			       // Clear EWI flag
  WWDG_EnableIT();			       // Enable EW interrupt
}

/*
********************************************************************************
** �������� �� SPI1_Configuration(void)
** �������� �� ����SPI1
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/ 
void SPI1_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;  //����SPI�ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/**********SPI��������****************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPIΪ2��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //ʱ�ӿ���ʱΪ0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					  //���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //�ڲ�NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//������Ԥ��ƵֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݴ���Ӹ�λ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRCֵ����Ķ���ʽ���Ϊ7��
	SPI_Init(SPI1, &SPI_InitStructure);//�������ϲ�����ʼ��SPI�ṹ��

	SPI_Cmd(SPI1, ENABLE);	//ʹ��SPI1
}
/*
********************************************************************************
** �������� �� SPI2_Configuration(void)
** �������� �� ����SPI2
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/ 
void SPI2_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;  //����SPI�ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/**********SPI��������****************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPIΪ2��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //ʱ�ӿ���ʱΪ0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					  //���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //�ڲ�NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//������Ԥ��ƵֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //���ݴ���Ӹ�λ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRCֵ����Ķ���ʽ���Ϊ7��
	SPI_Init(SPI2, &SPI_InitStructure);//�������ϲ�����ʼ��SPI�ṹ��

	SPI_Cmd(SPI2, ENABLE);	//ʹ��SPI2
}

//extern void printf(char* Str);
extern	TIM2_Config(void);
extern	TIM6_Config(void);
void system_init(void){	 
	Driver_MCU_Init();//����RCC
	SysTick_Configuration();
	USART1_Configuration(9600);	 //���ô���1
	printf("\nDriver_MCU_Init��ʼ�����");
	printf("\nUSART1_Configuration��ʼ�����");
	I2C_Configuration();
	printf("\nI2C_Configuration��ʼ�����");
	//SPI1_Configuration();
	//printf("\nSPI1_Configuration��ʼ�����");
	SPI2_Configuration();
	printf("\nSPI2_Configuration��ʼ�����");
	Init_MPU6050();
	printf("\nMPU6050��ʼ�����");
	TIM2_Config();
  printf("\nTIM2_Config��ʼ�����");
	TIM6_Config();
  printf("\nTIM6_Config��ʼ�����");
	//init_24L01();
	//printf("\ninit_24L01��ʼ�����");
	//init_24L01_send();
	//printf("\ninit_24L01_send��ʼ�����");
	EXTI_Configuration();
	printf("\nEXTI_Configuration��ʼ�����");
	NVIC_Configuration();
	printf("\nNVIC_Configuration��ʼ�����");
}
