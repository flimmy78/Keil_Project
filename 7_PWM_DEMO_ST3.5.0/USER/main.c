/*******************************************************************************
// ������ MPU6050 IIC���Գ���
*******************************************************************************/

#include "Prohead.h"
#include "Driver.h"
#include "stm32f10x.h"
#include <Type.h>  
#include <mpu6050_driver.h>
#include <System_Init.h> 


	
/* �������� -----------------------------------------------*/
void Delay(u32 nTime);
void Delayms(vu32 m);  


/*
********************************************************************************
** �������� �� Delay(vu32 nCount)
** �������� �� ��ʱ����
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
 void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
/*
********************************************************************************
** �������� �� void Delayms(vu32 m)
** �������� �� ����ʱ����	 m=1,��ʱ1ms
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
 void Delayms(vu32 m)
{
  u32 i;
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}

 //************************************************


   /*
********************************************************************************
** �������� �� main(void)
** �������� �� ������
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	system_init();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1){
	Delay_MS(1000);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	printf("\nGPIOA_6����");
	Delay_MS(1000);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	printf("\nGPIOA_6�ر�");
	}
	
	
	
	
	
// 	Init_MPU6050();
// 	while(1){
// 		Build_Data();
		Delayms(1);				 //��ʱ
//  }
	while(1);
}

/*************����***************/

