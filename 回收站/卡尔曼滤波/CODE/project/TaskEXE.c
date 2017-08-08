//###########################################################################
//
// FILE:    	TaskEXE.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//    ����ִ��        

//###########################################################################
#include "stm32f10x.h"
#include "main.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
#include "USARTCMD.h"
#include "Sensors.h"

TASKS tasks;
MOTORS MotorsTaskPauseBuf;

void NewTaskInt(uint16_t *p);	//�ƶ�������
void AllTaskReset(void);	//���������ʼ��
void TaskExecute(void);	//����ִ��
void TaskMotorReset(void);

/****************************************************
������:	TaskExecute
��ע: ����ִ��
****************************************************/
void TaskExecute(void)	//����ִ��
{
//	if(tasks.MotorReset.taskState == TASK_ENABLE)TaskMotorReset();	//
//	if(tasks.UpOne.taskState == TASK_ENABLE)TaskUpOne();
}
/****************************************************
������:	AllTaskReset
��ע: ���������ʼ��
****************************************************/
void AllTaskReset(void)	//���������ʼ��
{
	uint16_t temp;
	TASK_PARM *taskTemp;
	MOTOR_PARM *motorTemp;
	SensorsInt();
	for(temp = 0; temp < TASK_NUM; temp++)//��ʼ����������Ĳ���
	{
		taskTemp = &tasks.MotorReset + temp;
		taskTemp->taskID = temp+1;
		taskTemp->taskState = TASK_DISABLE;
		taskTemp->progressBar = 0;
		taskTemp->param1 = 0;
		taskTemp->param2 = 0;
		taskTemp->param3 = 0;
		taskTemp->param4 = 0;
		taskTemp->param5 = 0;
		taskTemp->delay1 = 0;
		taskTemp->delay2 = 0;
		taskTemp->delay3 = 0;		
	}
	tasks.pauseFlag = 0;
	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		motorTemp = &Motors.motor1 + temp;
		SetMotorBrake(motorTemp->motorID);
	}
}
/****************************************************
������:	NewTaskInt
��ע: �ƶ�������
****************************************************/
void NewTaskInt(uint16_t *p)
{
	uint16_t taskReady = 0;
	TASK_PARM *taskTemp;

	
	if(taskReady == 1)
	{
		ctrlMode = MODE_AUTO;
		AllTaskReset();
//		taskTemp = &tasks.MotorReset + (p[0] - CMD_MOTOR_RESET);
		taskTemp->param1 = p[1];
		taskTemp->param2 = p[2];
		taskTemp->param3 = p[3];
		taskTemp->param4 = p[4];
		taskTemp->param5 = p[5];
		taskTemp->progressBar = 0;
		taskTemp->taskState = TASK_ENABLE;
	}
}

/**********************************************************
������:	TaskMotorReset
��ע: ���е������λ����tasks.MotorReset.param2���ֳ�ʼ״̬
*************************************************
void TaskMotorReset(void)
{
	uint16_t barAssist = 0;
	if(tasks.MotorReset.progressBar == barAssist++)
	{
		tasks.MotorReset.param2 = 3;
		if(tasks.MotorReset.param1)tasks.MotorReset.param2 = 3;					//��λ����x��һ����		��0����300
		else //�������ʼ������
		{	
			msgToUpload = 0;
			AllTaskReset();				//���ܻ������ĵط�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return;
		}
		tasks.MotorReset.progressBar++;
		tasks.MotorReset.delay1 = 100;
	}

	if(tasks.MotorReset.param2 == 1)			//��������   ��12����12//��-60//��60������//��300������//��0����300
	{
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
	else if(tasks.MotorReset.param2 == 2)		//	lArmPos > 180
	{
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
	}
}
*/

