//###########################################################################
//    ������λ��������ָ��
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "protocol.h"
#include "tool.h"
#include <string.h>

void DealUsartCMD(void);
void UsartCMDManualInt(PROTOCOL_INFO_T* protocol_info);	//�ֶ����Ƴ�ʼ��
void UsartCMDManual(void);		//�ֶ����Ƶ��
//###################################Э��У�麯����###################################
/****************************************************
	������:	run_protocol_check
	����:	 	�������Э��У��
	����:		RUN_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ��-1�����ַ���������� -2�����ַ���������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t run_protocol_check(void* protocol){
	/*RUN_PROTOCOL_T* rp = protocol;
	if(!(rp->para1 >=0 && rp->para1 <=2)){//����У��
		#ifdef PRINT_ERR 
			printf("���ַ����������%d\n",rp->para1); 
		#endif
		return -1;
	}
	if(!(rp->para4 >=0 && rp->para4 <=2)){//����У��
		#ifdef PRINT_ERR 
			printf("���ַ����������%d\n",rp->para4); 
		#endif
		return -2;
	}
	if(!(rp->para8 >= 0 && rp->para8 <=1)){//�Ƿ���ָ��У��
		#ifdef PRINT_ERR 
			printf("�Ƿ���ָ���������%d\n",rp->para8); 
		#endif
		return -3;
	}
	#ifdef PRINT_ERR 
		printf("\n\n\n���ַ���%d �ٶȣ�%d\n���ַ���%d �ٶȣ�%d\n��ţ�%d���Ƿ���ָ�%d\n",
		rp->para1, rp->para2 << 8 | rp->para3,
		rp->para4, rp->para5 << 8 | rp->para6,
		rp->para7,rp->para8); 
	#endif*/
	return 0;
}
/****************************************************
	������:	angle_protocol_check
	����:		˫�ֽǶȿ��Ƶ��Э��У��
	����:		ANGLE_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ:	
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t angle_protocol_check(void* protocol){
	/*ANGLE_PROTOCOL_T* ap = protocol;
	if(!(ap->para16 <=1))//�Ƿ���ָ��У��
		return -1;*/
	return 0;
}

/****************************************************
	������:	head_protocol_check
	����:		ͷ�����Э��У��
	����:		HEAD_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ:	-1���Ƕ�У��������� -2���ٶ�У��������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t head_protocol_check(void* protocol){
	/*HEAD_PROTOCOL_T* hp = protocol;
	if(!(hp->para1 <=240))//�Ƕ�У��
		return -1;
	if(!(hp->para3 <=100))//�ٶ�У��
		return -2;
	if(!(hp->para4 <=1))//�Ƿ���ָ��У��
		return -3;*/
	return 0;
}

/****************************************************
	������:	wing_protocol_check
	����:		�����Э��У��
	����:		WING_PROTOCOL_TЭ��ʵ��ָ��
	����ֵ:	-1������ٶȲ������� -2���ҳ��ٶȲ������� -3���Ƿ���ָ���������
	����:		liyao 2015��9��8��14:10:51
****************************************************/
int8_t wing_protocol_check(void* protocol){
	/*WING_PROTOCOL_T* wp = protocol;
	if(!(wp->para2 >=0 && wp->para2 <=100))//����ٶ�У��
		return -1;
	if(!(wp->para4 >=0 && wp->para4 <=100))//�ҳ��ٶ�У��
		return -2;
	if(!(wp->para6 >= 0 && wp->para6 <=1))//�Ƿ���ָ��У��
		return -3;*/
	return 0;
}
/****************************************************
	������:	Run_Protocol_Handle
	����:		˫�ֿ�������Э�麯��
	����:		liyao 2015��10��16��15:21:46
****************************************************/
void Run_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*RUN_PROTOCOL_T* rp = &pi->protocol.run_protocol;
	#ifdef PRINT_ERR 
			printf("�յ��˶�ָ��\r\n");
	#endif
	#ifndef PRINT_ERR 
	ask_send(rp->para7);
	#endif*/

}

/****************************************************
	������:	Wing_Protocol_Handle
	����:		����������Э�麯��
	����:		liyao 2015��10��16��15:21:46
****************************************************/
void Wing_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*WING_PROTOCOL_T* wp = &pi->protocol.wing_protocol;
	#ifndef PRINT_ERR 
	ask_send(wp->para5);
	#endif*/
}

/****************************************************
	������:	Head_Protocol_Handle
	����:		ͷ����������Э�麯��
	����:		liyao 2015��10��16��15:21:46
****************************************************/
void Head_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*HEAD_PROTOCOL_T* hp = &pi->protocol.head_protocol;
	#ifndef PRINT_ERR 
	ask_send(hp->para3);
	#endif*/

}

/****************************************************
	������:	Head_Protocol_Handle
	����:		˫��ָ���Ƕ�����Э�麯��
	����:		liyao 2015��10��16��15:21:46
****************************************************/
void Angle_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*ANGLE_PROTOCOL_T* ap =  &pi->protocol.angle_protocol;
	#ifndef PRINT_ERR 
	ask_send(ap->para16);
	#endif*/

}

/****************************************************
	������:	PDR_Correnct_Protocol_Handle
	����:		��λУ������Э�麯��
	����:		liyao 2015��10��20��15:26:37
****************************************************/
void PDR_Correnct_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*PDRCORRECT_PROTOCOL_T* pcp =  &pi->protocol.PDR_Correnct_Protocol;
	#ifndef PRINT_ERR 
	ask_send(pcp->para14);
	#endif
	*/
}
/*****************************************************************
������:DealUsartCMD
��ע: ������λ�����������ܺ�������������˶�������ģʽ����ȡ��Ϣ��
******************************************************************/
void DealUsartCMD(void)
{
	PROTOCOL_INFO_T pi;
	while(Queue_Get(Recv_Protocol_Queue,&pi) == 0){ 
		heart_flag = 1;
		/*if(pi.handle != NULL){
			if(pi.check(&pi) < 0)
				break;
		}*/
		if(pi.handle != NULL){
			pi.handle(&pi);
		}
	}
}

