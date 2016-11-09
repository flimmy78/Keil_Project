////###########################################################################
////    ������λ��������ָ��
////###########################################################################
//#include "STM32_DevInit.h"

//void DealUsartCMD(void);
////###################################����������ƺ���###################################
///****************************************************
//	������:	Run_Protocol_Check
//	����:	 	�������Э��У��
//	����ֵ��-1�����ַ���������� -2�����ַ���������� -3���Ƿ���ָ���������
//	����:		liyao 2015��12��9��11:54:11
//****************************************************/
//int8_t Run_Protocol_Check(PROTOCOL_INFO_T* pi){
//	RUN_PROTOCOL_T* rp = &pi->protocol.run_protocol;
//	if(!(rp->para1 >=0 && rp->para1 <=3)){//����У��
//		#ifdef PRINT_ERR 
//			printf("���ַ����������%d\r\n",rp->para1); 
//		#endif 
//		return -1;
//	}
//	if(!(rp->para4 >=0 && rp->para4 <=3)){//����У��
//		#ifdef PRINT_ERR 
//			printf("���ַ����������%d\r\n",rp->para4); 
//		#endif
//		return -2;
//	}
//	#ifdef PRINT_ERR 
//		printf("\r\n\r\n\r\n���ַ���%d �ٶȣ�%d\r\n���ַ���%d �ٶȣ�%d\r\n��ţ�%d\r\n",
//		rp->para1, rp->para2 << 8 | rp->para3,
//		rp->para4, rp->para5 << 8 | rp->para6,
//		rp->para7); 
//	#endif
//	return 0;
//}
///****************************************************
//	������:	Run_Protocol_Handle
//	����:		˫�ֿ�������Э�麯��
//	����:		liyao 2015��12��9��11:57:32
//****************************************************/
//void Run_Protocol_Handle(PROTOCOL_INFO_T* pi){
//	RUN_PROTOCOL_T* rp = &pi->protocol.run_protocol;
//	#ifdef PRINT_ERR 
//			printf("\r\nִ���˶�ָ��\r\n");
//	#endif
//	#ifndef PRINT_ERR 
//	ask_send(rp->para7);
//	#endif
//	//�����������˶�
//	motor_L->Exec_Protocol = motor_R->Exec_Protocol = *pi;
//	motor_L->Motor_Run(motor_L, (MOTOR_DIR)rp->para1, rp->para2 << 8 | rp->para3);
//	motor_R->Motor_Run(motor_R, (MOTOR_DIR)rp->para4, rp->para5 << 8 | rp->para6);
//}
////###################################ָ��˫�ֱ�����ƺ���###################################
///****************************************************
//	������:	Custom_Protocol_Check
//	����:		˫�ֿ�������Э�麯��
//	����:		liyao 2015��12��9��11:54:11
//****************************************************/
//int8_t Custom_Protocol_Check(PROTOCOL_INFO_T* pi){
//	CUSTOM_PROTOCOL_T* cp = &pi->protocol.Custom_Protocol;
//	if(!(cp->para1 >=1 && cp->para1 <=3)){//����У��
//		#ifdef PRINT_ERR 
//			printf("���ַ����������%d\r\n",cp->para1); 
//		#endif 
//		return -1;
//	}
//	if(!(cp->para6 >= 1 &&cp->para6 <= 3)){//����У��
//		#ifdef PRINT_ERR 
//			printf("���ַ����������%d\r\n",cp->para4); 
//		#endif
//		return -2;
//	}
//	#ifdef PRINT_ERR 
//		printf("���ַ���%d �ٶȣ�%d ���룺%d \r\n���ַ���%d �ٶȣ�%d ���룺%d \r\n ��ţ�%d\r\n",
//		cp->para1, 
//		cp->para2 << 8 | cp->para3, cp->para4 << 8|cp->para5,
//		cp->para6, 
//		cp->para7 << 8 | cp->para8,cp->para9 << 8|cp->para10,
//		cp->para11); 
//	#endif
//	return 0;
//}
///****************************************************
//	������:	Custom_Protocol_Handle
//	����:		˫�ֿ�������Э�麯��
//	����:		liyao 2015��12��9��11:57:32
//****************************************************/
//void Custom_Protocol_Handle(PROTOCOL_INFO_T* pi){
//	CUSTOM_PROTOCOL_T* cp = &pi->protocol.Custom_Protocol;
//	#ifdef PRINT_ERR 
//			printf("\r\nִ��˫���Զ�����������\r\n"); 
//	#endif
//	#ifndef PRINT_ERR 
//		ask_send(cp->para11);
//	#endif
//	motor_L->Exec_Protocol = motor_R->Exec_Protocol = *pi;
//	motor_L->Motor_Custom(motor_L, (MOTOR_DIR)cp->para1, cp->para2 << 8 | cp->para3, cp->para4 << 8|cp->para5);
//	motor_R->Motor_Custom(motor_R, (MOTOR_DIR)cp->para6, cp->para7 << 8 | cp->para8, cp->para9 << 8|cp->para10);
//}

////###################################������������ƺ���###################################
///****************************************************
//	������:	Rudder_Protocol_Check
//	����:		�����������Э�麯��
//	����:		liyao 2015��12��9��11:54:11
//****************************************************/
//int8_t Rudder_Protocol_Check(PROTOCOL_INFO_T* pi){
//	RUDDER_PROTOCOL_T* rp = &pi->protocol.Rudder_Protocol;
//	if(rp->para1 > 180){//�Ƕȷ�Χ����
//		#ifdef PRINT_ERR 
//			printf("����ǶȲ�������%d\r\n",rp->para1); 
//		#endif 
//		return -1;
//	}
//	#ifdef PRINT_ERR 
//		printf("\r\n����Ƕȣ�%d\r\n", rp->para1); 
//	#endif
//	return 0;
//}
///****************************************************
//	������:	Rudder_Protocol_Handle
//	����:		�����������Э�麯��
//	����:		liyao 2015��12��9��11:57:32
//****************************************************/
//void Rudder_Protocol_Handle(PROTOCOL_INFO_T* pi){
//	RUDDER_PROTOCOL_T* up = &pi->protocol.Rudder_Protocol;
//	#ifdef PRINT_ERR 
//			printf("\r\nִ�ж��ָ��\r\n");
//	#endif
//	#ifndef PRINT_ERR 
//		ask_send(up->para2);
//	#endif
//	motor_L->Exec_Protocol = *pi;
//	RudderX->setRudderAngle(RudderX,up->para1);
//	RudderY->setRudderAngle(RudderY,up->para1);
//}
////###################################�ϱ�Э�鴦��###################################
//void State_Protocol_Send(){
//	PROTOCOL_T pt = {0}; 
//	pt.state_protocol.para1 = motor_L->Dir;
//	pt.state_protocol.para2 = 0;
//	pt.state_protocol.para3 = motor_L->SpeedCMD;
//	pt.state_protocol.para4 = motor_R->Dir;
//	pt.state_protocol.para5 = 0;
//	pt.state_protocol.para6 = motor_R->SpeedCMD;
//	Protocol_Send(STATE_PROTOCOL, pt, sizeof(STATE_PROTOCOL_T)); 
//}

////###################################Э����մ���###################################
///*****************************************************************
//������:DealUsartCMD
//��ע: ������λ�����������ܺ�������������˶�������ģʽ����ȡ��Ϣ��
//******************************************************************/
//void DealUsartCMD(void)
//{
//	PROTOCOL_INFO_T pi;
//	while(Queue_Get(Recv_Protocol_Queue,&pi) == 0){ 
//		if(pi.handle != NULL){
//			if(pi.check(&pi))
//				break;
//		}
//		if(pi.handle != NULL){
//			pi.handle(&pi);
//		}
//	}
//}



