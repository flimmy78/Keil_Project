/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      ���ſƼ� http://euse.taobao.com  --------------------------
 *
 */
#include "Prohead.h"
#include "Driver.h"

//��ʱ
void LedDelay(void)
{
		u32 Timer;
	  Timer = 0x2fffff;
		while(Timer--);
}

/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
main()
{
		//��ʼ��ϵͳʱ��
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}
}





