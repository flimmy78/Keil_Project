#include <ImitateI2C.h>
#include <Driver.h>
#include <stdio.h>
#include <I2C_driver.h> 
#include <math.h>
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

u8 TX_DATA[4];  	 //��ʾ�ݻ�����
u8 BUF[10];       //�������ݻ�����
char  test=0; 				 //IIC�õ�
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

//���ֽ�д��*******************************************

bool Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}
//���ֽڶ�ȡ*****************************************
u8 Single_Read(u8 SlaveAddress,u8 REG_Address)
{   
	u8 REG_data;  
	I2C_ReadByte(&REG_data,1,REG_Address, SlaveAddress);
	
// 	
// 	u8 REG_data;     	
// 	if(!I2C_Start())return FALSE;
//     I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
//     if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
//     I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
//     I2C_WaitAck();
//     I2C_Start();
//     I2C_SendByte(SlaveAddress+1);
//     I2C_WaitAck();

// 		REG_data= I2C_RadeByte();
//     I2C_NoAck();
//     I2C_Stop();
    //return TRUE;
	return REG_data;
}	
/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//��ʼ��MPU6050��������Ҫ��ο�pdf�����޸�************************
void Init_MPU6050(void)
{
// 	I2C_GPIO_Config();
//   Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
// 	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
// 	Single_Write(MPU6050_Addr,CONFIG, 0x06);
// 	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
// 	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
		I2C_WriteByte(0x00,PWR_MGMT_1,MPU6050_Addr);
		I2C_WriteByte(0x07,SMPLRT_DIV,MPU6050_Addr);
		I2C_WriteByte(0x06,CONFIG,MPU6050_Addr);
		I2C_WriteByte(0x18,GYRO_CONFIG,MPU6050_Addr);
		I2C_WriteByte(0x01,ACCEL_CONFIG,MPU6050_Addr);
		
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

 //********���ڷ�������***************************************
 void Send_data(char* axis)
 {
	uchar i;
	printf(axis);
  //USART1_SendData(axis);
  USART1_SendData(':');
  for(i=0;i<4;i++)
		USART1_SendData(TX_DATA[i]);
  USART1_SendData(' ');
  USART1_SendData(' ');
 }
 
 
 #define Kp 10.0f                        
#define Ki 0.008f                         
#define halfT 0.001f                 
typedef struct float_angle{
				float Rool;
				float Pitch;
				float Yaw;}S_FLOAT_ANGLE;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   
float exInt = 0, eyInt = 0, ezInt = 0;  
S_FLOAT_ANGLE Q_ANGLE;		
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;

  norm = sqrt(ax*ax + ay*ay + az*az);      
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
           
  vx = 2*(q1q3 - q0q2);												
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  ex = (ay*vz - az*vy) ;                           					
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								 
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  gx = gx + Kp*ex + exInt;					   							
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;				   							
					   
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  Q_ANGLE.Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  Q_ANGLE.Rool = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	
	printf("pitch:%f  \t\t  troll:%f\n",Q_ANGLE.Pitch,Q_ANGLE.Rool);

}
 
 void Build_Data(void){
	READ_MPU6050_PLUS();//��ȡMPU6050���ٶ�����
	//DATA_printf(TX_DATA,O_X);//ת��X�����ݵ�����
 	//Send_data("���ٶ�X");			 //����X����
	//DATA_printf(TX_DATA,O_Y);//ת��Y�����ݵ�����
	//Send_data("Y");			 //����Y����
	//DATA_printf(TX_DATA,O_Z);//ת��Z�����ݵ�����
	//Send_data("Z");			 //����Z����
	//DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
	//Send_data("T");			 //�����¶�����	
	
	READ_MPU6050_ANGLE();	         //��ȡMPU6050����
	//DATA_printf(TX_DATA,T_X);//ת��X�����ݵ�����
 	//Send_data("���ٶ�X");			 //����X����
	//DATA_printf(TX_DATA,T_Y);//ת��Y�����ݵ�����
	//Send_data("Y");			 //����Y����
	//DATA_printf(TX_DATA,T_Z);//ת��Z�����ݵ�����
	//Send_data("Z");			 //����Z����
	//DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
	//Send_data("T");			 //�����¶�����
	 //printf("\n���ٶ�X:%d\tY:%d\tZ:%d",O_X,O_Y,O_Z);
	 //printf("\t���ٶ�X:%d\tY:%d\tZ:%d\tT:%d",T_X,T_Y,T_Z,T_T);
	
	//USART1_SendData(0X0D);	 //����
	//USART1_SendData(0X0A);	 //�س�
	IMUupdate(T_X,T_Y,T_Z,O_X,O_Y,O_Z);
  
}

