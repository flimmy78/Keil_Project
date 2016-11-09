#include "stm32f10x.h"
#include "protocol.h"
#include "UartCMD.h"
//#define PRINT_ERR
//###################################���������###################################
//PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//����Э��ջ
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//����Э��ջ 
//static uint8_t _Recv_Protocol_Arr[RECV_PROTOCOL_NUM] = {0};//�Ѿ����յ�Э�������
int8_t heart_flag = -1;

//###################################���ڱ�����###################################
PROTOCOL_INFO_T _Recv_Protocol_Buff[RECV_PROTOCOL_BUFSIZE] = {0};//����Э�����
//-----------------------------------------------------

static uint8_t _recv_buf[RECV_BUFSIZE] = {0};
static int8_t _recv_index = 0;
static int8_t _head,_is_FE, _is_F8 = 0;
static uint16_t _sum = 0;
static uint8_t _send_index = 0x00;
//static uint8_t OLED_Len = 16;

static char _Uart_Tx_Buff[SEND_BUFSIZE] = {0};
QUEUE_T* _Uart_Tx_Queue,*Recv_Protocol_Queue;

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
	������:	is_special
	����: 	�ж��Ƿ���Ҫת��
	����:		����
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t is_special(uint8_t num){
	if(num == 0xFD || num == 0xF8 || num == 0xFE)
		return 1;
	return 0;
}

/****************************************************
	������:	fputc
	����: 	printf�ض���
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int fputc(int ch,FILE *f)
{	
		/*if(OLED_Len >=64){
			OLED_Len = 16;
			OLED_Clear(3,8);
		}
		if(ch != '\r' && ch != '\n'){
			OLED_ShowChar(OLED_Len%16*8,OLED_Len/16*2,ch);
			OLED_Len++;
		}
		if(ch == '\n'){
			OLED_Len += (16 - OLED_Len%16);
		}*/
	#ifndef PRINT_ERR
		Queue_Put(_Uart_Tx_Queue, &ch);
	#else
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
		
		
	#endif
	return(ch);	   
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
	������:	getProtocolInfo_Bychecksum
	����:		����Э�����ͻ��У���
	����:		PROTOCOL_INFO_TЭ��������Ϣ
	����ֵ:	У��ͽ��
	ע�⣺	ֻ���ص�8λ
	����:		liyao 2015��9��8��14:10:51
****************************************************/
uint8_t getProtocolInfo_Bychecksum(PROTOCOL_INFO_T* pi){
	uint8_t i = 0;
	uint16_t sum = 0;
	for(i = 1; i < 	pi->len-2; i++)
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
	uint8_t i = 0;
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//�ų�֡ͷ֡β
			special_char = char_special(data[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,special_char >> 8);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,(uint8_t)special_char);
			
		}else{
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,data[i]);
		}
	}
	return 0;
}

/****************************************************
	������:	Send_To_Buff
	����:		�򻺳���д�������������
	����:		PROTOCOL_INFO_TЭ��������Ϣ
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t Send_To_Buff(PROTOCOL_INFO_T* pi){
	uint8_t i = 0,special_H,special_L;
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//�ų�֡ͷ֡β
			special_char = char_special(data[i]);
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			Queue_Put(_Uart_Tx_Queue,&special_H);
			Queue_Put(_Uart_Tx_Queue,&special_L);
		}else{
			Queue_Put(_Uart_Tx_Queue, &data[i]);
		}
	}
	return 0;
}

/****************************************************
	������:	Buff_To_Uart
	����:		�ӻ�������ȡ��һ���ֽڷ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void Buff_To_Uart(void){
	int32_t data;
	if(Queue_Get(_Uart_Tx_Queue,&data) == 0){
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART1, USART_FLAG_TC); 
		USART_SendData(USART1,data);
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
	_Uart_Tx_Queue = Queue_Init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE);
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
/****************************************************
	������:	Create_Protocol_Info
	����:		����PROTOCOL_INFO_T����
	����:		Э�鳤�ȣ�Э�����ݣ�Э�鴥��������Э��У�麯��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,
					void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(PROTOCOL_INFO_T*)){
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
			if(*type == recv_protocol_infos[i].type &&//���֡����ƥ��
				_recv_index == recv_protocol_infos[i].len){//���֡����ƥ��
				pi = &recv_protocol_infos[i];//��Э��ջ���ҵ���ӦЭ����Ϣ
				
			}
		}
		if(pi == NULL){//У�鲻ͨ��
			_clean_recv_buf();
		}else{//У��ͨ��
			memcpy(&pi->protocol,_recv_buf,pi->len);//��Э�鸴�Ƶ�Э����Ϣ��
			Queue_Put(Recv_Protocol_Queue, pi);//��Э����Ϣ����Э�黺�����
			_clean_recv_buf();
		}
	}
	return 0;
}
//###################################�Զ�����չ������###################################
/****************************************************
	������:	ask_send
	����:		����Ӧ��
	����:		����Э������к�
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void ask_send(uint8_t serial_number){
	PROTOCOL_INFO_T pi = Create_Protocol_Info(sizeof(ASK_PROTOCOL_T),ASK_PROTOCOL,NULL,NULL);
	ASK_PROTOCOL_T* ap = &pi.protocol.ask_protocol;
	ap->head = 0xFD;
	ap->type = ASK_PROTOCOL;
	ap->para1 = _send_index++;
	ap->checksum = getProtocolInfo_Bychecksum(&pi);//(uint8_t)(ap->type + ap->para1);
	ap->tail = 0xF8;
	Send_To_Buff(&pi);
}

/****************************************************
	������:	state_protocol_send
	����:		������������Ϣ
	����:		Э��������Ϣʵ��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void state_protocol_send(PROTOCOL_INFO_T* pi){
	STATE_PROTOCOL_T* sp = &pi->protocol.state_protocol;
	sp->para12 =_send_index++;
	sp->checksum = getProtocolInfo_Bychecksum(pi);
	Send_To_Buff(pi);
}

/****************************************************
	������:	odometer_protocol_send
	����:		�����������Ϣ
	����:		Э��������Ϣʵ��
	����:		tc 2015��9��12��21:10:51
****************************************************/
void odometer_protocol_send(PROTOCOL_INFO_T* pi){
	ODOMETER_PROTOCOL_T* op = &pi->protocol.odometer_protocol;
	op->head = 0xFD;
	op->type = ODOMETER_PROTOCOL;
	op->para10 = _send_index++;
	op->checksum = getProtocolInfo_Bychecksum(pi);
	op->tail = 0xF8;
	Send_To_Buff(pi);
}

/****************************************************
	������:	deadreckoning_protocol_send
	����:		��λ��Ϣ
	����:		Э��������Ϣʵ��
	����:		tc 2015��9��12��21:10:51
****************************************************/
void deadreckoning_protocol_send(PROTOCOL_INFO_T* pi){
	DEADRECKONING_PROTOCOL_T* dp = &pi->protocol.deadreckoning_protocol;
	dp->head = 0xFD;
	dp->type = DEADRECKONING_PROTOCOL;
	dp->para14 = _send_index++;
	dp->checksum = getProtocolInfo_Bychecksum(pi);
	dp->tail = 0xF8;
	Send_To_Buff(pi); 
}

/****************************************************
	������:	respond_protocol_send
	����:		ִ�н������Э��
	����:		Э��������Ϣʵ��
	����:		liyao 2015��10��16��16:48:02
****************************************************/
void respond_protocol_send(PROTOCOL_INFO_T* pi){
	uint8_t protocol_NUM = 0;
	PROTOCOL_INFO_T proto_info;
	RESPOND_PROTOCOL_T* rp;
	if(pi->protocol.head_protocol.head == 0)
		return;
	if(pi->type == HEAD_PROTOCOL){
		protocol_NUM = pi->protocol.head_protocol.para3;
	}else if(pi->type == WING_PROTOCOL){
		protocol_NUM = pi->protocol.wing_protocol.para5;
	}else if(pi->type == RUN_PROTOCOL){
		protocol_NUM = pi->protocol.run_protocol.para7;
	}
	
	proto_info = Create_Protocol_Info(sizeof(RESPOND_PROTOCOL_T),RESPOND_PROTOCOL,NULL,NULL);
	rp = &proto_info.protocol.respond_protocol;
	rp->head = 0xFD;
	rp->type = RESPOND_PROTOCOL;
	rp->para1 = protocol_NUM;
	rp->para2 = pi->type;
	rp->para3 = 0;
	rp->para4 = _send_index++;
	rp->checksum = getProtocolInfo_Bychecksum(&proto_info);
	rp->tail = 0xF8;
	Send_To_Buff(&proto_info); 
	memset(pi,0,sizeof(PROTOCOL_INFO_T));
	
}

