#include <ImitateI2C.h>
#include <Driver.h>
#include <stdio.h>
#include <I2C_driver.h> 
#include <math.h>
#include <mpu6050_driver.h>
#include <Control_Driver.h>
#include <stm32f10x_systick.h> 
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
short T_X,T_Y,T_Z;		 //X,Y,Z�ᣬ���ٶ�
short O_X,O_Y,O_Z;		 //X,Y,Z�ᣬ���ٶ�
float A_X,A_Y,A_Z;		 //X,Y,Z�ᣬ���ٶ�
short T_T;
short temValue;



 Data_6050_str	Data_6050; 	//6050���ݽṹ
// void delay5ms(void)
// {
//    int i=5000;  
//    while(i) 
//    { 
//      i--; 
//    }  
// }

//���ֽ�д��*******************************************

// bool Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data)		     //void
// {
//   	if(!I2C_Start())return FALSE;
//     I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
//     if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
//     I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
//     I2C_WaitAck();	
//     I2C_SendByte(REG_data);
//     I2C_WaitAck();   
//     I2C_Stop(); 
//     delay5ms();
//     return TRUE;
// }
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
//     //return TRUE;
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
   O_X =	(BUF[1]<<8)|BUF[0];
   O_X = O_X/16.4; 						   //��ȡ����X����ٶ�����
	 //A_X= O_X*1.2*180/3.14;
	Data_6050.AX=	(BUF[1]<<8)|BUF[0];
	Data_6050.Acc_X = -1*asin((float)Data_6050.AX/0x4000)*57.32;	//Acc = AX * 9.8/0x4000;
	
   BUF[2]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
   O_Y =	(BUF[3]<<8)|BUF[2];
   O_Y = O_Y/16.4; 						   //��ȡ����Y����ٶ�����
	 //A_Y= O_Y*1.2*180/3.14;
	
   BUF[4]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
   O_Z =	(BUF[5]<<8)|BUF[4];
   O_Z = O_Z/16.4; 					       //��ȡ����Z����ٶ�����
	// A_Z= O_Z*1.2*180/3.14;

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
   T_X =	(BUF[1]<<8)|BUF[0];
   T_X = T_X/16.4; 						   //��ȡ����X����ٶ�����

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y = T_Y/16.4; 						   //��ȡ����Y����ٶ�����
	 Data_6050.GY=	(BUF[3]<<8)|BUF[2];
	 Data_6050.Gryo_Y = (float)Data_6050.GY /16.384;//������2000��/S
	
   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z=T_Z/16.4; 					       //��ȡ����Z����ٶ�����

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
 //kalman filter start
float angle, angle_dot; 		//�ⲿ��Ҫ���õı���
static float Q_angle=0.0008, Q_gyro=0.002, R_angle=0.5, dt=0.001;   //ע�⣺dt��ȡֵΪkalman�˲�������ʱ��;
static float P[2][2] =  {	{ 1, 0 },{ 0, 1 }					};	
static float Pdot[4] ={0,0,0,0};
static char C_0 = 1;
static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;

float PID;
 //kalman end
void Kalman_Filter(float angle_m,float gyro_m)			//gyro_m:gyro_measure
{
	angle+=(gyro_m-q_bias) * dt;//�������(�Խ��ٶȽ��л���)
	
	Pdot[0]=Q_angle - P[0][1] - P[1][0];// Pk-' ����������Э�����΢��
	Pdot[1]=- P[1][1];
	Pdot[2]=- P[1][1];
	Pdot[3]=Q_gyro;
	
	P[0][0] += Pdot[0] * dt;// Pk- ����������Э����΢�ֵĻ��� = ����������Э����
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;
		
	angle_err = angle_m - angle;//zk-������ƣ����������ǻ��ֺ��ֵ�ͼ��ٶȼƼ���ĽǶȲ
		
	PCt_0 = C_0 * P[0][0];
	PCt_1 = C_0 * P[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;//Kk
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * P[0][1];

	P[0][0] -= K_0 * t_0;//����������Э����
	P[0][1] -= K_0 * t_1;
	P[1][0] -= K_1 * t_0;
	P[1][1] -= K_1 * t_1;
	
	angle	+= K_0 * angle_err;//�������
	q_bias	+= K_1 * angle_err;//�������
	angle_dot = gyro_m-q_bias;//���ֵ��������ƣ���΢�� = ���ٶ�
}
  //kalman end*/
	
	/*//��Ԫ�� start
 #define Kp 10.0f                        
#define Ki 0.008f                         
#define halfT 0.001f                 
typedef struct float_angle{ float Rool; float Pitch; float Yaw;}S_FLOAT_ANGLE;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   
float exInt = 0, eyInt = 0, ezInt = 0;  
float angle_hb = 0;
S_FLOAT_ANGLE Q_ANGLE;	
float pitch;
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
  float norm;
  //float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
//  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
//  float q1q2 = q1*q2;
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
		
// 	//printf("pitch:%f  \t\t  troll:%f\n",Q_ANGLE.Pitch,Q_ANGLE.Rool);
// 	if(Q_ANGLE.Rool>WA){
// 		if(Q_ANGLE.Rool<0)Q_ANGLE.Rool=0-Q_ANGLE.Rool;
// 		setW((int)(Q_ANGLE.Rool));
// 	}else if(Q_ANGLE.Rool<SA){
// 		if(Q_ANGLE.Rool<0)Q_ANGLE.Rool=0-Q_ANGLE.Rool;
// 		setS((int)(Q_ANGLE.Rool));
// 	}else{
// 		setStop();
// 	}

}
	//��Ԫ�� end*/
 u16 count = 0;
u32 C_Y = 0;
extern int WA,SA;
extern int centerNum;
extern int riseNum;
extern int WA;
extern int SA;
u32 tem=0;
int     PID_KP=100;			//PID����������ʵ��5ʱP������ֱ����
float     PID_KD = 2;			//PID΢�ֲ���
extern int W,S,A,D;
 void Build_Data(void){

	 WA=centerNum+riseNum;
	 SA=centerNum-riseNum;
	  
	 //tem++;
	READ_MPU6050_PLUS();//��ȡMPU6050���ٶ�����
	//DATA_printf(TX_DATA,O_X);//ת��X�����ݵ�����
 	//Send_data("���ٶ�X");			 //����X����
	//DATA_printf(TX_DATA,O_Y);//ת��Y�����ݵ�����
	//Send_data("Y");			 //����Y����
	//DATA_printf(TX_DATA,O_Z);//ת��Z�����ݵ�����
	//Send_data("Z");			 //����Z����
	//DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
	//Send_data("T");			 //�����¶�����	
	READ_MPU6050_ANGLE();	         //��ȡMPU6050���ٶ�����
	//DATA_printf(TX_DATA,T_X);//ת��X�����ݵ�����
 	//Send_data("���ٶ�X");			 //����X����
	//DATA_printf(TX_DATA,T_Y);//ת��Y�����ݵ�����
	//Send_data("Y");			 //����Y����
	//DATA_printf(TX_DATA,T_Z);//ת��Z�����ݵ�����
	//Send_data("Z");			 //����Z����
	//DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
	//Send_data("T");			 //�����¶�����
	//printf("pitch:%f  \t\t  troll:%f\n",Q_ANGLE.Pitch,Q_ANGLE.Rool);
//	  IMUupdate(T_X*Gyro_Gr,T_Y*Gyro_Gr,T_Z*Gyro_Gr,O_X,O_Y,O_Z);
// 	
// 	C_Y+=O_Y;
// 	count++;
// 	if(count>=10){
// 		O_Y = C_Y/count;
 //�����˲�

   // angle_hb = (0.98) * (angle_hb - O_Y * 0.01) + (0.02)*(T_X);

/*
//-------------------------------------------------------
//�����˲�
//-------------------------------------------------------
static float angle,angle_dot; 		//�ⲿ��Ҫ���õı���
//-------------------------------------------------------	
static float bias_cf;
static const float dt=0.01;
//-------------------------------------------------------
void complement_filter(float angle_m_cf,float gyro_m_cf)
{
	bias_cf*=0.998;			//��������Ʈ��ͨ�˲���500�ξ�ֵ��
	bias_cf+=gyro_m_cf*0.002;
	angle_dot=gyro_m_cf-bias_cf;
	angle=(angle+angle_dot*dt)*0.90+angle_m_cf*0.05;	
	//���ٶȵ�ͨ�˲���20�ξ�ֵ����100��ÿ����㣬��ͨ5Hz��
}
//�����˲� end*/	
Kalman_Filter(Data_6050.Acc_X , Data_6050.Gryo_Y);

 PID=PID_KP * angle + PID_KD * angle_dot;
// 		  if(PID>0){
// 				setW((int)(PID));
// 			}else if(PID<0){
// 				setS(0-((int)(PID)));
// 			}else{
// 				setStop();
// 			}
/*			pitch = Q_ANGLE.Pitch;
			if(pitch>WA){
				//if(W==1&&pitch<=(WA+6)){
				//	setStop();
				//}else{
					if(pitch<0)		pitch=0-pitch;
					setW((int)(pitch));
				//}
			}else if(pitch<SA){
				//if(pitch>=(SA-6)){
				//	setStop();
				//}else{
					if(pitch<0)pitch=0-pitch;
					setS((int)(pitch));
				//}
			}else{
				setStop();
				W=0;
				S=0;
			}
			*/
			
// 			
PID*=10;
			if(PID<1){
				if(PID<0)		PID=0-PID;
				setS((int)(PID));
			}else if(PID>1){
				if(PID<0)PID=0-PID;
				setW((int)(PID));
			}else{
				setStop();
			}
			
// 			C_Y = 0;
// 			count=0;
// 	}

		
	
	

  
}

