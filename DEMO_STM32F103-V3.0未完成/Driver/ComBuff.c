#include "ComBuff.h"

static char _Uart_Tx_Buff[SEND_BUFSIZE_USART] = {0};
QUEUE_T* Uart_Tx_Queue;
#ifdef __24L01__H__INCLUDED__
static char _Nrf_Tx_Buff[SEND_BUFSIZE_NRF] = {0};
QUEUE_T* Nrf_Tx_Queue;
#endif

/****************************************************
	������:	WriteByteToUSART
	����:		���� ����
	����: 	�򴮿ڷ�������
****************************************************/
void WriteByteToUSART(USART_TypeDef* USARTx, u8 data){
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_SendData(USARTx,data);
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
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
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
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC); 
			USART_SendData(USART1,data);
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
		WriteByteToUSART(USART1, data);//����ģ�鷢�͵�ͬʱ����������
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
/****************************************************
	������:	ComBuff_Init
	����:		��ʼ��ȫ��ͨ�Ż�����
	����:		liyao 2016��4��4��22:02:12 
****************************************************/
void ComBuff_Init(void){
	Uart_Tx_Queue = Queue_Init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE_USART);
	#ifdef __24L01__H__INCLUDED__
	Nrf_Tx_Queue = Queue_Init( _Nrf_Tx_Buff,sizeof(char), SEND_BUFSIZE_NRF);
	#endif
}
