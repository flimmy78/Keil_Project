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
#include "protocol.h"

#define MOTOR_LAST_DELAY	10			//�ڶ��û���յ���λ�������Ժ�ֹͣ  80*USARTCMD����
USARTCMDRXBUF usartCMDRxBuf;
uint16_t manualUpdateTimeOut = 0;


void DealUsartCMD(void);
void UsartCMDRxBufInt(void);
void UsartCMDManualInt(PROTOCOL_INFO_T* protocol_info);	//�ֶ����Ƴ�ʼ��
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
	if(protocol_flag > -1){
		UsartCMDManualInt(&recv_protocol_infos[protocol_flag]);
		protocol_flag = -1;
	}
	/*liyao
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
		else;
		p[0]=p[1]=p[2]=p[3]=p[4]=p[5]=p[6]=p[7]=p[8]=p[9]=p[10]=p[11]=p[12]=p[13]=p[14]=p[15]=0;//ȫ������
	}	
	if(ctrlMode == MODE_MANUAL) UsartCMDManual();*/
}
/****************************************************
������:	UsartCMDManualInt
��ע:�ֶ�ģʽ����λ�������������ʼ��
****************************************************/
void UsartCMDManualInt(PROTOCOL_INFO_T* protocol_info)
{
	ctrlMode = MODE_MANUAL;
	heart_flag = 1;
	if(protocol_info->type == RUN_PROTOCOL){
		RUN_PROTOCOL_T* rp = protocol_info->protocol;
		if(protocol_info->check(rp))//Э��У��
			return;
		ask_send(rp->para7);
		if(rp->para8 == 1 || HeartIT == 1){ 
			HeartIT = 0;
			SetMotorParam(&MOTOR_RING_L, rp->para2 << 8 | rp->para3, rp->para1);
			SetMotorParam(&MOTOR_RING_R, rp->para5 << 8 | rp->para6, rp->para4);
		}
	}
	if(protocol_info->type == ANGLE_PROTOCOL){
		ANGLE_PROTOCOL_T* ap =  protocol_info->protocol;
		if(protocol_info->check(ap))//Э��У��
			return;
		ask_send(ap->para16);
		if(ap->para16 == 1){
			SetMotorParam2(&MOTOR_RING_L, ap->para2 << 8 | ap->para3, ap->para1, ap->para4 << 24 | ap->para5 << 16 | ap->para6 << 8 | ap->para7);
			SetMotorParam2(&MOTOR_RING_R, ap->para9 << 8 | ap->para10, ap->para8, ap->para11 << 24 | ap->para12 << 16 | ap->para13 << 8 | ap->para14);
		}
		
		/*
		ִ
		��
		��
		��
		*/
	}
	/* liyao
	int16_t cmdDir;
	uint16_t cmdspeed;
	
	ctrlMode = MODE_MANUAL;
	
	if(p[0] == CMD_MOTOR_MANUAL_1)
	{
		cmdspeed = p[2];			
		if(p[1] == 2){
			cmdDir = DIR_MOTOR_B;
		}
		else if(p[1] == 1){
			cmdDir = DIR_MOTOR_A;
		}			
		else cmdspeed = 0;										//����ֵ������Ҫ��
		SetMotorParam(ID_MOTOR_L, cmdspeed, cmdDir);
		cmdspeed = p[4];			
		if(p[3] == 2){
			cmdDir = DIR_MOTOR_B;
		}
		else if(p[3] == 1){
			cmdDir = DIR_MOTOR_A;
		}			
		else cmdspeed = 0;										//����ֵ������Ҫ��
		SetMotorParam(ID_MOTOR_R, cmdspeed, cmdDir);
	}	*/
	ScramFlag = 0;
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
			ScramFlag = 1;
			SetMotorParam(&MOTOR_RING_L, 0, 1);
			SetMotorParam(&MOTOR_RING_R, 0, 1);
			SetMotorParam(&MOTOR_HEAD, 0, 1);
			SetMotorParam(&MOTOR_WING_L, 0, 1);
			SetMotorParam(&MOTOR_WING_R, 0, 1);
		}
	}
}

