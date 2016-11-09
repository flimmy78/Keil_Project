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
#define ID_MOTOR_L		1		//����
#define ID_MOTOR_R		2		//����
#define ID_MOTOR_H		3		//ͷ��
#define ID_MOTOR_LC		4		//���
#define ID_MOTOR_RC		5		//�ҳ�

#define DIR_MOTOR_CLK	1
#define DIR_MOTOR_UCLK	-1

#define DIR_MOTOR_L_F	DIR_MOTOR_CLK		//����ǰ��
#define DIR_MOTOR_L_B	DIR_MOTOR_UCLK		//�����˺�
#define DIR_MOTOR_R_F	DIR_MOTOR_CLK		//����ǰ��
#define DIR_MOTOR_R_B	DIR_MOTOR_UCLK		//�����˺�
#define DIR_MOTOR_H_L	DIR_MOTOR_CLK		//ͷ����ת
#define DIR_MOTOR_H_R	DIR_MOTOR_UCLK		//ͷ����ת
#define DIR_MOTOR_LC_U	DIR_MOTOR_CLK		//�ҳ�����
#define DIR_MOTOR_LC_D	DIR_MOTOR_UCLK		//�ҳ��½�
#define DIR_MOTOR_RC_U	DIR_MOTOR_CLK		//�������
#define DIR_MOTOR_RC_D	DIR_MOTOR_UCLK		//����½�

#define SET_MOTOR_L_F		GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define SET_MOTOR_L_B		GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define SET_MOTOR_R_F		GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define SET_MOTOR_R_B		GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define SET_MOTOR_H_L		GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define SET_MOTOR_H_R		GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define SET_MOTOR_LC_U  	GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define SET_MOTOR_LC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define SET_MOTOR_RC_U 	 	GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define SET_MOTOR_RC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define GET_FUSE_SEN_L  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define GET_FUSE_SEN_R  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define GET_FUSE_SEN_HW  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)


#define GET_ENCODER1_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)
#define GET_ENCODER1_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)

#define GET_ENCODER2_A  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define GET_ENCODER2_B 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)

#define GET_ENCODER3_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
#define GET_ENCODER3_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)


#define ENCODER_CON_RES 	30000	//������������λֵ



/*******************************************
�ṹ�嶨��
*******************************************/
typedef struct						//����Ĳ���������Ҫ����һ�����õ�
{
	uint16_t	motorID;			//���ID 1-2
	int16_t		motorDir;			//����:1��-1	�˴�Ϊ����д�룬�Ƿ���������
	uint16_t	motorSpeed;			//��ǰ�ٶȣ��˴�Ϊ����д�룬�Ƿ���������
	int16_t		motorDirCMD;		//�˴�������λ������δִ�еķ���	����:1��-1	��λΪ0xff����Ҫ���л�����ǰ��Ҫ��PWM��0��Ȼ����ʱһ������л�����
	uint16_t	motorSpeedCMD;		//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
	uint16_t	setDirDelay;		//��ʱ������2015.06.17	//�ڸ����PWMǰ����Ҫ�ȸ�DIRһ��ʱ��Ĺ̶���ƽ
	uint16_t	setPWMDelay;		//��ʱ������2015.06.17 	//���л����DIRǰ����Ҫ�ȸ�PWMһ��ʱ��ĵ͵�ƽ
	uint16_t	fuseSensor;			//����˿������	1:�����������		0:����
	uint16_t	encoder;			//��ʼ����30000���ٴ˻��������Ӽ�����ÿ���ϴ���ָ���ʼֵ1000
//	int16_t	motorDirLock;			//0Ϊû������1Ϊ����˶�1������ס��-1Ϊ����˶�-1������ס
//	uint16_t locationReqFlag;		//��ǰ�ĵ��λ����Ϣ�Ƿ���Ҫ
//	uint16_t locationUpdateFlag;	//��ǰ������λ����Ϣ�Ƿ������µģ���������Ϣ���ᱣ�������������ڡ�n���������ں����㡣
//	int16_t location;				//�˴�Ϊ����������ķ������ݡ�
//	uint16_t nowCurrentReqFlag;	//��ǰ�ĵ��������Ϣ�Ƿ���Ҫ
//	uint16_t nowCurrentUpdateFlag;	//��ǰ�����ĵ�����Ϣ�Ƿ������µģ���������Ϣ���ᱣ�������������ڡ������������ں����㡣�ڶ�����Ϣʱ��
//	int16_t nowCurrent;			//��ǰ����
//	uint16_t manualUpdateTimeOut;	//���յ�������Ϣ��ʱ�����ã�һ���������ڼ�һ��Ϊ0ʱ��ֹͣ		
//	uint16_t brakeUnlockDelayFlag;	//ɲ������Ҫһ��ʱ�����ܽ���//��Ϊ��ʱЧ�ԣ����Գ�����ɲ�������ã�������Ϊ��0
//	uint16_t motorEnaFlag;			//����Ƿ񴦼��� //�涨ֻ��������������ʱ�仯
}MOTOR_PARM;				
#define MOTOR_NUM 5
typedef struct
{
	MOTOR_PARM motor1;	//����
	MOTOR_PARM motor2;	//����
	MOTOR_PARM motor3;	//ͷ�����
	MOTOR_PARM motor4;	//���
	MOTOR_PARM motor5;	//�ҳ�
}MOTORS;

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
