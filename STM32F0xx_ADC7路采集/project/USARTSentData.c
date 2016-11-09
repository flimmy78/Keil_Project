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

void UpLoadState(void);
void UsartSentDataBuf(void);
void Uart1WriteBuf(uint16_t *p, uint16_t num);

uint16_t uart1TxBuf[50];				//����λ���������ݻ���
uint16_t uart1TxBufRdCon = 0;
uint16_t uart1TxBufWtCon = 0;	
uint16_t uart1TxBufWtErr = 0;


void UpLoadState1(void)
{
	uint16_t txBuff[10];
	if(timeOverFlag)
	{
		txBuff [0] = timeOver;	
		Uart1WriteBuf(txBuff, 1);
		timeOverFlag = 0;
	}
}

/****************************************
������:	UpLoadState
��ע: �ϴ�״̬�����˵�ǰ״̬��ÿ500ms��ʼ��(1�ֽ�/5ms)��Ƶ�ʷ���һ������
****************************************/
void UpLoadState(void)
{
	#define SEND_DATA_BUF_NUM_1	8
	
	uint16_t msgToUpload;			//������Ϣ
	uint16_t txBuff[10];

	uint16_t encoderInc1 = 0;
	uint16_t encoderInc2 = 0;
	uint16_t encoderInc3 = 0;

	uint16_t encoderIncDir1 = 0;
	uint16_t encoderIncDir2 = 0;
	uint16_t encoderIncDir3 = 0;
	
//	��ñ������������������ֵ
//	������1
	if(Motors.motor1.encoder >= encoder1Last)
	{
		encoderIncDir1 = 2;										//�趨Ϊ1����
		encoderInc1 = Motors.motor1.encoder - encoder1Last;
	}
	else 
	{
		encoderIncDir1 = 0;										//�趨Ϊ-1�������ͨ��Э��
		encoderInc1 = encoder1Last - Motors.motor1.encoder;
	}
	encoder1Last = Motors.motor1.encoder;
	if(encoder1Last > 55000 || encoder1Last < 500)
	{
		encoder1Last = Motors.motor1.encoder = ENCODER_CON_RES;
	}
//	������2
	if(Motors.motor2.encoder >= encoder2Last)
	{
		encoderIncDir2 = 2;										//�趨Ϊ1����
		encoderInc2 = Motors.motor2.encoder - encoder2Last;
	}
	else 
	{
		encoderIncDir2 = 0;										//�趨Ϊ-1�������ͨ��Э��
		encoderInc2 = encoder2Last - Motors.motor2.encoder;
	}
	encoder2Last = Motors.motor2.encoder;
	if(encoder2Last > 55000 || encoder1Last < 500)
	{
		encoder2Last = Motors.motor2.encoder = ENCODER_CON_RES;
	}
	
//	������3
	if(Motors.motor3.encoder >= encoder3Last)
	{
		encoderIncDir3 = 2;										//�趨Ϊ1����
		encoderInc3 = Motors.motor3.encoder - encoder3Last;
	}
	else 
	{
		encoderIncDir3 = 0;										//�趨Ϊ-1�������ͨ��Э��
		encoderInc3 = encoder3Last - Motors.motor3.encoder;
	}
	encoder3Last = Motors.motor3.encoder;
	if(encoder3Last > 55000 || encoder3Last < 500)
	{
		encoder3Last = Motors.motor3.encoder = ENCODER_CON_RES;
	}
	
/*	//Debug
	encoderInc1 = Motors.motor1.encoder - ENCODER_CON_RES;
	encoderInc2 = Motors.motor2.encoder - ENCODER_CON_RES;
	encoderInc3 = Motors.motor3.encoder - ENCODER_CON_RES;
	// endDebug
*/
//���ϴ��������͵�������	
	txBuff[0] =  1;						//֡����
	txBuff[1] =  encoderIncDir1;			//������1����������
	txBuff[2] =  encoderInc1;			//������1������ֵ
	txBuff[3] =  encoderIncDir2;			
	txBuff[4] =  encoderInc2;
	txBuff[5] =  encoderIncDir3;			//������3����������
	txBuff[6] =  encoderInc3;			//������3������ֵ

	msgToUpload = (Motors.motor5.fuseSensor << 4) + (Motors.motor4.fuseSensor << 3) + (Motors.motor3.fuseSensor << 2) + (Motors.motor2.fuseSensor << 1) + Motors.motor1.fuseSensor;
	txBuff[7] =  msgToUpload;			//�ϴ�����Ϣ
	Uart1WriteBuf(txBuff, SEND_DATA_BUF_NUM_1);

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



