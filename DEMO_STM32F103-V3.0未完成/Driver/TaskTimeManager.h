#ifndef __TASKTIMEMANAGER_H__
#define __TASKTIMEMANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h" 
#include "stm32f10x_tim.h"

static void TaskTime_Monitor(void);
static void TaskTime_Monitor_Init(void);

#define	SYSTICK_COUNT SystemCoreClock/8/1000 //9000
#define SYSTICK_1US_COUNT	SYSTICK_COUNT/1000//9

//#define LOAD_MONITOR_ON
#ifdef	LOAD_MONITOR_ON 
	#define LOAD_TIMX 				TIM2
	#define COUNTER_ON 				LOAD_TIMX->CR1 |= TIM_CR1_CEN
	#define COUNTER_OFF 			LOAD_TIMX->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN))
#endif

#define EXTI_IT_DISABLE() __set_PRIMASK(1)
#define EXTI_IT_ENABLE()  __set_PRIMASK(0)

#define SYSTICK_IT_DISABLE() SysTick->CTRL &= (~(1ul << SysTick_CTRL_TICKINT_Pos));
#define SYSTICK_IT_ENABLE()  SysTick->CTRL |= (1ul << SysTick_CTRL_TICKINT_Pos);

#define TimeCycle(s,ms) (s*1000 + ms)
#define TASK_MAX_COUNT 64
#define TASK_FIRST_DELAY 3//�����������ʱ���ӳ�
typedef enum {TASK_INIT, TASK_WAIT,TASK_READY, TASK_SUSPEND, TASK_RECOVER, TASK_REMOVE}TaskState; 
typedef enum {Real_Mode,Count_Mode}TASK_MODE;//realģʽ�� �����ڿ�Խ�˶������ֻ����һ�� countģʽ�¿�Խ������ڻ�ִ�ж�� �ϸ�ִ�д���
//������Ϣ����
typedef struct _TaskTime_T TaskTime_T;
struct _TaskTime_T{
	int8_t 	Alias;				 	  //����ID
	int8_t 	_TaskID;		  	//�ڲ�����
	int8_t 	Priority;				  //���ȼ�
	TASK_MODE TaskMode;				//����ģʽ
	TaskState _TaskState;			//����״̬
	uint16_t 	_TaskCycleCount;//���ڼ�����
	uint16_t 	TaskCycle;		 	//������
	void(*Run)(void);				 	//ִ�еĺ���
	uint16_t		StoreCount;			//δ�ܰ�ʱִ�д���
	uint16_t	RunCount;			 	//ִ�д���
	uint16_t	RunElapsed;		 	//��ʱ ��λus
	TaskTime_T* _next;				//��һ������ָ��
	TaskTime_T* _prev;				//��һ������ָ��
};

typedef struct 
{
	uint8_t TaskSize;		//������������
	uint8_t TaskFreeSize;//������������
	TaskTime_T TThead;	//�б�ͷ
}TimeTaskInfo_T;
 

extern void TaskTime_Run(void);
extern void BaseClock_Init(void);
extern void TaskTime_Init(void);
extern int8_t TaskTime_Add(int8_t alias,uint16_t TaskCycle, int8_t Priority,void(*Run)(void), TASK_MODE TaskMode);
extern int8_t TaskTime_Remove(uint8_t id);
extern int8_t TaskTime_SuspendTask(uint8_t id);
extern int8_t TaskTime_RecoverTask(uint8_t id);  
extern void SysTick_Handler(void);
extern void DelayUS(int32_t);
extern void DelayMS(int32_t);
extern void DelayS(int32_t);
#endif


