//###########################################################################
//
// FILE:    MotorDrive.c
//
// TITLE:   �����������������
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "tool.h"
#include "DeviceConfig.h"
#include "USARTSentData.h"
#include "Strategy.h"
#include "MotorWingDriver.h"
#include "MotorHeadDriver.h"
#include <stdio.h>
#include <string.h>
#include "MotorDriver.h"

//����ٶ�
#define WHEELSPEEDMAX 100
#define HEADSPEEDMAX 100
#define WINGSPEEDMAX 100

MOTOR_PARM Motor1;
MOTOR_PARM Motor2;
MOTOR_PARM Motor3;
MOTOR_PARM Motor4;
MOTOR_PARM Motor5;


void MotorInt(void);
void ExecuteMotorParam(void);
void ExecuteMotorSpeed(MOTOR_PID *PID);	
void ExecuteMotorDIR(uint16_t motorID, int16_t dir);			//����1��-1	ֻ�ܱ�MotorExe����!!!

void ScanMotorLimit(void){
	ScanHeadMotorLimit();
	ScanWingMotorLimit();
}

void MotorGeneralInit(MOTOR_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
		motor->motorID = motor_id;			//���ID 1-2
		motor->Speed = 0;				//��ǰ�ٶ�
		motor->SpeedCMD = 0;		//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
		motor->Accelerate = 0;      //��ǰ���ٶ�
		motor->AccelerateCMD = 0;      //��ǰ���ٶ�����		
		motor->SpeedStrategy = 1;			//1:���ٲ��ԣ�2���ȼ��ٲ��ԣ�3���ȼ��ٲ���
		
		motor->motorDir = 1;			//�����ǰ�ٶ�
		motor->motorSpeed = 0;			//��ǰ�ٶ�
		motor->motorAccelerate = 0;      //��ǰ���ٶ�
		motor->motorDirCMD = 1;		//�˴�������λ�������ķ���ָ�1��2
		motor->motorSpeedCMD = 0;		//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
		motor->motorAccelerateCMD = 2*4;      //��ǰ���ٶ�����
		
		motor->SpeedTime = 0;      //�����ٶȣ��ۼ�������ʱ��
		motor->SpeedTimeLast = 0;      //�����ٶȣ��ۼ�������ʱ��
		
		motor->encoder = ENCODER_CON_RES;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
		motor->encoderLast = ENCODER_CON_RES;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
		motor->OdometerLast = 0;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
		
		motor->Current = 0;			//��ǰ����
		motor->CurrentLast = 0;			//�ϴε���
		motor->deltaCurrent = 0;			//�����仯��
		motor->CurrentMaxNum = 0;			//�ۼƵ�������Խ��ֵ����
		motor->CurrentCrossFlag = 0;
		motor->CurrentStrategy = 1;			//������������   1���������У�2��Խ�ϣ�3���ϰ������ֹͣ����	
	

		
		motor->fuseSensor = 0;			//����˿������	1:�����������		0:����		
		motor->motorTrouble = 0;    //1:�������,   2:������ٶȹ���   
		SetMotorParam(motor, 0, 1);
		
		motor->TIMx = TIMx;
		motor->GPIOx = GPIOx;
		motor->GPIO_Pin = GPIO_Pin;
		motor->angle_code = 1;//125;//0xFF;
		motor->encoderMAX = 0;//4500;
		motor->encoderCMD = 0;
		motor->flagCMD = 0;
		motor->angleMax = 0;
		motor->angleCMD = 1;
}


/****************************************************
������:MotorInt
��ע: �����ʼ��
****************************************************/
void MotorInt(void)	// ��ʼ���������
{
	memset(&Motor1, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor2, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor3, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor4, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor5, 0, sizeof(MOTOR_PARM)); 
	
	Motor1.Motor_Init = MotorGeneralInit; 
	Motor1.ExecuteMotor = NULL;
	Motor1.StopMotor = NULL;
	
	Motor2.Motor_Init = MotorGeneralInit; 
	Motor2.ExecuteMotor = NULL;
	Motor2.StopMotor = NULL;
	
	Motor3.Motor_Init = MotorGeneralInit; 
	Motor3.ExecuteMotor = ExecuteHeadMotor;
	Motor3.StopMotor = StopHeadMotor;
	
	Motor4.Motor_Init = MotorGeneralInit; 
	Motor4.ExecuteMotor = ExecuteWingMotor;
	Motor4.StopMotor = StopWingMotor;
	
	Motor5.Motor_Init = MotorGeneralInit; 
	Motor5.ExecuteMotor = ExecuteWingMotor;
	Motor5.StopMotor = StopWingMotor;
	
	
	Motor1.Motor_Init(&Motor1, ID_MOTOR_L, TIM15, GPIOA, GPIO_Pin_8);
	Motor2.Motor_Init(&Motor2, ID_MOTOR_R, TIM15, GPIOB, GPIO_Pin_3);
	Motor3.Motor_Init(&Motor3, ID_MOTOR_H, TIM14, GPIOB, GPIO_Pin_2);
	Motor4.Motor_Init(&Motor4, ID_MOTOR_LC, TIM16, GPIOB, GPIO_Pin_10);
	Motor5.Motor_Init(&Motor5, ID_MOTOR_RC, TIM17, GPIOB, GPIO_Pin_11);
	
	
}

/****************************************************
������:SetMotorParam
��ע: ���õ������
****************************************************/
void SetMotorParam(MOTOR_PARM* motor, uint16_t speed, int16_t dir)	//�ٶ�ֵ0-100
{ 
	if((dir != DIR_MOTOR_A  && dir != DIR_MOTOR_B) || speed > 100)
	{
		speed = 0; 			//������ȷ	//�ٶȲ��ܴ���100
		dir = DIR_MOTOR_A;
	}
	motor->motorSpeedCMD = speed;
	motor->motorDirCMD = dir;
	motor->OdometerCMD = 0;
	motor->MoveStrategy = SPEED_STRATEGY;
	motor->OdometerLast = 0;
//	if(motorTemp->motorDirCMD == DIR_MOTOR_A){
//		motorTemp->SpeedCMD = speed;
//	}else if(motorTemp->motorDirCMD == DIR_MOTOR_B){
//		motorTemp->SpeedCMD = -speed;
//	} 
}

/****************************************************
������:SetMotorParam2
��ע: ���õ������  �ٶ�+λ��  λ��Ϊ��
****************************************************/
void SetMotorParam2(MOTOR_PARM* motor, uint16_t speed, int16_t dir, uint32_t odometer)	//�ٶ�ֵ0-100
{
	if((dir != DIR_MOTOR_A  && dir != DIR_MOTOR_B) || speed > 100)
	{
		speed = 0; 			//������ȷ	//�ٶȲ��ܴ���100
		dir = DIR_MOTOR_A;
	}
	motor->motorSpeedCMD = speed;
	motor->motorDirCMD = dir;	
	motor->OdometerCMD = odometer;
	motor->MoveStrategy = ODOMETER_STRATEGY;
	motor->OdometerLast = motor->encoder;
//	if(motorTemp->motorDirCMD == DIR_MOTOR_A){
//		motorTemp->SpeedCMD = speed;
//	}else if(motorTemp->motorDirCMD == DIR_MOTOR_B){
//		motorTemp->SpeedCMD = -speed;
//	} 
}


/****************************************************
������:ExecuteMotorSpeed
��ע: ִ�е������
****************************************************/
void ExecuteMotorSpeed(MOTOR_PID *PID)	//�ٶ�ֵ0-2400	ֻ�ܱ�MotorExe����!!!
{	
	uint8_t dir=1;
	uint16_t speed=0;
	MOTOR_PARM *motorTemp;	
	motorTemp = &Motor1 + PID->motorID - 1;
	
	if(PID->ExecuteValue > 0){	dir = DIR_MOTOR_A ;	speed = PID->ExecuteValue;}
	else{		dir = DIR_MOTOR_B ;		speed = -PID->ExecuteValue;	}
	
	
	//-----------------���������������𵴣����͹���------------------------
	if((PID->ExecuteValue > -5) && (PID->ExecuteValue < 5)){
		if(PID->Goal>0){	dir = DIR_MOTOR_A ;	}else{ 	dir = DIR_MOTOR_B ;}
		speed=0;
	}
	//-----------------��Ӧ����-------------------------
	if((PID->Goal == 0) && (PID->CurrentValue < 2 || PID->CurrentValue > -2)){
		speed=0;
		PID->Error = 0;
		PID->ErrorLast = 0;
		PID->Integrator = 0;
	}		
	
	//----------------���Ƽ��ٶ�-----------------------	
	if((motorTemp->Accelerate > 0  && motorTemp->Accelerate > 30) || motorTemp->motorTrouble == 2){
		speed = 0;
		motorTemp->motorTrouble = 2;
	}
	if((motorTemp->Accelerate < 0  && motorTemp->Accelerate < -30) || motorTemp->motorTrouble == 2){
		speed = 0;
		motorTemp->motorTrouble = 2;
	}
	//----------------�����ٶ�-----------------------	
	if((motorTemp->Speed > 0  && motorTemp->Speed > 60) || motorTemp->motorTrouble == 1){
		speed = 0;
		motorTemp->motorTrouble = 1;
	}
	if((motorTemp->Speed < 0  && motorTemp->Speed < -60) || motorTemp->motorTrouble == 1){
		speed = 0;
		motorTemp->motorTrouble = 1;
	}

	if(motorTemp->motorDirCMD == 1) speed = 0;
	if( speed > 2400) speed = 2400; 										//�ٶȲ��ܴ���2400 	
	
	#ifdef PRINT_ERR 
		if(PID->motorID == ID_MOTOR_L){
			dir = Motor1.motorDirCMD;
			//speed = 500;
		}else if(PID->motorID == ID_MOTOR_R){
			dir = Motor2.motorDirCMD;
			//speed = 500;
		}
	#endif
	//dir = 0;
	//speed = 500;
	if(PID->motorID == ID_MOTOR_L)
	{
		//----------------���Ԥ����-----------------------	
		if(speed < PRELOAD_FORCE_L && motorTemp->motorSpeedCMD != 0 && ScramFlag == 0) speed = PRELOAD_FORCE_L;
		TIM_SetCompare1(TIM15, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_L_F;	else SET_MOTOR_L_B;
	}
	else if(PID->motorID == ID_MOTOR_R)
	{
		//----------------���Ԥ����-----------------------	
		if(speed < PRELOAD_FORCE_R && motorTemp->motorSpeedCMD != 0 && ScramFlag == 0) speed = PRELOAD_FORCE_R;
		TIM_SetCompare2(TIM15, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_R_F;	else SET_MOTOR_R_B;
	}
	else if(PID->motorID == ID_MOTOR_H)
	{
		TIM_SetCompare1(TIM14, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_H_L;	else SET_MOTOR_H_R;
	}
	else if(PID->motorID == ID_MOTOR_LC)
	{
		TIM_SetCompare1(TIM16, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_LC_U;	else SET_MOTOR_LC_D;
	}
	else if(PID->motorID == ID_MOTOR_RC)
	{
		TIM_SetCompare1(TIM17, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_RC_U;	else SET_MOTOR_RC_D;
	}	
}























