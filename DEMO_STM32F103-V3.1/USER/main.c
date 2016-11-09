/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/   
#include "STM32_DevInit.h" 


u8 taskid = 0; 
u8 led_flag = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOA, GPIO_Pin_12 | GPIO_Pin_15,(BitAction)led_flag); 
}

 /*-------------------------------------------------------------------------------------------------------
|  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
 
int main(void)
{	 
	TaskTime_Init();		//��ʼ���������
	Protocol_Init();		//��ʼ��Э��ջ
	ComBuff_Init();			//��ʼ��ͨ�Ż�����
	Driver_MCU_Init();	//��ʼ��MCU
	BaseClock_Init();		//��ʼ��ʱ��
	printf("main done\r\n"); 
	
	//--------------LED-------------------------
	TaskTime_Add(0, TimeCycle(0,500), LED_TEST, Count_Mode);
	//------------���ڷ���----------------------
	TaskTime_Add(1, TimeCycle(0,1), Buff_To_Uart, Count_Mode);
	//------------Э�鴦��----------------------
	TaskTime_Add(2, TimeCycle(0,30), FetchProtocolHandle, Count_Mode); 
	while(1){ 
		TaskTime_Run();
	}
}





