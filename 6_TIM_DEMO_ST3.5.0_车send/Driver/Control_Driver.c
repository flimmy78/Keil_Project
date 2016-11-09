#include <stdio.h> 
#include <Control_Driver.h>
#include <Type.h>  
#include <stm32f10x_tim.h>

char W=0,S=0,A=0,D=0;
int WS=0,SS=0,AS=0,DS=0,Base = 0;
extern short T_X,T_Y,T_Z;		 //X,Y,Z�ᣬ���ٶ�
extern short O_X,O_Y,O_Z;		 //X,Y,Z�ᣬ���ٶ�
extern short T_T;
void USART1_Handle(u8* receiveData){
	u8 direct = *receiveData;
	//u8 negative = *++receiveData;
	int speed = (*(receiveData+2))<<8|(*(receiveData+3));
// 	printf("direct:");
// 	printf("speed:%d",speed);
	//printf("USART1_Handle:");
	
// 	USART1_SendData(*receiveData);
// 	USART1_SendData(*(++receiveData));
// 	USART1_SendData(*(++receiveData));
// 	USART1_SendData(*(++receiveData));
// 	USART1_SendData(direct);
 	//printf("USART1_Handle:%c",direct);
//	printf("inUSART1_Handle");
// 	if(1==1){
// 		TIM_SetCompare1(TIM2, receiveData);
// 		printf("set������%d",receiveData);
// 		
// 	}
 	if(direct=='w'||direct=='W'){
 		W=1,S=0,A=0,D=0;
		WS = speed;
		printf("\n����W,�ٶȣ�%d",WS);
 	}else if(direct=='s'||direct=='S'){
		W=0,S=1,A=0,D=0;
		SS = speed;
		printf("\n����S,�ٶȣ�%d",SS);
	}else if(direct=='a'||direct=='A'){
		W=0,S=0,A=1,D=0;
		AS = speed;
		printf("\n����A,�ٶȣ�%d",AS);
	}else if(direct=='d'||direct=='D'){
		W=0,S=0,A=0,D=1;
		DS = speed;
		printf("\n����D,�ٶȣ�%d",DS);
	}else if(direct=='b'||direct=='B')
	{
		Base = speed;
		W=0,S=0,A=0,D=0;
		printf("\n����B,�����ٶȣ�%d",Base);
	}
	if(direct=='v'||direct=='V'){
		printf("\n���ٶ�X:%d\tY:%d\tZ:%d",O_X,O_Y,O_Z);
	  printf("\t���ٶ�X:%d\tY:%d\tZ:%d\tT:%d",T_X,T_Y,T_Z,T_T);
		printf("\nWS:%d\tSS:%d\tDS:%d\tAS:%d\tBase:%d",WS,SS,DS,AS,Base);
	}
	if(direct=='z'||direct=='Z'){
		Base+=200;
		printf("\n����B����200,�����ٶȣ�%d",Base);
	}
	TIM_SetCompare1(TIM2,10000-(Base+WS));
	TIM_SetCompare2(TIM2,10000-(Base+DS));
	TIM_SetCompare3(TIM2,10000-(Base+SS));
	TIM_SetCompare4(TIM2,10000-(Base+AS));

	//TIM_PrescalerConfig(TIM2,7200-1,TIM_PSCReloadMode_Immediate);
	//TIM_SetIC1Prescaler(TIM2,7200);
	
}

