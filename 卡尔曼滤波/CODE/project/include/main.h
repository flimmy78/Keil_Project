/******************** (C) COPYRIGHT 2009 Chen Yao ******************************
* File Name          : main.h
* Author             : Chen Yao
* Version            : V1.0
* Date               : 05/02/2009
* Description        : This file contains the headers of main.c
********************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx.h"
#include "DeviceConfig.h"

#define PRINT_ERR //����ģʽ

#define ADC_SAMPLING_NUM 1
#define ADC_CHANNEL_NUM 5
//#define ADC_valueBufSize  CurrentFilterNum*ADCNum

#define PIDPeriod 400    //PIDʱ������
#define SpeedPeriod 1000    //�ٶ�ʱ������
#define CurrentPeriod 400    //����ʱ������
#define UpdatePeriod 400    //�ϴ�����ʱ������

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern volatile uint32_t timeTick;
extern uint32_t timeTickLast;
extern uint16_t ctrlMode;		//����ģʽ
extern uint8_t ScramFlag;		//1:��ͣģʽ   0:����
extern int8_t HeartIT;  

extern uint16_t timeOver;
extern uint16_t timeOverCon;
extern uint16_t timeOverFlag;
 
extern uint8_t PIDPeriodFlag;   
extern uint8_t SpeedPeriodFlag;   
extern uint8_t CurrentPeriodFlag;   
extern uint8_t UpdatePeriodFlag;   

extern volatile uint16_t ADC_valueBuf[ADC_SAMPLING_NUM][ADC_CHANNEL_NUM];

/* Private function prototypes -----------------------------------------------*/

#endif
