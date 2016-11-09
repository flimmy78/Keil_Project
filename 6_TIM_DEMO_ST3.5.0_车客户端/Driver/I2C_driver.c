/********************************************************************
* �ļ� :I2C_driver.c
* �汾 ��V1
* ���� ��I2C���ߵ����ݴ���
* ���� : ��Ң
* ʱ�� ��2014-2-19
* ˵�� ��
********************************************************************/

#include"stm32f10x_i2c.h"
#include"stm32f10x_rcc.h"
#define	EQUIP_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
#define	I2C1_SLAVE_ADDRESS7	0xA0 
#define I2C_Speed	400000
#define	EQUIP_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define I2C_PageSize	16

void I2C_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	I2C_InitTypeDef 	I2C_InitStructure;

	//1����ʱ�� GPIOB��I2C1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	//2��PB10  PB11	���ÿ�©���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//3��I2C ����
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	//����I2CΪI2Cģʽ
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//I2C����ģʽ
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;//���������ַ
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	//ʹ��Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//I2C_AcknowledgedAddres������Ӧ��7λ��ַ����10λ��ַ��
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;//����

	//4��ʹ��I2Cģ��
	I2C_Cmd(I2C1,ENABLE);
	I2C_Init(I2C1,&I2C_InitStructure);

}

void I2C1_Init(void)
{
 	I2C_Configuration();	
}

void I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress)
{	
	//I2C_WaitEepromStandbyState();						    
	/* [1]Send Start Condition  ������ʼ�ź�*/
	I2C_GenerateSTART(I2C1,ENABLE);

	/* [2]Test On EV5 and clear it  ��ʼ�ź��ѷ��Ͳ�������¼� */
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

	/* [3]Send EEPROM address for write  ����������ַ*/
 	I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);

	/* [4]Test on Ev6 and clear it ��ַ���ͽ��� */
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	
	/* [5]Send EEPROM's internal address to  write ���������ڲ�д���ַ */
	I2C_SendData(I2C1,WriteAddress);

	/* [6]Test on EV8 _1 and clear it ��λ�Ĵ����� */
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));


	/* [7]Send the byte to be writeen ��������*/
	I2C_SendData(I2C1,SendByte);

	/* [8]Test on EV8 and clear it ���ͻ�������*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));


	/* [9]Send STOP condition ����ֹͣ�ź� */
	I2C_GenerateSTOP(I2C1,ENABLE);
}


void I2C_PageWrite(u8* pBuffer, u8 length, u16 WriteAddress, u8 DeviceAddress)
{
	//I2C_WaitEepromStandbyState();
	/*[1]Send START condition ������ʼ����*/
	I2C_GenerateSTART(I2C1,ENABLE);

	/*[2]Test on EV5 and clear it ��ʼ�źŷ����Ƿ�ɹ�*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

	/*[3]Send EEPROM address for write ����������ַ*/
	I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);

	/*[4]Test on EV6 and clear it  ����������ַ�Ƿ�ɹ�*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*[5]Send EEPROM'S internal address to write to �������ݵ�д���׵�ַ*/
	I2C_SendData(I2C1,WriteAddress);
		
	/*[6]Test on EV8 and clear it  �����ڲ���ַ�Ƿ�ɹ�*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/*[7]Send data to Written ��������*/
				while(length--)
				{
				 	/*Send the current byte ���͵�ǰһ���ֽ�*/
					 	I2C_SendData(I2C1,*pBuffer);
				 	/* Point to the next byte to be written ��ַ++*/
						pBuffer++;
				 	/*Test on EV8 and clear it ���ͻ������Ƿ�Ϊ��*/
					while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
				 }
	/*[8]Send STOP condition ����ֹͣ�ź�*/
	I2C_GenerateSTOP(I2C1,ENABLE);
}







void I2C_ReadByte(u8* pBuffer, u8 length, u16 ReadAddress, u8 DeviceAddress)
{
//	I2C_WaitEepromStandbyState();
	/*Send START Condition ������ʼ�ź�*/
	I2C_GenerateSTART(I2C1,ENABLE);

	/*Test on EV5 and clear it �����ʼ�ź��Ƿ��ͳɹ�*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

	/* In the case of a singel data transfer disable ACK before readint the data*/
	if(length==1)
	{
		I2C_AcknowledgeConfig(I2C1,DISABLE);//���NumByteToRead����1�ֽ�һӦ��
	}
	/*Send EEPROM ADDRESS for write */
	I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);

	/* Test on EV6 and clear it*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Enable I2C*/
	I2C_Cmd(I2C1,ENABLE);

	/* Send EEPROM'S internal address to write to*/
	I2C_SendData(I2C1,ReadAddress);

	/*Test on EV8 and clear it*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/*Send START condition again*/
	I2C_GenerateSTART(I2C1,ENABLE);

	/*Test on EV5 and clear it*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

	/*Send EEPROM address for read*/
	I2C_Send7bitAddress(I2C1,EQUIP_Addr,I2C_Direction_Receiver);

	/*Test on EV6 and clear it*/
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /*Read Data*/
	while(length)
	{
		/*Test on EV7 and clear it*/
		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			if(length == 2)				
			{
				/*Disable Acknowledgement*/
				I2C_AcknowledgeConfig(I2C1,DISABLE); 	//һ�������һ���ֽڹر�Ӧ��
			}
			if(length == 1) 
			{
				/*Send STOP condition */  	//���һλҪ�ر�Ӧ�𣬷���ֹͣ�ź�
				I2C_GenerateSTOP(I2C1,ENABLE);
			}
			/*Read a byte from the EEPROM*/
			*pBuffer = I2C_ReceiveData(I2C1);
			/*Point to the next location where the byte read will be saved*/
			pBuffer++;
			/*Decrement the read bytes counter*/
			length--;
		}
	}
	/*Enable Acknowledgement to be ready for anotherreception*/
   I2C_AcknowledgeConfig(I2C1,ENABLE);		//�����ٴ�Ӧ��		
}


//void I2C_WaitEepromStandbyState(void)
//{
//	return;
// 	vu16 SR1_Tmp = 0;

//   do
//   {
//     /* Send START condition */
//     I2C_GenerateSTART(I2C1, ENABLE);
//     /* Read I2C1 SR1 register */
//     SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
//     /* Send EEPROM address for write */
//     I2C_Send7bitAddress(I2C1, EQUIP_Addr, I2C_Direction_Transmitter);
//   }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
//   
//   /* Clear AF flag */
//   I2C_ClearFlag(I2C1, I2C_FLAG_AF);	
//}

// void I2C_BufferWrite(u8 *pBuffer,u8 WriteAddr,u16 NumByteToWrite)
//  {
//  	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

//   	Addr = WriteAddr % I2C_PageSize;
//   	count = I2C_PageSize - Addr;
//   	NumOfPage =  NumByteToWrite / I2C_PageSize;
//   	NumOfSingle = NumByteToWrite % I2C_PageSize;

// 	I2C_WaitEepromStandbyState();
//  
//   /* If WriteAddr is I2C_PageSize aligned  */
//   if(Addr == 0) 
//   {
//     /* If NumByteToWrite < I2C_PageSize */
//     if(NumOfPage == 0) 
//     {
//       I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
//       I2C_WaitEepromStandbyState();
//     }
//     /* If NumByteToWrite > I2C_PageSize */
//     else  
//     {
//       while(NumOfPage--)
//       {
//         I2C_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
//     	I2C_WaitEepromStandbyState();
//         WriteAddr +=  I2C_PageSize;
//         pBuffer += I2C_PageSize;
//       }

//       if(NumOfSingle!=0)
//       {
//         I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
//         I2C_WaitEepromStandbyState();
//       }
//     }
//   }
//   /* If WriteAddr is not I2C_PageSize aligned  */
//   else 
//   {
//     /* If NumByteToWrite < I2C_PageSize */
//     if(NumOfPage== 0) 
//     {
//       I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
//       I2C_WaitEepromStandbyState();
//     }
//     /* If NumByteToWrite > I2C_PageSize */
//     else
//     {
//       NumByteToWrite -= count;
//       NumOfPage =  NumByteToWrite / I2C_PageSize;
//       NumOfSingle = NumByteToWrite % I2C_PageSize;	
//       
//       if(count != 0)
//       {  
//         I2C_PageWrite(pBuffer, WriteAddr, count);
//         I2C_WaitEepromStandbyState();
//         WriteAddr += count;
//         pBuffer += count;
//       } 
//       
//       while(NumOfPage--)
//       {
//         I2C_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
//         I2C_WaitEepromStandbyState();
//         WriteAddr +=  I2C_PageSize;
//         pBuffer += I2C_PageSize;  
//       }
//       if(NumOfSingle != 0)
//       {
//         I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
//         I2C_WaitEepromStandbyState();
//       }
//     }
//   }  	
//  }


