//###########################################################################
//
// FILE:    MotorDrive.c
//
// TITLE:   �����������������
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "DeviceConfig.h"
#include "USARTSentData.h"

MOTORS Motors;

void MotorInt(void);
void SetMotorParam(uint16_t motorID, uint16_t speed, int16_t dir);
void ExecuteMotorParam(void);
void ExecuteMotorSpeed(uint16_t motorID, uint16_t speed);	//�ٶ�ֵ0-100	ֻ�ܱ�MotorExe����!!!
void ExecuteMotorDIR(uint16_t motorID, int16_t dir);		//����1��-1	ֻ�ܱ�MotorExe����!!!

void MotorInt(void)	// ��ʼ���������
{
	uint16_t temp;
	MOTOR_PARM *motorTemp;

	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		motorTemp = &Motors.motor1 + temp;
		motorTemp->motorID = temp +1;			//���ID 1-2
		motorTemp->motorDir = 0;				//����:1��-1	�˴�Ϊ����д�룬�Ƿ���������	��λ��Ӳ��Ĭ��״̬Ϊ0�ķ���
		motorTemp->motorSpeed = 0;			//��ǰ�ٶȣ��˴�Ϊ����д�룬�Ƿ���������
		motorTemp->motorDirCMD = 0xff;		//�˴�������λ������δִ�еķ���	����:1��-1	��λΪ0xff����Ҫ���л�����ǰ��Ҫ��PWM��0��Ȼ����ʱһ������л�����
		motorTemp->motorSpeedCMD = 0xff;	//�˴�������λ������δִ�е��ٶ�	0-100		��λΪ0xff�����������ǰ��Ҫ�ȶ�DIRһ��ʱ��
//		motorTemp->setDirDelay = 2;			//�ڸ����PWMǰ����Ҫ�ȸ�DIRһ��ʱ��Ĺ̶���ƽ
//		motorTemp->setPWMDelay = 2;			//���л����DIRǰ����Ҫ�ȸ�PWMһ��ʱ��ĵ͵�ƽ
		motorTemp->fuseSensor = 0;				//����˿������	1:�����������		0:����
		motorTemp->encoder = ENCODER_CON_RES;		//��ʼ����30000���ٴ˻��������Ӽ�
		SetMotorParam(motorTemp->motorID, 0, 1);
	}
	ExecuteMotorParam();
}

void SetMotorParam(uint16_t motorID, uint16_t speed, int16_t dir)	//�ٶ�ֵ0-100
{
	MOTOR_PARM *motorTemp;
	
	motorTemp = &Motors.motor1 + motorID - 1;
	if((dir != 1  && dir != -1) || speed > 2400)
	{
		speed = 0; 			//������ȷ	//�ٶȲ��ܴ���100
		dir = 1;
	}

	motorTemp = &Motors.motor1 + motorID - 1;
	motorTemp->motorSpeedCMD = speed;
	motorTemp->motorDirCMD = dir;
}
/*************************************************************
��ǰdir��pwm��״̬�����Ѿ��Ǿ���һ��1ms���ڵ��ȶ�״̬
*************************************************************/
void ExecuteMotorParam(void)
{
	uint16_t temp;
	uint16_t dirChangeFlag = 0;			//���������Ƿ�ı��˷���
	MOTOR_PARM *motorTemp;
	
	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		motorTemp = &Motors.motor1 + temp;
		if(motorTemp->motorDirCMD != 0xff)								//�����δִ�еķ�������
		{
			if(motorTemp->motorDirCMD == motorTemp->motorDir)				//�����͵�ǰ����һ�µ����
			{
				motorTemp->motorDirCMD = 0xff;							//���÷���ָ���Ѿ�ִ��
			}
			else															//�����͵�ǰ����һ�µ����			
			{
				if(motorTemp->motorSpeed != 0)								//�����ǰ�ٶȲ�Ϊ0�����Ȱ��ٶ���Ϊ0
				{
					ExecuteMotorSpeed(motorTemp->motorID, 0);
					motorTemp->motorSpeed = 0;
				}
				else														//��ǰ�ٶ�Ϊ0�����
				{
					ExecuteMotorDIR(motorTemp->motorID, motorTemp->motorDirCMD);			//�ı䷽��
					motorTemp->motorDir = motorTemp->motorDirCMD;		
					motorTemp->motorDirCMD = 0xff;
					dirChangeFlag = 1;
				}
			}
		}

		if(motorTemp->motorSpeedCMD == 0)									//���PWM����Ϊ0�������������ִ��
		{
			ExecuteMotorSpeed(motorTemp->motorID, motorTemp->motorSpeedCMD);
			motorTemp->motorSpeed = motorTemp->motorSpeedCMD;
			motorTemp->motorSpeedCMD = 0xff;							//����pwmָ���Ѿ�ִ��
		}
		
		if(motorTemp->motorDirCMD == 0xff)								//���û��δִ�еķ����������PWMָ��
		{
			if(motorTemp->motorSpeedCMD != 0xff)							//�����δִ�е�PWM����
			{
				if(!dirChangeFlag)											//�������û��ִ�й�����ı䣬����Կ�ʼִ��PWMָ����߻�ȵ�����ִ��
				{
					ExecuteMotorSpeed(motorTemp->motorID, motorTemp->motorSpeedCMD);
					motorTemp->motorSpeed = motorTemp->motorSpeedCMD;
					motorTemp->motorSpeedCMD = 0xff;					//����pwmָ���Ѿ�ִ��
				}
			}
		}
		dirChangeFlag = 0;//Ϊ��һ��ѭ����ʼ��
	}
}

void ExecuteMotorSpeed(uint16_t motorID, uint16_t speed)	//�ٶ�ֵ0-100	ֻ�ܱ�MotorExe����!!!
{
	if( speed > 2400)return; 											//�ٶȲ��ܴ���100
	if(motorID == ID_MOTOR_L)
	{
		TIM_SetCompare1(TIM15, speed);
	}
	else if(motorID == ID_MOTOR_R)
	{
		TIM_SetCompare2(TIM15, speed);
	}
	else if(motorID == ID_MOTOR_H)
	{
		TIM_SetCompare1(TIM14, speed);
	}
	else if(motorID == ID_MOTOR_LC)
	{
		TIM_SetCompare1(TIM16, speed);
	}
	else if(motorID == ID_MOTOR_RC)
	{
		TIM_SetCompare1(TIM17, speed);
	}
}

void ExecuteMotorDIR(uint16_t motorID, int16_t dir)			//����1��-1	ֻ�ܱ�MotorExe����!!!
{
	if(dir != 1  && dir != -1)return; 									//������ȷ
	if(motorID == ID_MOTOR_L)
	{
		if(dir == DIR_MOTOR_L_F)SET_MOTOR_L_F;
		else SET_MOTOR_L_B;
	}
	else if(motorID == ID_MOTOR_R)
	{
		if(dir == DIR_MOTOR_R_F)SET_MOTOR_R_F;
		else SET_MOTOR_R_B;
	}
	else if(motorID == ID_MOTOR_H)
	{
		if(dir == DIR_MOTOR_H_L)SET_MOTOR_H_L;
		else SET_MOTOR_H_R;
	}
	else if(motorID == ID_MOTOR_LC)
	{
		if(dir == DIR_MOTOR_LC_U)SET_MOTOR_LC_U;
		else SET_MOTOR_LC_D;
	}
	else if(motorID == ID_MOTOR_RC)
	{
		if(dir == DIR_MOTOR_RC_U)SET_MOTOR_RC_U;
		else SET_MOTOR_RC_D;
	}	
}


