#include "stm32f10x.h"
#include <NRF24L01_Driver.h>
#include <Driver.h>

u8  TX_ADDRESS[RX_ADR_WIDTH]= {0x54,0x11,0x34,0x33,0x96};    //���͵����յ�ĵ�ַ

u8 Rx_Address0[TX_ADR_WIDTH] = {0x10,0x19,0x90,0x04,0x19}; //����ͨ��0�ĵ�ַ
u8 Rx_Address1[TX_ADR_WIDTH] = {0x21,0x11,0x34,0x33,0x96}; //����ͨ��1�ĵ�ַ
u8 Rx_Address2[TX_ADR_WIDTH] = {0x32,0x11,0x34,0x33,0x96}; //����ͨ��2�ĵ�ַ
u8 Rx_Address3[TX_ADR_WIDTH] = {0x43,0x11,0x34,0x33,0x96}; //����ͨ��3�ĵ�ַ
u8 Rx_Address4[TX_ADR_WIDTH] = {0x54,0x11,0x34,0x33,0x96}; //����ͨ��4�ĵ�ַ
u8 Rx_Address5[TX_ADR_WIDTH] = {0x65,0x11,0x34,0x33,0x96}; //����ͨ��5�ĵ�ַ

/**********************************************************
** ������: RF_SPI_Init
** ���ܣ�nRF2401��SPI�ӿڼ��õ���GPIO��ʼ������ 
** ���룺��
** �������
** ���أ���
***********************************************************/
void RF_GPIO_Init(void)
{

}

/**********************************************************
** ������: SPIx_ReadWriteByte
** ���ܣ�SPI2��дһ���ֽں���
** ���룺data--��Ҫ���͵�1�ֽ���
** �������
** ���أ����ض�ȡ��1�ֽ�����
***********************************************************/
//u8 SPIx_ReadWriteByte(u8 data)
//{
	
//   while((SPI2->SR&1<<1)==0);//�ȴ���������				  
// 	SPI2->DR=data;	 	  //����һ��byte   
// 	while((SPI2->SR&1<<0)==0);//�ȴ�������һ��byte  
//	return SPIx_Send(SPI2,data); //���ؽ��յ�����
//}
/**********************************************************
** ������: RF_Write_Reg
** ���ܣ�дnRF24L01�ļĴ���
** ���룺reg-�Ĵ�����ַ
		 value-д�������
** �������
** ���أ���
***********************************************************/
void RF_Write_Reg(u8 reg, u8 value)
{
    CSN_L; ////CSN=0��ʹ��nRF24L01Ƭѡ
    SPIx_Send(SPI2,reg);   //д�Ĵ�����ַ
    SPIx_Send(SPI2,value); //д����
    CSN_H; ////CSN=1��ʧ��nRF24L01Ƭѡ
}
/**********************************************************
** ������: RF_Read_Reg
** ���ܣ���nRF24L01�ļĴ���
** ���룺reg-Ҫ��ȡ�ļĴ�����ַ
** �������
** ���أ�reg_val-��ȡ�ļĴ�������
***********************************************************/
u8 RF_Read_Reg(u8 reg)
{  
    u8 reg_val;
    CSN_L;//CSN=0
    SPIx_Send(SPI2,reg);         //ѡ��Ҫ��ȡ�ļĴ���
    reg_val=SPIx_Send(SPI2,0xff);//��ȡ����
    CSN_H;//CSN=1
    return(reg_val);        // ���ض�ȡ������
}
/**********************************************************
** ������: RF_Read_Buf
** ���ܣ���nRF24L01�Ļ���������
** ���룺reg-Ҫ��ȡ�ļĴ�����ַ
		 *pBuf-�洢���ݵ������ַָ��
		 bytes-��ȡ�������ֽ���
** �������
** ���أ�status-����״̬�ֽ�
***********************************************************/
u8 RF_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
     u8 status,i;
     CSN_L;//CSN=0
     status = SPIx_Send(SPI2,reg);     //ѡ��Ĵ���
    for(i=0;i<bytes;i++)
        pBuf[i] = SPIx_Send(SPI2,0Xff);//������ȡbytes���ֽ�����
    CSN_H;////CSN=1
    return(status);
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
u8 RF_Write_Buf(u8 reg, u8 *pBuf,u8 bytes)
{
    u8 status,i;
    CSN_L;
    status = SPIx_Send(SPI2,reg);//SPIx_ReadWriteByte(reg);    //ѡ��Ҫд��ļĴ���
    for(i=0; i<bytes; i++) //����д��bytes���ֽ�����
        SPIx_Send(SPI2,*pBuf++);
    CSN_H;
    return(status);
}
/**********************************************************
** ������: init_24L01
** ���ܣ�nRF24L01���õĳ�ʼ������
** ���룺��
** �������
** ���أ���
** ��ע��û���õ�������ʹ���ڲ�Ĭ��ֵ
***********************************************************/
void init_24L01(void)
{       
				GPIO_InitTypeDef GPIO_InitStructure;
// 	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 
// 	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ�� 
// 	RCC->APB2ENR|=1<<5;       //PORTDʱ��ʹ��  
// 	RCC->APB1ENR|=1<<14;      //SPI2ʱ��ʹ��
// 	RCC->APB2ENR|=1<<0; //AFIO����ʱ��ʱ��ʹ��	

 
	//PA3--IRQ���������� �ĳ�pc4 		    
// 	GPIOA->CRL&=0XFFFF0FFF;//
// 	GPIOA->CRL|=0X00008000;//��/��������	 
// 	GPIOA->ODR|=1<<3;      //PA3����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	
// 	//PD12--CE,��������ĳ�pc5  
// 	GPIOD->CRH&=0XFFF0FFFF;
// 	GPIOD->CRH|=0X00030000;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//CSN-PA12  �������  �ĳ�pc6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//����ΪSPI2�ӿڳ�ʼ������
	//���ţ�CLK-PA13,MISO-PA14,MOSI-PA15,CSN-PA12
// 	GPIOB->CRH&=0X0000FFFF;//PB13.14.15�������
// 	GPIOB->CRH|=0XBBB30000;//PB12������� 	 
// 	GPIOB->ODR|=0XF000;    //PB12.13.14.15����  
	
	
	
	
// 	
// 	SPI2->CR1|=0<<10;//ȫ˫��ģʽ	
// 	SPI2->CR1|=1<<9; //���nss����
// 	SPI2->CR1|=1<<8;  

// 	SPI2->CR1|=1<<2; //SPI����ģʽ
// 	SPI2->CR1|=0<<11;//8bit���ݸ�ʽ	
// 	SPI2->CR1|=0<<1; //����ģʽ��SCKΪ1 CPOL=0
// 	SPI2->CR1|=0<<0; //���ݲ����ӵ�һ��ʱ����ؿ�ʼ,CPHA=0  
// 	SPI2->CR1|=1<<3; //Fsck=36MHz/4��24L01�����SPIʱ��Ϊ10MHz��
// 	SPI2->CR1|=0<<7; //MSBfirst 
// 	
// 	SPI2->CR1|=1<<6; //SPI�豸ʹ��
//       RF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x3f);      //����ͨ���Զ�Ӧ������
//       RF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x3f);  //��������ͨ������
//       RF_Write_Reg(NRF_WRITE_REG + SETUP_AW, 0x03);  //���õ�ַ���(��������ͨ��),5�ֽڿ��
//       RF_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
//       RF_Write_Reg(NRF_WRITE_REG + RF_CH, 0);        // ѡ����Ƶͨ��(0)
//       RF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0X0f);   //��Ƶ�Ĵ���,���ݴ�����(2Mbps),���书��(0dBm),�������Ŵ�������
//       RF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e); //��ʼ��Ϊ���ݷ���ģʽ
}
/**********************************************************
** ������: RX_Mode
** ���ܣ���nRF24L01��Ϊ����ģʽ
** ���룺channel-����ͨ��
** �������
** ���أ���
***********************************************************/
void RX_Mode(u8 channel)
{
   CE_L;   
 
   if(channel==0)//���ý���ͨ��0��ַ
   {
       RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P0), Rx_Address0, TX_ADR_WIDTH); 
       RF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, 32); //��������ͨ��0����Ч���ݿ��(1~32�ֽ�)
   }
   if(channel==1)//���ý���ͨ��1��ַ
   {
       RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P1), Rx_Address1, TX_ADR_WIDTH); 
       RF_Write_Reg(NRF_WRITE_REG + RX_PW_P1, 32); //��������ͨ��1����Ч���ݿ��(1~32�ֽ�)
   }
   if(channel==2) //���ý���ͨ��2��ַ
   {
        RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
        RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P2), Rx_Address2, TX_ADR_WIDTH);
        RF_Write_Reg(NRF_WRITE_REG + RX_PW_P2, 32); //��������ͨ��2����Ч���ݿ��(1~32�ֽ�)
        RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P2) , 0x32);
   } 
   if(channel==3)  //���ý���ͨ��3��ַ
   {
        RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
        RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P3), Rx_Address3, TX_ADR_WIDTH);
        RF_Write_Reg(NRF_WRITE_REG + RX_PW_P3, 32); //��������ͨ��3����Ч���ݿ��(1~32�ֽ�)
        RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P3) , 0x43);
   } 
   if(channel==4)
   {
        RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
        RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P4), Rx_Address4, TX_ADR_WIDTH);
        RF_Write_Reg(NRF_WRITE_REG + RX_PW_P4, 32); //��������ͨ��4����Ч���ݿ��(1~32�ֽ�)
        RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P4) , 0x54);
   }
   if(channel==5)
   {
       RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
       RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P5), Rx_Address5, TX_ADR_WIDTH);
       RF_Write_Reg(NRF_WRITE_REG + RX_PW_P5, 32); //��������ͨ��5����Ч���ݿ��(1~32�ֽ�)
       RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P5) , 0x65);
   } 
     
  RF_Write_Reg(NRF_WRITE_REG+EN_AA,0x3f); //ʹ������ͨ�����Զ�Ӧ��
	RF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3f); //ʹ������ͨ���Ľ��յ�ַ
	RF_Write_Reg(NRF_WRITE_REG+RF_CH,0); //����RFͨ��Ϊ0
	RF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��
	RF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
   	CE_H; //CE�ø�,�������ģʽ
}
/**********************************************************
** ������: TX_Mode
** ���ܣ���nRF24L01��Ϊ����ģʽ
** ���룺��
** �������
** ���أ���
***********************************************************/
void TX_Mode(void)
{
  CE_L;
  RF_Write_Buf((NRF_WRITE_REG + TX_ADDR), TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P0), TX_ADDRESS, TX_ADR_WIDTH); //�����Զ�Ӧ��ͨ��0��ַ��RX_Addr0��TX_AdrҪ��ͬ
  RF_Write_Reg(NRF_WRITE_REG+EN_AA,0x3f); //ʹ������ͨ�����Զ�Ӧ��
	RF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3f); //ʹ������ͨ���Ľ��յ�ַ
	RF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	RF_Write_Reg(NRF_WRITE_REG+RF_CH,0); //����RFͨ��Ϊ0
	RF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��
	RF_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	CE_H;//CE�ø�,10us����������
}

/**********************************************************
** ������:nRF24L01_RxPacket
** ���ܣ�nRF24L01���ݰ����գ�����Ӳ�ȴ���ʽ
** ���룺*rx_buf-�洢���ݵ������ַָ��
** �������
** ���أ�0xff,����0-5Ϊ��Ӧͨ��ֵ
***********************************************************/
u8 nRF24L01_RxPacket(u8 *rx_buf)
{   
    u8 status;
	status=RF_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����ֵ
	RF_Write_Reg(NRF_WRITE_REG+STATUS,status);
  if(status&RX_OK)//���յ�����
	{
		
		RF_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//��ȡ����
		RF_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ���
		if(status&Ch0) //ͨ��0���յ�����
			return Ch0;
		if(status&Ch1)//ͨ��1���յ�����
			return Ch1;
		if(status&Ch2) //ͨ��2���յ�����
			return Ch2;
		if(status&Ch3)	//ͨ��3���յ�����
			return Ch3;
		if(status&Ch4)
			return Ch4;
		if(status&Ch5)
			return Ch5;
	}else{
		return 0xff;//û�յ��κ�����
	}
	return 0;
}
/**********************************************************
** ������:nRF24L01_TxPacket
** ���ܣ�nRF24L01��������
** ���룺*tx_buf-Ҫ���͵������׵�ַָ��
** �������
** ���أ��������״̬
***********************************************************/
u8 nRF24L01_TxPacket(u8 *tx_buf)
{
    u8 status;
    CE_L;//CE=0   
    RF_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // װ������    
    RF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);            // IRQ�շ�����ж���Ӧ��16λCRC��������
    CE_H; //�ø�CE���������ݷ���
    while(RF_IRQ!=0);//�ȴ��������
	status=RF_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����ֵ
	RF_Write_Reg(NRF_WRITE_REG+STATUS,status); //���TX_DS��MAX_RT�жϱ�־
	if(status&MAX_TX)//�ﵽ����ط�����
	{
		RF_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ���
		return MAX_TX;
	}
	else if(status&TX_OK)//�������
	{
		return TX_OK;
	}
	else
		return 0xff;//����ԭ����ʧ��
} 
