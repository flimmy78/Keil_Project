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

/*����˵��
NRF24L01��
	CSN��PC4
	IRQ��PC5
	CE ��PA4
	SCK��PA5
	MISO��PA6
	MOSI��PA7

*/

/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/

u8 taskid = 0;
u8 led_flag = 0;

u8 HeartBeat = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	//GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)led_flag);
	GPIO_WriteBit(GPIOD, GPIO_Pin_2,(BitAction)led_flag);
}

u8 TxBuf[32]={10,1,2,3,4,5,6};//{0x08,0xEF,0x03,0x04,0x05,0x06,0x07,0x08};
u8 activecount = 0;
void NRF_Test(){ 
	printf("�:%d\r\n",activecount++); 
}
void NRF_Init(){
	nrf1->CE_GPIO = GPIOA;
	nrf1->CE_GPIO_Pin = GPIO_Pin_4;
	nrf1->IRQ_GPIO = GPIOC;
	nrf1->IRQ_GPIO_Pin = GPIO_Pin_5;
	nrf1->CSN_GPIO = GPIOC;
	nrf1->CSN_GPIO_Pin = GPIO_Pin_4;
	nrf1->SPI = SPI1;
	NRF24L01_Init(nrf1); 
} 

void draw(){
	printf("%d\t%d\t%d\t%d\r\n",
	MPU6050->X + MPU6050->Y,
	MPU6050->X - MPU6050->Y,
	-MPU6050->X + MPU6050->Y,
	-MPU6050->X - MPU6050->Y);
}
extern u8 cutcount;
void MPU6050_Task(void){
	MPU6050->Get_MPU6050(MPU6050);
	//printf("%d\t%d\r\n", MPU6050->Pitch,MPU6050->Roll);
	Motor_Speed_Calculate(motor_UL); 
	Motor_Speed_Calculate(motor_UR); 
	Motor_Speed_Calculate(motor_DL); 
	Motor_Speed_Calculate(motor_DR); 
	//printf("S:%d\tS:%d\t",motor_UR->Pwm_Cur,motor_DL->Pwm_Cur);
	//printf("S:%d\tS:%d\t",motor_UR->Pwm_Cur,motor_DL->Pwm_Cur);
//	printf("UR:%d\tDL:%d\t",
//		(int16_t)motor_UR->PID_Balance.RealValue,(int16_t)motor_DL->PID_Balance.RealValue);
//	printf("X:%d\tY:%d\t\n",MPU6050->X,MPU6050->Y);
} 
int main(void)
{	 
	TimeTask_Init();		//��ʼ���������
	Protocol_Init();		//��ʼ��Э��ջ
	ComBuff_Init();			//��ʼ��ͨ�Ż����� 
	BaseClock_Init();		//��ʼ��ʱ��
	#ifdef __MPU6050_H_
	MPU6050_Init();			//��ʼ��MPU6050
	#endif
	Motor_Init();
	Driver_MCU_Init();	//��ʼ��MCU 
	NRF_Init();					//��ʼ��NRF24L01����ģ��
	printf("main done\r\n");  
	//------------���ؼ���----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------���ڷ���----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------����ģ�鷢��----------------------
	TimeTask_Add(2, Create_SysTickStamp(0, 5, 0), Buff_To_NRF,Real_Time_Mode); 
	//------------NRF24L01����--------------
	TimeTask_Add(4, Create_SysTickStamp(0, 300, 0), LED_TEST,Real_Time_Mode);
	//------------Э����ȡ����--------------
	TimeTask_Add(5, Create_SysTickStamp(0, 20, 0), FetchProtocolHandle,Real_Time_Mode);
	//------------MPU6050��̬����-----------
	TimeTask_Add(6, Create_SysTickStamp(0, 20, 0), MPU6050_Task,Real_Time_Mode);
	//------------�ϱ�Э��-----------
	TimeTask_Add(7, Create_SysTickStamp(0, 100, 0), StateReport,Real_Time_Mode);
	//------------ͼ�λ�����-----------	
	//TimeTask_Add(6, Create_SysTickStamp(0, 100, 0), draw,Real_Time_Mode); 
	//------------����ٶȵ���-----------
	//TimeTask_Add(9, Create_SysTickStamp(0, 2, 0), Execute_Motor_Task,Real_Time_Mode);
	
	while(1){ 
		TimeTask_Run();	
	}
}





