
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ο�������guanfu_wang���롣
//ALIENTEKս��STM32������V3
//SCCB ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/18
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
void SCCB_SDA_OUT(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
 	//sda
  GPIO_InitStructure.GPIO_Pin = ov7670->SIOD_GPIO_Pin;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(ov7670->SIOD_GPIOx, &GPIO_InitStructure);
 	GPIO_SetBits(ov7670->SIOD_GPIOx,ov7670->SIOD_GPIO_Pin);						 // �����
}

void SCCB_SDA_IN(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
 	//sda
  GPIO_InitStructure.GPIO_Pin = ov7670->SIOD_GPIO_Pin;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(ov7670->SIOD_GPIOx, &GPIO_InitStructure);
}
//��ʼ��SCCB�ӿ�
//CHECK OK
void SCCB_Init(void)
{			
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB�˿�ʱ��
	//sda
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_7);						 // �����

	//scl
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�����
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_6);						 // �����
 
	SCCB_SDA_OUT();	   
}			 

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    SCCB_SDA(1);     //�����߸ߵ�ƽ	   
    SCCB_SCL(1);	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    DelayUS(50);  
    SCCB_SDA(0);
    DelayUS(50);	 
    SCCB_SCL(0);	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    SCCB_SDA(0);
    DelayUS(50);	 
    SCCB_SCL(1);	
    DelayUS(50); 
    SCCB_SDA(1);	
    DelayUS(50);
}  
//����NA�ź�
void SCCB_No_Ack(void)
{
	DelayUS(50);
	SCCB_SDA(1);	
	SCCB_SCL(1);	
	DelayUS(50);
	SCCB_SCL(0);	
	DelayUS(50);
	SCCB_SDA(0);	
	DelayUS(50);
}
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SDA(1);	
		else SCCB_SDA(0);
		dat<<=1;
		DelayUS(50);
		SCCB_SCL(1);	
		DelayUS(50);
		SCCB_SCL(0);		   
	}			 
	SCCB_SDA_IN();		//����SDAΪ���� 
	DelayUS(50);
	SCCB_SCL(1);			//���յھ�λ,���ж��Ƿ��ͳɹ�
	DelayUS(50);
	if(SCCB_READ_SDA)res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	SCCB_SCL(0);		 
	SCCB_SDA_OUT();		//����SDAΪ���    
	return res;  
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		DelayUS(50);
		SCCB_SCL(1);
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		DelayUS(50);
		SCCB_SCL(0);
	}	
	SCCB_SDA_OUT();		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	DelayUS(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	DelayUS(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID	  
	DelayUS(100);	 
  	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	DelayUS(100);	  
	SCCB_Stop();   
	DelayUS(100);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����	  
	DelayUS(100);
  	val=SCCB_RD_Byte();		 	//��ȡ����
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}















