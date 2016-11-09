#ifndef __TOOL_H
#define __TOOL_H
#include <stdio.h>
#include "stm32f0xx.h"
//#include "stm32f10x_systick.h"



//###################################ϵͳ����ʱ��###################################

#define HZ 1000000  //Ƶ��
#define SYSTICKCOUNT (SystemCoreClock/8) //1���ڵļ���ֵ
#define SYSTICK_STEP (SYSTICKCOUNT/HZ)
#define USMAX 60000000 //60��=60000000΢��

/****************************************************
	�ṹ����:	SysTickStamp_t
	����:	ʱ���ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct 
{
	 int16_t st_s;
	 int16_t st_ms;
	 int16_t st_us;
} SysTickStamp_t;
//###################################ʱ�����###################################
/****************************************************
	������:	SysTickStamp_t
	����:	ϵͳ��ʱ��δ�
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
//extern volatile int32_t microsecond ;

#define microsecond getMicrosecond()
extern volatile int32_t second;
extern int32_t getMicrosecond(void);
extern void BaseClock_Init(void);
extern SysTickStamp_t GetSysTickStamp(void);
extern int32_t GetTimeStamp(void);
extern SysTickStamp_t Create_SysTickStamp(uint16_t s,uint16_t ms, uint16_t us);
extern SysTickStamp_t Elapse(SysTickStamp_t* now,SysTickStamp_t* before);
extern int32_t Elapse_Us(int32_t now,int32_t before);
extern int32_t SysTickStampToUs(SysTickStamp_t* sysTickStamp);
extern SysTickStamp_t UsToSysTickStamp(int32_t us);
extern void DelayUS(uint32_t);
extern void DelayMS(uint32_t);
extern void DelayS(int);
extern void DelayX(SysTickStamp_t);

//###################################�������###################################
#define TIMETASK_MAXNUM 15	//�����������


typedef struct TimeTask_s TimeTask_t;
//extern volatile uint16_t TimeTaskBusy; //��æ����
//extern volatile uint16_t TimeTaskFree;	//���м���
//extern volatile uint8_t TimeTaskFlag;		//��æ�����б�־λ

enum TASK_MODE{Real_Time_Mode,Count_Mode};//real_timeģʽ�� �����ڿ�Խ�˶������ֻ����һ�� countģʽ�¿�Խ������ڻ�ִ�ж�� �ϸ�ִ�д���

/****************************************************
	�ṹ����:	TimeTask_s
	����:	����ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
struct TimeTask_s
{
	int8_t id;
	int8_t alias;
	enum TASK_MODE taskmode;//����ģʽ��ʵʱģʽ������ģʽ��
	int32_t interval;//ִ������ ��λ��us
	int32_t willTime;//Ԥ���´�ִ��ʱ��
	int32_t baseTime;//����ʱ��
	int32_t lastBeginTime;//������п�ʼʱ��
	int32_t lastEndTime;//������н���ʱ��
	uint16_t runTime;//ִ��ʱ��
	uint16_t timeOutCount;//��ʱ����
	uint16_t runCount;//ִ�д���
	int32_t whileCount;//�ȴ���ʱ����
	void (*task_handle)(void);//ִ�к���
	TimeTask_t* next;
} ;

typedef struct 
{
	uint8_t Task_Size;		//������������
	uint8_t Task_FreeSize;//������������
	int32_t LastBeginTime;//������п�ʼʱ��
	int32_t LastEndTime;//������н���ʱ��
	int32_t RunTime;//ִ��ʱ��
	int32_t TimeQuantum;//������ѭ��ִ�о�����ʱ��
	TimeTask_t TThead;	//����ͷ
}TimeTaskInfo_t ;

extern TimeTask_t TimeTaskList[TIMETASK_MAXNUM];
extern void TimeTask_Init(void);
extern int8_t TimeOut(int8_t taskID, int32_t timeOutUs, int8_t condition);
extern void TimeTask_Run(void);
extern int8_t TimeTask_Add(int8_t title,SysTickStamp_t interval,void(*_task_handle)(void),enum TASK_MODE taskmode);
extern int8_t TimeTask_Remove(uint8_t id);
extern void TimeTask_Monitor(void); //���ؼ���  

//###################################����###################################
#define QUEUE_MAXNUM 10

/****************************************************
	�ṹ����:	QUEUE_T
	����:	����ͷʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct{
	void* data;
	
	uint8_t single_size;
	uint16_t count;
	
	uint16_t start;
	uint16_t end;
	uint16_t num;
	
	uint8_t use_state; 
}QUEUE_T;

extern QUEUE_T* Queue_Init(void* _array,uint8_t _single_size ,uint16_t _count);
/*extern int8_t queue_put(QUEUE_T* , int32_t );
extern int8_t queue_get(QUEUE_T* , int32_t* );*/
extern int8_t Queue_Put(QUEUE_T* queue,void* _data);
extern int8_t Queue_Get(QUEUE_T* queue,void* _data);
extern uint16_t Queue_Size(QUEUE_T* queue);
extern uint16_t Queue_Empty_size(QUEUE_T* queue);
extern void Queue_Free(QUEUE_T* queue);




#endif
