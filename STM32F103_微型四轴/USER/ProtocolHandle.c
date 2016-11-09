#include "ProtocolHandle.h"

#include "STM32_DevInit.h"
u16 cutcount = 0; 
void HeartBeat_P_Handle(PROTOCOL_INFO_T* pi){
	HEARTBEAT_PROTOCOL_T hp = pi->protocol.HeartBeat_P;
	HeartBeat = 2; 
	//printf("�յ�����:%d,%d,%X,%X\r\n", hp.para1,pi->checksum, hp.para1,pi->checksum);
	//printf("�յ�����:%d\r\n", hp.para1);
}

void Run_P_Handle(PROTOCOL_INFO_T* pi){
	RUN_PROTOCOL_T rp = pi->protocol.Run_P;
	motor_UL->Pwm_Base = motor_UR->Pwm_Base = motor_DL->Pwm_Base = motor_DR->Pwm_Base = rp.para2<<8|rp.para3;
	printf("�յ��˶�ָ��\r\n");
}

void Rotate_P_Handle(PROTOCOL_INFO_T* pi){
	ROTATE_PROTOCOL_T rop = pi->protocol.Rotate_P;
	printf("�յ�ƫ����ָ��\r\n");
	//NRF24L01_Init(nrf1);
	printf("%d\r\n",nrf1->status);
//	nrf1->CE_Reset(nrf1);   //CE�ø�,�������ģʽ
//	nrf1->RF_Write_Reg(nrf1,FLUSH_TX,0xff);//���TX FIFO�Ĵ���
//	nrf1->RF_Write_Reg(nrf1,FLUSH_RX,0xff);//���RX FIFO�Ĵ���
//	nrf1->CE_Set(nrf1);   //CE�ø�,�������ģʽ
}

void StateReport(void){	
	PROTOCOL_T pt = {0};
	if(HeartBeat-- == 0){
		NRF24L01_Init(nrf1); 
		printf("����ֹͣ:%d\r\n",cutcount++);
	}
	
	pt.State_P.para1 = MPU6050->X / 100;//������	��127
	pt.State_P.para2 = MPU6050->Y / 100;//������	��127
	pt.State_P.para3 = (MPU6050->Z / 100) >> 8;//ƫ���Ǹ�8λ 0-65535
	pt.State_P.para4 = (MPU6050->Z / 100)& 0xff;//ƫ���ǵ�8λ
	pt.State_P.para5 = 0;//�߶ȸ�8λ ��λcm
	pt.State_P.para6 = 0;//�߶ȵ�8λ	��λcm
	pt.State_P.para7 = 0;//�Ƿ�������̬����	0�� 1��
	pt.State_P.para8 = 0;//�Ƿ�����ϵ�	0�� 1��
	pt.State_P.para9 = 0;//�������
	pt.State_P.para10 = 0;//���� 0-100
	pt.State_P.para11 = (u8)MPU6050->T; 
	//Protocol_Send(STATE_PROTOCOL, pt, sizeof(STATE_PROTOCOL_T));
}
