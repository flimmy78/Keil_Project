#include <Type.h>
/***********��������****************/
u8 BasePWM = 0;//����PWM
u8 DeviationPWM = 10;//��̬�ж����
u8 W_PWM = 0,D_PWM = 0,L_PWM = 0,R_PWM = 0;//��ǰpwm
u8 W_Static = 0,D_Static = 0,L_Static = 0,R_Static = 0;//�ȶ�pwm�ļ�¼
extern short T_X,T_Y,T_Z;		 //X,Y,Z�ᣬ���ٶ�
extern short O_X,O_Y,O_Z;		 //X,Y,Z�ᣬ���ٶ�
extern short T_T; //�¶�

/***********��������****************/
void Save_StaticPWM(void);
void initPWM(void);



void initPWM(void){
	while(O_Z<10)
	{
		BasePWM++;
	}
}

void BalancePosture(){
	if(0<T_X&&T_X<DeviationPWM){//���X��Ϊ����
		
		if(W_PWM>D_PWM)
		{
			W_PWM--;
		}else if(W_PWM<D_PWM){
			D_PWM--;
		}
		
	}else if(0<T_Y && T_Y<DeviationPWM){//���Y��Ϊ����
		L_PWM++;
	}else{
		Save_StaticPWM();
	}
}

void Save_StaticPWM(void){
	W_Static = W_PWM;
	D_Static = D_PWM;
	L_Static = L_PWM;
	R_Static = R_PWM;
}







