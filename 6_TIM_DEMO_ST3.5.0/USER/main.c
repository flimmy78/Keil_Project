#include <Prohead.h>
#include <Driver.h>
#include <stm32f10x.h>
#include <Type.h>  
#include <NRF24L01_Driver.h>
#include <mpu6050_driver.h>
#include <System_Init.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>
#include <NRF24L01_Driver_send.h>
/*
********************************************************************************
** �������� �� main(void)
** �������� �� ������
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
�˿�ռ�ã�

��ʱ���� A0|A1|A2|A3
����1��	 A9|A10
ģ��spi��B6|B7
LED��    B10

NRF24L01_SPI2_SCK:	PB13
NRF24L01_SPI2_MISO:	PB14
NRF24L01_SPI2_MOSI:	PB15
NRF24L01_IRQ:				PC4
NRF24L01_CE:				PC5
NRF24L01_CSN:				PC6


2_NRF24L01_SPI1_SCK:	PA5
2_NRF24L01_SPI1_MISO:	PA6
2_NRF24L01_SPI1_MOSI:	PA7
2_NRF24L01_IRQ:				PC7
2_NRF24L01_CE:				PC8
2_NRF24L01_CSN:				PC9


*********************************************************************************/
char* runState = "ͬ��";
int main(void)
{	
	//u8 Tx_Buf[32]={"�װ��� �Ұ��㣡"};
	
	system_init();
	//RX_Mode(0);//���ý���ͨ��0
	RX_Mode(4);
	while(1){
		
		Delay_MS(300);				 //��ʱ
		Build_Data();
// 		if(equals((u8*)"ͬ��",(u8*)runState,4)){
// 			printf("���չ���������");
// 			runState="";
// 		}else{
// 			printf("���չ���ʧ�ܣ�");
// 		}
		//
		//nRF24L01_TxPacket_send(Tx_Buf);
		
		//printf("������ʼ");
		//EXTI_GenerateSWInterrupt(EXTI_Line4);//�������һ���жϣ��ж���4
		//printf("�������");
		//RX_Mode(0);//���ý���ͨ��0
		//RX_Mode(4);//���ý���ͨ��4
	
		//Build_Data();
		//printf("\n�ҵ�main123");
		//printf("%s",temData2);
		//USART1_SendData(direct);
		//printf("2710:%d",0x2710);

	}
}

/*************����***************/

