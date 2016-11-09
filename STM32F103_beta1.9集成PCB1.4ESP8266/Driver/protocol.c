#include "STM32_DevInit.h"
//###################################���������###################################
//PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//����Э��ջ
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//����Э��ջ 
//static uint8_t _Recv_Protocol_Arr[RECV_PROTOCOL_NUM] = {0};//�Ѿ����յ�Э�������
int8_t heart_flag = -1;

//###################################���ڱ�����###################################
PROTOCOL_INFO_T _Recv_Protocol_Buff[RECV_PROTOCOL_BUFSIZE] = {0};//����Э�����


PROTOCOL_INFO_T _tmp_pi = {0};
PROTOCOL_INFO_T* tmp_pi = &_tmp_pi;
//-----------------------------------------------------
/*static STATE_PROTOCOL_T _state_protocol;
static ODOMETER_PROTOCOL_T _odometer_protocol;
static DEADRECKONING_PROTOCOL_T _deadreckoning_protocol;
static ASK_PROTOCOL_T _ask_protocol;
static RUN_PROTOCOL_T _run_protocol;
static ANGLE_PROTOCOL_T _angle_protocol; 
static HEAD_PROTOCOL_T _head_protocol;
static WING_PROTOCOL_T _wing_protocol;*/

static uint8_t _recv_buf[RECV_BUFSIZE] = {0};
static int8_t _recv_index = 0;
static int8_t _head,_is_FE, _is_F8 = 0;
static uint16_t _sum = 0;
static uint8_t _send_index = 0x00;

QUEUE_T* Recv_Protocol_Queue;
//###################################���ں�����###################################
/****************************************************
	������:	clean_recv_buf
	����:		���Э��ջ����д���Э��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void _clean_recv_buf(void){
	uint8_t i = 0;
	_is_F8 = _is_FE = _recv_index = _head = 0;
	for(i = 0; i<RECV_BUFSIZE; i++)
		_recv_buf[i] = 0;
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
	������:	Create_Protocol_Info
	����:		����PROTOCOL_INFO_T����
	����:		Э�鳤�ȣ�Э�����ݣ�Э�鴥��������Э��У�麯��
	ע��: 	ͨ��protocol_flag��־λ��ʾ�Ƿ�������µ�Э��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(PROTOCOL_INFO_T*)){
	PROTOCOL_INFO_T pi;
	pi.len = len;
	pi.type = type;
	pi.handle = handle;
	pi.check = check;
	return pi;
}
/****************************************************
	������:	put_byte
	����:		����Э�����ݲ�������װ
	����:		Э������
	ע��: 	ͨ��protocol_flag��־λ��ʾ�Ƿ�������µ�Э��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t put_byte(uint16_t data){
	uint8_t i = 0;
	uint8_t* type = NULL;
	PROTOCOL_INFO_T* pi = NULL; 
	//--------����֡ͷ--------
	if(data == 0xFD){//�����֡ͷ
		if(_head == 0){//ԭ��֡ͷ
			_head = 0xFD;
		}else{//ԭ��֡ͷ����������
			_clean_recv_buf();
			_head = 0xFD;
		}
	}else if(_head == 0){//���û��֡ͷ
		return 0;
	}
	//-------����ת������-------
	
	if(data == 0xFE || _is_FE){//����FE���ϵ�һ���������ڶ��ν����ϵڶ�������
		if(_is_FE){
			switch(data){
				case 0x7D: data = 0xFD;break;
				case 0x78: data = 0xF8;break;
				case 0x7E: data = 0xFE;break;
//				case 0xF8: _recv_buf[_recv_index++] = 0xFE;
//									 data = 0xF8;_is_F8 = 1;
//									 break;//У���ΪFE�������
			}
			_is_FE = 0;
		}else{
			_is_FE = 1;
			return 0;
		}
	//-------����֡β����--------
	}else if(data == 0xF8){
		_is_F8 = 1;
	}
	//--------�洢����----------
	_recv_buf[_recv_index++] = data;
	//USART_SendData(USART1,data);
	
	//--------�����֡������� ������֤-------
	if(_is_F8){
		//У���У��
		for(i = 1 ,_sum = 0; i < _recv_index - 2; i++){
			_sum += _recv_buf[i];
		}
		/*У�����ʱ�ر�
		if((uint8_t)_sum != _recv_buf[_recv_index-2]){
			_clean_recv_buf();
			return 0;
		}*/
			
		//֡���ͺͳ���У��
		type = &_recv_buf[1];
		for(i = 0; i < RECV_PROTOCOL_NUM; i++){ 
			if(*type == recv_protocol_infos[i].type){//���֡����ƥ��
				if(_recv_index == recv_protocol_infos[i].len + 5){//���֡����ƥ��
					pi = &recv_protocol_infos[i];//��Э��ջ���ҵ���ӦЭ����Ϣ
				}
			}
		}
		if(pi == NULL){//У�鲻ͨ��
			_clean_recv_buf();
		}else{//У��ͨ��
			pi->head = _recv_buf[0];
			pi->type = _recv_buf[1];
			memcpy(&pi->protocol,&_recv_buf[2],pi->len);//��Э�鸴�Ƶ�Э����Ϣ��
			pi->serial = _recv_buf[2+pi->len];
			pi->checksum = _recv_buf[3+pi->len];
			pi->tail = _recv_buf[4+pi->len];
			Queue_Put(Recv_Protocol_Queue, pi);//��Э����Ϣ����Э�黺�����
			_clean_recv_buf();
		}
	}
	return 0;
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
	Send_To_Buff(tmp_pi, Bluetooth_Tx_Queue);
} 

/*****************************************************************
������:FetchProtocolHandle
��ע: ������λ�����������ܺ���
******************************************************************/
void FetchProtocolHandle(void)
{
	PROTOCOL_INFO_T pi;
	while(Queue_Get(Recv_Protocol_Queue,&pi) == 0){
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
	������:	Protocol_Init
	����:		��ʼ��ȫ��Э������У�顢ִ�к���
	����:		liyao 2015��9��8��14:10:51     
	�޸ģ�  tc 2015��9��12�� 22:00:00  
****************************************************/
void Protocol_Init(void){
	//�������ݶ��г�ʼ��
	PROTOCOL_INFO_T* pi;
	Recv_Protocol_Queue = Queue_Init( _Recv_Protocol_Buff,sizeof(PROTOCOL_INFO_T), RECV_PROTOCOL_BUFSIZE); 
	_clean_recv_buf();

//����Э��ջ��ʼ��
	pi = &recv_protocol_infos[0];
	pi->len = sizeof(RUN_PROTOCOL_T);
	pi->type = RUN_PROTOCOL; 
	pi->handle = Run_Protocol_Handle;
	pi->check = Run_Protocol_Check;
	
	pi = &recv_protocol_infos[1];
	pi->len = sizeof(RUDDER_PROTOCOL_T);
	pi->type = RUDDER_PROTOCOL; 
	pi->handle = Rudder_Protocol_Handle;
	pi->check = Rudder_Protocol_Check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(CUSTOM_PROTOCOL_T);
	pi->type = CUSTOM_PROTOCOL; 
	pi->handle = Custom_Protocol_Handle;
	pi->check = Custom_Protocol_Check;
	
/*	pi = &recv_protocol_infos[1];
	pi->len = sizeof(ANGLE_PROTOCOL_T);
	pi->type = ANGLE_PROTOCOL; 
//	pi->handle = Angle_Protocol_Handle;
//	pi->check = angle_protocol_check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(HEAD_PROTOCOL_T);
	pi->type = HEAD_PROTOCOL;
//	pi->handle = Head_Protocol_Handle; 
//	pi->check = head_protocol_check; 
	
	pi = &recv_protocol_infos[3];
	pi->len = sizeof(WING_PROTOCOL_T);
	pi->type = WING_PROTOCOL;
//	pi->handle = Wing_Protocol_Handle;
//	pi->check = wing_protocol_check; 
	
	pi = &recv_protocol_infos[4];
	pi->len = sizeof(PDRCORRECT_PROTOCOL_T);
	pi->type = PDRCORRECT_PROTOCOL;
//	pi->handle = PDR_Correnct_Protocol_Handle; 
//	pi->check = NULL; */
}

