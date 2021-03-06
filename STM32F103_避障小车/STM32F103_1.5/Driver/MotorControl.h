#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "stm32f10x.h"
#include "STM32_DevInit.h"
#include "protocol.h"

#define MAX_ENC	160	//单位之间内编码器最大变化量
#define ENC_MM 1
#define INIT_ENC 0x7FFF

typedef enum{ MOTOR_L, MOTOR_R }MOTOR_ID;	//左右电机ID
typedef enum{ MOTOR_STOP = 0x0 ,MOTOR_UP = 0x1, MOTOR_DOWN = 0x2 }MOTOR_DIR;//电机运动方�
//指令类型:空指令，直线运动，指定运动，角度运动，弧线运动
typedef enum{ NONE, STRAIGHT, CUSTOM, ANGLE, ARC}CMD_TYPE;

typedef struct _Motor_T Motor_T;
struct _Motor_T{
	MOTOR_ID Id;
	TIM_TypeDef* PWM_TIMx;	//PWM输出定时器
	TIM_TypeDef* ENC_TIMx;	//编码器捕获定时器
	uint8_t TIM_Channel;		//PWM定时器通道
	GPIO_TypeDef* GPIOx;		//编码器GPIO组
	uint16_t INA_GPIO_Pinx;	//编码器A项IO Pin
	uint16_t INB_GPIO_Pinx;	//编码器B项IO Pin
	uint8_t Overflow_Flag;	//编码溢出标志位（定时器计数中断）
	PROTOCOL_INFO_T 	Exec_Protocol;//正在执行的协议
	CMD_TYPE Cmd_Type;			//指令类型   直线，原地转，弧线运动
	
	float Speed_Enc;				//最大允许速度分成0-100等分   *（0-100）=真实速度指令
	PID_T PID_Speed;				//速度环
	PID_T PID_Location;			//位置环
	uint16_t Encoder_Cur;		//当前编码器值
	uint16_t Encoder_Last;	//上次编码器值
	int32_t Encoder_Diff;		//目标编码值与当前编码值的差
	int32_t Location_Diff;	//左右电机编码值的差
	
	//int16_t Speed;					//
	int16_t SpeedCMD;				//命令速度
	uint16_t Encoder_Begin;
	uint16_t Encoder_CMD;
	uint16_t Enc_Angle_Code;
	int16_t SpeedBAK;				//用于暂停运动时恢复运动速度
	int16_t Out_PWM;				//最终输出PWM
	MOTOR_DIR Dir;					//电机当前转向（编码器动态值）
	MOTOR_DIR DirCMD;				//命令方向
	MOTOR_DIR DirBAK;				//用于暂停运动时恢复运动方向
	uint8_t IsOpposite;	
	
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

