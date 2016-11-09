#include "stm32f0xx.h"
#define Gyro_Gr	0.0002663			
#define	EQUIP_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
typedef struct{
	int16_t GX;	int16_t GY;	int16_t GZ;					   //x,y,z����ٶȼĴ���ֵ
	float Gryo_X;float Gryo_Y;float Gryo_Z;	   //X,Y,Z����ٶ����ֵ
	int16_t AX;	int16_t AY;	int16_t AZ;					   //x,y,z����ٶȼĴ���ֵ
	float Acc_X; float Acc_Y; float Acc_Z;	   //x,y,z����ٶ����ֵ
	}Data_6050_str;
extern uint8_t Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
extern void Init_MPU6050(void);
extern void READ_MPU6050_PLUS(void) ; 
extern void READ_MPU6050_ANGLE(void);
//extern void Send_data(uchar axis);
extern void Build_Data(void);
