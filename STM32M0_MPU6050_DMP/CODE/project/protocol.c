#include "protocol.h"

//#define PRINT_ERR
//###################################���������###################################
PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//����Э��ջ
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//����Э��ջ 
int8_t heart_flag = -1;
//proto_flag -1 ��Э�� >-1�����Ӧrecv_pis�е�����
int8_t protocol_flag = -1;

//###################################���ڱ�����###################################
//-----------------------------------------------------
static STATE_PROTOCOL_T _state_protocol;
static ODOMETER_PROTOCOL_T _odometer_protocol;
static ASK_PROTOCOL_T _ask_protocol;
static RUN_PROTOCOL_T _run_protocol;
static ANGLE_PROTOCOL_T _angle_protocol; 
static HEAD_PROTOCOL_T _head_protocol;
static WING_PROTOCOL_T _wing_protocol;
static uint8_t _recv_buf[RECV_BUFSIZE] = {0};
static int8_t _recv_index = 0;
static int8_t _head,_is_FE, _is_F8 = 0;
static uint16_t _sum = 0;

static char _Uart_Tx_Buff[SEND_BUFSIZE] = {0};
static QUEUE_T* _queue;

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
	/*queue_put(_queue, ch);*/
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
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

//###################################Э��У�麯����###################################
/****************************************************
	������:	run_protocol_check
	����:	 	�������Э��У��
	����:		RUN_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ��-1�����ַ���������� -2�����ַ���������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t run_protocol_check(void* protocol){
	RUN_PROTOCOL_T* rp = protocol;
	if(!(rp->para1 >=0 && rp->para1 <=2)){//����У��
		#ifdef PRINT_ERR 
			printf("���ַ����������%d\n",rp->para1); 
		#endif
		return -1;}
	if(!(rp->para4 >=0 && rp->para4 <=2)){//����У��
		#ifdef PRINT_ERR 
			printf("���ַ����������%d\n",rp->para4); 
		#endif
		return -2;}
	if(!(rp->para8 >= 0 && rp->para8 <=1)){//�Ƿ���ָ��У��
		#ifdef PRINT_ERR 
			printf("�Ƿ���ָ���������%d\n",rp->para8); 
		#endif
		return -3;}
	
		#ifdef PRINT_ERR 
			printf("\n\n\n���ַ���%d �ٶȣ�%d\n���ַ���%d �ٶȣ�%d\n��ţ�%d���Ƿ���ָ�%d\n",
			rp->para1, rp->para2 << 8 | rp->para3,
			rp->para4, rp->para5 << 8 | rp->para6,
			rp->para7,rp->para8); 
		#endif
	return 0;
}
/****************************************************
	������:	angle_protocol_check
	����:		˫�ֽǶȿ��Ƶ��Э��У��
	����:		ANGLE_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ:	
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t angle_protocol_check(void* protocol){
	ANGLE_PROTOCOL_T* ap = protocol;
	if(!(ap->para16 <=1))//�Ƿ���ָ��У��
		return -1;
	return 0;
}

/****************************************************
	������:	head_protocol_check
	����:		ͷ�����Э��У��
	����:		HEAD_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ:	-1���Ƕ�У��������� -2���ٶ�У��������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t head_protocol_check(void* protocol){
	HEAD_PROTOCOL_T* hp = protocol;
	if(!(hp->para1 <=240))//�Ƕ�У��
		return -1;
	if(!(hp->para3 <=100))//�ٶ�У��
		return -2;
	if(!(hp->para4 <=1))//�Ƿ���ָ��У��
		return -3;
	return 0;
}

/****************************************************
	������:	wing_protocol_check
	����:		�����Э��У��
	����:		WING_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ:	-1������ٶȲ������� -2���ҳ��ٶȲ������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t wing_protocol_check(void* protocol){
	WING_PROTOCOL_T* wp = protocol;
	if(!(wp->para2 >=0 && wp->para2 <=100))//����ٶ�У��
		return -1;
	if(!(wp->para4 >=0 && wp->para4 <=100))//�ҳ��ٶ�У��
		return -2;
	if(!(wp->para6 >= 0 && wp->para6 <=1))//�Ƿ���ָ��У��
		return -3;
	return 0;
}
//###################################���⺯����###################################
/****************************************************
	������:	getProtocolInfo_Bytype
	����:		����Э�����ͻ��Э����Ϣ
	����:		SEND_PROTO_TYPEö�ٷ���Э������
	����ֵ:	-1������ٶȲ������� -2���ҳ��ٶȲ������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
PROTOCOL_INFO_T* getProtocolInfo_Bytype(SEND_PROTO_TYPE type){ 
	uint8_t i = 0; 
	for(i = 0; i < SEND_PROTOCOL_NUM; i++){
		if(protocol_send_infos[i].type == type)
			return &protocol_send_infos[i];
	}
	return NULL;
}

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
		sum+=((uint8_t*)pi->protocol)[i];
	
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
	uint8_t* data = pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//�ų�֡ͷ֡β
			special_char = char_special(data[i]);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,special_char >> 8);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,(uint8_t)special_char);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		}else{
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,data[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
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
	uint8_t i = 0;
	uint16_t special_char;
	uint8_t* data = pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//�ų�֡ͷ֡β
			special_char = char_special(data[i]);
			queue_put(_queue, special_char >> 8);
			queue_put(_queue, special_char);
		}else{
			queue_put(_queue, data[i]);
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
	if(queue_get(_queue,&data) == 0){
		USART_ClearFlag(USART1, USART_FLAG_TC); 
		USART_SendData(USART1,data);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
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
	_queue = queue_init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE);
	_clean_recv_buf();
	PROTOCOL_INFO_T* pi;
	
	//����Э��ջ��ʼ��
	pi = &protocol_send_infos[0];
	pi->len = sizeof(STATE_PROTOCOL_T);
	pi->state = -1;
	pi->type = STATE_PROTOCOL;
	pi->protocol = &_state_protocol;
	
	pi = &protocol_send_infos[1];
	pi->len = sizeof(ASK_PROTOCOL_T);
	pi->state = -1;
	pi->type = ASK_PROTOCOL;
	pi->protocol = &_ask_protocol;
	
	//tc--------------begin
	pi = &protocol_send_infos[2];
	pi->len = sizeof(ODOMETER_PROTOCOL_T);
	pi->state = -1;
	pi->type = ODOMETER_PROTOCOL;
	pi->protocol = &_odometer_protocol;
	//tc----------------end

//����Э��ջ��ʼ��
	pi = &recv_protocol_infos[0];
	pi->len = sizeof(RUN_PROTOCOL_T);
	pi->state = -1;
	pi->type = RUN_PROTOCOL; 
	pi->protocol = &_run_protocol;
	pi->check = run_protocol_check;
	
	pi = &recv_protocol_infos[1];
	pi->len = sizeof(ANGLE_PROTOCOL_T);
	pi->state = -1;
	pi->type = ANGLE_PROTOCOL; 
	pi->protocol = &_angle_protocol;
	pi->check = angle_protocol_check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(HEAD_PROTOCOL_T);
	pi->state = -1;
	pi->type = HEAD_PROTOCOL;
	pi->protocol = &_head_protocol;
	pi->check = head_protocol_check; 
	
	pi = &recv_protocol_infos[3];
	pi->len = sizeof(WING_PROTOCOL_T);
	pi->state = -1;
	pi->type = WING_PROTOCOL;
	pi->protocol = &_wing_protocol;
	pi->check = wing_protocol_check; 
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
	PROTOCOL_INFO_T * pi = NULL; 
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
				pi = &recv_protocol_infos[i];
				protocol_flag = i;
			}
		}
		if(pi == NULL){//У�鲻ͨ��
			_clean_recv_buf();
		}else{//У��ͨ��
			pi->state = 0; 
			memcpy(pi->protocol,_recv_buf,pi->len);
			pi->state = 1;
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
	PROTOCOL_INFO_T* pi = getProtocolInfo_Bytype(ASK_PROTOCOL);
	ASK_PROTOCOL_T* ap = pi->protocol;
	ap->head = 0xFD;
	ap->type = ASK_PROTOCOL;
	ap->para1 = serial_number;
	ap->checksum = getProtocolInfo_Bychecksum(pi);//(uint8_t)(ap->type + ap->para1);
	ap->tail = 0xF8;
	Send_To_Buff(pi);
}

/****************************************************
	������:	state_protocol_send
	����:		������������Ϣ
	����:		Э��������Ϣʵ��
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void state_protocol_send(PROTOCOL_INFO_T* pi){
	STATE_PROTOCOL_T* sp = pi->protocol;
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
	ODOMETER_PROTOCOL_T* sp = pi->protocol;
	sp->checksum = getProtocolInfo_Bychecksum(pi);
	Send_To_Buff(pi);
}

