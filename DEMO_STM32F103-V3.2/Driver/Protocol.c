#include "stm32f10x.h"
#include "Protocol.h"
#include "STM32_DevInit.h"
#include "ProtocolHandle.h"
//#define PRINT_ERR
//###################################���������###################################
//PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//����Э��ջ
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//����Э��ջ 
//static uint8_t _Recv_Protocol_Arr[RECV_PROTOCOL_NUM] = {0};//�Ѿ����յ�Э�������  
//###################################���ڱ�����###################################
PROTOCOL_INFO_T _Recv_Protocol_Buff[RECV_PROTOCOL_BUFSIZE] = {0};//����Э�����

#define UART1_RPQUEUE_SIZE		30 //����Э�黺�������洢����Э�飩
#define UART1_RBUFF_SIZE		60 //�����ֽڻ��������ܹ�����һ��Э���С�Ļ�������
#define UART1_SQUEUE_SIZE 1000 //���ڷ��Ͷ��л�����
#define UART1_SBUFF_SIZE		100//DMA�����ֽڻ����� 

Protocol_Resolver_T _UART1_Resolver;
PROTOCOL_INFO_T _UART1_Protocol_QueueBuf[UART1_RPQUEUE_SIZE] = {0};
uint8_t _UART1_Recv_Buf[UART1_RBUFF_SIZE] = {0}; 

uint8_t _UART1_Send_Queue[UART1_SQUEUE_SIZE] = {0};
uint8_t _UART1_Send_Buf[UART1_SBUFF_SIZE] = {0};
Protocol_Resolver_T *UART1_Resolver = &_UART1_Resolver;

PROTOCOL_INFO_T _tmp_pi = {0};
PROTOCOL_INFO_T* tmp_pi = &_tmp_pi;
//----------------------------------------------------- 
 
static uint8_t _send_index = 0x00;  
//###################################���ں�����###################################
/****************************************************
	������:	clean_recv_buf
	����:		���Э��ջ����д���Э��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void _clean_recv_buf(Protocol_Resolver_T* pr){ 
	pr->Is_FE = pr->Recv_Index = pr->Recv_State = 0; 
	memset(pr->Recv_Buf, 0, pr->Rbuff_Size); 
}

/****************************************************
	������:	_Fetch_Protocol
	����:		��ȡ��ִ���Ѿ������Э��
	����:		liyao 2016��9��8��10:54:34
****************************************************/
void _Fetch_Protocol(Protocol_Resolver_T* pr){
	PROTOCOL_INFO_T pi;
	while(Queue_Get(pr->Protocol_Queue,&pi) == 0){
		/*if(pi.handle != NULL){
			if(pi.check(&pi) < 0)
				break;
		}*/
		if(pi.handle != NULL){
			pi.handle(&pi);
		}
	}
}



/****************************************************
	������:	Protocol_Put
	����:		����Э�����ݲ�������װ
	����:		Э������
	ע��: 	ͨ��protocol_flag��־λ��ʾ�Ƿ�������µ�Э��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t _Protocol_Put(Protocol_Resolver_T* pr,uint16_t data){
	uint8_t i,loop, protocol_type = 0; 
	PROTOCOL_INFO_T* pi = NULL;  
	if(pr->Recv_Index == RECV_BUFSIZE)//��������Խ�� 
		_clean_recv_buf(pr);  
	//--------�洢����----------
	pr->Recv_Buf[pr->Recv_Index++] = data;
	
	//Э�����״̬��
	do{
		switch(pr->Recv_State){
			case 0:	//����֡ͷ
						if(data == 0xFD){
							pr->Recv_State++; 
						} else{
							_clean_recv_buf(pr); 
						} 
						break;
			case 1:	//�����������
						if(data == 0xF8){
							pr->Recv_State++;
							loop = 1;
						}else if(data == 0xFE){ 
							pr->Is_FE = 1; 
						}else if(pr->Is_FE){
							switch(data){
								case 0x7D: data = 0xFD;break;
								case 0x78: data = 0xF8;break;
								case 0x7E: data = 0xFE;break;
							}
							pr->Recv_Buf[(--pr->Recv_Index)-1] = data;
							pr->Is_FE = 0;
						} 
						break;
			case 2: //У���У�� 
						for(i = 1 ,pr->CheckSum = 0; i < pr->Recv_Index - 2; i++){
							pr->CheckSum += pr->Recv_Buf[i];
						}
						/*У�����ʱ�ر�*/
						if( 1==2 && (uint8_t)pr->CheckSum != pr->Recv_Buf[pr->Recv_Index-2]){
							_clean_recv_buf(pr);
						}else{
							pr->Recv_State++;
							loop = 1;
						} 
						break;
			case 3: //֡���ͺͳ��Ƚ���ƥ��
						protocol_type = pr->Recv_Buf[1];
						for(i = 0; i < RECV_PROTOCOL_NUM; i++){ 
							if(protocol_type == recv_protocol_infos[i].type &&//���֡����ƥ��
								pr->Recv_Index == recv_protocol_infos[i].len + 5){//���֡����ƥ��
								pi = &recv_protocol_infos[i];//��Э��ջ���ҵ���ӦЭ����Ϣ 
							}
						}
						if(pi == NULL){//У�鲻ͨ��
							_clean_recv_buf(pr);
						}else{
							pr->Recv_State++;
							loop = 1;
						}
						break;
			case 4://��װЭ�� 
						pi->head = pr->Recv_Buf[0];
						pi->type = pr->Recv_Buf[1];
						memcpy(&pi->protocol,&pr->Recv_Buf[2],pi->len);//��Э�鸴�Ƶ�Э����Ϣ��
						pi->serial = pr->Recv_Buf[2+pi->len];
						pi->checksum = pr->Recv_Buf[3+pi->len];
						pi->tail = pr->Recv_Buf[4+pi->len];
						Queue_Put(pr->Protocol_Queue, pi);//��Э����Ϣ����Э�黺�����
						_clean_recv_buf(pr);
						break;
		}
	}while(loop--);
	 
	return 0;
}
/****************************************************
	������:	char_special
	����:		ԭ�ַ�
	����: 	�ַ�ת��
****************************************************/
uint16_t char_special(uint8_t num){
	switch(num){
		case 0xFD:return 0xFE<<8 | 0x7D;
		case 0xF8:return 0xFE<<8 | 0x78;
		case 0xFE:return 0xFE<<8 | 0x7E;
	}
	return num;
}


//###################################���⺯����###################################

/****************************************************
	������:	getCheckSum_ByProtocolInfo
	����:		����Э����Ϣ���У���
	����:		PROTOCOL_INFO_TЭ��������Ϣ
	����ֵ:	У��ͽ��
	ע�⣺	ֻ���ص�8λ
	����:		liyao 2015��9��8��14:10:51
****************************************************/
uint8_t getCheckSum_ByProtocolInfo(PROTOCOL_INFO_T* pi){
	uint8_t i = 0;
	uint16_t sum = pi->type + pi->serial;//�ۼ�type��serial
	for(i = 0; i < 	pi->len; i++)//�ۼ����в���
		sum+=((uint8_t*)&pi->protocol)[i];
	return (uint8_t)sum;
}

/****************************************************
	������:	Send_To_Uart
	����:		ͨ��uartֱ�ӷ���Э��
	����:		PROTOCOL_INFO_TЭ��������Ϣ
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t Send_To_Uart(PROTOCOL_INFO_T* pi){
	uint8_t i = 0,special_H,special_L;
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	
	USART_SendData_Block(PROTOCOL_USART, pi->head);//д��֡ͷ 
	USART_SendData_Block(PROTOCOL_USART, pi->type);//д��֡����
	for(i = 0; i < pi->len; i++ ){			 //д�����
		if(data[i] == 0xFD || data[i] == 0xF8 || data[i] == 0xFE){//ת��
			special_char = char_special(data[i]);
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			USART_SendData_Block(PROTOCOL_USART, special_H);
			USART_SendData_Block(PROTOCOL_USART, special_L);
		}else{
			USART_SendData_Block(PROTOCOL_USART, data[i]);
		}
	}
	USART_SendData_Block(PROTOCOL_USART, pi->serial); //д�����
	USART_SendData_Block(PROTOCOL_USART, pi->checksum);//д��У���
	USART_SendData_Block(PROTOCOL_USART, pi->tail);//д��֡β
	return 0;
}


/****************************************************
	������:	Protocol_Init
	����:		��ʼ��ȫ��Э������У�顢ִ�к���
	����:		liyao 2015��9��8��14:10:51     
	�޸ģ�  tc 2015��9��12�� 22:00:00  
****************************************************/
void Protocol_Init(void){
	//�������ݶ��г�ʼ��
	PROTOCOL_INFO_T* pi; 
	UART1_Resolver->Protocol_Queue = Queue_Init( _UART1_Protocol_QueueBuf,sizeof(PROTOCOL_INFO_T), UART1_RPQUEUE_SIZE); 
	UART1_Resolver->Recv_Buf = _UART1_Recv_Buf;
//	UART1_Resolver->Send_Queue = Queue_Init( _UART1_Send_Queue,sizeof(uint8_t), UART1_SQUEUE_SIZE); 
//	UART1_Resolver->Send_Buf = _UART1_Send_Buf;
	
	UART1_Resolver->RPQueue_Size = UART1_RPQUEUE_SIZE;
	UART1_Resolver->Rbuff_Size = UART1_RBUFF_SIZE;
	UART1_Resolver->Protocol_Put = _Protocol_Put;
	UART1_Resolver->Fetch_Protocol = _Fetch_Protocol;
	//_clean_recv_buf();

//����Э��ջ��ʼ��
	pi = &recv_protocol_infos[0];
	pi->len = sizeof(HEARTBEAT_PROTOCOL_T);
	pi->type = HEARTBEAT_PROTOCOL; 
	pi->handle = HeartBeat_P_Handle;
//	pi->check = HeartBeat_P_check;
	
	pi = &recv_protocol_infos[1];
	pi->len = sizeof(RUN_PROTOCOL_T);
	pi->type = RUN_PROTOCOL; 
	pi->handle = Run_P_Handle;
//	pi->check = Run_P_Check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(ROTATE_PROTOCOL_T);
	pi->type = ROTATE_PROTOCOL; 
	pi->handle = Rotate_P_Handle;
//	pi->check = Rotate_P_Check;
}
/****************************************************
	������:	Create_Protocol_Info
	����:		����PROTOCOL_INFO_T����
	����:		Э�鳤�ȣ�Э�����ݣ�Э�鴥��������Э��У�麯��
	ע��: 	ͨ��protocol_flag��־λ��ʾ�Ƿ�������µ�Э��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(void*)){
	PROTOCOL_INFO_T pi;
	pi.len = len;
	pi.type = type;
	pi.handle = handle;
	pi.check = check;
	return pi;
}

//###################################�Զ�����չ������###################################
/****************************************************
	������:	Protocol_Send
	����:		����Э��
	����:		PROTOCOL_INFO_T�ṹ���
	����:		liyao 2016��4��4��14:00:09
****************************************************/
void Protocol_Send(SEND_PROTO_TYPE type,PROTOCOL_T protocol_t,u8 len){
	memset(tmp_pi, 0,sizeof(PROTOCOL_INFO_T));
	tmp_pi->len = len;
	tmp_pi->head = 0xFD;	
	tmp_pi->type = type; 
	tmp_pi->protocol = protocol_t;
	tmp_pi->serial = _send_index++;
	tmp_pi->checksum = getCheckSum_ByProtocolInfo(tmp_pi);
	tmp_pi->tail = 0xF8;
	Send_To_Buff(tmp_pi);	
}

/*****************************************************************
������:FetchProtocols
��ע: ������λ�����������ܺ���
******************************************************************/
void FetchProtocols(void)
{
	UART1_Resolver->Fetch_Protocol(UART1_Resolver);
}
