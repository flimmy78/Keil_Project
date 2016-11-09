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
#include "stm32f0xx.h"
#include "main.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
  
void SensorsInt(void);			//��������ʼ��
void DealSensors(void);
void ReadEncoder(void);
void ReadFuse(void);

uint16_t encoder1PinTemp;				//�����ã��洢��ʱ�ı���
uint16_t encoder1PinStable;				//�����ã��洢��ʱ�ı���
uint16_t encoder1PinTempTimeCon = 0;	//�����ã�������ʱ��

uint16_t encoder2PinTemp;				//�����ã��洢��ʱ�ı���
uint16_t encoder2PinStable;				//�����ã��洢��ʱ�ı���
uint16_t encoder2PinTempTimeCon = 0;	//�����ã�������ʱ�

uint16_t encoder3PinTemp;				//�����ã��洢��ʱ�ı���
uint16_t encoder3PinStable;				//�����ã��洢��ʱ�ı���
uint16_t encoder3PinTempTimeCon = 0;	//�����ã�������ʱ��

uint16_t encoder1Last = ENCODER_CON_RES;//�����洢��һ�ֱ�����������
uint16_t encoder2Last = ENCODER_CON_RES;//�����洢��һ�ֱ�����������
uint16_t encoder3Last = ENCODER_CON_RES;//�����洢��һ�ֱ�����������

/****************************************************
������:DealSensors
��ע: ���������ź��ܺ���
****************************************************/
void DealSensors(void)
{
	ReadEncoder();
	ReadFuse();
}
/****************************************************
������:SensorsInt
��ע: ��������ʼ������
****************************************************/
void SensorsInt(void)			//��������ʼ��
{
//	�������ͱ���˿״̬��ʼ�������Ѿ���MotorInt()�����
	encoder1PinTemp = GET_ENCODER1_A;	
	encoder1PinStable = GET_ENCODER1_A;
	encoder2PinTemp = GET_ENCODER2_A;	
	encoder2PinStable = GET_ENCODER2_A;
	encoder3PinTemp = GET_ENCODER3_A;	
	encoder3PinStable = GET_ENCODER3_A;
}
/****************************************************
������:ReadSensors
��ע: ��ȡ���������
****************************************************/
void ReadEncoder(void)
{
//��ȡ������1	
	if(encoder1PinTempTimeCon)
	{	
		if(encoder1PinTemp != GET_ENCODER1_A )encoder1PinTempTimeCon = 0;		//	��������������з����ı䣬�����ʱ�����㣬�ȶ�ֵ���ı�
		else if(--encoder1PinTempTimeCon == 0)								//	��������0������Ϊ���ȶ��ġ�
		{
			encoder1PinStable = encoder1PinTemp;
			if(encoder1PinStable == 0)
			{
				if(GET_ENCODER1_B)Motors.motor1.encoder++;			//	A�����仯ʱ����A�ı�Ե����ʱ����������ΪB���ȶ�״̬������B��������
				else Motors.motor1.encoder--;
			}
			else
			{
				if(GET_ENCODER1_B)Motors.motor1.encoder--;
				else Motors.motor1.encoder++;
			}
		}
	}
	else if(encoder1PinStable != GET_ENCODER1_A)
	{
		encoder1PinTemp = GET_ENCODER1_A;
		encoder1PinTempTimeCon = 1;
	}
//��ȡ������2	
	if(encoder2PinTempTimeCon)
	{	
		if(encoder2PinTemp != GET_ENCODER2_A )encoder2PinTempTimeCon = 0;		//	��������������з����ı䣬�����ʱ�����㣬�ȶ�ֵ���ı�
		else if(--encoder2PinTempTimeCon == 0)								//	��������0������Ϊ���ȶ��ġ�
		{
			encoder2PinStable = encoder2PinTemp;
			if(encoder2PinStable == 0)
			{
				if(GET_ENCODER2_B)Motors.motor2.encoder++;			//	A�����仯ʱ����A�ı�Ե����ʱ����������ΪB���ȶ�״̬������B��������
				else Motors.motor2.encoder--;
			}
			else
			{
				if(GET_ENCODER2_B)Motors.motor2.encoder--;
				else Motors.motor2.encoder++;
			}
		}
	}
	else if(encoder2PinStable != GET_ENCODER2_A)
	{
		encoder2PinTemp = GET_ENCODER2_A;
		encoder2PinTempTimeCon = 1;
	}
//��ȡ������3	
	if(encoder3PinTempTimeCon)
	{	
		if(encoder3PinTemp != GET_ENCODER3_A )encoder3PinTempTimeCon = 0;		//	��������������з����ı䣬�����ʱ�����㣬�ȶ�ֵ���ı�
		else if(--encoder3PinTempTimeCon == 0)								//	��������0������Ϊ���ȶ��ġ�
		{
			encoder3PinStable = encoder3PinTemp;
			if(encoder3PinStable == 0)
			{
				if(GET_ENCODER3_B)Motors.motor3.encoder++;			//	A�����仯ʱ����A�ı�Ե����ʱ����������ΪB���ȶ�״̬������B��������
				else Motors.motor3.encoder--;
			}
			else
			{
				if(GET_ENCODER3_B)Motors.motor3.encoder--;
				else Motors.motor3.encoder++;
			}
		}
	}
	else if(encoder3PinStable != GET_ENCODER3_A)
	{
		encoder3PinTemp = GET_ENCODER3_A;
		encoder3PinTempTimeCon = 1;
	}
}

/****************************************************
������:ReadFuse
��ע:��ȡ����˿
****************************************************/
void ReadFuse(void)
{
	Motors.motor1.fuseSensor = !GET_FUSE_SEN_L;
	Motors.motor2.fuseSensor = !GET_FUSE_SEN_R;
	Motors.motor3.fuseSensor = !GET_FUSE_SEN_HW;
}

