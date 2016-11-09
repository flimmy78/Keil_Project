#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
/**********************************************************
** ������: TIM6_Config
** ��������:  ������ʱ������
** �������: ��
** �������: ��
** ˵������ʱʱ��=(Ԥ��Ƶ��+1��*������ֵ+1)	/TIM6ʱ��(72MHz)����λ(s)
   �������ʱ��t=(7200*10000)/72000000s=1s
***********************************************************/
void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʹ��TIM6ʱ��

	/*��������*/
	TIM_TimeBaseStructure.TIM_Period = 10000-1;//����ֵ10000   
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
}
/*
* 	TIM6_Config
*******************************************************************************/

