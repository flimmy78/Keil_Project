//###########################################################################
//
// FILE:    	USARTCMD.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//    ������λ��������ָ��
//###########################################################################
#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
#include "main.h"

#define MOTOR_LAST_DELAY	80			//�ڶ��û���յ���λ�������Ժ�ֹͣ  80*USARTCMD����
USARTCMDRXBUF usartCMDRxBuf;
uint16_t manualUpdateTimeOut = 0;


void DealUsartCMD(void);
void UsartCMDRxBufInt(void);
void UsartCMDManualInt(uint16_t *p);	//�ֶ����Ƴ�ʼ��
void UsartCMDManual(void);		//�ֶ����Ƶ��
/****************************************************
������:	UsartCMDRxBufInt
��ע: ��ʼ�����ڽ��ܻ�����
****************************************************/
void UsartCMDRxBufInt(void)
{
	usartCMDRxBuf.usartCMDBufCon = 0;
	usartCMDRxBuf.usartCMDRxBuf0[0] = 0;//֡����Ϊ0ʱ��ʾ���˻����е������Ѿ������  
	usartCMDRxBuf.usartCMDRxBuf1[0] = 0;//  
	usartCMDRxBuf.usartCMDRxBuf2[0] = 0;//  
	usartCMDRxBuf.usartCMDRxBuf3[0] = 0;//  
	usartCMDRxBuf.usartCMDRxBuf4[0] = 0;//  
}

/*****************************************************************
������:DealUsartCMD
��ע: ������λ�����������ܺ�������������˶�������ģʽ����ȡ��Ϣ��
******************************************************************/
void DealUsartCMD(void)
{
	uint16_t temp;
	uint16_t *p;
	temp = usartCMDRxBuf.usartCMDBufCon + 1;	//�ӵ�ǰ������д���ַ�ĺ�һ����ַ��ȡ���ݣ�������ʷ��Զ���ݿ�ʼ��ȡ���ٶ����������㹻�������ܱ��������ݸ���
	if(temp == 5)temp = 0;	
	while(temp != usartCMDRxBuf.usartCMDBufCon)
	{
		p = usartCMDRxBuf.usartCMDRxBuf0 + temp * USART_CMD_RXBUR_NUM;	//  
		temp++;
		if(temp == 5)temp = 0;	
		if(p[0] == 0);
		else if(p[0] == CMD_MOTOR_MANUAL_1)	{UsartCMDManualInt(p);		p[0] = 0;}
		else if(p[0] == CMD_MOTOR_MANUAL_2)	{UsartCMDManualInt(p); 		p[0] = 0;}
		else;
		p[0]=p[1]=p[2]=p[3]=p[4]=p[5]=p[6]=p[7]=p[8]=p[9]=p[10]=p[11]=p[12]=p[13]=p[14]=p[15]=0;//ȫ������
	}	
	if(ctrlMode == MODE_MANUAL)UsartCMDManual();
}
/****************************************************
������:	UsartCMDManualInt
��ע:�ֶ�ģʽ����λ�������������ʼ��
p[0] ֡����
p[1] 1����
p[2] �ٶȸ�8λ
p[3] �ٶȵ�8λ
p[4] 2����
p[5] �ٶȸ�8λ
p[6] �ٶȵ�8λ
p[7] 3����
p[8] �ٶȸ�8λ
p[9] �ٶȵ�8λ

****************************************************/
void UsartCMDManualInt(uint16_t *p)
{
	int16_t cmdDir;
	uint16_t cmdspeed;
	
	ctrlMode = MODE_MANUAL;
	
	if(p[0] == CMD_MOTOR_MANUAL_1)
	{
		cmdspeed = (p[2] << 8)+ p[3];			
		if(p[1] == 2)cmdDir = DIR_MOTOR_L_F;
		else if(p[1] == 0)cmdDir = DIR_MOTOR_L_B;	
		else cmdspeed = 0;										//����ֵ������Ҫ��
		SetMotorParam(ID_MOTOR_L, cmdspeed, cmdDir);
		
		cmdspeed = (p[5] << 8)+ p[6];			
		if(p[4] == 2)cmdDir = DIR_MOTOR_R_F;
		else if(p[4] == 0) cmdDir = DIR_MOTOR_R_B;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_R, cmdspeed, cmdDir);
	}
	else if(p[0] == CMD_MOTOR_MANUAL_2)
	{
		cmdspeed = (p[2] << 8)+ p[3];			
		if(p[1] == 2)cmdDir = DIR_MOTOR_H_L;
		else if(p[1] == 0) cmdDir = DIR_MOTOR_H_R;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_H, cmdspeed, cmdDir);
		
		cmdspeed = (p[5] << 8)+ p[6];			
		if(p[4] == 2)cmdDir = DIR_MOTOR_LC_U;
		else if(p[4] == 0) cmdDir = DIR_MOTOR_LC_D;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_LC, cmdspeed, cmdDir);
		
		cmdspeed = (p[8] << 8)+ p[9];			
		if(p[7] == 2)cmdDir = DIR_MOTOR_RC_U;
		else if(p[7] == 0) cmdDir = DIR_MOTOR_RC_D;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_RC, cmdspeed, cmdDir);
	}
	else cmdspeed = 0;
	
	manualUpdateTimeOut = MOTOR_LAST_DELAY;			//��λ���������ݣ���manualUpdateTimeOut����
}

/****************************************************
������:	UsartCMDManual
��ע:�ֶ�ģʽ�µĵ������
****************************************************/
void UsartCMDManual(void)
{
	if(manualUpdateTimeOut)
	{
		manualUpdateTimeOut--;
		if(manualUpdateTimeOut == 0)
		{
			SetMotorParam(ID_MOTOR_L, 0, Motors.motor1.motorDir);
			SetMotorParam(ID_MOTOR_R, 0, Motors.motor2.motorDir);
			SetMotorParam(ID_MOTOR_H, 0, Motors.motor3.motorDir);
			SetMotorParam(ID_MOTOR_LC, 0, Motors.motor4.motorDir);
			SetMotorParam(ID_MOTOR_RC, 0, Motors.motor5.motorDir);
		}
	}
}

