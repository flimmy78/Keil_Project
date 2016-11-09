//###########################################################################
//
// FILE:    	Sensor.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//  ��ȡ��������Ϣ
//###########################################################################
#include <stdio.h>
#include <string.h>
#include "stm32f0xx.h"
#include "main.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
#include "Strategy.h"
#include "USARTSentData.h"

 
void SensorsInt(void);			//��������ʼ��
void DealSensors(void);
void ReadCurrent(void);
void ReadFuse(void);
int16_t GetSpeed(MOTOR_PARM *motor);
uint32_t cal_ADC_value[ADC_CHANNEL_NUM];//�洢ƽ��ֵ������ֵ
/****************************************************
������:DealSensors
��ע: ���������ź��ܺ���
****************************************************/
void DealSensors(void)
{
	ReadFuse();
}


/****************************************************
������:SensorsInt
��ע: ��������ʼ������
****************************************************/
void SensorsInt(void)			//��������ʼ��
{
//	�������ͱ���˿״̬��ʼ�������Ѿ���MotorInt()�����

}

/****************************************************
������:GetSpeed
��ע: �������ٶ�
****************************************************/
int16_t GetSpeed(MOTOR_PARM *motor)
{		
	int32_t encodeTemp=0;
//	int32_t timeTemp=0;
	uint32_t eTemp=0;
	uint32_t tTemp=0;	

	eTemp = motor->encoder;
	tTemp = timeTick;	
	encodeTemp = eTemp - motor->encoderLast;
//	timeTemp = tTemp - motor->SpeedTimeLast;
	motor->encoderLast = eTemp;
	motor->SpeedTimeLast = tTemp;	
	
	//motor->Speed = (SpeedPeriod * encodeTemp)/timeTemp;	
	motor->Speed = encodeTemp;	
	if(motor->Speed > 0){
		motor->motorDir = DIR_MOTOR_A;
		motor->motorSpeed = motor->Speed;
	}else{
		motor->motorDir = DIR_MOTOR_B;
		motor->motorSpeed = -motor->Speed;
	}		

	return motor->Speed;
}

/****************************************************
������:DealEncoder
��ע: ������AD�ɼ�
****************************************************/
void ReadCurrent(void)
{	
	uint16_t i=0,j=0;	
	memset(cal_ADC_value,0,ADC_CHANNEL_NUM);//������� liyao
	// ���в���ȡƽ��ֵ	liyao
	for(j = 0; j < ADC_CHANNEL_NUM; j++){
		for(i = 0; i < ADC_SAMPLING_NUM; i++)
			cal_ADC_value[j] += ADC_valueBuf[i][j];
		cal_ADC_value[j] /= ADC_SAMPLING_NUM;
		if(cal_ADC_value[i]>0x0fff)
			cal_ADC_value[i]=0x0fff;
	}
	
	/*for(j = 0; j < ADC_SAMPLING_NUM; j++)
		for(i = 0; i < ADC_CHANNEL_NUM; i++)
			cal_ADC_value[i] += ADC_valueBuf[j][i];
	//	���в������ۼ�ȡƽ��ֵ	liyao
	for(i = 0; i < ADC_CHANNEL_NUM; i++){
		cal_ADC_value[i] = cal_ADC_value[i] / ADC_SAMPLING_NUM;
		if(cal_ADC_value[i]>0x0fff)
			cal_ADC_value[i]=0x0fff;
	}*/
	
	Motor1.CurrentLast = Motor1.Current;
	Motor2.CurrentLast = Motor2.Current;
	Motor_H.ElectricityLast = Motor_H.Electricity;
	Motor_WL.ElectricityLast = Motor_WL.Electricity;
	Motor_WR.ElectricityLast = Motor_WR.Electricity; 
	
	Motor1.Current = cal_ADC_value[ADC_L_CHANNEL];
	Motor2.Current = cal_ADC_value[ADC_R_CHANNEL];
	Motor_H.Current = cal_ADC_value[ADC_H_CHANNEL];// 
	Motor_H.Angle = 10 * cal_ADC_value[ADC_H_Angle_CHANNEL];//Kalman_Filter(10 * cal_ADC_value[ADC_H_Angle_CHANNEL],&Motor_H.kalman);
	Motor_WL.Electricity = cal_ADC_value[ADC_W_CHANNEL];
	Motor_WR.Electricity = cal_ADC_value[ADC_W_CHANNEL];
	
	//Motor3.motorSpeed = cal_ADC_value[ADC_H_Angle_CHANNEL];
	
	Motor1.deltaCurrent = Motor1.Current - Motor1.CurrentLast;
	Current_Strategy(&Motor1);
	Current_Strategy(&Motor2);
	
	//------------�������------------------
	if((Motor1.CurrentStrategy == OBSTACLE_CURRENT) || (Motor2.CurrentStrategy == OBSTACLE_CURRENT) ){
		Pids.PID1.ExecuteValue = 0;
		Pids.PID2.ExecuteValue = 0;
		Pids.PID1.Goal = 0;
		Pids.PID2.Goal = 0;
		Motor1.SpeedCMD = 0;
		Motor2.SpeedCMD = 0;
	}	
}

/****************************************************
������:ReadFuse
��ע:��ȡ����˿
****************************************************/
void ReadFuse(void)
{
	Motor1.fuseSensor = !GET_FUSE_SEN_L;
	Motor2.fuseSensor = !GET_FUSE_SEN_R;
	Motor_H.FuseSensor = !GET_FUSE_SEN_HW;
}

