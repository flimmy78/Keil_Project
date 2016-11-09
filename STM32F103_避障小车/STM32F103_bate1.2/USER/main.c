/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/ 
//#include "stm32f10x.h"
#include "STM32_DevInit.h"

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

uint8_t test_flag = 0;
void test(void){
	if(test_flag){
		motor_L->Motor_Run(motor_L, MOTOR_UP, 500);
		motor_R->Motor_Run(motor_R, MOTOR_UP, 500);
	}else{
		motor_L->Motor_Run(motor_L, MOTOR_DOWN, 500);
		motor_R->Motor_Run(motor_R, MOTOR_DOWN, 500);
	}
	test_flag = ~test_flag;
}

int main(void)
{	
	
	TimeTask_Init();		//��ʼ���������
	Protocol_Init();		//��ʼ��Э��ջ
	Rudder_Init(); 			//�����ʼ��
	Motor_Init();				//���������ʼ��
	Driver_MCU_Init();
	BaseClock_Init();
	printf("main done\r\n");
	//------------���ؼ���----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------���ڷ���----------------------
	//TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------LEDָʾ��----------------------
	taskid = TimeTask_Add(2, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	//------------���----------------------
	TimeTask_Add(3, Create_SysTickStamp(0, 10, 0), Rudder_Run, Real_Time_Mode);
	//------------������----------------------
	TimeTask_Add(4, Create_SysTickStamp(0, 10, 0), Ultrasonic_Run, Real_Time_Mode);
	//------------����----------------------
	TimeTask_Add(5, Create_SysTickStamp(0, 500, 0), test, Real_Time_Mode);
	
		DelayMS(500);

	while(1){
		//TIM_SetCompare3(TIM3, 10000); 
		//TIM_SetCompare4(TIM3, 10000); 
		TimeTask_Run();	
	}
	/*while(1){
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	
		
		//Delay_MS(500);
		printf("%d\r\n",microsecond);
				//halRfSendPacket(TxBuf,8);
		//SPI_Send(SPI1,0xb0);
		//data = SPI_Receive(SPI1);
	//printf("CCxxx0_PARTNUM:%x",data);
		//CC1101_Main1();
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);	
		//data = SPI_Send(SPI1,0x55);
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);
		
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);	
		//data = SPI_Receive(SPI2);
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);	
		//printf("SPI2:%x\r\n",data);
		//GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		//printf("SPI2:%x\r\n",SPI_Receive(SPI2));
		//SPI1_Send(0);
		//Delay_MS(500);
	}*/
		//��ʼ��ϵͳʱ��
	/*
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}*/
}





