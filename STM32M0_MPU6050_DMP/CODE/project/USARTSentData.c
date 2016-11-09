//###########################################################################
//
// FILE: USARTSentData.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
// �༭������Ҫ�Ӵ��ڷ��͵����ݣ��Լ�����λ���������������������
// �ڿ���ʱ�䷢�ͻ����е����ݣ��������������з��͵��µ���ʱ(��Ҫ�ǵȴ��ϸ��ֽڷ������)
//###########################################################################

#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "Sensors.h"
#include "main.h"
#include "motorDriver.h"
#include "protocol.h"

void UpLoadState(void);
void UsartSentDataBuf(void);
void Uart1WriteBuf(uint16_t *p, uint16_t num);

uint16_t uart1TxBuf[50];				//����λ���������ݻ���
uint16_t uart1TxBufRdCon = 0;
uint16_t uart1TxBufWtCon = 0;	
uint16_t uart1TxBufWtErr = 0;


/****************************************
������:	UpLoadState
��ע: �ϴ�״̬�����˵�ǰ״̬��ÿ500ms��ʼ��(1�ֽ�/5ms)��Ƶ�ʷ���һ������
****************************************/
void UpLoadState(void)
{
	PROTOCOL_INFO_T* protocol_info = getProtocolInfo_Bytype(STATE_PROTOCOL);
	STATE_PROTOCOL_T* state_protocol = protocol_info->protocol;
	state_protocol->head = 0xFD;
	state_protocol->type = STATE_PROTOCOL;
	state_protocol->para1 = Motor1.motorDir;		//������1�������֣� 
  state_protocol->para2 = Motor1.motorSpeed;	//��������            
  state_protocol->para3 = Motor2.motorDir;   //������2�������֣� 
  state_protocol->para4 = Motor2.motorSpeed;	//��������
  state_protocol->para5 = Motor_H.Angle;	//ͷ���Ƕ�
//  state_protocol->para6 = Motor1.encoder>>24;   											//���Ƕ�            
//  state_protocol->para7 = Motor1.encoder>>16;                     		//�ҳ�Ƕ�            
//  state_protocol->para8 = Motor1.encoder>>8; 		//���ֵ���            
//  state_protocol->para9 = Motor1.encoder; 		//���ֵ���            
//  state_protocol->para10 =  Motor2.encoder>>24;		//ͷ������            
//  state_protocol->para11 =  Motor2.encoder>>16;   //˫�����            
//  state_protocol->para12 =  Motor2.encoder>>8;                    		//Reserved   
//  state_protocol->para13 =  Motor2.encoder;                    		//Reserved   
	if(Pids.PID1.Goal>0){
		state_protocol->para6 = 2;
		state_protocol->para7 = Pids.PID1.Goal>>8;
		state_protocol->para8 = Pids.PID1.Goal;
	}else{
		state_protocol->para6 = 0;
		state_protocol->para7 = (-Pids.PID1.Goal)>>8;
		state_protocol->para8 = (-Pids.PID1.Goal);
	}
	if(Pids.PID1.ExecuteValue>0){
		state_protocol->para9 = 2;
		state_protocol->para10 = Pids.PID1.ExecuteValue>>8;
		state_protocol->para11 = Pids.PID1.ExecuteValue;
	}else{
		state_protocol->para9 = 0;
		state_protocol->para10 = (-Pids.PID1.ExecuteValue)>>8;
		state_protocol->para11 = (-Pids.PID1.ExecuteValue);
	}

  state_protocol->para10 =  0;		//ͷ������            
  state_protocol->para11 =  0;   //˫�����            
  state_protocol->para12 =  0;                    		//Reserved   
  state_protocol->para13 = 0;                    		//Reserved   


//  state_protocol->para6 = 0;   											//���Ƕ�            
//  state_protocol->para7 = 0;                     		//�ҳ�Ƕ�            
//  state_protocol->para8 = Motor1.Current; 		//���ֵ���            
//  state_protocol->para9 = Motor2.Current; 		//���ֵ���            
//  state_protocol->para10 = Motor3.Current;		//ͷ������            
//  state_protocol->para11 = Motor4.Current;   //˫�����            
//  state_protocol->para12 = 0;                    		//Reserved   
//	state_protocol->para13 = (ScramFlag == 1?
//			state_protocol->para13|(1<<0):state_protocol->para13&(~(1<<0))); //������Ϣ��λ ��1       
  state_protocol->para14 = 0;                 		//������Ϣ��λ ��1 
	state_protocol->checksum = 0;
	state_protocol->tail = 0xF8;
	state_protocol_send(protocol_info);
	/*
	#define SEND_DATA_BUF_NUM_1	12
	
	uint16_t msgToUpload;			//������Ϣ
	uint16_t txBuff[12];
	uint16_t ss;
	uint16_t ss2;
	if(Motor1.Speed > 0){
		ss = Motor1.Speed;
	txBuff[1] =  1;    //Motor1.motorSpeed>>8;			//������1����������
	}else{
		ss = -Motor1.Speed;
	txBuff[1] =  2;    //Motor1.motorSpeed>>8;			//������1����������
	}
	if(Motor2.Speed > 0){
		ss2 = Motor2.Speed;
	txBuff[3] =  1;    //Motor1.motorSpeed>>8;			//������1����������
	}else{
		ss2 = -Motor2.Speed;
	txBuff[3] =  2;    //Motor1.motorSpeed>>8;			//������1����������
	}
//���ϴ��������͵�������	
	txBuff[0] =  1;						//֡����
	txBuff[2] =  ss;//Motor1.SpeedStrategy;         //Motor1.motorSpeed;			//������1������ֵ
	txBuff[4] =  ss2;//Motor1.encoder>>16;

	if(Pids.PID1.Goal > 0){
		ss2 = Pids.PID1.Goal;
	txBuff[5] =  1;    //Motor1.motorSpeed>>8;			//������1����������
	}else{
		ss2 = -Pids.PID1.Goal;
	txBuff[5] =  2;    //Motor1.motorSpeed>>8;			//������1����������
	}
	txBuff[6] =  ss2>>2;//Motor1.encoder;			//������3������ֵ

	
	if(Pids.PID1.ExecuteValue > 0){
		ss2 = Pids.PID1.ExecuteValue;
	txBuff[9] =  1;    //Motor1.motorSpeed>>8;			//������1����������
	}else{
		ss2 = -Pids.PID1.ExecuteValue;
	txBuff[9] =  2;    //Motor1.motorSpeed>>8;			//������1����������
	}
	
	msgToUpload = (Motor5.fuseSensor << 4) + (Motor4.fuseSensor << 3) + (Motor3.fuseSensor << 2) + (Motor2.fuseSensor << 1) + Motor1.fuseSensor;
	txBuff[7] =  msgToUpload;			//�ϴ�����Ϣ
	txBuff[8] =  Motor1.Current>>8;
	txBuff[9] =  Motor1.Current;			//������3������ֵ
	txBuff[10] =  Motor2.Current>>8;			//������3����������
	txBuff[11] =  Motor2.Current;			//������3������ֵ
	Uart1WriteBuf(txBuff, SEND_DATA_BUF_NUM_1);*/
}

/****************************************
������:	UsartSentDataBuf
��ע: �ڿ��е�ʱ��ѭ����������
****************************************/
void UsartSentDataBuf(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)		//�ȴ����ݷ���
	{
		if(uart1TxBufRdCon != uart1TxBufWtCon)
		{
			uart1TxBufRdCon++;
			if(uart1TxBufRdCon == 50)uart1TxBufRdCon = 0;
			USART_SendData(USART1, uart1TxBuf[uart1TxBufRdCon]);
		}
	}
}

/****************************************
������:	Uart1WriteBuf
��ע: ��Ҫ���͵�����д�����ͻ���,��������
��֡ͷ,֡β,У���

ʹ���ַ�ת�壺					
FD			F8			FE	
FE 7D		FE 78		FE 7E
****************************************/
void Uart1WriteBuf(uint16_t *p, uint16_t num)
{
	uint16_t	k = 0;
	uint16_t n = 0;
	uint16_t sum = 0;
	uint16_t temp[50];
	//����Ҫд�뻺������ݣ���֡ͷ֡βУ��ͣ������ַ�ת��
	if(num >= 50){uart1TxBufWtErr++; return;}		//���ݳ��ȳ���
	temp[0] = 0xfd;				//֡ͷ
	for(k = 0, n = 1; k < num; k++, n++)
	{
		sum+= p[k];				//���
		if		(p[k] == 0xfd)	{temp[n] = 0xfe; temp[++n] = 0x7d;}
		else if	(p[k] == 0xfe)	{temp[n] = 0xfe; temp[++n] = 0x7e;}
		else if	(p[k] == 0xf8)	{temp[n] = 0xfe; temp[++n] = 0x78;}
		else 					{temp[n] = p[k];}
	}
	temp[n] = sum & 0x00ff;		//У���
	temp[++n] = 0xf8;			//֡β

	//д���ݵ�������
	k = 0;
	num = n+1;//ǰ���������ţ�����ת��ʵ�ʵĸ���
	if(num >= 50){uart1TxBufWtErr++; return;}		//���ݳ��ȳ���
	while(k < num)				
	{
		n = uart1TxBufWtCon + 1;	//����n�����Ժ���һ��δд����ֽ��Ƿ�δ����
		if(n == 50)n = 0;
		if(n == uart1TxBufRdCon)	//���Ƿ�д��δ�������ݵ�ǰһ���ֽ���
		{
			uart1TxBufWtErr++;		//ײ������++
			return;					//����д��
		}
		
		uart1TxBufWtCon++;			//uart1TxBufWtCon��ͣ�����Ѿ�д�����ֽ���
		if(uart1TxBufWtCon == 50)uart1TxBufWtCon = 0;
		uart1TxBuf[uart1TxBufWtCon] = temp[k];
		k++;
	}
}



