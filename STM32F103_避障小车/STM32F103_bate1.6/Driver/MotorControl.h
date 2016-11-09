#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "stm32f10x.h"
#include "STM32_DevInit.h"
#include "protocol.h"

#define MAX_ENC	160	//��λ֮���ڱ��������仯��
#define ENC_MM 1
#define INIT_ENC 0x7FFF
#define RING_ENC 3000

typedef enum{ MOTOR_L, MOTOR_R }MOTOR_ID;	//���ҵ��ID
typedef enum{ MOTOR_STOP = 0x0 ,MOTOR_UP = 0x1, MOTOR_DOWN = 0x2 }MOTOR_DIR;//����˶����
//ָ������:��ָ�ֱ���˶���ָ���˶����Ƕ��˶��������˶�
typedef enum{ NONE, STRAIGHT, CUSTOM, ANGLE, ARC}CMD_TYPE;

typedef struct _Motor_T Motor_T;
struct _Motor_T{
	MOTOR_ID Id;
	TIM_TypeDef* PWM_TIMx;	//PWM�����ʱ��
	TIM_TypeDef* ENC_TIMx;	//����������ʱ��
	uint8_t TIM_Channel;		//PWM��ʱ��ͨ��
	GPIO_TypeDef* GPIOx;		//������GPIO��
	uint16_t INA_GPIO_Pinx;	//������A��IO Pin
	uint16_t INB_GPIO_Pinx;	//������B��IO Pin
	uint8_t Overflow_Flag;	//���������־λ����ʱ�������жϣ�
	PROTOCOL_INFO_T 	Exec_Protocol;//����ִ�е�Э��
	CMD_TYPE Cmd_Type;			//ָ������   ֱ�ߣ�ԭ��ת�������˶�
	
	float Speed_Enc;				//��������ٶȷֳ�0-100�ȷ�   *��0-100��=��ʵ�ٶ�ָ��
	PID_T PID_Speed;				//�ٶȻ�
	PID_T PID_Location;			//λ�û�
	uint16_t Encoder_Cur;		//��ǰ������ֵ
	uint16_t Encoder_Last;	//�ϴα�����ֵ
	int32_t Encoder_Diff;		//Ŀ�����ֵ�뵱ǰ����ֵ�Ĳ�
	int32_t Location_Diff;	//���ҵ������ֵ�Ĳ�
	
	int16_t Speed;					//��ʱ�ٶ�
	int16_t SpeedCMD;				//�����ٶ�
	//uint16_t Encoder_Begin;
	uint16_t Encoder_CMD;
	//uint16_t Encoder_CMD_Diff;
	float Enc_Angle_Code;
	int16_t SpeedBAK;				//������ͣ�˶�ʱ�ָ��˶��ٶ�
	int16_t Out_PWM;				//�������PWM
	MOTOR_DIR Dir;					//�����ǰת�򣨱�������ֵ̬��
	MOTOR_DIR DirCMD;				//�����
	MOTOR_DIR DirBAK;				//������ͣ�˶�ʱ�ָ��˶�����
	uint8_t IsOpposite;			//�Ƿ���

	
	void (*Motor_Run)(Motor_T* motor, MOTOR_DIR dir, uint16_t speed);
	void (*Motor_Custom)(Motor_T* motor, MOTOR_DIR dir, uint16_t speed, int16_t Encoder);
	uint16_t (*Motor_Get_Encoder)(Motor_T* motor);
	
};

extern Motor_T *motor_L, *motor_R;
extern void Motor_Init(void); 
extern void Motor_GPIO_Configuration(void);
extern void Motor_TIM_Configuration(void);
extern void Motor_PID(void);

#endif

