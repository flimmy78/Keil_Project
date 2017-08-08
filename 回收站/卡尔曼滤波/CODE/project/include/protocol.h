#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include <stm32f0xx.h>
#include <stdio.h>
#include <string.h>
#include "tool.h"
/*-----�ַ�ת��-----
FD->FE 7D
F8->FE 78
FE->FE 7E		*/

#define SEND_PROTOCOL_NUM 2	//����Э��ʵ������
#define RECV_PROTOCOL_NUM 4	//����Э��ʵ������
#define RECV_BUFSIZE 50	//���ջ��������ܹ�����һ��Э���С�Ļ�������
#define SEND_BUFSIZE 500	//���ͻ�����������Э��ʹ�ӡ��Ϣʱ�Ļ�������

/*����Э������ö��*/
typedef enum {RUN_PROTOCOL = 0x01,HEAD_PROTOCOL = 0x02,WING_PROTOCOL = 0x03,ANGLE_PROTOCOL = 0x4}RECV_PROTO_TYPE;
/*����Э������ö��*/
typedef enum {STATE_PROTOCOL = 0x01,ASK_PROTOCOL = 0x02}SEND_PROTO_TYPE;
/****************************************************
	�ṹ����:	PROTOCOL_INFO_T
	����: Э����Ϣ����
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	int8_t len;
	int8_t type;
	int8_t state;//-1 �� 0 ����� 1������(Э�����)
	void* protocol;
	void (*handle)(void);
	int8_t (*check)(void*);
}PROTOCOL_INFO_T;

//###################################����Э����###################################
/****************************************************
	�ṹ����:	STATE_PROTOCOL_T
	����: ������״̬Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	int8_t para1;//������1�������֣�
	int8_t para2;//��������
	int8_t para3;//������2�������֣�
	int8_t para4;//��������
	int8_t para5;//ͷ���Ƕ�
	int8_t para6;//���Ƕ�
	int8_t para7;//�ҳ�Ƕ�
	int8_t para8;//���ֵ���
	int8_t para9;//���ֵ���
	int8_t para10;//ͷ������
	int8_t para11;//˫�����
	int8_t para12;//Reserved
	int8_t para13;//Reserved
	int8_t para14;//������Ϣ��λ ��1
	uint8_t checksum;
	uint8_t tail;
}STATE_PROTOCOL_T;

/****************************************************
	�ṹ����:	ASK_PROTOCOL_T
	����: Ӧ��Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//���
	uint8_t checksum;
	uint8_t tail;
}ASK_PROTOCOL_T;

//###################################����Э����###################################
/****************************************************
	�ṹ����:	RUN_PROTOCOL_T
	����:	˫�ֿ�������Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	int8_t para1;//���ַ���
	uint8_t para2;//�����ٶȸ�8λ
	uint8_t para3;//�����ٶȵ�8λ
	int8_t para4;//���ַ���
	uint8_t para5;//�����ٶȸ�8λ
	uint8_t para6;//�����ٶȵ�8λ
	uint8_t para7;//���
	int8_t para8;//�Ƿ���ָ��
	uint8_t checksum;
	uint8_t tail;
}RUN_PROTOCOL_T;

/****************************************************
	�ṹ����:	RUN_PROTOCOL_T
	����:	˫��ָ���Ƕ�����Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//���ַ���
	uint8_t para2;//�����ٶȸ�8λ
	uint8_t para3;//�����ٶȵ�8λ
	uint8_t para4;//�����˶�����
	uint8_t para5;//�����˶�����
	uint8_t para6;//�����˶�����
	uint8_t para7;//�����˶�����
	uint8_t para8;//���ַ���
	uint8_t para9;//�����ٶȸ�8λ
	uint8_t para10;//�����ٶȵ�8λ
	uint8_t para11;//�����˶�����
	uint8_t para12;//�����˶�����
	uint8_t para13;//�����˶�����
	uint8_t para14;//�����˶�����
	uint8_t para15;//���
	int8_t para16;//�Ƿ���ָ��
	uint8_t checksum;
	uint8_t tail;
}ANGLE_PROTOCOL_T;

/****************************************************
	�ṹ����:	HEAD_PROTOCOL_T
	����:	ͷ����������Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//ͷ���Ƕ�
	uint8_t para2;//ͷ���ٶ�
	uint8_t para3;//���
	uint8_t para4;//�Ƿ���ָ��
	uint8_t checksum;
	uint8_t tail;
}HEAD_PROTOCOL_T;

/****************************************************
	�ṹ����:	WING_PROTOCOL_T
	����:	����������Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	int8_t para1;//���Ƕ�
	int8_t para2;//����ٶ�
	int8_t para3;//�ҳ�Ƕ�
	int8_t para4;//�ҳ��ٶ�
	int8_t para5;//���
	int8_t para6;//�Ƿ���ָ��
	uint8_t checksum;
	uint8_t tail;
}WING_PROTOCOL_T;
//###################################������###################################
extern int8_t protocol_flag;
extern int8_t heart_flag;
extern PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM];
extern void Protocol_Init(void);
extern int8_t put_byte(uint16_t data);
//int8_t _Send_To_Uart(PROTOCOL_INFO_T* protocol_info);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info);
extern PROTOCOL_INFO_T* getProtocolInfo_Bytype(SEND_PROTO_TYPE type);
extern void ask_send(uint8_t serial_number);
extern void state_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void Buff_To_Uart(void);
#endif
