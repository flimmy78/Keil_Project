#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "tool.h" 
#include "stm32f10x_rcc.h"
/*
 *���߰�
 *author����Ң 
 *time��2015��8��26��10:41:23
 */
 
//###################################ʱ����غ���###################################
// volatile uint32_t jiffies ;
//volatile int32_t microsecond ;
volatile int32_t second = 0;
volatile uint8_t SysTick_Flag = 0;
u32 DelayCount = 0;
int32_t getMicrosecond(){
	int32_t SysTick_Count = 0;
	SysTick_Count = HZ - SysTick->VAL/SYSTICK_STEP;
	if(SysTick_Count < HZ/2 && SysTick_Flag){
		SysTick_Flag = 0;
		second += HZ;
	}else if(SysTick_Count > HZ/2 && SysTick_Flag == 0){
		SysTick_Flag = 1;
	}
		
	if((second + SysTick_Count) >= USMAX)
		second = 0;
	SysTick_Count = SysTick_Count + second;
	return SysTick_Count;
}
void BaseClock_Init(){
	//--------------������ʱ��-------------------
	/*TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,	ENABLE);  
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = USMAX - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;			//1 000 000 HZ����
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);*/
	//--------------������ʱ��end-------------------
	
	//ʱ��
		
	//����
	/*SysTick_CounterCmd(SysTick_Counter_Clear);
	SysTick_SetReload(9000000-1);
	SysTick_CounterCmd(SysTick_Counter_Enable);
	SysTick_ITConfig(DISABLE); */
	SysTick_Config(SYSTICKCOUNT);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //SysTick_CLKSource_HCLK_Div8
}
/****************************************************
	������:	getSysTickStamp
	����:	��ȡ��ǰʱ��
	����ֵ:	ʱ���
	����:	liyao 2015��9��8��14:10:51
****************************************************/
SysTickStamp_t GetSysTickStamp(){
	return UsToSysTickStamp(microsecond);
}

/****************************************************
	������:	getTimeStamp
	����:	��ȡ��ǰʱ��
	����ֵ:	ʱ���
	����:	liyao 2015��9��8��14:10:51
****************************************************/
int32_t GetTimeStamp(){
	return microsecond;
}
/****************************************************
	������:	create_SysTickStamp
	����:	����ʱ���ʵ��
	����:	�룬���룬΢��
	����ֵ:	ʱ���
	����:	liyao 2015��9��8��14:10:51
****************************************************/
SysTickStamp_t Create_SysTickStamp(uint16_t s,uint16_t ms, uint16_t us){
	SysTickStamp_t sts;
	memset(&sts,0,sizeof(SysTickStamp_t));
	if(s > 59 || ms > 999 || us > 999)
		return sts;
	sts.st_s = s;
	sts.st_ms = ms;
	sts.st_us = us;
	return sts;
}

/****************************************************
	������:	elapse
	����:	ʱ��Ƚϲ�ֵ ���Կ�Խ�����
	����:	����ʱ�䣬��ǰʱ��
	����ֵ:	����ʱ�� - ��ǰʱ��Ĳ�ʱ���
	����:	liyao 2015��9��8��14:10:51
****************************************************/
SysTickStamp_t Elapse(SysTickStamp_t* now,SysTickStamp_t* before){
	SysTickStamp_t res;
	/*������ ÿ��14187��
	int32_t now_us,before_us,res_us,tmp;
	now_us = now.st_s * 1000000 + now.st_ms * 1000 + now.st_us;
	before_us = before.st_s * 1000000 + before.st_ms * 1000 + before.st_us;
	
	res_us = now_us - before_us;
	res.st_s = res_us / 1000000;
	res.st_ms = (tmp = res_us % 1000000) / 1000;
	res.st_us = tmp % 1000;*/
	
	/*������ ÿ��41740��*/
	res.st_s  = now->st_s  - before->st_s;
	res.st_ms = now->st_ms - before->st_ms;
	res.st_us = now->st_us - before->st_us;
	
	if(res.st_us < 0){
		res.st_ms--;
		res.st_us += 1000;
	}
	if(res.st_ms < 0){
		res.st_s--;
		res.st_ms += 1000;
	}
	if(res.st_s < 0){
		res.st_s += 60;
	}
	return res; 
}

/****************************************************
	������:	elapse_us
	����:	ʱ��Ƚϲ�ֵ
	����:	����ʱ�䣬��ǰʱ��
	����ֵ:	����ʱ�� - ��ǰʱ��Ĳ�΢��ֵ
	����:	liyao 2015��9��8��14:10:51
****************************************************/
int32_t Elapse_Us(int32_t now,int32_t before){
	if(now < before){ 
		//printf("now:%d,before%d,tick:%d\r\n",now,before,SysTick->VAL);
		return USMAX + (now - before); //��ֹ����� ������ָ��� ����590000��000001
	}else
		return now - before;
}

/****************************************************
	������:	SysTickStampToUs
	����:	ʱ���ת΢��
	����:	ʱ���
	����ֵ:	ʱ�����Ӧ��΢��
	����:	liyao 2015��9��8��14:10:51
****************************************************/
int32_t SysTickStampToUs(SysTickStamp_t* sysTickStamp){
	return sysTickStamp->st_s * 1000000 + sysTickStamp->st_ms * 1000 + sysTickStamp->st_us;
}

/****************************************************
	������:	UsToSysTickStamp
	����:	΢��תʱ���
	����:	΢��
	����ֵ:	΢���Ӧ��ʱ���
	����:	liyao 2015��9��8��14:10:51
****************************************************/
SysTickStamp_t UsToSysTickStamp(int32_t us){
	SysTickStamp_t sts;
	sts.st_s = us / 1000000;
	sts.st_ms = us % 1000000 / 1000;
	sts.st_us = us % 1000000 % 1000;
	return sts;
}
/****************************************************
	������:	delayUS
	����:	΢����ʱ
	����:	΢��ֵ
	����:	liyao 2015��9��8��14:10:51
****************************************************/
void DelayUS(uint32_t num){ 
	int32_t before = microsecond;
	int32_t lapse;
	do{
		lapse = microsecond - before;
		if(lapse < 0)
			lapse = USMAX + lapse;
	}while(lapse < num);
} 

/****************************************************
	������:	delayMS
	����:	������ʱ
	����:	����ֵ
	����:	liyao 2015��9��8��14:10:51
****************************************************/
void DelayMS(uint32_t num){
	int32_t lapse;
	int32_t before;
	num *= 1000;
	 before = microsecond;
	do{
		lapse = microsecond - before;
		if(lapse < 0)
			lapse = USMAX + lapse;
	}while(lapse < num);
}

/****************************************************
	������:	delayS
	����:	����ʱ
	����:	��ֵ
	����:	liyao 2015��9��8��14:10:51
****************************************************/
void DelayS(int num){
	int32_t before,lapse;
	if(num > 59000000) return;
	num *= 1000000;
	before = microsecond;
	
	do{
		lapse = microsecond - before;
		if(lapse < 0)
			lapse = USMAX + lapse;
	}while(lapse < num);		
}

/****************************************************
	������:	delayX
	����:	ʱ�����ʱ
	����:	ʱ���
	ע��:	���1000΢��+1000����+59��
	����:	liyao 2015��9��8��14:10:51
****************************************************/
void DelayX(SysTickStamp_t sts){
	if(sts.st_s > 59 || sts.st_ms > 999 || sts.st_us > 999)
		return;
	DelayS(sts.st_s);
	DelayMS(sts.st_ms);
	DelayUS(sts.st_us);
}

//###################################������غ���###################################

QUEUE_T queue_list[QUEUE_MAXNUM] = {0}; 
u8 queue_locd = 0;
/****************************************************
	������:	queue_init
	����:	���г�ʼ��
	����:	�����������鵥��Ԫ�ش�С������Ԫ�ظ���
	����ֵ:	���о�� �� NULL
	����:	liyao 2015��9��8��14:10:51
****************************************************/
QUEUE_T* Queue_Init(void* _array,uint8_t _single_size ,uint16_t _count){ 
	uint8_t i = 0;
	QUEUE_T* queue = NULL;
	for(i = 0; i < QUEUE_MAXNUM; i++)
		if(queue_list[i].use_state == 0)
			queue = &queue_list[i];
	if(queue == NULL)
		return NULL;
	queue->use_state = 1;
	queue->single_size = _single_size;
	queue->count = _count;
	queue->start = queue->end = queue->num =0; 
	queue->data = _array;
	return queue;
}

/****************************************************
	������:	queue_put
	����:	���ݲ��뵽����
	����:	���о��������
	����:	liyao 2015��9��8��14:10:51
****************************************************/
/*int8_t queue_put(QUEUE_T* queue,int32* _data){
		if(queue->num == queue->count)  
			return -1;
		switch(queue->single_size){ 
			case 1:((int8_t*)queue->data)[queue->start] = (int8_t)_data ;	break;
			case 2:((int16_t*)queue->data)[queue->start] = (int16_t)_data;	break;
			case 4:((int32_t*)queue->data)[queue->start] = (int32_t)_data;	break;
			default:return -1;
		}
		queue->start = ((++queue->start) % queue->count);
		queue->num ++;
		return 0;
}*/

/****************************************************
	������:	queue_get
	����:	�Ӷ���ȡ������
	����:	���о�����������������
	����:	liyao 2015��9��8��14:10:51
****************************************************/
/*int8_t queue_get(QUEUE_T* queue, int32_t* _data){
		if(queue->num == 0) 
			return -1;
		switch(queue->single_size){
			case 1:*_data = ((int8_t*)queue->data)[queue->end];
										  ((int8_t*)queue->data)[queue->end] = 0;	break;
			case 2:*_data = ((int16_t*)queue->data)[queue->end];
											((int16_t*)queue->data)[queue->end] = 0;			break;
			case 4:*_data = ((int32_t*)queue->data)[queue->end];
											((int32_t*)queue->data)[queue->end] = 0;break;
			default:return -1;
		}
		queue->end = ((++queue->end) % queue->count);
		queue->num --;
		return 0;
}*/
#define QUEUE_LOCK do{if(queue->locked == 1) return -1;else queue->locked = 1;}while(0);
#define QUEUE_UNLOCK do{queue->locked = 0;}while(0);
/****************************************************
	������:	queue_put 
	����:	���ݲ��뵽����
	����:	���о��������
	����:	liyao 2015��9��8��14:10:51
****************************************************/
int8_t Queue_Put(QUEUE_T* queue,void* _data){
		if(queue->num == queue->count)  
			return -1;
		QUEUE_LOCK;
		memcpy((char*)queue->data + (queue->start * queue->single_size),_data,queue->single_size);
		queue->start = ((++queue->start) % queue->count);
		queue->num ++;
		QUEUE_UNLOCK
		return 0;
}

/****************************************************
	������:	queue_get
	����:	�Ӷ���ȡ������
	����:	���о�����������������
	����:	liyao 2015��9��8��14:10:51
****************************************************/
int8_t Queue_Get(QUEUE_T* queue, void* _data){
		if(queue->num == 0) 
			return -1;
		QUEUE_LOCK;
		memcpy(_data,(char*)queue->data + (queue->end * queue->single_size),queue->single_size);
		queue->end = ((++queue->end) % queue->count);
		queue->num --;
		QUEUE_UNLOCK;
		return 0;
}

/****************************************************
	������:	queue_size
	����:		��ȡ���е�ǰ��Ա����
	����:	���о��
	����ֵ���������г�Ա����
	����:	liyao 2015��9��8��14:10:51
****************************************************/
uint16_t Queue_Size(QUEUE_T* queue){
	return queue->num;
}

/****************************************************
	������:	queue_empty_size
	����:	��ȡ����ʣ���λ����
	����:	���о��
	����ֵ������ʣ���λ����
	����:	liyao 2015��9��8��14:10:51
****************************************************/
uint16_t Queue_Empty_Size(QUEUE_T* queue){
	return queue->count - queue->num;
}

/****************************************************
	������:queue_free
	����:	�ͷŶ���
	����:	���о��
	����:	liyao 2015��9��8��14:10:51
****************************************************/
void Queue_Free(QUEUE_T* queue){
	queue->use_state = 0;
}
/* Usage:
	QUEUE_T* queue = queue_init(test ,sizeof(uint8_t)  ,10 );  
	queue_put(queue,(int8_t)200,0,0);
	queue_put(queue,(int8_t)201,0,0);
	queue_get(queue,(int8_t*)&i,NULL,NULL); */

//###################################��ʱ������к���###################################
//���������
TimeTask_t TimeTaskList[TIMETASK_MAXNUM] = {0};//����ջ
//volatile uint16_t TimeTaskBusy = 1; //��æ����
//volatile uint16_t TimeTaskFree = 1;	//���м���
//volatile uint8_t TimeTaskFlag = 0;		//��æ�����б�־λ
//���ڱ�����
TimeTaskInfo_t TimeTaskInfo = {0};//������Ϣ
TimeTask_t* _timeTask_head = &TimeTaskInfo.TThead;//�������ͷ

/****************************************************
	������:	TimeTask_Init
	����:	��ʱ������г�ʼ��
	����:	liyao 2015��9��8��14:10:51
****************************************************/
void TimeTask_Init(){
	uint8_t i = 0;
	_timeTask_head->next = NULL;
	for(i = 0; i<TIMETASK_MAXNUM; i++){
		TimeTaskList[i].id = -1;
		TimeTaskList[i].task_handle = NULL;
		TimeTaskList[i].interval = 0;
		TimeTaskList[i].lastBeginTime = 0;
		TimeTaskList[i].lastEndTime = 0;
		TimeTaskList[i].next = NULL;
	}
};


/****************************************************
	������:	TimeTask_add
	����:	��Ӷ�ʱ����
	����:	���ʱ��(ʱ���),ִ�к���,����ģʽ
	����ֵ:	>0������id���ɹ��� -1������ջ����ʧ�ܣ�
	˵��:	�������м���Ӵ�С���� ����������ִ�� ���ⱻ���С������äռ
	����:	liyao 2015��9��8��14:10:51
****************************************************/
int8_t TimeTask_Add(int8_t title,SysTickStamp_t interval,void(*_task_handle)(void),enum TASK_MODE taskmode){
	uint8_t i = 0;  
	TimeTask_t* tmpTimeTask,*beforeTimeTask,*newTimeTask;
	for(i = 0; i<TIMETASK_MAXNUM; i++){//Ѱ������ջ��λ
		if(TimeTaskList[i].id == -1){
			newTimeTask = &TimeTaskList[i];
			newTimeTask->id = i;//��ֵid
			newTimeTask->alias = title;
			newTimeTask->taskmode = taskmode;
			newTimeTask->interval = SysTickStampToUs(&interval);//��������
			newTimeTask->baseTime = microsecond;//�´�����ʱ��
			newTimeTask->task_handle = _task_handle;//������ָ��
			newTimeTask->timeOutCount = 0;
			newTimeTask->runCount = 0; 
			newTimeTask->next = NULL;
			newTimeTask->lastBeginTime = microsecond;
			TimeTaskInfo.Task_Size++;
			TimeTaskInfo.Task_FreeSize = TIMETASK_MAXNUM - TimeTaskInfo.Task_Size;
			break; 
		}
	}
	
	if(i == TIMETASK_MAXNUM)//����ջ���ж�
		return -1;
	
	if(_timeTask_head->next == NULL){//�״�ֱ�Ӳ���
		TimeTaskList[i].id = i;
		TimeTaskList[i].next = NULL;
		_timeTask_head->next = &TimeTaskList[i];
		return 0;
	}else{//
		beforeTimeTask =  _timeTask_head;
		tmpTimeTask = _timeTask_head->next;
		
		do{
			if(newTimeTask->interval > tmpTimeTask->interval){//����������ʱ���ԭ�ж��еĴ�
				newTimeTask->next = tmpTimeTask;
				beforeTimeTask->next = newTimeTask;
				break;
			}
			if(tmpTimeTask->next == NULL){//�����һ��Ԫ�ػ���û�и�С�ģ������
				tmpTimeTask->next = newTimeTask;
				newTimeTask->next = NULL;
				break;
			}
			beforeTimeTask = tmpTimeTask;
			tmpTimeTask = tmpTimeTask->next;
		}while(tmpTimeTask != NULL);
		
	}
	return newTimeTask->id;
}
/****************************************************
	������:	TimeTask_add
	����:		��������������û���κ�����ִ��ʱ����
	����:		ִ�к���
	ע��:		������������ʱ�����������С�����������Լ
	����:		liyao 2015��9��8��14:10:51
****************************************************/
/*void TimeTask_AddS(void(*_task_handle)(void)){
	_timeTask_head->task_handle = _task_handle;
}*/

/****************************************************
	������:	TimeTask_add
	����:		�Ƴ�����
	����:		�������ʱ���ص�����id
	����ֵ:	>0���ɹ� -1��ʧ��
	ע��:		������������ʱ�����������С�����������Լ
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t TimeTask_Remove(uint8_t id){
	TimeTask_t* tmpTimeTask,*beforeTimeTask;
	beforeTimeTask =  _timeTask_head;
	tmpTimeTask = _timeTask_head->next;
	do{
		if(tmpTimeTask->id == id){
			beforeTimeTask->next = tmpTimeTask->next;
			tmpTimeTask->id = -1;
			tmpTimeTask->task_handle = NULL;
			TimeTaskInfo.Task_Size--;
			TimeTaskInfo.Task_FreeSize = TIMETASK_MAXNUM - TimeTaskInfo.Task_Size;
			return 0;
		}
		beforeTimeTask = tmpTimeTask;
		tmpTimeTask = tmpTimeTask->next;
	}while(tmpTimeTask != NULL);
	return -1;
}

/****************************************************
	������:	TimeOut
	����:	��ʱ�ȴ���������
	����:	liyao 2015��10��30��11:50:41
****************************************************/
int8_t TimeOut(int8_t taskID, int32_t timeOutUs, int8_t condition){
	TimeTask_t* curTT;
	if(condition == 0)//�������� ֱ�ӷ���
		return 0;
	curTT = &TimeTaskList[taskID];
	if(curTT->whileCount == 0)//��һ�θ�ֵ��ʱʱ��
		curTT->whileCount = timeOutUs;
	TimeTask_Run();						//�ж�ʱ���Ƿ񵽴�
	if(curTT->whileCount == 0)//��ʱ����
		return 0;
	return 1;									//δ��ʱ�����ȴ�
}

/****************************************************
	������:	TimeTask_Run
	����:		��ʱ����������к���
	ע��:		���ü�����������С�������������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
void TimeTask_Run(void){
	TimeTask_t* curTT = _timeTask_head->next;
//	TimeTask_t* lastTT = NULL;
	int32_t lapse = 0;
	int32_t beginMicrosecond = 0;
	int8_t  HasRun = 0;

	while(curTT != NULL){
		beginMicrosecond = microsecond;
		lapse = Elapse_Us(beginMicrosecond, curTT->baseTime);//�����ϴ�ִ��ʱ���us��
		if(curTT->whileCount != 0){//��ǰ�����ڵȴ���
			if(curTT->whileCount <=lapse)
				curTT->whileCount = 0;
		}else if(lapse >= curTT->interval){ //ʱ�����ڵ�����������ִ��
			if(HasRun == 0){
				TimeTaskInfo.LastBeginTime = beginMicrosecond;//��¼��������ʼ���е�ʱ��
				HasRun = 1;	//�������п�ִ������
		  }
			if(curTT->taskmode == Real_Time_Mode)//ʵʱģʽ
				curTT->baseTime = beginMicrosecond;//���λ�׼ʱ�����ʵʱʱ��
			else if(curTT->taskmode == Count_Mode)	//����ģʽ
				curTT->baseTime = curTT->willTime;//���λ�׼ʱ������ϴ�Ԥ��ʱ��
			
			
			curTT->lastBeginTime = microsecond;//��¼��������֮ǰ��ʱ��
			if(curTT->task_handle != NULL) curTT->task_handle(); 
			curTT->lastEndTime = microsecond;//��¼�������к��ʱ��
			curTT->runTime = curTT->lastEndTime - curTT->lastBeginTime;//��������ʱ��
			
			//curTT->timeOutCount += microsecond - curTT->baseTime;
			curTT->willTime = ((curTT->baseTime + curTT->interval) % USMAX);//���ݻ�׼ʱ��˳�Ӻ�����Ԥ��ʱ��
			curTT->timeOutCount += ((microsecond > curTT->willTime)?1:0);//�����ǰ��ʱ���Ѿ�Ӱ�����´����е�ʱ����ʱ
 
			curTT->runCount ++;	//���д����ۼ�
		}
//		lastTT = curTT;
		curTT = curTT->next;
		
	}
	if(HasRun){
		TimeTaskInfo.LastEndTime = microsecond;//��¼�������������е�ʱ��
		TimeTaskInfo.RunTime = Elapse_Us(TimeTaskInfo.LastEndTime, TimeTaskInfo.LastBeginTime);
		TimeTaskInfo.TimeQuantum += TimeTaskInfo.RunTime;
		HasRun = 0;
	}
/*	if(TimeTaskFlag == 0){//û�е�ʱ��������
		curTT = _timeTask_head;
		//�жϿ��к���Ҫִ�е�ʱ��+��ǰʱ�� �Ƿ�С���´ζ�ʱ�����ʱ��
		if(!(lastTT->task_handle != NULL && (microsecond + curTT->runTime)  < lastTT->willTime))
			return;
		curTT->lastBeginTime = microsecond;//��¼��������֮ǰ��ʱ��
		if(curTT->task_handle != NULL) curTT->task_handle(); 
		curTT->lastEndTime = microsecond;//��¼�������к��ʱ��
		
		curTT->runTime = (curTT->lastEndTime - curTT->lastBeginTime);
		curTT->runCount ++;
	}*/
//	TimeTaskFlag = 0;//����ͳ�Ƽ�ʱ����
}

/****************************************************
	������:	TimeTask_Monitor
	����:		���ؼ���(��ӡ�������е������״̬�͸������)
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int32_t last_microsecond = 0;
void TimeTask_Monitor(void){
	TimeTask_t* curTT = _timeTask_head;
	while(curTT != NULL){
		printf("���:%d\t���:%d\t����ʱ��:%d\t���д���:%d\t��ʱ����:%d\t\r\n",
		curTT->alias,
		curTT->interval,
		(curTT->runTime),
		curTT->runCount,
		curTT->timeOutCount
		);
		
		curTT->runCount = curTT->timeOutCount = 0;
		curTT = curTT->next;
		
	}
	
	last_microsecond = Elapse_Us(microsecond, last_microsecond);
	last_microsecond = TimeTaskInfo.TimeQuantum * 100 / last_microsecond;
	printf("��ǰʱ�䣺%dus��CPUʹ���ʣ�%d%%,����������%d \r\n\r\n\r\n\r\n\r\n",microsecond,last_microsecond,TimeTaskInfo.Task_Size);
	last_microsecond = microsecond;
	TimeTaskInfo.TimeQuantum = 0;
				
}
















































