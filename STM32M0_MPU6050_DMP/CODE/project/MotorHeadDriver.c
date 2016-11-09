#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorHeadDriver.h" 
#include "Sensors.h"
#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "main.h"
 
/****************************************************
������:MotorHeadInit
����: ͷ�����ʵ���ʼ��
����:	��Ң 2015��9��14��13:44:23
****************************************************/	 
void MotorHeadInit(MOTOR_HEAD_PARM* motor,MOTOR_ID motor_id, TIM_TypeDef* TIMx,	GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
	memset(motor, 0,sizeof(MOTOR_WING_PARM)); 
	motor->TIMx = TIMx;
	motor->GPIOx = GPIOx;
	motor->GPIO_Pin = GPIO_Pin;
	motor->Angle_Code = 1;//125;//0xFF;
	motor->LimitFlag = 0;
	
	//Kalman_Init(&motor->kalman,0.001,0.942);
	motor->MotorHeadInit = MotorHeadInit; 
	motor->ExecuteHeadMotor = ExecuteHeadMotor;
	motor->SetHeadMotorParam = SetHeadMotorParam;	
	motor->StopHeadMotor = StopHeadMotor;
}
/****************************************************
������:Head_Self_Inspection
����: ִ��ͷ�������ʼ��
****************************************************/	 
void Head_Self_Inspection(MOTOR_HEAD_PARM* motor_H){
	#ifdef PRINT_ERR  
		printf("ͷ����ʼ����ʼ\r\n");
	#endif
	//������λ
	SetHeadMotorParam(motor_H, 500, 0); 
	motor_H->ExecuteHeadMotor(motor_H);
	while(GET_LIMIT_HEAD_L == 0){ReadCurrent();} //�ȴ������� 
	ReadCurrent();//ִ��AD�ɼ�
	motor_H->AngleMin =  motor_H->Angle;
	#ifdef PRINT_ERR 
		printf("ͷ�����λ���\r\n");
	#endif
	//����ҹ�λ����
	SetHeadMotorParam(motor_H, 500, 0xFFFF);
	motor_H->ExecuteHeadMotor(motor_H);
	while(GET_LIMIT_HEAD_R == 0){ReadCurrent();}//�ȴ��ﵽ����λ
	#ifdef PRINT_ERR 
		printf("ͷ���ҹ�λ���\r\n");
	#endif
	
	//��¼����ѹ
	motor_H->AngleMax = motor_H->Angle;
	motor_H->Angle_Code = (motor_H->AngleMax -  motor_H->AngleMin) / 240;
	#ifdef PRINT_ERR 
		printf("��ʼ����ϣ�AngleMax:%d-----AngleMin:%d-----Angle_Code:%d",
		motor_H->AngleMax,motor_H->AngleMin,motor_H->Angle_Code);
	#endif
	//ͷ����λ
	SetHeadMotorParam(motor_H, 500, 120);
	motor_H->ExecuteHeadMotor(motor_H);
	#ifdef PRINT_ERR 
		printf("ͷ����ʼ�����\r\n");
	#endif
}

/****************************************************
������:ExecuteHeadMotor
����: ִ�е������
****************************************************/	 
void ExecuteHeadMotor(MOTOR_HEAD_PARM* motor){
	
	GPIO_WriteBit( motor->GPIOx, motor->GPIO_Pin, (BitAction)motor->MotorDirCMD);
	motor->MotorDir = motor->MotorDirCMD;
	motor->Speed = motor->SpeedCMD;
	TIM_SetCompare1(motor->TIMx, motor->Speed);
}

/****************************************************
������:SetHeadMotorParam
����: ���õ������
****************************************************/
 void SetHeadMotorParam(MOTOR_HEAD_PARM* motor, uint16_t speed,uint16_t Angle){
	 Angle = Angle * motor->Angle_Code;
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	if(Angle < motor->Angle){
		motor->MotorDirCMD = DIR_MOTOR_HEAD_L;
	}else if(Angle > motor->Angle){
		motor->MotorDirCMD = DIR_MOTOR_HEAD_R;
	} 
	motor->AngleCMD = Angle;
}

/****************************************************
������:StopHeadMotor
����: ���ֹͣ
****************************************************/
void StopHeadMotor(MOTOR_HEAD_PARM* motor){
	TIM_SetCompare1(motor->TIMx, 0);
}

/****************************************************
������:ScanHeadMotorLimit
����: ɨ��ͷ����λ���
****************************************************/
void ScanHeadMotorLimit(void){
	if(GET_LIMIT_HEAD_L){ //������λ��Ϊ1
		Motor_H.LimitFlag |= (1<<1);//��λ��1 
		if(Motor_H.MotorDir == DIR_MOTOR_HEAD_L){//������������
		Motor_H.StopHeadMotor(&Motor_H);//���ֹͣ
		#ifdef PRINT_ERR 
			printf("ͷ������λ\r\n");
		#endif
		}
	}else{
		Motor_H.LimitFlag &= ~(1<<1);//��λ��0
	}
	
	if(GET_LIMIT_HEAD_R){	//������λ��Ϊ1
		Motor_H.LimitFlag |= (1<<0);//��λ��1
		if(Motor_H.MotorDir == DIR_MOTOR_HEAD_R ){//������������
		Motor_H.StopHeadMotor(&Motor_H);
		#ifdef PRINT_ERR 
			printf("ͷ������λ\r\n");
		#endif
		}
	}else{
		Motor_H.LimitFlag &= ~(1<<0);//��λ��0
	}
	if(Motor_H.MotorDir == DIR_MOTOR_HEAD_L && Motor_H.Angle <= Motor_H.AngleCMD){
		Motor_H.StopHeadMotor(&Motor_H);
		Motor_H.LimitFlag = 0;
		Motor_H.MotorDir = 3;
		#ifdef PRINT_ERR 
			printf("ͷ����תĿ�굽λ\r\n");
		#endif
	}else if(Motor_H.MotorDir == DIR_MOTOR_HEAD_R && Motor_H.Angle >= Motor_H.AngleCMD){
		Motor_H.StopHeadMotor(&Motor_H);
		Motor_H.LimitFlag = 0;
		Motor_H.MotorDir = 3;
		#ifdef PRINT_ERR 
			printf("ͷ����תĿ�굽λ\r\n");
		#endif
	}
}



















