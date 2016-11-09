#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "Queue.h"

/*-----�ַ�ת��-----
FD->FE 7D
F8->FE 78
FE->FE 7E		*/
#define RECV_PROTOCOL_NUM 3	//����Э��ʵ������
#define RECV_PROTOCOL_BUFSIZE 50
#define RECV_BUFSIZE 31	

/*����Э������ö��*/
typedef enum {
HEARTBEAT_PROTOCOL = 0x00,	//����Э��
RUN_PROTOCOL = 0x01,	//����ǰ�������˶�Э��
ROTATE_PROTOCOL = 0x02,//ƫ�����˶�������Э��
}RECV_PROTO_TYPE;
/*����Э������ö��*/
typedef enum {
ASK_PROTOCOL = 0x00,//Ӧ��Э��
STATE_PROTOCOL = 0x01,//״̬�ϱ�Э��
}SEND_PROTO_TYPE;


/****************************************************
	�ṹ����:	Protocol_Resolver_T
	����: Э�������
	���ߣ�liyao 2016��9��7��17:55:45
****************************************************/
typedef struct _Protocol_Resolver_T Protocol_Resolver_T;
struct _Protocol_Resolver_T{
	QUEUE_T  *Protocol_Queue;
	uint16_t RPQueue_Size;
	
	uint8_t  *Recv_Buf;
	uint8_t	 Rbuff_Size;
	int8_t 	 Recv_Index; 
	
	int8_t 	 Is_FE;
	uint16_t CheckSum;
	uint8_t  Recv_State;
	int8_t (*Protocol_Put)(Protocol_Resolver_T* pr,uint16_t data);
	void (*Fetch_Protocol)(Protocol_Resolver_T* pr);
};

//typedef struct _Uart_Sender_T Uart_Sender_T;
//struct _Uart_Sender_T Uart_Sender_T{
//	QUEUE_T  *Send_Queue;
//	uint16_t Squeue_Size;
//	uint8_t  *Send_Buf;
//	uint8_t	 Sbuff_Size;
//	int8_t 	 Send_Index;
//};

//###################################����Э����###################################

/****************************************************
	�ṹ����:	ASK_PROTOCOL_T
	����: Ӧ��Э��ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	uint8_t para1;//�������
}ASK_PROTOCOL_T;

/****************************************************
	�ṹ����:	STATE_PROTOCOL_T
	����: ����״̬Э��ʵ��
	���ߣ�liyao 2016��4��4��13:03:42
****************************************************/
typedef struct{
	uint8_t para1;//������	��127
	uint8_t para2;//������	��127
	uint8_t para3;//ƫ���Ǹ�8λ 0-65535
	uint8_t para4;//ƫ���ǵ�8λ
	uint8_t para5;//�߶ȸ�8λ ��λcm
	uint8_t para6;//�߶ȵ�8λ	��λcm
	uint8_t para7;//�Ƿ�������̬����	0�� 1��
	uint8_t para8;//�Ƿ�����ϵ�	0�� 1��
	uint8_t para9;//�������
	uint8_t para10;//���� 0-100
}STATE_PROTOCOL_T;

//###################################����Э����###################################
/****************************************************
	�ṹ����:	HEARTBEAT_PROTOCOL_T
	����:	����Э��ʵ��
	���ߣ�liyao 2016��4��4��13:06:27
	��:		fd 00 09 02 57 f8
****************************************************/
typedef struct{
	uint8_t para1;//������
}HEARTBEAT_PROTOCOL_T;

/****************************************************
	�ṹ����:	RUN_PROTOCOL_T
	����:	�����˶���������Э��ʵ��
	���ߣ�liyao 2016��4��4��13:09:03
	��:	fd 01 02 00 10 01 02 f8
****************************************************/
typedef struct{
	uint8_t para1;//�˶����� T��D��U��B��L��R��S ����ǰ������ͣ
	uint8_t para2;//�ٶȸ�8λ
	uint8_t para3;//�ٶȵ�8λ
}RUN_PROTOCOL_T;


/****************************************************
	�ṹ����:	ROTATE_PROTOCOL_T
	����:	ƫ���ǿ�������Э��ʵ��
	���ߣ�liyao 2016��4��4��13:11:42
	��:	fd 02 02 00 10 01 02 f8
****************************************************/
typedef struct{
	uint8_t para1;//�˶����� L��R��S  B��D �������ҡ�ͣ���ع顢����Ĭ��ƫ��
	uint8_t para2;//�ٶȸ�8λ
	uint8_t para3;//�ٶȵ�8λ
}ROTATE_PROTOCOL_T;

typedef union 
{	
	ASK_PROTOCOL_T						Ask_P;
	STATE_PROTOCOL_T					State_P; 
	
	HEARTBEAT_PROTOCOL_T HeartBeat_P;
	RUN_PROTOCOL_T		Run_P;
	ROTATE_PROTOCOL_T	Rotate_P;
}PROTOCOL_T;
/****************************************************
	�ṹ����:	PROTOCOL_INFO_T
	����: Э����Ϣ����
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct _PROTOCOL_INFO_T PROTOCOL_INFO_T ;
struct _PROTOCOL_INFO_T{
	u8 len;
	u8 head;		//֡ͷ
	u8 type;		//֡����
	u8 serial;	//���
	u8 checksum;//У���
	u8 tail;		//֡β
	PROTOCOL_T protocol;
	void (*handle)(PROTOCOL_INFO_T*);
	int8_t (*check)(void*);
};


//###################################������################################### 
extern Protocol_Resolver_T *UART1_Resolver; 
extern void Protocol_Init(void);
extern PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(void*)); 
int8_t Send_To_Uart(PROTOCOL_INFO_T* protocol_info);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info);
extern void FetchProtocols(void);
extern uint16_t char_special(uint8_t num);
#endif
