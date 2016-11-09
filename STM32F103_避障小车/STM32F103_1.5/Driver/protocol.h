#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "tool.h"




/*-----�ַ�ת��-----
FD->FE 7D
F8->FE 78
FE->FE 7E		*/

#define RECV_PROTOCOL_NUM 3	//����Э��ʵ������
#define RECV_PROTOCOL_BUFSIZE 50
#define RECV_BUFSIZE 50	//���ջ��������ܹ�����һ��Э���С�Ļ�������
#define SEND_BUFSIZE 1000	//���ͻ�����������Э��ʹ�ӡ��Ϣʱ�Ļ�������

/*����Э������ö��*/
typedef enum {
	RUN_PROTOCOL 					= 0x01,
	HEAD_PROTOCOL					= 0x02,
	
	WING_PROTOCOL					= 0x03,
	CUSTOM_PROTOCOL 			= 0x04,
	PDRCORRECT_PROTOCOL		= 0x05,
	RUDDER_PROTOCOL				= 0x06
}RECV_PROTO_TYPE;
/*����Э������ö��*/
typedef enum {
	STATE_PROTOCOL			  = 0x01,
	ASK_PROTOCOL 					= 0x02,
	ODOMETER_PROTOCOL 		= 0x03,
	DEADRECKONING_PROTOCOL = 0x04,
	RESPOND_PROTOCOL			= 0x5
}SEND_PROTO_TYPE;

//###################################����Э����###################################
/****************************************************
	�ṹ����:	STATE_PROTOCOL_T
	����: ������״̬Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//������1�������֣�
	uint8_t para2;//��������
	uint8_t para3;//������2�������֣�
	uint8_t para4;//��������
	uint8_t para5;//ͷ���Ƕ�
	uint8_t para6;//���Ƕ�
	uint8_t para7;//�ҳ�Ƕ�
	uint8_t para8;//���ֵ���
	uint8_t para9;//���ֵ���
	uint8_t para10;//ͷ������
	uint8_t para11;//˫�����
	uint8_t para12;//Reserved
	uint8_t para13;//Reserved
	uint8_t para14;//������Ϣ��λ ��1
	uint8_t checksum;
	uint8_t tail;
}STATE_PROTOCOL_T;

/****************************************************
	�ṹ����:	ODOMETER_PROTOCOL_T
	����: ���������Э��
	���ߣ�tc 2015��9��12��21:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//�������
	uint8_t para2;//�������
	uint8_t para3;//�������
	uint8_t para4;//�������
	uint8_t para5;//�������
	uint8_t para6;//�������
	uint8_t para7;//�������
	uint8_t para8;//�������
	uint8_t para9;//����
	uint8_t para10;//����
	uint8_t checksum;
	uint8_t tail;
}ODOMETER_PROTOCOL_T;

/****************************************************
	�ṹ����:	DEADRECKONING_PROTOCOL_T
	����: ��λ��ϢЭ��
	���ߣ�tc 2015��9��16��15:30:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//��λX����
	uint8_t para2;//��λX����
	uint8_t para3;//��λX����
	uint8_t para4;//��λX����
	uint8_t para5;//��λY����
	uint8_t para6;//��λY����
	uint8_t para7;//��λY����
	uint8_t para8;//��λY����
	uint8_t para9;//��λTH����
	uint8_t para10;//��λTH����
	uint8_t para11;//��λTH����
	uint8_t para12;//��λTH����
	uint8_t para13;//����
	uint8_t para14;//����
	uint8_t checksum;
	uint8_t tail;
}DEADRECKONING_PROTOCOL_T;

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

/****************************************************
	�ṹ����:	RESPOND_PROTOCOL_T
	����: ִ�н������Э��
	���ߣ�liyao 2015��10��16��16:33:41
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//���
	uint8_t para2;//��λ����������
	uint8_t para3;//��λ��֡����
	uint8_t para4;//ָ���Ƿ����
	uint8_t checksum;
	uint8_t tail;
}RESPOND_PROTOCOL_T;

//###################################����Э����###################################
/****************************************************
	�ṹ����:	RUN_PROTOCOL_T
	����:	˫�ֿ�������Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
	��:		fd 01 01 00 32 01 00 32 ff 00 f8
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
	uint8_t checksum;
	uint8_t tail;
}RUN_PROTOCOL_T;


/****************************************************
	�ṹ����:	CUSTOM_PROTOCOL_T
	����:	˫��ָ����������Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
	��:		fd 04 01 00 25 01 ff 01 00 25 01 ff ff  00 f8	
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//���ַ���
	uint8_t para2;//�����ٶȸ�8λ
	uint8_t para3;//�����ٶȵ�8λ
	uint8_t para4;//�����˶�����
	uint8_t para5;//�����˶�����
	uint8_t para6;//���ַ���
	uint8_t para7;//�����ٶȸ�8λ
	uint8_t para8;//�����ٶȵ�8λ
	uint8_t para9;//�����˶�����
	uint8_t para10;//�����˶�����
	uint8_t para11;//���
	uint8_t checksum;
	uint8_t tail;
}CUSTOM_PROTOCOL_T;

/****************************************************
	�ṹ����:	RUDDER_PROTOCOL_T
	����:	˫�ֿ�������Э��ʵ��
	���ߣ�liyao 2015��12��9��11:29:44
	��: fd 06 5A ff 00 f8
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//����������Ƕ�
	uint8_t para2;//���
	uint8_t checksum;
	uint8_t tail;
}RUDDER_PROTOCOL_T;
/****************************************************
	�ṹ����:	HEAD_PROTOCOL_T
	����:	ͷ����������Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
	��:		fd 02 f0 00 88 01 f7 f8
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
	��:		fd 03 10 00 20 00 88 01 f7 f8
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
/****************************************************
	�ṹ����:	PDRCORRECT_PROTOCOL_T
	����:	��λ����Э��ʵ��
	���ߣ�liyao 2015��10��20��13:57:15
	��:		
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//��λX����
	uint8_t para2;//��λX����
	uint8_t para3;//��λX����
	uint8_t para4;//��λX����
	uint8_t para5;//��λY����
	uint8_t para6;//��λY����
	uint8_t para7;//��λY����
	uint8_t para8;//��λY����
	uint8_t para9;//��λTH����
	uint8_t para10;//��λTH����
	uint8_t para11;//��λTH����
	uint8_t para12;//��λTH����
	uint8_t para13;//���
	uint8_t para14;//�Ƿ���ָ��
	uint8_t checksum;
	uint8_t tail;
}PDRCORRECT_PROTOCOL_T;

typedef union 
{	
	STATE_PROTOCOL_T					state_protocol;
	ODOMETER_PROTOCOL_T				odometer_protocol;
	DEADRECKONING_PROTOCOL_T	deadreckoning_protocol;
	ASK_PROTOCOL_T						ask_protocol;
	RESPOND_PROTOCOL_T				respond_protocol;
	
	

	
	RUN_PROTOCOL_T		run_protocol;
//	ANGLE_PROTOCOL_T	angle_protocol;
	HEAD_PROTOCOL_T		head_protocol;
	WING_PROTOCOL_T		wing_protocol;
	PDRCORRECT_PROTOCOL_T PDR_Correnct_Protocol;
	
	RUDDER_PROTOCOL_T					Rudder_Protocol;
	CUSTOM_PROTOCOL_T					Custom_Protocol;
}PROTOCOL_T;
/****************************************************
	�ṹ����:	PROTOCOL_INFO_T
	����: Э����Ϣ����
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct _PROTOCOL_INFO_T PROTOCOL_INFO_T;
struct _PROTOCOL_INFO_T{
	int8_t len;
	int8_t type;
	PROTOCOL_T protocol;
	void (*handle)(PROTOCOL_INFO_T*);
	int8_t (*check)(PROTOCOL_INFO_T*);
};


//###################################������###################################
extern QUEUE_T* Recv_Protocol_Queue;
extern int8_t heart_flag;
extern PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM];
extern void Protocol_Init(void);
extern PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(PROTOCOL_INFO_T*));
extern int8_t put_byte(uint16_t data);
//int8_t _Send_To_Uart(PROTOCOL_INFO_T* protocol_info);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info);
extern void ask_send(uint8_t serial_number);
extern void state_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void odometer_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void deadreckoning_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void respond_protocol_send(PROTOCOL_INFO_T* pi);
extern void Buff_To_Uart(void);
#endif
