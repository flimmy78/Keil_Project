//###########################################################################
//123
// FILE:    troubleshoot.c
//
// TITLE:   �������
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################

#include "stm32f0xx.h"
#include "Strategy.h"
#include "main.h"
#include "DeviceConfig.h"
#include "MotorDriver.h"

void Scram(MOTOR_PID *PID, MOTOR_PARM *motor);	// ��ͣ
void ResetSYS(void);
uint8_t ScramFlag = 0;   


/****************************************************
������:Scram
��ע: ��ͣ
****************************************************/
void Scram(MOTOR_PID *PID, MOTOR_PARM *motor)	// ��ʼ���������
{
	PID->ExecuteValue = 0;
	PID->Goal = 0;
	motor->SpeedCMD = 0;
	motor->motorSpeedCMD = 0;
	//motor->motorAccelerateCMD = 0xffff;	
}

/****************************************************
������:ResetSYS
��ע: ����ϵͳ
****************************************************/
void ResetSYS(void)	// ��ʼ���������
{
	MotorInt();
	PIDInt();
}

