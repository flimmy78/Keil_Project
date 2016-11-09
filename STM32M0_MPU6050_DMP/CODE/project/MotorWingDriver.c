#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorWingDriver.h"
#include "Sensors.h"
#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "main.h"
/****************************************************
������:MotorWingInit
����: �����ʵ���ʼ��
����:	��Ң 2015��9��14��13:44:23
****************************************************/	 
void MotorWingInit(MOTOR_WING_PARM* motor,MOTOR_ID motor_id, TIM_TypeDef* TIMx,	GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
	memset(motor, 0,sizeof(MOTOR_WING_PARM)); 
	
	motor->TIMx = TIMx;
	motor->GPIOx = GPIOx;
	motor->GPIO_Pin = GPIO_Pin;
	motor->Angle_Code = 1;//125;//0xFF;
	motor->Encoder = ENCODER_CON_RES;
	motor->EncoderMAX = 0;//4500;
	motor->EncoderCMD = 0;
	motor->LimitFlag = 0;
	
	motor->MotorWingInit = MotorWingInit; 
	motor->ExecuteWingMotor = ExecuteWingMotor;
	motor->StopWingMotor = StopWingMotor;
	motor->SetWingMotorParam = SetWingMotorParam;
}

/****************************************************
������:Wing_Self_Inspection
����: ִ�г������ʼ��
����:	��Ң 2015��9��14��13:44:23
****************************************************/	 
void Wing_Self_Inspection(MOTOR_WING_PARM* motor_WL,MOTOR_WING_PARM* motor_WR){
	#ifdef PRINT_ERR 
		printf("����ʼ����ʼ\r\n");
	#endif
	//�����λ 
	motor_WL->SetWingMotorParam(motor_WL, 500, DIR_MOTOR_LC_D, 0xFFFF); 
	motor_WR->SetWingMotorParam(motor_WR, 500, DIR_MOTOR_RC_D, 0xFFFF);
	motor_WL->ExecuteWingMotor(motor_WL);
	motor_WR->ExecuteWingMotor(motor_WR);
	while(GET_LIMIT_WING_L == 0){;} //|| Motor5.flagCMD == 0){;}
	#ifdef PRINT_ERR 
		printf("����¹�λ���\r\n");
	#endif
	//��������
	motor_WL->SetWingMotorParam(motor_WL, 500, DIR_MOTOR_LC_U, 0xFFFF);
	motor_WR->SetWingMotorParam(motor_WR, 500, DIR_MOTOR_RC_U, 0xFFFF);
	motor_WL->ExecuteWingMotor(motor_WL);
	motor_WR->ExecuteWingMotor(motor_WR);
	//�ȴ����дﵽ����
	while(motor_WL->EncoderLast != motor_WL->Encoder){// || motor_WR->EncoderLast != motor_WR->Encoder){//�ȴ��ﵽ����λ
		motor_WL->EncoderLast = motor_WL->Encoder;
		motor_WR->EncoderLast = motor_WR->Encoder;
		delayMS(100);
	}
	#ifdef PRINT_ERR 
		printf("����Ϲ�λ���\r\n");
	#endif
	//��¼������� 
	motor_WL->EncoderMAX = (motor_WL->EncoderCMD - motor_WL->Encoder);//SetWingMotorParamʱ����10 ���ﱣ��ֵһ��
	motor_WR->EncoderMAX = (motor_WR->EncoderCMD - motor_WR->Encoder);
	motor_WL->Angle_Code = motor_WL->EncoderMAX * 10 / 360;
	motor_WR->Angle_Code = motor_WR->EncoderMAX * 10 / 360;
	#ifdef PRINT_ERR 
		printf("��ʼ����ϣ�encode4MAX:%d-----encode5MAX:%d\r\n Angle_Code4:%d-----Angle_Code5:%d",
		motor_WL->EncoderMAX,motor_WR->EncoderMAX,motor_WL->Angle_Code,motor_WR->Angle_Code);
	#endif
	//���¹�λ
	motor_WL->SetWingMotorParam(motor_WL, 500, DIR_MOTOR_LC_D, 0xFFFF);
	motor_WR->SetWingMotorParam(motor_WR, 500, DIR_MOTOR_RC_D, 0xFFFF);
	motor_WL->ExecuteWingMotor(motor_WL);
	motor_WR->ExecuteWingMotor(motor_WR);
	//�ȴ����д�����λ��
	while(GET_LIMIT_WING_L == 0); //|| Motor5.flagCMD == 0){;}
	motor_WL->StopWingMotor(motor_WL);
	motor_WR->StopWingMotor(motor_WR);

	//�������
	motor_WL->Encoder = motor_WL->EncoderCMD = 0;
	motor_WR->Encoder = motor_WR->Encoder = 0;
	motor_WL->Encoder = motor_WL->EncoderCMD = 0;
	motor_WR->Encoder = motor_WR->Encoder = 0;
	#ifdef PRINT_ERR 
		printf("����ʼ�����\r\n");
	#endif
	
	
}
	

/****************************************************
������:ExecuteWingMotor
����: ִ�е������
����:	��Ң 2015��9��14��13:44:23
****************************************************/	 
void ExecuteWingMotor(MOTOR_WING_PARM* motor){
	
	GPIO_WriteBit(motor->GPIOx, motor->GPIO_Pin, (BitAction)motor->MotorDirCMD);
	motor->MotorDir = motor->MotorDirCMD;
	motor->Speed = motor->SpeedCMD;
	motor->Encoder = motor->EncoderCMD;
	TIM_SetCompare1(motor->TIMx, motor->Speed);
}

/****************************************************
������:SetWingMotorParam
����: ���õ������
����:	���ʵ�壬�ٶȣ����򣬽Ƕȣ�0-120��
����:	��Ң 2015��9��14��13:44:23
****************************************************/
 void SetWingMotorParam(MOTOR_WING_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle){
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	motor->MotorDirCMD = dir;
	motor->EncoderCMD = (angle * motor->Angle_Code / 10); 
	motor->Encoder = motor->EncoderCMD;
}

/****************************************************
������:StopWingMotor
����: ���ֹͣ
����:	��Ң 2015��9��14��13:44:23
****************************************************/
void StopWingMotor(MOTOR_WING_PARM* motor){
	TIM_SetCompare1(motor->TIMx, 0);
	motor->Encoder = 0;
}

/****************************************************
������:ScanWingMotorLimit
����: ɨ������λ���
����:	��Ң 2015��9��14��13:44:23
****************************************************/
void ScanWingMotorLimit(void){
	if(GET_LIMIT_WING_L){//������λ��Ϊ1
		Motor_WL.LimitFlag |= (1<<1);//��λ��1 
		Motor_WL.EncoderSUM = 0;
		if(Motor_WL.MotorDir == DIR_MOTOR_LC_D){//������������
			Motor_WL.StopWingMotor(&Motor_WL);//���ֹͣ
			#ifdef PRINT_ERR 
				printf("�������λ\r\n");
			#endif
		}
	}else{
		Motor_WL.LimitFlag &= ~(1<<1);//��λ��0
	}
	
	if(GET_LIMIT_WING_R){//������λ��Ϊ1
		Motor_WR.LimitFlag |= (1<<0);//��λ��1
		Motor_WR.EncoderSUM = 0;
		if(Motor_WR.MotorDir == DIR_MOTOR_RC_D){
			Motor_WR.StopWingMotor(&Motor_WR);
			Motor_WR.LimitFlag = 1;
			#ifdef PRINT_ERR 
				printf("�������λ\r\n");
			#endif
		}
	}else{
		Motor_WR.LimitFlag &= ~(1<<1);//��λ��0
	}
}



















