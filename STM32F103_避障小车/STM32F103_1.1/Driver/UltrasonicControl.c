#include "UltrasonicControl.h"


Ultrasonic_T _Ultrasonic;
Ultrasonic_T* Ultrasonic = &_Ultrasonic;
/****************************************************
	������:	Ultrasonic_Send
	����:		���ͳ���������
	����:		
	����:		2015��11��27�� 09:49:48
****************************************************/
void Ultrasonic_Send(Ultrasonic_T* ultra,Ultrasonic_CMD cmd){
	if(cmd == DISTANCE)
		ultra->Distance_State = ultra->Distance = 0;
	
	ultra->Uart_CMD = cmd;
	
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET) ;
	USART_SendData(UART4, cmd);
	USART_ClearFlag(UART4, USART_FLAG_TC);
}

/****************************************************
	������:	Ultrasonic_Recv
	����:		���ղ������������صĽ��
	����:		
	����:		2015��11��27�� 09:49:48
****************************************************/
void Ultrasonic_Recv(Ultrasonic_T* ultra, uint8_t data){
	if(ultra->Uart_CMD == TEMPERATURE){	//������¶�����
		ultra->Temperature = data - 45;		//����-45 = ʵ���¶�
	}else if(ultra->Uart_CMD == DISTANCE){	//����ǳ������������
		if(ultra->Distance_State == 0){
			ultra->Distance = data;
			ultra->Distance = ultra->Distance <<8;
			ultra->Distance_State = 1;
		}else if(ultra->Distance_State == 1){
			ultra->Distance = ultra->Distance | data;
			ultra->Distance_State = 2;
		}
	}
}
//------------------------------------------------------------------------
uint8_t Ultrasonic_flag = 0;
extern uint8_t Rudder_Pause;
void Ultrasonic_Run(void){
	if(Ultrasonic->Distance_State == 2){//���������ݾ���
		if(Ultrasonic->Distance < 100)		//�ϰ�����10cmʱ
			Rudder_Pause = 1;								//���ͣת
		else
			Rudder_Pause = 0;
		
	}
	#ifdef PRINT_ERR
	if(Ultrasonic->Distance_State == 2)
		printf("�¶ȣ�%d,���룺%d\r\n",Ultrasonic->Temperature,Ultrasonic->Distance);
	#endif
	if(Ultrasonic_flag)
		Ultrasonic_Send(Ultrasonic, TEMPERATURE);
	else
		Ultrasonic_Send(Ultrasonic, DISTANCE);
	Ultrasonic_flag = ~Ultrasonic_flag;
	
	
}

