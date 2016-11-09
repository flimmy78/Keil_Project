/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/ 
#include "Driver.h"
//#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "STM32_DevInit.h"
#include "tool.h"
#include "protocol.h"



/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/

u8 taskid = 0;
u8 led_flag = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)led_flag);
	GPIO_WriteBit(GPIOD, GPIO_Pin_2,(BitAction)led_flag);
	//while(TimeOut(taskid,500000,1));
	//printf("��ʱ\r\n");
	
}
int main(void)
{	
	TimeTask_Init();		//��ʼ���������
	Protocol_Init();		//��ʼ��Э��ջ
	Driver_MCU_Init();	    //��ʼ��MCU
	BaseClock_Init();		//��ʼ��ʱ��
	printf("main done\r\n");
	//------------���ؼ���----------------------
	TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------���ڷ���----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------����----------------------
	taskid = TimeTask_Add(4, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	while(1){
		TimeTask_Run();	
	}
}





