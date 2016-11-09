#include <Type.h>
#define Gyro_Gr	0.0002663				
typedef struct{
	int16_t GX;	int16_t GY;	int16_t GZ;					   //x,y,z����ٶȼĴ���ֵ
	float Gryo_X;float Gryo_Y;float Gryo_Z;	   //X,Y,Z����ٶ����ֵ
	int16_t AX;	int16_t AY;	int16_t AZ;					   //x,y,z����ٶȼĴ���ֵ
	float Acc_X; float Acc_Y; float Acc_Z;	   //x,y,z����ٶ����ֵ
	}Data_6050_str;
extern bool Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
extern u8 Single_Read(u8 SlaveAddress,u8 REG_Address);
extern void Init_MPU6050(void);
extern void READ_MPU6050_PLUS(void);
extern void READ_MPU6050_ANGLE(void);
//extern void Send_data(uchar axis);
extern void Build_Data(void);
