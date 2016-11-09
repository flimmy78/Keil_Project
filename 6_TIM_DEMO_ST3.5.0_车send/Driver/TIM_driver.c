#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
/**********************************************************
** ������: TIM2_Config
** ��������:  ������ʱ������
** �������: ��
** �������: ��
** ˵������ʱʱ��=(Ԥ��Ƶ��+1��*������ֵ+1)	/TIM6ʱ��(72MHz)����λ(s)
   �������ʱ��t=(7200*10000)/72000000s=1s
** IO�ڣ�PB10
***********************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO�ṹ��	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//��PB0\1\2\3����Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*��������72000000/Ԥ���д���(1000)/������=Ԥ��Ƶ*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 10000;//����ֵ10000   
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ��� 65535����
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//��ʼֵ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	
	
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//��ʱ������ΪPWM1ģʽ
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM����Ƚϼ��Ը�
 	//PWMģʽ���� ��ͨ������TIM2_CH2
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	             
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	   //��������<1000ʱ,PWMΪ�ߵ�ƽ
		TIM_OCInitStructure.TIM_Pulse = 2000;					      				 //������������1000ʱ,��ƽ��������
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);
		TIM_SetCompare1(TIM2, 10000);
		TIM_SetCompare2(TIM2, 10000);
		TIM_SetCompare3(TIM2, 10000);
		TIM_SetCompare4(TIM2, 10000);
		
		
		//TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIMx��CCRx�ϵ�Ԥװ�ؼĴ���
		//TIM_ARRPreloadConfig(TIM2, ENABLE);//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	//���ж�
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
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
	
	/*��������72000000/Ԥ���д���(1000)/������=Ԥ��Ƶ*/
	TIM_DeInit(TIM6);
	TIM_TimeBaseStructure.TIM_Period = 100;//����ֵ10000   
	TIM_TimeBaseStructure.TIM_Prescaler = 2400-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ��� 65535����
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //��ʼֵ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	//���ж�
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
}


