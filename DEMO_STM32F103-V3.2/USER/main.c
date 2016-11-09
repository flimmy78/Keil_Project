/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/   
#include "STM32_DevInit.h" 


u8 taskid = 0; 
u8 led_flag = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOA, GPIO_Pin_12 | GPIO_Pin_15,(BitAction)led_flag); 
}

void test_uartsend(void){ //����DMA��ͨ  ׼�����ڷ�����
	int i = 1024;
//	for(i = 0; i < 1024; i++){
//		USART_SendData_Block(USART1, Uart1_DMA_Receiver.Uartx_DMA_Recv_Buff[i]);
//	}
	uint8_t data[100] = {0};
	int16_t cnt = 0;
	if((cnt = Uart1_DMA_Receiver.ReadTo(&Uart1_DMA_Receiver,0xf8,data,100))>0){
		Uart1_DMA_Sender.Write(&Uart1_DMA_Sender,data,cnt);
		//if(cnt>1)printf("1");
//		DMA1_Channel4->CMAR = (u32)data;
//		DMA1_Channel4->CNDTR = cnt;
//		DMA_Cmd(DMA1_Channel4, ENABLE);
//		for(i = 0;i<cnt;i++)
//		USART_SendData_Block(USART1, data[i]);
	}
	//printf("area:%d\r\n",DMA_GetCurrDataCounter(Uart1_DMA_Receiver.DMAx));
}
 /*-------------------------------------------------------------------------------------------------------
|  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
 
int main(void)
{	  
	TaskTime_Init();		//��ʼ���������
	Protocol_Init();		//��ʼ��Э��ջ
	ComBuff_Init();			//��ʼ��ͨ�Ż�����
	Log_Init();					//��ʼ����־���
	Driver_MCU_Init();	//��ʼ��MCU
	BaseClock_Init();		//��ʼ��ʱ��
	printf("main done\r\n"); 
	
	//--------------LED-------------------------
	TaskTime_Add(TaskID++, TimeCycle(0,500), LED_TEST, Count_Mode);
	//------------���ڷ���----------------------
	TaskTime_Add(TaskID++, TimeCycle(0,1), Buff_To_Uart, Count_Mode);
	//------------Э�鴦��----------------------
	TaskTime_Add(TaskID++, TimeCycle(0,30), FetchProtocols, Count_Mode); 
	
	TaskTime_Add(TaskID++, TimeCycle(0,1), test_uartsend, Count_Mode); 
	while(1){ 
		TaskTime_Run();
	}
}





