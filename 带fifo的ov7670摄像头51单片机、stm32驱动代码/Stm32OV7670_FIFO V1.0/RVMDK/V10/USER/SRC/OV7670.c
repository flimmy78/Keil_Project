/*******************************************************************************
*  Copyright (C) 2010 - All Rights Reserved
*		
* ��������:	��ɼ���
* ��Ȩ����: ��ɵ���	
* ��������:	2012��8��18�� 
* ������ʷ:	2012��8��18���װ�
* Version:  1.0 
* Demo �Ա���ַ��http://store.taobao.com/shop/view_shop.htm?asker=wangwang&shop_nick=qifeidianzi
**********************************************************************************************************************************************
����������������������������������������������������������������������������������������������������������������������������������������������
����������������������һ������������������������������������������������������������һ����������������������������һһһһһһһ��������������
������һһһһ��һһһһһһ������������һһһһһһһһ�������������������ą�����һһ������һ������������������һһһһ����һһһ������������
������������һ������һ��һ������������һһһһ�ą���һ����һһ������������һһһһһһһһһһһ������������������������һһһһ��������������
�������ą���һ��������������������������������������һ��һһ��������������һһ����һһ������һһ������������������������һһ������������������
������һ����һһһһһһһһһһһ������������������һһ������������������һһһһһһһһһһ������������һһһһһһһһһһһһһһһ������
������һһһһһ����һһ��һ����������������������һһһһһ��������������һһ����һһ������һ������������һһ����������һһ������һһ��������
��������������һ��һ��һ��һ������������������������һ����һһ������������һһһһһһһһһһ��������������������������һһ������������������
����һһһ��������һһһ��һ������������������������һһ����������������������������һ��������������һ������������������һһ������������������
������������һ����������һһ��������������������������һһһһһһ������������������һһһһһһһһһ������������������һһ������������������
��������һһһ������һһһһ����������������������������һһһһһ��������������������һһһһһһһ��������������һһһһ��������������������
����������������������������������������������������������������������������������������������������������������������������������������������
**********************************************************************************************************************************************/

/***************************************************************************
��    �ƣ� OV7670.C
��    �ܣ� �Ӻ�������
��    �ߣ� H&G
��    ���� V1.0
����ʱ�䣺 2011-6-10
***************************************************************************/

/***************************************************************************
ͷ�ļ�
***************************************************************************/
#include<I2C.h>
#include<delay.h>
#include<ov7670.h>
/***************************************************************************
��    �ƣ�unsigned char WrCmos7670(unsigned char regID, unsigned char regDat)
��    �ܣ���CMOSָ���Ĵ�����дֵ
��ڲ�����regID  ָ���Ĵ�����ַ
          regDat ��д��ָ���Ĵ�����ֵ 
���ڲ�����1    д��ɹ�
          0    д��ʧ��
˵    ����
���÷�����m=WrCmos7670(0x3a,0x04);
***************************************************************************/
unsigned char WrCmos7670(unsigned char regID, unsigned char regDat)
{
	StartI2C0();
	if(0==I2CWrite0(OV7670_DEVICE_WRITE_ADDRESS))           //CMOS������ַ��д��
	{
		StopI2C0();
		return(0);
	}
	delay_us(100);
  	if(0==I2CWrite0(regID))         //CMOS�Ĵ�����ַ
	{
		StopI2C0();
		return(0);
	}
	delay_us(100);
  	if(0==I2CWrite0(regDat))       //��д��ָ���Ĵ�����ֵ
	{
		StopI2C0();
		return(0);
	}
  	StopI2C0();
	
  	return(1);
}  	

unsigned char rdCmos7670Reg(unsigned char regID)
{
	unsigned char regDat;
	StartI2C0();
	if(0==I2CWrite0(OV7670_DEVICE_WRITE_ADDRESS))
	{
		StopI2C0();
		return(0);
	}
	delay_us(100);
  	if(0==I2CWrite0(regID))
	{
		StopI2C0();
		return(0);
	}
	StopI2C0();
	
	delay_us(100);

	StartI2C0();
	if(0==I2CWrite0(OV7670_DEVICE_READ_ADDRESS))
	{
		StopI2C0();
		return(0);
	}
	delay_us(100);
  	regDat=I2CRead0();
  	NoAck0();
  	StopI2C0();
  	return regDat;
}

/***************************************************************************
��    �ƣ�void set_Cmos7670reg(void)
��    �ܣ�CMOS�Ĵ�������
��ڲ�������
���ڲ�������
˵    ����
���÷�����set_Cmos7670reg();
***************************************************************************/
void set_Cmos7670reg(void)
{	 

	WrCmos7670(0x3a, 0x04);
	WrCmos7670(0x40, 0xd0);
	WrCmos7670(0x12, 0x14);
	WrCmos7670(0x32, 0x80);
	WrCmos7670(0x17, 0x16);
	WrCmos7670(0x18, 0x04);
	WrCmos7670(0x19, 0x02);
	WrCmos7670(0x1a, 0x7b);
	WrCmos7670(0x03, 0x06);
	WrCmos7670(0x0c, 0x00);
	WrCmos7670(0x3e, 0x00);
	WrCmos7670(0x70, 0x3a);
	WrCmos7670(0x71, 0x35);
	WrCmos7670(0x72, 0x11);
	WrCmos7670(0x73, 0x00);
	WrCmos7670(0xa2, 0x02);
	WrCmos7670(0x11, 0x81);
	
	WrCmos7670(0x7a, 0x20);
	WrCmos7670(0x7b, 0x1c);
	WrCmos7670(0x7c, 0x28);
	WrCmos7670(0x7d, 0x3c);
	WrCmos7670(0x7e, 0x55);
	WrCmos7670(0x7f, 0x68);
	WrCmos7670(0x80, 0x76);
	WrCmos7670(0x81, 0x80);
	WrCmos7670(0x82, 0x88);
	WrCmos7670(0x83, 0x8f);
	WrCmos7670(0x84, 0x96);
	WrCmos7670(0x85, 0xa3);
	WrCmos7670(0x86, 0xaf);
	WrCmos7670(0x87, 0xc4);
	WrCmos7670(0x88, 0xd7);
	WrCmos7670(0x89, 0xe8);
	
	WrCmos7670(0x13, 0xe0);
	WrCmos7670(0x00, 0x00);
	
	WrCmos7670(0x10, 0x00);
	WrCmos7670(0x0d, 0x00);
	WrCmos7670(0x14, 0x28);
	WrCmos7670(0xa5, 0x05);
	WrCmos7670(0xab, 0x07);
	WrCmos7670(0x24, 0x75);
	WrCmos7670(0x25, 0x63);
	WrCmos7670(0x26, 0xA5);
	WrCmos7670(0x9f, 0x78);
	WrCmos7670(0xa0, 0x68);
	WrCmos7670(0xa1, 0x03);
	WrCmos7670(0xa6, 0xdf);
	WrCmos7670(0xa7, 0xdf);
	WrCmos7670(0xa8, 0xf0);
	WrCmos7670(0xa9, 0x90);
	WrCmos7670(0xaa, 0x94);
	WrCmos7670(0x13, 0xe5);

	WrCmos7670(0x0e, 0x61);
	WrCmos7670(0x0f, 0x4b);
	WrCmos7670(0x16, 0x02);
	WrCmos7670(0x1e, 0x37);
	WrCmos7670(0x21, 0x02);
	WrCmos7670(0x22, 0x91);
	WrCmos7670(0x29, 0x07);
	WrCmos7670(0x33, 0x0b);
	WrCmos7670(0x35, 0x0b);
	WrCmos7670(0x37, 0x1d);
	WrCmos7670(0x38, 0x71);
	WrCmos7670(0x39, 0x2a);
	WrCmos7670(0x3c, 0x78);
	WrCmos7670(0x4d, 0x40);
	WrCmos7670(0x4e, 0x20);
	WrCmos7670(0x69, 0x00);
	WrCmos7670(0x6b, 0x60);
	WrCmos7670(0x74, 0x19);
	WrCmos7670(0x8d, 0x4f);
	WrCmos7670(0x8e, 0x00);
	WrCmos7670(0x8f, 0x00);
	WrCmos7670(0x90, 0x00);
	WrCmos7670(0x91, 0x00);
	WrCmos7670(0x92, 0x00);
	WrCmos7670(0x96, 0x00);
	WrCmos7670(0x9a, 0x80);
	WrCmos7670(0xb0, 0x84);
	WrCmos7670(0xb1, 0x0c);
	WrCmos7670(0xb2, 0x0e);
	WrCmos7670(0xb3, 0x82);
	WrCmos7670(0xb8, 0x0a);



	WrCmos7670(0x43, 0x14);
	WrCmos7670(0x44, 0xf0);
	WrCmos7670(0x45, 0x34);
	WrCmos7670(0x46, 0x58);
	WrCmos7670(0x47, 0x28);
	WrCmos7670(0x48, 0x3a);
	WrCmos7670(0x59, 0x88);
	WrCmos7670(0x5a, 0x88);
	WrCmos7670(0x5b, 0x44);
	WrCmos7670(0x5c, 0x67);
	WrCmos7670(0x5d, 0x49);
	WrCmos7670(0x5e, 0x0e);
	WrCmos7670(0x64, 0x04);
	WrCmos7670(0x65, 0x20);
	WrCmos7670(0x66, 0x05);
	WrCmos7670(0x94, 0x04);
	WrCmos7670(0x95, 0x08);
	WrCmos7670(0x6c, 0x0a);
	WrCmos7670(0x6d, 0x55);
	WrCmos7670(0x6e, 0x11);
	WrCmos7670(0x6f, 0x9f);
	WrCmos7670(0x6a, 0x40);
	WrCmos7670(0x01, 0x40);
	WrCmos7670(0x02, 0x40);
	WrCmos7670(0x13, 0xe7);
	WrCmos7670(0x15, 0x00);  
	
	
	WrCmos7670(0x4f, 0x80);
	WrCmos7670(0x50, 0x80);
	WrCmos7670(0x51, 0x00);
	WrCmos7670(0x52, 0x22);
	WrCmos7670(0x53, 0x5e);
	WrCmos7670(0x54, 0x80);
	WrCmos7670(0x58, 0x9e);
	
	WrCmos7670(0x41, 0x08);
	WrCmos7670(0x3f, 0x00);
	WrCmos7670(0x75, 0x05);
	WrCmos7670(0x76, 0xe1);
	WrCmos7670(0x4c, 0x00);
	WrCmos7670(0x77, 0x01);
	WrCmos7670(0x3d, 0xc2);	
	WrCmos7670(0x4b, 0x09);
	WrCmos7670(0xc9, 0x60);
	WrCmos7670(0x41, 0x38);
	WrCmos7670(0x56, 0x40);
	
	WrCmos7670(0x34, 0x11);
	WrCmos7670(0x3b, 0x02); 
								
	WrCmos7670(0xa4, 0x89);
	WrCmos7670(0x96, 0x00);
	WrCmos7670(0x97, 0x30);
	WrCmos7670(0x98, 0x20);
	WrCmos7670(0x99, 0x30);
	WrCmos7670(0x9a, 0x84);
	WrCmos7670(0x9b, 0x29);
	WrCmos7670(0x9c, 0x03);
	WrCmos7670(0x9d, 0x4c);
	WrCmos7670(0x9e, 0x3f);
	WrCmos7670(0x78, 0x04);
	
	WrCmos7670(0x79, 0x01);
	WrCmos7670(0xc8, 0xf0);
	WrCmos7670(0x79, 0x0f);
	WrCmos7670(0xc8, 0x00);
	WrCmos7670(0x79, 0x10);
	WrCmos7670(0xc8, 0x7e);
	WrCmos7670(0x79, 0x0a);
	WrCmos7670(0xc8, 0x80);
	WrCmos7670(0x79, 0x0b);
	WrCmos7670(0xc8, 0x01);
	WrCmos7670(0x79, 0x0c);
	WrCmos7670(0xc8, 0x0f);
	WrCmos7670(0x79, 0x0d);
	WrCmos7670(0xc8, 0x20);
	WrCmos7670(0x79, 0x09);
	WrCmos7670(0xc8, 0x80);
	WrCmos7670(0x79, 0x02);
	WrCmos7670(0xc8, 0xc0);
	WrCmos7670(0x79, 0x03);
	WrCmos7670(0xc8, 0x40);
	WrCmos7670(0x79, 0x05);
	WrCmos7670(0xc8, 0x30);
	WrCmos7670(0x79, 0x26); 
	WrCmos7670(0x09, 0x00);	
	
	
}

/***************************************************************************
��    �ƣ�unsigned char Cmos7670_init(void)
��    �ܣ�CMOS��ʼ��
��ڲ�������
���ڲ�����1 ��ʼ���ɹ�
          0 ��ʼ��ʧ��
˵    ����
���÷�����m=Cmos7670_init();
***************************************************************************/
unsigned char Cmos7670_init(void)
{
	unsigned char mmm;	

	
	InitI2C0();

	mmm=0x80;
	if(0==WrCmos7670(0x12, mmm)) 
	{
		return 0 ;
	}
	delay_ms(10);

  	set_Cmos7670reg();

	return 1; 
} 

