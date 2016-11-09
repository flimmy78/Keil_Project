/**
  ******************************************************************************
  * @file    main.c 
  * @author  ��Ң
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 ���հ汾
  ******************************************************************************
*/ 


/* Include---------------------------------------------------------------*/

#include "stm32f0xx.h"
#include "tool.h"
#include "protocol.h" 
#include "stm32f0xx_wwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//------------ι��------------------
void FeedDog(void){
	WWDG_SetCounter(127); 
}
/* Private function prototypes -----------------------------------------------*/

extern void STM32_DevInit(void);
uint8_t led_flag = 0;
void test(void){
	led_flag = ~ led_flag;
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,(BitAction)led_flag);
}

extern void WDG_Configuration(void); 
int main(void)
{
//�������ʼ��
	Protocol_Init();		//��ʼ��Э��ջ
	TimeTask_Init();		//��ʼ���������
	STM32_DevInit();
	BaseClock_Init();
	
	printf("main done\r\n");	
	//WWDG_Enable(127);//UpLoadState
	
	//------------���ڷ���----------------------
	//TimeTask_Add(1,Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------ι��------------------------
	//TimeTask_Add(2,Create_SysTickStamp(0, 25, 0), FeedDog,Real_Time_Mode);
	
	//------------���ؼ���----------------------
	//TimeTask_Add(3,Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	TimeTask_Add(4,Create_SysTickStamp(0, 500, 0), test,Real_Time_Mode);
						
	while(1){
		TimeTask_Run();//030   30us
		
	}
	
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

