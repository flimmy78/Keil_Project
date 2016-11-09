#include <I2C_driver.h>
#include <stdio.h>
#include "tool.h"
//*****************************************//
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
//#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)


//****************************

#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�

uint8_t TX_DATA[4];  	 //��ʾ�ݻ�����
uint8_t BUF[10];       //�������ݻ�����
//char  test=0; 				 //IIC�õ�
short T_X,T_Y,T_Z;		 //X,Y,Z�ᣬ���ٶ�
short O_X,O_Y,O_Z;		 //X,Y,Z�ᣬ���ٶ�
short T_T;

void delay5ms(void)
{
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
void  USART1_SendData(uint8_t SendData)
{
	USART_SendData(USART1, SendData);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
}


//��ʼ��MPU6050��������Ҫ��ο�pdf�����޸�************************
void Init_MPU6050(void)
{
	uint8_t data[10] = {0};
	Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x80);
	delayMS(500); 
  Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
	/*Mult_Read(MPU6050_Addr,0x06,6,data);
	data[6] = '\r';
	data[7] = '\n';
	printf("%s",data);
	uint8_t rev = ((data[5] & 0x01) << 2) | ((data[3] & 0x01) << 1) |
        (data[1] & 0x01);//010

    if (rev) {
       
        if (rev == 1)
           ;
        else if (rev == 2)
            ;
        else {
            printf("Unsupported software product rev %d.\n",rev);
            
        }
    }*/
	
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
	Single_Write(MPU6050_Addr,CONFIG, 0x06);
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
}
//******��ȡMPU6050���ٶ�����****************************************
void READ_MPU6050_PLUS(void){
	 //��ȡ���ٶ�����ACCEL_XOUT_H
	 BUF[0]=Single_Read(MPU6050_Addr,ACCEL_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,ACCEL_XOUT_H);
   O_X=	(BUF[1]<<8)|BUF[0];
   O_X/=16.4; 						   //��ȡ����X����ٶ�����

   BUF[2]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
   O_Y=	(BUF[3]<<8)|BUF[2];
   O_Y/=16.4; 						   //��ȡ����Y����ٶ�����
   BUF[4]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
   O_Z=	(BUF[5]<<8)|BUF[4];
   O_Z/=16.4; 					       //��ȡ����Z����ٶ�����

   BUF[6]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
   BUF[7]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
	 T_T = (float)((T_T-32)/1.8);
}
//******��ȡMPU6050���ٶ�����****************************************
void READ_MPU6050_ANGLE(void)
{
	 //��ȡ���ٶ�����
   BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=16.4; 						   //��ȡ����X����ٶ�����

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=16.4; 						   //��ȡ����Y����ٶ�����
   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=16.4; 					       //��ȡ����Z����ٶ�����

   BUF[6]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
   BUF[7]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
	 T_T = (float)((T_T-32)/1.8);
}

 void Build_Data(void){
	READ_MPU6050_PLUS();//��ȡMPU6050���ٶ�����
	printf("���ٶ�X:%d\tY:%d\tZ:%d\tT:%d",O_X,O_Y,O_Z,T_T);
	READ_MPU6050_ANGLE();	         //��ȡMPU6050����
	printf("\t���ٶ�X:%d\tY:%d\tZ:%d\tT:%d\r\n",T_X,T_Y,T_Z,T_T);
	 
	
	//USART1_SendData(0X0D);	 //����
	//USART1_SendData(0X0A);	 //�س�
  
}

