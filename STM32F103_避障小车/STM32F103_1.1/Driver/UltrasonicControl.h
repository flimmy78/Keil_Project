#ifndef __ULTRASONICCONTROL_H__
#define __ULTRASONICCONTROL_H__

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

typedef enum{TEMPERATURE = 0x50, DISTANCE = 0x55}Ultrasonic_CMD;
typedef struct _Ultrasonic_T Ultrasonic_T;
struct _Ultrasonic_T{
	uint8_t Temperature;//�¶�
	uint16_t Distance;	//���� ��λmm
	uint8_t Distance_State;//0:��8λ������ 1����8λ������ 2������������
	Ultrasonic_CMD Uart_CMD;//���һ�δ�������
};


extern Ultrasonic_T* Ultrasonic;
extern void Ultrasonic_Send(Ultrasonic_T* ultra,Ultrasonic_CMD cmd);
extern void Ultrasonic_Recv(Ultrasonic_T* ultra, uint8_t data);
extern void Ultrasonic_Run(void);

#endif

