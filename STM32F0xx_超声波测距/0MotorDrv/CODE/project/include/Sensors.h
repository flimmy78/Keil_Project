#ifndef SENSORS_H
#define SENSORS_H

extern void SensorsInt(void);			//��������ʼ��
extern void DealSensors(void);
extern void ReadEncoder(void);

extern uint16_t encoder1Last;//�����洢��һ�ֱ�����������
extern uint16_t encoder2Last;//�����洢��һ�ֱ�����������
extern uint16_t encoder3Last;//�����洢��һ�ֱ�����������



#endif  

