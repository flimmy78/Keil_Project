#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "NRF24L01_Driver.h"

NRF24L01_T NRF_SPI2_SEND;
NRF24L01_T* nrf1= &NRF_SPI2_SEND;

u8  TX_ADDRESS[ADR_WIDTH]= {0x01,0x02,0x03,0x04,0x05};    //���͵����յ�ĵ�ַ

//
u8 Dev_Address0[ADR_WIDTH] = {0x05,0x04,0x03,0x02,0x01}; //����ͨ��0�ĵ�ַ
u8 Dev_Address1[ADR_WIDTH] = {0x21,0x11,0x34,0x33,0x96}; //����ͨ��1�ĵ�ַ
u8 Dev_Address2[ADR_WIDTH] = {0x32,0x11,0x34,0x33,0x96}; //����ͨ��2�ĵ�ַ
u8 Dev_Address3[ADR_WIDTH] = {0x43,0x11,0x34,0x33,0x96}; //����ͨ��3�ĵ�ַ
u8 Dev_Address4[ADR_WIDTH] = {0x54,0x11,0x34,0x33,0x96}; //����ͨ��4�ĵ�ַ
u8 Dev_Address5[ADR_WIDTH] = {0x65,0x11,0x34,0x33,0x96}; //����ͨ��5�ĵ�ַ

u8 _SPIx_Send(SPI_TypeDef* SPIx,u8 Buff)
{
		//�ȴ�SPI���У������ͻ������ձ�־λ��
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		//����1���ֽ�
		SPI_I2S_SendData(SPIx, Buff);
		//�����ջ������ǿձ�־λ
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		return (u8)SPI_I2S_ReceiveData(SPIx);
}
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
//	return _SPIx_Send(nrf->SPI,data); //���ؽ��յ�����
//}
/**********************************************************
** ������: RF_Write_Reg
** ���ܣ�дnRF24L01�ļĴ���
** ���룺reg-�Ĵ�����ַ
		 value-д�������
** �������
** ���أ���
***********************************************************/
void _RF_Write_Reg(NRF24L01_T* nrf,u8 reg, u8 value)
{
    nrf->CSN_Reset(nrf); ////CSN=0��ʹ��nRF24L01Ƭѡ
    _SPIx_Send(nrf->SPI,reg);   //д�Ĵ�����ַ
    _SPIx_Send(nrf->SPI,value); //д����
    nrf->CSN_Set(nrf); ////CSN=1��ʧ��nRF24L01Ƭѡ
}
/**********************************************************
** ������: RF_Read_Reg
** ���ܣ���nRF24L01�ļĴ���
** ���룺reg-Ҫ��ȡ�ļĴ�����ַ
** �������
** ���أ�reg_val-��ȡ�ļĴ�������
***********************************************************/
u8 _RF_Read_Reg(NRF24L01_T* nrf,u8 reg)
{  
    u8 reg_val;
    nrf->CSN_Reset(nrf);//CSN=0
    _SPIx_Send(nrf->SPI,reg);         //ѡ��Ҫ��ȡ�ļĴ���
    reg_val=_SPIx_Send(nrf->SPI,0xff);//��ȡ����
    nrf->CSN_Set(nrf);//CSN=1
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
u8 _RF_Read_Buf(NRF24L01_T* nrf,u8 reg, u8 *pBuf, u8 bytes)
{
     u8 status,i;
     nrf->CSN_Reset(nrf);//CSN=0
     status = _SPIx_Send(nrf->SPI,reg);     //ѡ��Ĵ���
    for(i=0;i<bytes;i++)
        pBuf[i] = _SPIx_Send(nrf->SPI,0Xff);//������ȡbytes���ֽ�����
    nrf->CSN_Set(nrf);////CSN=1
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
u8 _RF_Write_Buf(NRF24L01_T* nrf,u8 reg, u8 *pBuf,u8 bytes)
{
    u8 status,i;
    nrf->CSN_Reset(nrf);
    status = _SPIx_Send(nrf->SPI,reg);//SPIx_ReadWriteByte(reg);    //ѡ��Ҫд��ļĴ���
    for(i=0; i<bytes; i++) //����д��bytes���ֽ�����
        _SPIx_Send(nrf->SPI,*pBuf++);
    nrf->CSN_Set(nrf);
    return(status);
}



/**********************************************************
** ������: TX_Mode
** ���ܣ���nRF24L01��Ϊ����ģʽ
** ���룺��
** �������
** ���أ���
***********************************************************/
void _Set_TX_Mode(NRF24L01_T* nrf)
{
  nrf->CE_Set(nrf);
  nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + TX_ADDR), TX_ADDRESS, ADR_WIDTH);    // Writes TX_Address to nRF24L01
  nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P0), TX_ADDRESS, ADR_WIDTH); //�����Զ�Ӧ��ͨ��0��ַ��RX_Addr0��TX_AdrҪ��ͬ
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_AA,0x3f); //ʹ������ͨ�����Զ�Ӧ��
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_RXADDR,0x3f); //ʹ������ͨ���Ľ��յ�ַ
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_CH,0); //����RFͨ��Ϊ0
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+CONFIG,0x0e); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	nrf->CE_Reset(nrf);//CE�ø�,10us����������
}

/**********************************************************
** ������: RX_Mode
** ���ܣ���nRF24L01��Ϊ����ģʽ
** ���룺channel-����ͨ��
** �������
** ���أ���
***********************************************************/
void _Set_RX_Mode(NRF24L01_T* nrf)
{
   nrf->CE_Set(nrf);   
 
   /*if(channel==0)//���ý���ͨ��0��ַ
   {
       nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P0), Dev_Address0, ADR_WIDTH); 
       nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P0, 32); //��������ͨ��0����Ч���ݿ��(1~32�ֽ�)
   }
   if(channel==1)//���ý���ͨ��1��ַ
   {
       nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P1), Dev_Address1, ADR_WIDTH); 
       nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P1, 32); //��������ͨ��1����Ч���ݿ��(1~32�ֽ�)
   }
   if(channel==2) //���ý���ͨ��2��ַ
   {
        nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
        nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P2), Dev_Address2, ADR_WIDTH);
        nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P2, 32); //��������ͨ��2����Ч���ݿ��(1~32�ֽ�)
        nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P2) , 0x32);
   } 
   if(channel==3)  //���ý���ͨ��3��ַ
   {
        nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
        nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P3), Dev_Address3, ADR_WIDTH);
        nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P3, 32); //��������ͨ��3����Ч���ݿ��(1~32�ֽ�)
        nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P3) , 0x43);
   } 
   if(channel==4)
   {
        nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
        nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P4), Dev_Address4, ADR_WIDTH);
        nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P4, 32); //��������ͨ��4����Ч���ݿ��(1~32�ֽ�)
        nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P4) , 0x54);
   }
   if(channel==5)
   {
       nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
       nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P5), Dev_Address5, ADR_WIDTH);
       nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P5, 32); //��������ͨ��5����Ч���ݿ��(1~32�ֽ�)
       nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P5) , 0x65);
   } */
  nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P0), Dev_Address0, ADR_WIDTH); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P0, 32); //��������ͨ��0����Ч���ݿ��(1~32�ֽ�)
	 
	nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P1), Dev_Address1, ADR_WIDTH); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P1, 32); //��������ͨ��1����Ч���ݿ��(1~32�ֽ�)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P2) , Dev_Address2[0]);
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P2, 32); //��������ͨ��2����Ч���ݿ��(1~32�ֽ�)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P3) , Dev_Address3[0]);
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P3, 32); //��������ͨ��3����Ч���ݿ��(1~32�ֽ�)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P4) , Dev_Address4[0]); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P4, 32); //��������ͨ��4����Ч���ݿ��(1~32�ֽ�)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P5) , Dev_Address5[0]); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P5, 32); //��������ͨ��5����Ч���ݿ��(1~32�ֽ�)
	    
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_AA,0x3f); //ʹ������ͨ�����Զ�Ӧ��
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_RXADDR,0x3f); //ʹ������ͨ���Ľ��յ�ַ
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_CH,0); //����RFͨ��Ϊ0
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_SETUP,0x0f); //����TX�������,0db����,2Mbps,���������濪��
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
  
	 nrf->CE_Reset(nrf); //CE�ø�,�������ģʽ
}


/**********************************************************
** ������:nRF24L01_RxPacket
** ���ܣ�nRF24L01���ݰ����գ�����Ӳ�ȴ���ʽ
** ���룺*rx_buf-�洢���ݵ������ַָ��
** �������
** ���أ�0xff,����0-5Ϊ��Ӧͨ��ֵ
***********************************************************/
u8 _nRF24L01_RxPacket(NRF24L01_T* nrf,u8 *rx_buf)
{   
    u8 status;
	status=nrf->RF_Read_Reg(nrf,STATUS); //��ȡ״̬�Ĵ�����ֵ
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+STATUS,status);
  if(status&RX_OK)//���յ�����
	{
		
		nrf->RF_Read_Buf(nrf,RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//��ȡ����
		nrf->RF_Write_Reg(nrf,FLUSH_RX,0xff);//���RX FIFO�Ĵ���
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
u8 _nRF24L01_TxPacket(NRF24L01_T* nrf,u8 *tx_buf)
{
    u8 status;
    nrf->CE_Set(nrf);//CE=0   
    nrf->RF_Write_Buf(nrf,WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // װ������    
    nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + CONFIG, 0x0e);            // IRQ�շ�����ж���Ӧ��16λCRC��������
    nrf->CE_Reset(nrf); //�ø�CE���������ݷ���
    while(nrf->IRQ_Get(nrf)!=0);//�ȴ��������
	status=nrf->RF_Read_Reg(nrf,STATUS); //��ȡ״̬�Ĵ�����ֵ
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+STATUS,status); //���TX_DS��MAX_RT�жϱ�־
	if(status&MAX_TX)//�ﵽ����ط�����
	{
		nrf->RF_Write_Reg(nrf,FLUSH_TX,0xff);//���TX FIFO�Ĵ���
		printf("��ʱ�ط�\r\n");
		return MAX_TX;
	}
	else if(status&TX_OK)//�������
	{
		printf("�������\r\n");
		return TX_OK;
	}
	else{
		printf("δ֪ԭ��\r\n");
		return 0xff;//����ԭ����ʧ��
	}
} 
void _CE_Set(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CE_GPIO,nrf->CE_GPIO_Pin,Bit_SET);
}
void _CE_Reset(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CE_GPIO,nrf->CE_GPIO_Pin,Bit_RESET);
}
void _CSN_Set(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CSN_GPIO,nrf->CSN_GPIO_Pin,Bit_SET);
}
void _CSN_Reset(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CSN_GPIO,nrf->CSN_GPIO_Pin,Bit_RESET);
}
u8 _IRQ_Get(NRF24L01_T* nrf){
	return GPIO_ReadInputDataBit(nrf->IRQ_GPIO,nrf->IRQ_GPIO_Pin);
}
/**********************************************************
** ������: init_24L01
** ���ܣ�nRF24L01���õĳ�ʼ������
** ���룺��
** �������
** ���أ���
** ��ע��û���õ�������ʹ���ڲ�Ĭ��ֵ
***********************************************************/
void NRF24L01_Init(NRF24L01_T* nrf)
{  
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//---CE PC4		//оƬ��������
	GPIO_InitStructure.GPIO_Pin = nrf->CE_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(nrf->CE_GPIO, &GPIO_InitStructure);
	
	//---CSN PA4	//spiʹ��
	GPIO_InitStructure.GPIO_Pin = nrf->CSN_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(nrf->CSN_GPIO, &GPIO_InitStructure);
	
	//---IRQ PC5	//оƬ�ж�����
	GPIO_InitStructure.GPIO_Pin = nrf->IRQ_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(nrf->IRQ_GPIO, &GPIO_InitStructure);
	
	nrf->CE_Set = _CE_Set;
	nrf->CE_Reset = _CE_Reset;
	nrf->CSN_Set = _CSN_Set;
	nrf->CSN_Reset = _CSN_Reset;
	nrf->IRQ_Get = _IRQ_Get;
	
	
	nrf->RF_Write_Reg = _RF_Write_Reg;
	nrf->RF_Write_Buf = _RF_Write_Buf;
	nrf->RF_Read_Reg = _RF_Read_Reg;
	nrf->RF_Read_Buf = _RF_Read_Buf;
	nrf->nRF24L01_TxPacket = _nRF24L01_TxPacket;
	nrf->nRF24L01_RxPacket = _nRF24L01_RxPacket;
	nrf->Set_TX_Mode = _Set_TX_Mode;
	nrf->Set_RX_Mode = _Set_RX_Mode;
	//void (*NRF24L01_Init)(void);
	
}

