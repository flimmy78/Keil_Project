//###########################################################################
//123
// FILE:    Strategy.c
//
// TITLE:   ���Ʋ��Գ���
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################

#include "stm32f0xx.h"
#include "Strategy.h"
#include "USARTSentData.h"
#include "MotorDriver.h"
#include "main.h"

int16_t  Speed_PID(MOTOR_PID *PID, MOTOR_PARM *motor);
int16_t  PID_PI(MOTOR_PID *PID);
int16_t  PID_PID(MOTOR_PID *PID);
uint16_t Accelerate_PID(MOTOR_PID *PID, MOTOR_PARM *motor);
int16_t  PID_Strategy(MOTOR_PID *PID, MOTOR_PARM *motor);
uint16_t Speed_Strategy(MOTOR_PARM *motor);
uint16_t Current_Strategy(MOTOR_PARM *motor);
void PIDInt(void);	// ��ʼ�����PID
int16_t Move_Strategy(MOTOR_PARM *motor);

PIDS   Pids;

uint16_t const PID_WH_S_C[3] = {0x27ff,0x0140,0x000f};    //speed contant
uint16_t const PID_WH_S_A[3] = {0x27ff,0x0140,0x000f};    // speed accelerate
uint16_t const PID_WH_C_C[3] = {0x0af0,0x0050,0x000f};    //current cross
uint16_t const PID_H_S_C[3] = {0x000f,0x000f,0x000f};     //
uint16_t const PID_H_S_A[3] = {0x000f,0x000f,0x000f};
uint16_t const PID_W_S_C[3] = {0x000f,0x000f,0x000f};
uint16_t const PID_W_S_A[3] = {0x000f,0x000f,0x000f};


/****************************************************
������:PIDInt
��ע: ���PID��ʼ��
****************************************************/
void PIDInt(void)	// ��ʼ�����PID
{
	uint16_t temp;
	MOTOR_PID *PIDTemp;
	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		PIDTemp = &Pids.PID1 + temp;
		PIDTemp->motorID = temp +1;			//���ID 1-2

		PIDTemp->Kp = 0;			
		PIDTemp->Ki = 0;			
		PIDTemp->Kd = 0;
		PIDTemp->ATimeLast=0;
		PIDTemp->Goal = 0;			
		PIDTemp->GoalLast = 0;
		PIDTemp->CurrentValue = 0;
		PIDTemp->ExecuteValue = 0;	
		PIDTemp->Integrator = 0;	
		PIDTemp->Error = 0;			
		PIDTemp->ErrorLast = 0;
		PIDTemp->Integrator2 = 0;	
		PIDTemp->Error2 = 0;			
		PIDTemp->ErrorLast2 = 0;
		PIDTemp->ExeFlag = 0;  //�������ִ�б�־
	}
}

/****************************************************
������:Move_Strategy
��ע: �˶����Ʋ���
****************************************************/
int16_t Move_Strategy(MOTOR_PARM *motor)
{	
	int32_t deltaOdometer = 0;
	double comp=1;
	if(motor->MoveStrategy == SPEED_STRATEGY){ //�ٶȿ��Ʋ���
		if(motor->motorDirCMD == DIR_MOTOR_A){
			motor->SpeedCMD = motor->motorSpeedCMD;
		}else if(motor->motorDirCMD == DIR_MOTOR_B){
			motor->SpeedCMD = -motor->motorSpeedCMD;
		} 		
	}else if(motor->MoveStrategy == ODOMETER_STRATEGY){ //��̿��Ʋ���
		
		//---------------������ͬ������-------------------------
		if(motor->motorID == ID_MOTOR_L){
				comp = ODOMETER_COMP_L; //����ϵͳ����
		}else if(motor->motorID == ID_MOTOR_R){
				comp = ODOMETER_COMP_R;//����ϵͳ����
		}
		
		if(motor->encoder > motor->OdometerLast){
				deltaOdometer = comp * motor->OdometerCMD - (motor->encoder - motor->OdometerLast);
		}else {
			deltaOdometer = comp * motor->OdometerCMD + (motor->encoder - motor->OdometerLast);
		}
		if(motor->motorDirCMD == DIR_MOTOR_A){
			motor->SpeedCMD = motor->motorSpeedCMD;
		}else if(motor->motorDirCMD == DIR_MOTOR_B){
			motor->SpeedCMD = -motor->motorSpeedCMD;
		} 			
		if(motor->SpeedCMD > 7 && motor->SpeedCMD <-7 && deltaOdometer < 500){
			if(motor->motorDirCMD == DIR_MOTOR_A){motor->SpeedCMD = 7;}else if(motor->motorDirCMD == DIR_MOTOR_B){motor->SpeedCMD = -7;}			
		}
			
		if(motor->SpeedCMD < 7 && motor->SpeedCMD>-7){
			if(motor->motorDirCMD == DIR_MOTOR_A){motor->SpeedCMD = 7;}else if(motor->motorDirCMD == DIR_MOTOR_B){motor->SpeedCMD = -7;}			
		}

		if(deltaOdometer < 25){
			ScramFlag = 1;
		}		
	}		
	return 1;	
}

/****************************************************
������:Odometer_Strategy
��ע: ��̿��Ʋ���
****************************************************/
int16_t Odometer_Strategy(MOTOR_PARM *motor)
{		
	return 1;	
}

/****************************************************
������:Speed_PID
��ע: �ٶ�PID���Ʋ���
****************************************************/
int16_t Speed_PID(MOTOR_PID *PID, MOTOR_PARM *motor)
{		
	
	int16_t spdCMD;

	//-------------�ٶȾ���ѡ��Ӽ��ٻ�����-------------------	
	Speed_Strategy(motor);
	
	//debug
	//motor->SpeedStrategy = CONSTANT_SPEED;
	
	//-------------������ӦPID����------------------	
	spdCMD = motor->SpeedCMD * 4;
	PID->CurrentValue = motor->Speed * 4; //�ٶȻ����ռ�ձ�
	
	if(motor->SpeedStrategy == CONSTANT_SPEED){   //���ٲ���	
		PID->Goal = spdCMD;
	}else if(motor->SpeedStrategy == ACCELERATE_SPEED){       //���ٲ���	
		//-----------------------------------------------------
		if(timeTick > (PID->ATimeLast + ATime)){  //�жϼ��ٶ�ʱ�䵥λ
			PID->ATimeLast = timeTick;
			PID->Goal += motor->motorAccelerateCMD;			
		}
		//-----------------------------------------------------
		if(timeTick < PID->ATimeLast){ //�ж�timeTick�Ƿ����
			PID->ATimeLast = timeTick;
		}
		//-----------------------------------------------------		
		if(PID->Goal > spdCMD){
			PID->Goal = spdCMD;
			motor->SpeedStrategy = CONSTANT_SPEED;
		}	
	}else if(motor->SpeedStrategy == DECELERATE_SPEED){   //���ٲ���	
		//-----------------------------------------------------
		if(timeTick>(PID->ATimeLast + ATime)){  //�жϼ��ٶ�ʱ�䵥λ
			PID->ATimeLast = timeTick;
			PID->Goal -= motor->motorAccelerateCMD;
		}
		//-----------------------------------------------------
		if(timeTick < PID->ATimeLast){ //�ж�timeTick�Ƿ����
			PID->ATimeLast = timeTick;
		}
		//-----------------------------------------------------		
		if(PID->Goal < spdCMD){
			PID->Goal = spdCMD;
			motor->SpeedStrategy = CONSTANT_SPEED;
		}
	} 
	if(motor->CurrentStrategy == OBSTACLE_CURRENT){   //���ϰ�����
		//PID->Goal = 0;//��ͣ
	}
	//-------------ѡ����ӦPID����------------------	
	PID_Strategy(PID, motor);
	//-------------����PID------------------	
	PID_PI(PID); //PID����ִ��	

//	//-----------------��������-----------------
//	if(motor->motorSpeed > motor->motorSpeedCMD && motor->SpeedStrategy != CONSTANT_SPEED){		
//				//---------------�����ּ���ͬ������-------------------------
//		if(motor->motorID == ID_MOTOR_L){
//				PID->ExecuteValue -=PRELOAD_FORCE_DEC_L ; 
//		}else if(motor->motorID == ID_MOTOR_R){
//				PID->ExecuteValue -=PRELOAD_FORCE_DEC_R ;
//		}
//	}		
	return 1;	
}

/****************************************************
������:Speed_Strategy
��ע: �ٶȾ��� 
****************************************************/
uint16_t Speed_Strategy(MOTOR_PARM *motor)
{	
	int16_t deltaSpeed=0;	
	deltaSpeed=motor->SpeedCMD - motor->Speed;
	if(deltaSpeed > 4){
		motor->SpeedStrategy = ACCELERATE_SPEED;
	}else if(deltaSpeed < -4){
		motor->SpeedStrategy = DECELERATE_SPEED;
	}else{
		motor->SpeedStrategy = CONSTANT_SPEED;
	}	
	return motor->SpeedStrategy;
}

/****************************************************
������:Current_Strategy
��ע: ��������  
****************************************************/
uint16_t Current_Strategy(MOTOR_PARM *motor)
{	
	//-------------------�Ʋ���ϰ�------------------------------------
	if(motor->Current > OBSTACLE_CURRENT_MAX){
		motor->CurrentMaxNum++;
	}else if((motor->Current < (OBSTACLE_CURRENT_MAX>>1)) && (motor->CurrentLast < (OBSTACLE_CURRENT_MAX>>1))){
		motor->CurrentMaxNum = 0;
	}	
	if(motor->CurrentMaxNum > OBSTACLE_CURRENT_MAX_NUM){
		motor->CurrentStrategy = OBSTACLE_CURRENT;
	}
	
//	//-------------------�Ʋ�Խ��------------------------------------
//	if((motor->Current > CROSS_CURRENT_MAX) && (motor->deltaCurrent > CROSS_DELTA_CURRENT_MAX)){
//		motor->CurrentCrossFlag = 1;
//	}else if((motor->Current > CROSS_CURRENT_MAX) && (motor->CurrentCrossFlag > 0) && (motor->CurrentCrossFlag < 2)){
//		motor->CurrentCrossFlag ++;
//	}else if((motor->Current < CROSS_CURRENT_MAX) && (motor->CurrentLast < CROSS_CURRENT_MAX)){
//		motor->CurrentCrossFlag = 0;
//	}else if((motor->Current > CROSS_CURRENT_MAX) && (motor->CurrentCrossFlag == 2)){
//		motor->CurrentCrossFlag = 0;
//		motor->CurrentStrategy = CROSS_CURRENT;
//	}	
	
	return 1;
}

/****************************************************
������:PID_Strategy
��ע: PIDѡ�����
****************************************************/
int16_t PID_Strategy(MOTOR_PID *PID, MOTOR_PARM *motor)
{	
	if(motor->SpeedStrategy == CONSTANT_SPEED){   //���ٲ���	
		PID->Kp=PID_WH_S_C[0];
		PID->Ki=PID_WH_S_C[1];
		PID->Kd=PID_WH_S_C[2];
	}else if((motor->SpeedStrategy == ACCELERATE_SPEED) || (motor->SpeedStrategy == DECELERATE_SPEED)){       //���ٲ���	
		PID->Kp=PID_WH_S_A[0];
		PID->Ki=PID_WH_S_A[1];
		PID->Kd=PID_WH_S_A[2];		
	} 
	if((motor->SpeedStrategy == CONSTANT_SPEED) && (motor->CurrentStrategy == CROSS_CURRENT)){       //Խ�ϲ���
		PID->Kp=PID_WH_S_A[0];
		PID->Ki=PID_WH_S_A[1];
		PID->Kd=PID_WH_S_A[2];		
	}else if(motor->CurrentStrategy == OBSTACLE_CURRENT){   //���ϰ�����		
		//��ͣ
	}
	return 1;
}

/****************************************************
������:PID_PI
��ע: PI���Ʋ���
****************************************************/
int16_t PID_PI(MOTOR_PID *PID)
{	
	int32_t delta = 0;
	int32_t delta2 = 0;
	double comp = 1;
	uint32_t temp=0;
	if(PID->GoalLast != PID->Goal){
		PID->GoalLast = PID->Goal;
		//PID->Integrator = 0;
	}
	
	//---------------������ͬ������-------------------------
	if(PID->motorID == ID_MOTOR_L){
//			PID->Error2 = Motors.motor2.Speed - Motors.motor1.Speed;
//			PID->Integrator2 += PID->Error2;
//			delta2 = 5 * PID->Error2 + 1 * PID->Integrator2;
			//delta2 = -100;
			PID->ErrorLast2 = PID->Error2;
			comp = SPEED_COMP_L; //����ϵͳ����
	}else if(PID->motorID == ID_MOTOR_R){
//			PID->Error2 =8 * (Motors.motor1.Speed - Motors.motor2.Speed);
//			PID->Integrator2 += PID->Error2;
//			delta2 = 4 * PID->Error2 + 0.3 * PID->Integrator2;
			//delta2 = 120;
			PID->ErrorLast2 = PID->Error2;	
			comp = SPEED_COMP_R;//����ϵͳ����
	}
	PID->Error = comp * PID->Goal - PID->CurrentValue;
	PID->Integrator += PID->Error;
	PID->ErrorLast = PID->Error;
	
	delta = PID->Kp * PID->Error + PID->Ki * PID->Integrator;
	
	if(delta > 0){	temp = delta;	delta = temp>>10;}
	else{	temp = -delta;	delta = -(temp>>10);	}	
	
	PID->ExecuteValue = PID->CurrentValue + delta +  delta2;//PID->CurrentValue ;
	PID->ExeFlag = 1;
	
	
	return PID->ExecuteValue;	
}


/****************************************************
������:PID_PI
��ע: PI���Ʋ���
****************************************************/
int16_t PID_PI_D_S(MOTOR_PID *PID)
{	
	int32_t delta = 0;
	uint32_t temp=0;
	if(PID->GoalLast != PID->Goal){
		PID->GoalLast = PID->Goal;
		//PID->Integrator = 0;
	}
	
	PID->Error = PID->Goal - PID->CurrentValue;
	PID->Integrator += PID->Error;
	PID->ErrorLast = PID->Error;
	
	delta = PID->Kp * PID->Error + PID->Ki * PID->Integrator;
	
	if(delta > 0){	temp = delta;	delta = temp>>8;}
	else{	temp = -delta;	delta = -(temp>>8);	}
	
	PID->ExecuteValue = PID->CurrentValue + delta;//PID->CurrentValue ;
	PID->ExeFlag = 1;
	
	//PID->ExecuteValue=0;
	return PID->ExecuteValue;	
}

/****************************************************
������:PID_PID
��ע: PID���Ʋ���
****************************************************/
int16_t PID_PID(MOTOR_PID *PID)
{	
	int32_t delta=0;
	PID->Error = PID->CurrentValue - PID->Goal;
	PID->Integrator += PID->Error;
	PID->ErrorLast = PID->Error;
	PID->GoalLast = PID->Goal;
	
	delta = PID->Kp * PID->Error + PID->Ki * PID->Integrator + PID->Kd * (PID->Error - PID->ErrorLast);
	
	PID->ExecuteValue = PID->CurrentValue + delta;
	return PID->ExecuteValue;	
}
