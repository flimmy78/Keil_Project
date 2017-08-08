#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorHeadDriver.h" 
#include "Sensors.h"
#include "stdio.h"
#include "tool.h"
#include "main.h"
 

/****************************************************
������:Head_Self_Inspection
��ע: ִ��ͷ�������ʼ��
****************************************************/	 
void Head_Self_Inspection(){
	#ifdef PRINT_ERR  
		printf("ͷ����ʼ����ʼ\r\n");
	#endif
	MOTOR_PARM* motor3 = &Motor3;
	//������λ
	SetHeadMotorParam(motor3, 500, 0); 
	motor3->ExecuteMotor(motor3);
	while(GET_LIMIT_HEAD_L == 0){ReadCurrent();} //�ȴ������� 
	ReadCurrent();//ִ��AD�ɼ�
	motor3->angleMin =  motor3->angle;
	#ifdef PRINT_ERR 
		printf("ͷ�����λ���\r\n");
	#endif
	//����ҹ�λ����
	SetHeadMotorParam(motor3, 500, 0xFFFF);
	motor3->ExecuteMotor(motor3);
	while(GET_LIMIT_HEAD_R == 0){ReadCurrent();}//�ȴ��ﵽ����λ
	#ifdef PRINT_ERR 
		printf("ͷ���ҹ�λ���\r\n");
	#endif
	
	//��¼����ѹ
	motor3->angleMax = motor3->angle;
	motor3->angle_code = (motor3->angleMax -  motor3->angleMin) / 240;
	#ifdef PRINT_ERR 
		printf("��ʼ����ϣ�angleMax:%d-----angleMin:%d-----angle_code:%d",
		motor3->angleMax,motor3->angleMin,motor3->angle_code);
	#endif
	//ͷ����λ
	SetHeadMotorParam(motor3, 500, 120);
	motor3->ExecuteMotor(motor3);
	#ifdef PRINT_ERR 
		printf("ͷ����ʼ�����\r\n");
	#endif
}

/****************************************************
������:ExecuteHeadMotor
��ע: ִ�е������
****************************************************/	 
void ExecuteHeadMotor(MOTOR_PARM* motor){
	
	GPIO_WriteBit( motor->GPIOx, motor->GPIO_Pin, (BitAction)motor->motorDirCMD);
	motor->motorDir = motor->motorDirCMD;
	motor->Speed = motor->SpeedCMD;
	motor->encoder = motor->encoderCMD;
	if(motor->motorID == ID_MOTOR_R){
		TIM_SetCompare2(motor->TIMx, motor->Speed);
	}else{
		TIM_SetCompare1(motor->TIMx, motor->Speed);
	}
}

/****************************************************
������:SetHeadMotorParam
��ע: ���õ������
****************************************************/
 void SetHeadMotorParam(MOTOR_PARM* motor, uint16_t speed,uint16_t angle){
	 angle = angle * motor->angle_code;
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	if(angle < motor->angle){
		motor->motorDirCMD = DIR_MOTOR_HEAD_L;
	}else if(angle > motor->angle){
		motor->motorDirCMD = DIR_MOTOR_HEAD_R;
	} 
	motor->angleCMD = angle;
}

/****************************************************
������:StopHeadMotor
��ע: ���ֹͣ
****************************************************/
void StopHeadMotor(MOTOR_PARM* motor){
	TIM_SetCompare1(motor->TIMx, 0);
}

/****************************************************
������:ScanHeadMotorLimit
��ע: ɨ��ͷ����λ���
****************************************************/
void ScanHeadMotorLimit(void){
	if(GET_LIMIT_HEAD_L && Motor3.motorDir == DIR_MOTOR_HEAD_L){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 1;
		#ifdef PRINT_ERR 
			printf("ͷ������λ\r\n");
		#endif
	}
	if(GET_LIMIT_HEAD_R && Motor3.motorDir == DIR_MOTOR_HEAD_R ){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 1;
		#ifdef PRINT_ERR 
			printf("ͷ������λ\r\n");
		#endif
	}
	if(Motor3.motorDir == DIR_MOTOR_HEAD_L && Motor3.angle <= Motor3.angleCMD){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 0;
		Motor3.motorDir = 3;
		#ifdef PRINT_ERR 
			printf("ͷ����תĿ�굽λ\r\n");
		#endif
	}else if(Motor3.motorDir == DIR_MOTOR_HEAD_R && Motor3.angle >= Motor3.angleCMD){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 0;
		Motor3.motorDir = 3;
		#ifdef PRINT_ERR 
			printf("ͷ����תĿ�굽λ\r\n");
		#endif
	}
}



















