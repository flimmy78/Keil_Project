#include "stm32f10x.h"
#include <NRF24L01_Driver_send.h>
#include <NRF24L01_Driver.h>
#include <Driver.h>
#include <stdio.h>
extern u8  TX_ADDRESS[5];    //���͵����յ�ĵ�ַ

extern u8 Rx_Address0[5]; //����ͨ��0�ĵ�ַ
extern u8 Rx_Address1[5]; //����ͨ��1�ĵ�ַ
extern u8 Rx_Address2[5]; //����ͨ��2�ĵ�ַ
extern u8 Rx_Address3[5]; //����ͨ��3�ĵ�ַ
extern u8 Rx_Address4[5]; //����ͨ��4�ĵ�ַ
extern u8 Rx_Address5[5]; //����ͨ��5�ĵ�ַ


/**********************************************************
** ������: RF_Write_Reg
** ���ܣ�дnRF24L01�ļĴ���
** ���룺reg-�Ĵ�����ַ
		 value-д�������
** �������
** ���أ���
***********************************************************/
void RF_Write_Reg_send(u8 reg, u8 value)
{
    CSN_L_2; ////CSN=0��ʹ��nRF24L01Ƭѡ
    SPI_Send(SPI1,reg);   //д�Ĵ�����ַ
    SPI_Send(SPI1,value); //д����
    CSN_H_2; ////CSN=1��ʧ��nRF24L01Ƭѡ
}

/**********************************************************
** ������: RF_Write_Buf
** ���ܣ�д���ݵ�nRF24L01�����ݻ�����
** ���룺reg-Ҫд��ļĴ�����ַ
		 *pBuf-д�����ݵ������ַָ��
		 bytes-д��������ֽ���
** �������
** ���أ�status-����״̬�ֽ�
***********************************************************/
u8 RF_Write_Buf_send(u8 reg, u8 *pBuf,u8 bytes)
{
    u8 status,i;
    CSN_L_2;
    status = SPI_Send(SPI1,reg);//SPIx_ReadWriteByte(reg);    //ѡ��Ҫд��ļĴ���
    for(i=0; i<bytes; i++) //����д��bytes���ֽ�����
        SPI_Send(SPI1,*pBuf++);
    CSN_H_2;
    return(status);
}
void init_24L01_send(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_7);
	
	//PD12--CE,��������ĳ�pc8 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//CSN-PA12  �������  �ĳ�pc9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_7);
	
	//PD12--CE,��������ĳ�pc8 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//CSN-PA12  �������  �ĳ�pc9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);*/
	
	CE_L_2;
  RF_Write_Buf_send((NRF_WRITE_REG + TX_ADDR), TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  RF_Write_Buf_send((NRF_WRITE_REG + RX_ADDR_P0), TX_ADDRESS, TX_ADR_WIDTH); //�����Զ�Ӧ��ͨ��0��ַ��RX_Addr0��TX_AdrҪ��ͬ
  RF_Write_Reg_send(NRF_WRITE_REG+EN_AA,0x3f); //ʹ������ͨ�����Զ�Ӧ��
	//RF_Write_Reg_send(NRF_WRITE_REG+EN_AA,0x00); //ʹ������ͨ�����Զ�Ӧ��
	RF_Write_Reg_send(NRF_WRITE_REG+EN_RXADDR,0x3f); //ʹ������ͨ���Ľ��յ�ַ
	//RF_Write_Reg_send(NRF_WRITE_REG+EN_RXADDR,0x00); //ʹ������ͨ���Ľ��յ�ַ
	RF_Write_Reg_send(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	//RF_Write_Reg_send(NRF_WRITE_REG+SETUP_RETR,0x00);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	RF_Write_Reg_send(NRF_WRITE_REG+RF_CH,0); //����RFͨ��Ϊ0
	RF_Write_Reg_send(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��
	RF_Write_Reg_send(NRF_WRITE_REG+CONFIG,0x0e); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	CE_H_2;//CE�ø�,10us����������
}

/**********************************************************
** ������: RF_Read_Reg
** ���ܣ���nRF24L01�ļĴ���
** ���룺reg-Ҫ��ȡ�ļĴ�����ַ
** �������
** ���أ�reg_val-��ȡ�ļĴ�������
***********************************************************/
u8 RF_Read_Reg_send(u8 reg)
{  
    u8 reg_val;
    CSN_L_2;//CSN=0
    SPI_Send(SPI1,reg);         //ѡ��Ҫ��ȡ�ļĴ���
    reg_val=SPI_Send(SPI1,0xff);//��ȡ����
    CSN_H_2;//CSN=1
    return(reg_val);        // ���ض�ȡ������
}
/**********************************************************
** ������:nRF24L01_TxPacket
** ���ܣ�nRF24L01��������
** ���룺*tx_buf-Ҫ���͵������׵�ַָ��
** �������
** ���أ��������״̬
***********************************************************/
u8 nRF24L01_TxPacket_send(u8 *tx_buf)
{
    u8 status;
    CE_L_2;//CE=0   
    RF_Write_Buf_send(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // װ������    
    RF_Write_Reg_send(NRF_WRITE_REG + CONFIG, 0x0e);            // IRQ�շ�����ж���Ӧ��16λCRC��������
    CE_H_2; //�ø�CE���������ݷ���
    while(RF_IRQ_2!=0);//�ȴ��������
	//Delay_MS(1);	
	status=RF_Read_Reg_send(STATUS); //��ȡ״̬�Ĵ�����ֵ
//	USART1_SendData(0x11);//��ȡ����
 // USART1_SendData(status);//��ȡ����
	printf("send����ֵ:%x\r\n",status);
	RF_Write_Reg_send(NRF_WRITE_REG+STATUS,status); //���TX_DS��MAX_RT�жϱ�־
	if(status&MAX_TX)//�ﵽ����ط�����
	{
		RF_Write_Reg_send(FLUSH_TX,0xff);//���TX FIFO�Ĵ���
		printf("�ط�:%x\r\n",status);
		return MAX_TX;
	}
	else if(status&TX_OK)//�������
	{
		return TX_OK;
	}
	else
		return 0xff;//����ԭ����ʧ��
} 


