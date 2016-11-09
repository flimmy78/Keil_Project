/******************** (C) COPYRIGHT 2009 Chen Yao ******************************
* File Name          : DeviceConfig.h
* Author             : Chen Yao
* Version            : V1.0
* Date               : 05/02/2009
* Description        : This file contains the Device Config
********************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_CONFIG_H
#define __DEVICE_CONFIG_H
#include "stm32f0xx.h"
#include "Kalman.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define uint16_t	uint16_t
#define uint32_t	uint32_t
#define int16_t  	int16_t
#define int32_t		int32_t

/*******************************************
�����˿���ģʽ	ctrlMode
*******************************************/
#define MODE_MANUAL	0x01
#define MODE_AUTO	0x02 

/*******************************************
�ϴ�����Ϣ����
*******************************************/
//#define MSG_NORMAL							0		//û����Ϣ
//#define MSG_MOTION_NOT_READY			1		//�˶���ʼ������������
//#define MSG_LOCATION_NUM_ERR				2		//�˶���ʼ������������
//#define MSG_PARAM_ERR					3		//��������
//#define MSG_MOTOR_NOT_READY			4		//���δ׼����
//#define MSG_MOTOR_ERR					5		//����������������



/*******************************************
ͨ���еĵ������ʱʱ��
*******************************************/



//������������
/*******************************************
�������֣����֣�ͷ������ᣬ�ҳ�
*******************************************/
/*#define ID_MOTOR_L		1		//����
#define ID_MOTOR_R		2		//����
#define ID_MOTOR_H		3		//ͷ��
#define ID_MOTOR_LC		4		//���
#define ID_MOTOR_RC		5		//�ҳ�*/

typedef enum {ID_MOTOR_L = 1,ID_MOTOR_R,ID_MOTOR_H,ID_MOTOR_LC,ID_MOTOR_RC}MOTOR_ID;

#define DIR_MOTOR_CLK	1
#define DIR_MOTOR_UCLK	-1

#define DIR_MOTOR_L_F	DIR_MOTOR_CLK		//����ǰ��
#define DIR_MOTOR_L_B	DIR_MOTOR_UCLK		//�����˺�
#define DIR_MOTOR_R_F	DIR_MOTOR_CLK		//����ǰ��
#define DIR_MOTOR_R_B	DIR_MOTOR_UCLK		//�����˺�
#define DIR_MOTOR_HEAD_L		0		//ͷ����ת
#define DIR_MOTOR_HEAD_R		1		//ͷ����ת
#define DIR_MOTOR_LC_U	1		//�ҳ���������
#define DIR_MOTOR_LC_D	0		//�ҳ��½�����
#define DIR_MOTOR_RC_U	1		//�����������
#define DIR_MOTOR_RC_D	0		//����½�����

#define DIR_MOTOR_A	2
#define DIR_MOTOR_B	0




#define SET_MOTOR_L_F		GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define SET_MOTOR_L_B		GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define SET_MOTOR_R_F		GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define SET_MOTOR_R_B		GPIO_ResetBits(GPIOB, GPIO_Pin_3)				
#define SET_MOTOR_H_L		GPIO_SetBits(GPIOB, GPIO_Pin_2)					//�������ͷ��ת
#define SET_MOTOR_H_R		GPIO_ResetBits(GPIOB, GPIO_Pin_2)				//�������ͷ��ת
#define SET_MOTOR_LC_U  	GPIO_SetBits(GPIOB, GPIO_Pin_10)			//������������
#define SET_MOTOR_LC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_10)		//������������
#define SET_MOTOR_RC_U 	 	GPIO_SetBits(GPIOB, GPIO_Pin_11)			//��������ҳ���
#define SET_MOTOR_RC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_11)		//��������ҳ���

#define GET_FUSE_SEN_L  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)		//��ȡ����˿״̬
#define GET_FUSE_SEN_R  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)		//��ȡ����˿״̬
#define GET_FUSE_SEN_HW  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)		//��ȡ����˿״̬


#define GET_ENCODER1_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)		//���ֱ�����A��ȡ
#define GET_ENCODER1_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)		//���ֱ�����B��ȡ

#define GET_ENCODER2_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)		//���ֱ�����A��ȡ
#define GET_ENCODER2_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)		//���ֱ�����B��ȡ

//liyao 2015��9��6��13:39:57
#define GET_ENCODER_WING_L 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) //��������
#define GET_ENCODER_WING_R  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)	//�ҳ������

#define GET_LIMIT_HEAD_L 			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)	//��ͷ��λ��
#define GET_LIMIT_HEAD_R  			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)	//��ͷ��λ��
#define GET_LIMIT_WING_L 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)	//�����λ��
#define GET_LIMIT_WING_R  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)	//�ҳ���λ��

//#define GET_ENCODER3_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
//#define GET_ENCODER3_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)


#define ENCODER_CON_RES 	0x7fffffff	//������������λֵ


#define ADC_H_Angle_CHANNEL 0 //ADC_Angle_H ͷ������Ƕ� PB1
#define ADC_W_CHANNEL 1	//ADC_W	���������PB0
#define ADC_H_CHANNEL 2	//ADC_H	ͷ��������� PA5
#define ADC_R_CHANNEL 3	//ADC_R �ҵ������	PA1
#define ADC_L_CHANNEL 4	//ADC_L ��������	PA0

/*
#define MOTOR_RING_L	Motor1	//����
#define MOTOR_RING_R 	Motor2	//����
#define MOTOR_HEAD		Motor3	//ͷ�����
#define MOTOR_WING_L	Motor4	//���
#define MOTOR_WING_R	Motor5	//�ҳ�*/

/*******************************************
�ṹ�嶨��
*******************************************/
typedef struct _MOTOR_PARM MOTOR_PARM ;
struct _MOTOR_PARM						//����Ĳ���������Ҫ����һ�����õ�
{
	MOTOR_ID	motorID;			//���ID 1-2
	
	int16_t	  Speed;				//��ǰ�ٶ�
	int16_t 	SpeedCMD;		//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
	int16_t	  Accelerate;      //��ǰ���ٶ�
	int16_t 	AccelerateCMD;      //��ǰ���ٶ�����
	uint8_t   SpeedStrategy;			//1:���ٲ��ԣ�2���ȼ��ٲ��ԣ�3���ȼ��ٲ���
	
	uint8_t		motorDir;			//�����ǰ�ٶ�
	uint16_t	motorSpeed;			//��ǰ�ٶ�
	uint16_t	motorAccelerate;      //��ǰ���ٶ�
	uint8_t		motorDirCMD;		//�˴�������λ�������ķ���ָ�1��2
	uint16_t	motorSpeedCMD;		//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
	uint16_t	motorAccelerateCMD;      //��ǰ���ٶ�����
	
	uint32_t	SpeedTime;      //�����ٶȣ��ۼ�������ʱ��
	uint32_t	SpeedTimeLast;      //�����ٶȣ��ۼ�������ʱ��
	
	uint32_t	encoder;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
	uint32_t	encoderLast;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
	uint32_t	OdometerCMD;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
	uint32_t	OdometerLast;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
	uint8_t   MoveStrategy;			//1:�ٶȲ��ԣ�2����̲���

	uint16_t  PreloadForce;			//Ԥ������С
	uint8_t   PreloadForceTime;			//Ԥ������С


	uint16_t  Current;			//��ǰ����
	uint16_t  CurrentLast;			//�ϴε���
	uint16_t  deltaCurrent;			//�����仯��
	uint16_t  CurrentMaxNum;			//�ۼƵ�������Խ��ֵ����
	uint8_t   CurrentCrossFlag;
	uint8_t   CurrentStrategy;			//������������   1���������У�2��Խ�ϣ�3���ϰ������ֹͣ����	
	
	uint8_t	  fuseSensor;			//����˿������	1:�����������		0:����
	uint8_t	  motorTrouble;			 //0: ����    1:�������,   2:������ٶȹ���  
	
		//liyao
	uint16_t encoderMAX;
	uint16_t encoderCMD;
	
	
	void (*Motor_Init)(MOTOR_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//��ʼ������
	//void (*Self_Inspection)(MOTOR_PARM* motor); //�Լ캯��
	//void (*ExecuteMotor)(MOTOR_PARM* motor);
	//void (*StopMotor)(MOTOR_PARM* motor);
	
};

/****************************************************
	�ṹ����:	MOTOR_HEAD_PARM
	����: ͷ�����ʵ��
	����:	liyao 2015��9��14��12:24:31
****************************************************/
typedef struct _MOTOR_HEAD_PARM MOTOR_HEAD_PARM ;
struct _MOTOR_HEAD_PARM						//����Ĳ���������Ҫ����һ�����õ�
{
	MOTOR_ID	motorID;				//���ID 1-2
	
	int16_t	  Speed;					//��ǰ�ٶ�
	int16_t 	SpeedCMD;				//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
	uint8_t		MotorDir;				//�����ǰ�ٶ�
	uint8_t		MotorDirCMD;		//�˴�������λ�������ķ���ָ�1��2
	uint16_t  Current;				//��ǰ����
	uint16_t  CurrentLast;		//�ϴε���
	uint16_t 	Electricity;		//AD�ɼ�������Ϣ
	uint16_t 	ElectricityLast;//���һ��AD�ɼ�������Ϣ
	uint8_t 	FuseSensor;			//����˿״̬
	
	TIM_TypeDef* TIMx;		//��ӦPWM��ʱ��
	GPIO_TypeDef* GPIOx;	//��ӦPWM	IO��
	uint16_t GPIO_Pin;		//��ӦPWM	IO��
	
	int8_t 		LimitFlag;
	uint16_t 	Angle_Code;	//1��Ƕȶ�Ӧ��λ��ֵ�������
	uint16_t  AngleMax;		//���Ƕ�
	uint16_t  AngleMin;		//��С�Ƕ�
	uint16_t	AngleCMD;		//Ŀ��Ƕ�
	uint16_t	Angle;			//��ǰ�Ƕ�
	
	//KALMAN_T kalman;
	
	
	void (*MotorHeadInit)(MOTOR_HEAD_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//��ʼ������
	void (*ExecuteHeadMotor)(MOTOR_HEAD_PARM* motor);
	void (*StopHeadMotor)(MOTOR_HEAD_PARM* motor);
	void (*SetHeadMotorParam)(MOTOR_HEAD_PARM* motor, uint16_t speed,uint16_t Angle);
};

/****************************************************
	�ṹ����:	MOTOR_WING_PARM
	����: �����ʵ��
	����:	liyao 2015��9��14��12:24:31
****************************************************/
typedef struct _MOTOR_WING_PARM MOTOR_WING_PARM ;
struct _MOTOR_WING_PARM						//����Ĳ���������Ҫ����һ�����õ�
{
	MOTOR_ID	MotorID;			//���ID 1-2
	
	int16_t	  Speed;				//��ǰ�ٶ�
	int16_t 	SpeedCMD;		//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
	uint8_t		MotorDir;			//����ִ�еķ���
	uint8_t		MotorDirCMD;		//�����
	uint32_t	Encoder;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
	uint32_t	EncoderLast;			//��ʼ����0x7fffffff���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ
	int32_t  EncoderSUM;	//��λ����ۼ�ֵ;
	
	uint16_t EncoderMAX;	//���������ֵ
	uint16_t EncoderCMD;	//Ŀ�������ֵ
	
	uint16_t Electricity;//AD�ɼ�������Ϣ
	uint16_t 	ElectricityLast;		//���һ��AD�ɼ�������Ϣ
	
	TIM_TypeDef* TIMx;		//��ӦPWM��ʱ��
	GPIO_TypeDef* GPIOx;	//��ӦPWM	IO��
	uint16_t GPIO_Pin;		//��ӦPWM	IO��
	
	int8_t LimitFlag;	//1��λ��
	uint16_t Angle_Code;	//1��Ƕȶ�Ӧ������ֵ�������
	 
	
	void (*MotorWingInit)(MOTOR_WING_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//��ʼ������
	void (*ExecuteWingMotor)(MOTOR_WING_PARM* motor);
	void (*StopWingMotor)(MOTOR_WING_PARM* motor);
	void (*SetWingMotorParam)(MOTOR_WING_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle);
};  


#define MOTOR_NUM 5

extern MOTOR_PARM Motor1;	//����
extern MOTOR_PARM Motor2;	//����
extern MOTOR_HEAD_PARM Motor_H;	//ͷ�����
extern MOTOR_WING_PARM Motor_WL;	//���
extern MOTOR_WING_PARM Motor_WR;	//�ҳ�


typedef enum{Self_Inspection, Normal}SYSTEM_MODE;
extern SYSTEM_MODE System_Mode;

/*******************************************                               
����BUF 
*******************************************/
#define USART_CMD_RXBUR_NUM 16
typedef struct
{
	uint16_t usartCMDBufCon;//ָ��ǰ��û��д���λ��
	uint16_t usartCMDRxBuf0[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf1[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf2[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf3[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf4[USART_CMD_RXBUR_NUM];
	
}USARTCMDRXBUF;
/***********************************************                               
���������   ������0��0�ڻ������б�ʾ�����Ѵ���
***********************************************/
#define CMD_MOTOR_MANUAL_1	0x01		//�����ֿ���
#define CMD_MOTOR_MANUAL_2	0x02		//ͷ�������ҳ����


/*******************************************
����ID����
*******************************************/
#define TASK_ENABLE 1
#define TASK_DISABLE 0
typedef struct
{
	uint16_t taskID;			// 1-6
	uint16_t taskState;		//�����Ƿ�ʹ��  ENABLE DISABLE
	uint16_t progressBar;	//������
	uint16_t param1;
	uint16_t param2;
	uint16_t param3;
	uint16_t param4;
	uint16_t param5;
	uint32_t delay1;
	uint32_t delay2;
	uint32_t delay3;
}TASK_PARM;

#define TASK_NUM 1
typedef struct
{
	TASK_PARM MotorReset;	 
	uint16_t pauseFlag;
}TASKS;


#endif
