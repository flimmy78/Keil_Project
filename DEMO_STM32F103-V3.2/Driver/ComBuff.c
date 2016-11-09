#include "ComBuff.h"
  
static char _Uart_Tx_Buff[SEND_BUFSIZE_USART] = {0};
QUEUE_T* Uart_Tx_Queue;
#ifdef __24L01__H__INCLUDED__
static char _Nrf_Tx_Buff[SEND_BUFSIZE_NRF] = {0};
QUEUE_T* Nrf_Tx_Queue;
#endif
//--------------------------�ڲ���������-----------------------------
#ifdef UART1_DMA_RECEIVER 
	Uartx_DMA_Receiver_T Uart1_DMA_Receiver = {0};
	uint8_t Uartx_DMA_Recv_Buff[UART1_DMA_RECV_SIZE] = {0}; 
#endif
	
#ifdef UART1_DMA_SENDER 
	Uartx_DMA_Sender_T Uart1_DMA_Sender = {0};
	uint8_t Uartx_DMA_Send_Buff[UART1_DMA_SEND_SIZE] = {0}; 
#endif

//--------------------------�ڲ���������-----------------------------
int8_t _UsartReadByte(Uartx_DMA_Receiver_T* udr,uint8_t* data);
int16_t _UsartReadTo(Uartx_DMA_Receiver_T* udr, uint8_t value, uint8_t *data, uint8_t len);
int16_t _UsartRead(Uartx_DMA_Receiver_T* udr, uint8_t *data, uint8_t len);
	
int8_t _UsartWrite(Uartx_DMA_Sender_T* uds, uint8_t *data, uint8_t len);
int8_t _UsartWriteByte(Uartx_DMA_Sender_T* uds,uint8_t data);
/****************************************************
	������:	ComBuff_Init
	����:		��ʼ��ȫ��ͨ�Ż�����
	����:		liyao 2016��4��4��22:02:12 
****************************************************/
void ComBuff_Init(void){ 
	#ifdef UART1_DMA_RECEIVER
	Uart1_DMA_Receiver.Uartx_DMA_Recv_Buff = Uartx_DMA_Recv_Buff;
	Uart1_DMA_Receiver.DMAx = DMA1_Channel5;
	Uart1_DMA_Receiver.DMA_BuffSize = UART1_DMA_RECV_SIZE;
	Uart1_DMA_Receiver.ReadByte = _UsartReadByte;
	Uart1_DMA_Receiver.ReadTo = _UsartReadTo;
	Uart1_DMA_Receiver.Read = _UsartRead;
	#endif
	
	#ifdef UART1_DMA_SENDER 
	Uart1_DMA_Sender.Uartx_DMA_Send_Buff = Uartx_DMA_Send_Buff;
	Uart1_DMA_Sender.DMAx = DMA1_Channel4;
	Uart1_DMA_Sender.Write = _UsartWrite;
	Uart1_DMA_Sender.WriteByte = _UsartWriteByte;
	#endif
	
	Uart_Tx_Queue = Queue_Init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE_USART);
	#ifdef __24L01__H__INCLUDED__
	Nrf_Tx_Queue = Queue_Init( _Nrf_Tx_Buff,sizeof(char), SEND_BUFSIZE_NRF);
	#endif
}

/****************************************************
	������:	_UsartWrite
	����:		ͨ�����ڼ��̷���һ������
	����:		������ , �������
	����ֵ���ɹ�0 ʧ��-1
	����:		liyao 2016��9��14��10:55:11
****************************************************/
int8_t _UsartWrite(Uartx_DMA_Sender_T* uds,uint8_t* data, uint8_t len){ 
	while(uds->OverFlag);
	memcpy(uds->Uartx_DMA_Send_Buff, data, len);
	uds->DMAx->CMAR = (u32)uds->Uartx_DMA_Send_Buff;
	uds->DMAx->CNDTR = len;
	DMA_Cmd(uds->DMAx, ENABLE); 
	uds->OverFlag = 1; 
	return 0;
}

/****************************************************
	������:	_UsartWriteByte
	����:		ͨ�����ڼ��̷���1�ֽ�
	����:		������ , �������
	����ֵ���ɹ�0 ʧ��-1
	����:		liyao 2016��9��14��10:55:07
****************************************************/
int8_t _UsartWriteByte(Uartx_DMA_Sender_T* uds,uint8_t data){ 
	while(uds->OverFlag); 
	uds->Byte_Data = data;
	uds->DMAx->CMAR = (u32)(&uds->Byte_Data);
	uds->DMAx->CNDTR = 1;
	DMA_Cmd(uds->DMAx, ENABLE); 
	uds->OverFlag = 1; 
	return 0;
}


/****************************************************
	������:	_UsartReadByte
	����:		�Ӵ��ڻ�������ȡ1���ֽ�
	����:		������ , �������
	����ֵ���ɹ�0 ʧ��-1
	����:		liyao 2016��9��8��15:17:44
****************************************************/
int8_t _UsartReadByte(Uartx_DMA_Receiver_T* udr,uint8_t* data){
	uint16_t maxCur = (udr->DMA_BuffSize - DMA_GetCurrDataCounter(udr->DMAx));
	if((udr->Reversal == 0 && udr->Recv_Cur < maxCur) || udr->Reversal == 1){ 
		*data = udr->Uartx_DMA_Recv_Buff[udr->Recv_Cur]; 
		if(++udr->Recv_Cur % udr->DMA_BuffSize == 0)
			udr->Recv_Cur = udr->Reversal = 0;
		return 0;
	}
	return -1;
}

/****************************************************
	������:	_UsartReadTo
	����:		�Ӵ��ڻ�������ȡN���ֽ�,ֱ������value
	����:		������,ƥ��ֵ,��������,�������鳤��
	����ֵ���ɹ���ȡ�����ֽ��� ��������ָ������-1 ��������ռ䲻�� -2
	����:		liyao 2016��9��8��15:17:49	
****************************************************/
int16_t _UsartReadTo(Uartx_DMA_Receiver_T* udr, uint8_t value, uint8_t *data, uint8_t len){
	uint16_t i = 0,tmp_Recv_Cur = udr->Recv_Cur, maxCur = (udr->DMA_BuffSize - DMA_GetCurrDataCounter(udr->DMAx));
	uint8_t tmp_Reversal = udr->Reversal;
	for(i = 0; i < len; i++){ 
		if((tmp_Reversal == 0 && tmp_Recv_Cur < maxCur) || tmp_Reversal == 1){
			data[i] = udr->Uartx_DMA_Recv_Buff[tmp_Recv_Cur]; 
			if(++tmp_Recv_Cur % udr->DMA_BuffSize == 0)
				tmp_Recv_Cur = tmp_Reversal = 0;
			if(data[i] == value){
				udr->Recv_Cur = tmp_Recv_Cur;
				udr->Reversal = tmp_Reversal;
				return i+1;
			}
		}else{
			//Log.waring("��ָ������\r\n");
			return -1;
		}
	}
	Log.error("��������ռ䲻��\r\n");
	return -2;
}

/****************************************************
	������:	_UsartRead
	����:		�Ӵ��ڻ�������ȡN���ֽ�
	����:		������,��������,�������鳤��
	����ֵ���ɹ���ȡ�����ֽ��� ������������-1 
	����:		liyao 2016��9��8��15:17:49	
****************************************************/
int16_t _UsartRead(Uartx_DMA_Receiver_T* udr, uint8_t *data, uint8_t len){
	uint16_t i = 0,cnt = 0, maxCur = (udr->DMA_BuffSize - DMA_GetCurrDataCounter(udr->DMAx)); 
	for(i = 0; i < len; i++){
		if((udr->Reversal == 0 && udr->Recv_Cur < maxCur) || udr->Reversal == 1){
			data[i] = udr->Uartx_DMA_Recv_Buff[udr->Recv_Cur]; 
			cnt++;
			if(++udr->Recv_Cur % udr->DMA_BuffSize == 0)
				udr->Recv_Cur = udr->Reversal = 0;
		}else{
			return cnt;
		}
	} 
	return -1;
}
/****************************************************
	������:	fputc
	����: 	printf�ض���
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int fputc(int ch,FILE *f)
{
	#ifndef PRINT_ERR
		Queue_Put(Uart_Tx_Queue, &ch); 
	#else
		#ifdef UART1_DMA_SENDER
			Uart1_DMA_Sender.WriteByte(&Uart1_DMA_Sender,ch);
		#else
			USART_SendData_Block(DEBUG_USART, ch); 
		#endif 
	#endif
	return(ch);	   
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
	
	Queue_Put(Uart_Tx_Queue, &pi->head);//д��֡ͷ
	Queue_Put(Uart_Tx_Queue, &pi->type);//д��֡���� 
	for(i = 0; i < pi->len; i++ ){			 //д�����
		if(data[i] == 0xFD || data[i] == 0xF8 || data[i] == 0xFE){//ת��
			special_char = char_special(data[i]); 
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			Queue_Put(Uart_Tx_Queue,&special_H);
			Queue_Put(Uart_Tx_Queue,&special_L);
		}else{
			Queue_Put(Uart_Tx_Queue, &data[i]);
		}
	}
	Queue_Put(Uart_Tx_Queue, &pi->serial);//д�����
	Queue_Put(Uart_Tx_Queue, &pi->checksum);//д��У���
	Queue_Put(Uart_Tx_Queue, &pi->tail);//д��֡β
	return 0;
}

/****************************************************
	������:	Buff_To_Uart
	����:		�ӻ�������ȡ��һ���ֽڷ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void Buff_To_Uart(void){
	int32_t data;
	uint8_t count = 5;
	while(count--)
		if(Queue_Get(Uart_Tx_Queue,&data) == 0){ 
			#ifdef UART1_DMA_SENDER
				Uart1_DMA_Sender.WriteByte(&Uart1_DMA_Sender,data);
			#else
				USART_SendData_Block(DEBUG_USART, ch); 
			#endif 
		}else{
			break;
		}
}
#ifdef __24L01__H__INCLUDED__
/****************************************************
	������:	Buff_To_NRF
	����:		�ӻ�������ȡ�����31���ֽڷ�����NRF24L01
	����:		liyao 2016��4��4��00:53:27	
****************************************************/
void Buff_To_NRF(void){
	u8 data,i = 0; 
	while(Queue_Get(Nrf_Tx_Queue,&data) == 0){
		i++;
		nrf1->TxBuf[i] = data;
		nrf1->TxBuf[0] = i;		
		USART_SendData_Block(DEBUG_USART, data); //����ģ�鷢�͵�ͬʱ����������
		if(i == 31)
			break;
	}
	if(i > 0){
		nrf1->Set_TX_Mode(nrf1);
		nrf1->nRF24L01_TxPacket(nrf1,nrf1->TxBuf);
		nrf1->Set_RX_Mode(nrf1);
	}
}
#endif

