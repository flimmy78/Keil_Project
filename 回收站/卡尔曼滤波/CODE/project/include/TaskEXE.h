#ifndef TASKEXE_H
#define TASKEXE_H

extern TASKS tasks;
extern MOTORS MotorsTaskPauseBuf;

extern void AllTaskReset(void);	//���������ʼ��
extern void NewTaskInt(uint16_t *cmd);	//�ƶ�������
extern void TaskExecute(void);	//����ִ��


#endif  
