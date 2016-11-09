/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/ 
//
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
extern uint8_t Rudder_Pause;
uint8_t t = 0;
void test(void){
	if(test_flag){
// 		motor_L->Motor_Run(motor_L, MOTOR_DOWN, 20);
// 		motor_R->Motor_Run(motor_R, MOTOR_DOWN, 20);
// 		motor_L->Motor_Run(motor_L, MOTOR_STOP, 20);
// 		motor_R->Motor_Run(motor_R, MOTOR_STOP, 20);
	}else{
//  		motor_L->Motor_Run(motor_L, MOTOR_UP, 30);
//  		motor_R->Motor_Run(motor_R, MOTOR_UP, 30);

	}
	test_flag = ~test_flag;
	//printf("%d\r\n",t++);
			
	printf("Lcur:%d\tRcur:%d\tLdiff:%d\tRdiff:%d\r\n",motor_L->Encoder_Cur,motor_R->Encoder_Cur,motor_L->Location_Diff,motor_R->Location_Diff);
	printf("Lres:%f\res:%f\tLspeed:%f\tRSpeed:%f\r\n",motor_L->PID_Location.Res,motor_R->PID_Location.Res,motor_L->PID_Speed.Res,motor_R->PID_Speed.Res);
	
	/*if(motor_L->Dir == MOTOR_UP){
		printf("ǰ\r\n");
	}else if(motor_L->Dir == MOTOR_DOWN){
		printf("��\r\n");
	}else{
		printf("ͣ\r\n");
	}*/
	
	
	/*printf("L:%d\r\n", TIM_GetCounter(TIM3));
	printf("R:%d\r\n", TIM_GetCounter(TIM4));*/
}

int main(void)
{	
	
	TimeTask_Init();		//��ʼ���������
	Protocol_Init();		//��ʼ��Э��ջ
	Rudder_Init(); 			//�����ʼ��
	Motor_Init();				//���������ʼ��
	Driver_MCU_Init();
	BaseClock_Init();
	OLED_Init();
	printf("main done\r\n");
	//------------���ؼ���----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------���ڷ���----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------LEDָʾ��----------------------
	taskid = TimeTask_Add(2, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	//------------���----------------------
	TimeTask_Add(3, Create_SysTickStamp(0, 5, 0), Rudder_Run, Real_Time_Mode);
	//------------������----------------------
	//TimeTask_Add(4, Create_SysTickStamp(0, 50, 0), Ultrasonic_Run, Real_Time_Mode);
	//------------����----------------------
	//TimeTask_Add(5, Create_SysTickStamp(0, 500, 0), test, Real_Time_Mode);
	//------------���PID����----------------------
	TimeTask_Add(6, Create_SysTickStamp(0, 25, 0), Motor_PID, Real_Time_Mode);
	//------------Э�鴦��----------------------
	TimeTask_Add(7, Create_SysTickStamp(0, 25, 0), DealUsartCMD, Real_Time_Mode);
	
	/*motor_L->Motor_Run(motor_L, MOTOR_UP, 80);
 	motor_R->Motor_Run(motor_R, MOTOR_UP, 80);*/
		
	while(1){
		TimeTask_Run();
	}
}





