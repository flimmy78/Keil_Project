#include "RudderControl.h"
RUDDER_T  _RudderX = {0};
RUDDER_T  _RudderY = {0};
RUDDER_T* RudderX = &_RudderX;
RUDDER_T* RudderY = &_RudderY;
/****************************************************
	������:	setRudderAngle
	����:	���ö���Ƕ�
	����ֵ:	
	����:	liyao 2015��11��20��17:14:40
****************************************************/
void _setRudderAngle(RUDDER_T* rudder,uint16_t angle){
	uint16_t PWM = angle * rudder->Angle_Code + RUDDER_MIN_WIDTH;
	printf("Angle_Code:%f,angle:%d,PWM:%d\r\n",rudder->Angle_Code,angle,PWM);
	switch(rudder->TIM_Channel){
		case 1:TIM_SetCompare1(rudder->TIMx, PWM);break;
		case 2:TIM_SetCompare2(rudder->TIMx, PWM);break;
		case 3:TIM_SetCompare3(rudder->TIMx, PWM);break;
		case 4:TIM_SetCompare4(rudder->TIMx, PWM);break;
	}
}

/****************************************************
	������:	Rudder_Init
	����:	��ʼ��������ò���
	����ֵ:	
	����:	liyao 2015��11��20��17:14:37
****************************************************/
void Rudder_Init(void){
	_RudderX.TIMx = TIM4;
	_RudderX.TIM_Channel = 1;
	_RudderX.TIM_Period_Pulse = RUDDER_MAX_WIDTH;//0�㣺0.5ms----180�㣺2.5ms
	_RudderX.Angle_Code = (RUDDER_MAX_WIDTH - RUDDER_MIN_WIDTH)*1.0/RUDDER_MAX_ANGLE;//2500-500/180��
	_RudderX.setRudderAngle = _setRudderAngle;
	
	_RudderY.TIMx = TIM4;
	_RudderY.TIM_Channel = 2;
	_RudderY.TIM_Period_Pulse = RUDDER_MAX_WIDTH;//0�㣺0.5ms----180�㣺2.5ms
	_RudderY.Angle_Code = (RUDDER_MAX_WIDTH - RUDDER_MIN_WIDTH)*1.0/RUDDER_MAX_ANGLE;
	_RudderY.setRudderAngle = _setRudderAngle;
}
