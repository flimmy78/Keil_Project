/*******************************************************************************
*  Copyright (C) 2010 - All Rights Reserved
*		
* �������:	��ɼ���
* ��Ȩ����: ��ɵ���	
* ��������:	2012��8��18�� 
* �����ʷ:	2012��8��18���װ�
* Version:  1.0 
* Demo �Ա���ַ��http://store.taobao.com/shop/view_shop.htm?asker=wangwang&shop_nick=qifeidianzi
**********************************************************************************************************************************************
����������������������������������������������������������������������������������������������������������������������������������������������
����������������������һ������������������������������������������������������������һ����������������������������һһһһһһһ��������������
������һһһһ��һһһһһһ������������һһһһһһһһ�������������������ą�����һһ������һ������������������һһһһ����һһһ������������
������������һ������һ��һ������������һһһһ�ą���һ����һһ������������һһһһһһһһһһһ������������������������һһһһ��������������
�������ą���һ��������������������������������������һ��һһ��������������һһ����һһ������һһ������������������������һһ������������������
������һ����һһһһһһһһһһһ������������������һһ������������������һһһһһһһһһһ������������һһһһһһһһһһһһһһһ������
������һһһһһ����һһ��һ����������������������һһһһһ��������������һһ����һһ������һ������������һһ����������һһ������һһ��������
��������������һ��һ��һ��һ������������������������һ����һһ������������һһһһһһһһһһ��������������������������һһ������������������
����һһһ��������һһһ��һ������������������������һһ����������������������������һ��������������һ������������������һһ������������������
������������һ����������һһ��������������������������һһһһһһ������������������һһһһһһһһһ������������������һһ������������������
��������һһһ������һһһһ����������������������������һһһһһ��������������������һһһһһһһ��������������һһһһ��������������������
����������������������������������������������������������������������������������������������������������������������������������������������
**********************************************************************************************************************************************/

#include "TFT.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"
#include "delay.h"
#include "stdio.h"	 
					 
//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 

void LCD_WR_DATA(u16 inputdata)
{

	LCD_RS=1;
	LCD_RD=1;
	LCD_CS=0;  
	LCD_WR=0;
	GPIOB->ODR=(GPIOB->ODR|0Xff00)&(inputdata);
	LCD_WR=1;  
	LCD_WR=0;
	GPIOB->ODR=(GPIOB->ODR|0Xff00)& (inputdata<<8);
	LCD_WR=1;		
	LCD_CS=1;
} 
//д�Ĵ�������
void LCD_WR_REG(u16 data)
{ 
	LCD_RS=0;//д��ַ
	LCD_RD=1;  
 	LCD_CS=0;  	
	LCD_WR=0;
	GPIOB->ODR=(GPIOB->ODR|0Xff00)& (data);  
	LCD_WR=1;  
	LCD_WR=0; 
	GPIOB->ODR=(GPIOB->ODR|0Xff00)&(data)<<8;
	LCD_WR=1; 
 	LCD_CS=1;   
} 	


//д�Ĵ���
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
//���Ĵ���
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	u16 t1,t2,t;
	LCD_WR_REG(LCD_Reg);   
	GPIOB->CRL=0X88888888;
	GPIOB->CRH=0X88888888; 
	GPIOB->ODR=0XFFFF;    //ȫ�������

	LCD_RS=1;
	LCD_CS=0;

	LCD_RD=0;		   
	LCD_RD=1;  
	t1=(0xff00&GPIOB->IDR);	 	 

	LCD_RD=0;				   
	LCD_RD=1;
	t2=(0xff00&GPIOB->IDR);   
	t=(t2>>8)|t1; 
	LCD_CS=1; 	  
	GPIOB->CRL=0X33333333; 
	GPIOB->CRH=0X33333333; 
	GPIOB->ODR=0XFFFF;    //ȫ�������
	return t;  
}   

void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(R34);
}	 

void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);
}

//��ʼ��lcd		
void LCD_Init(void)
{ 
 	RCC->APB2ENR|=1<<3;
 	RCC->APB2ENR|=1<<4;

	RCC->APB2ENR|=1<<0;    	 
 	
	GPIOB->CRH=0X33333333;
	GPIOB->CRL=0X33333333; 	 
	GPIOB->ODR=0XFFFF;

	GPIOC->CRH=0X33333333;
	GPIOC->CRL=0X33333333; 	 
	GPIOC->ODR=0XFFFF;
	
	LCD_RST=0;
	delay_ms(1000); 
	LCD_RST=1; 					 
	delay_ms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	DeviceCode = LCD_ReadReg(0x0000);   
	printf(" LCD ID:%x\n",DeviceCode);  
	LCD_WriteReg(0x00e7,0x0010);      
    LCD_WriteReg(0x0000,0x0001);
    LCD_WriteReg(0x0001,0x0100);     
    LCD_WriteReg(0x0002,0x0700); 
    LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );   
    LCD_WriteReg(0x0004,0x0000);                                   
    LCD_WriteReg(0x0008,0x0207);	           
    LCD_WriteReg(0x0009,0x0000);         
    LCD_WriteReg(0x000a,0x0000);        
    LCD_WriteReg(0x000c,0x0001);         
    LCD_WriteReg(0x000d,0x0000);         
    LCD_WriteReg(0x000f,0x0000);

    LCD_WriteReg(0x0010,0x0000);   
    LCD_WriteReg(0x0011,0x0007);
    LCD_WriteReg(0x0012,0x0000);                                                                 
    LCD_WriteReg(0x0013,0x0000);                 
    delay_ms(50); 
    LCD_WriteReg(0x0010,0x1590);   
    LCD_WriteReg(0x0011,0x0227);
    delay_ms(50); 
    LCD_WriteReg(0x0012,0x009c);                  
    delay_ms(50); 
    LCD_WriteReg(0x0013,0x1900);   
    LCD_WriteReg(0x0029,0x0023);
    LCD_WriteReg(0x002b,0x000e);
    delay_ms(50); 
    LCD_WriteReg(0x0020,0x0000);                                                            
    LCD_WriteReg(0x0021,0x013f);           
	delay_ms(50); 

    LCD_WriteReg(0x0030,0x0007); 
    LCD_WriteReg(0x0031,0x0707);   
    LCD_WriteReg(0x0032,0x0006);
    LCD_WriteReg(0x0035,0x0704);
    LCD_WriteReg(0x0036,0x1f04); 
    LCD_WriteReg(0x0037,0x0004);
    LCD_WriteReg(0x0038,0x0000);        
    LCD_WriteReg(0x0039,0x0706);     
    LCD_WriteReg(0x003c,0x0701);
    LCD_WriteReg(0x003d,0x000f);
    delay_ms(50); 
    LCD_WriteReg(0x0050,0x0000);
    LCD_WriteReg(0x0051,0x00ef);                   
    LCD_WriteReg(0x0052,0x0000);                    
    LCD_WriteReg(0x0053,0x013f);   
    
    LCD_WriteReg(0x0060,0xa700);        
    LCD_WriteReg(0x0061,0x0001); 
    LCD_WriteReg(0x006a,0x0000);
    LCD_WriteReg(0x0080,0x0000);
    LCD_WriteReg(0x0081,0x0000);
    LCD_WriteReg(0x0082,0x0000);
    LCD_WriteReg(0x0083,0x0000);
    LCD_WriteReg(0x0084,0x0000);
    LCD_WriteReg(0x0085,0x0000);
  
    LCD_WriteReg(0x0090,0x0010);     
    LCD_WriteReg(0x0092,0x0000);  
    LCD_WriteReg(0x0093,0x0003);
    LCD_WriteReg(0x0095,0x0110);
    LCD_WriteReg(0x0097,0x0000);        
    LCD_WriteReg(0x0098,0x0000);  
    //������ʾ����    
    LCD_WriteReg(0x0007,0x0133);   
    LCD_WriteReg(0x0020,0x0000);                                                            
    LCD_WriteReg(0x0021,0x013f);   
	LCD_Clear(BLACK);
}  		  


u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);
}		 

u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 t;	
	if(x>=LCD_W||y>=LCD_H)return 0;//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);
	LCD_WR_REG(R34);       //ѡ��GRAM��ַ 
	GPIOB->CRL=0X88888888; 
	GPIOB->CRH=0X88888888;
	GPIOB->ODR=0XFFFF;     //ȫ�������

	LCD_RS=1;
	LCD_CS=0;

	LCD_RD=0;					   
 	LCD_RD=1;
 	//dummy READ
	LCD_RD=0;					   
 	LCD_RD=1;
	t=DATAIN;  
	LCD_CS=1;  

	GPIOB->CRL=0X33333333; 
	GPIOB->CRH=0X33333333; 
	GPIOB->ODR=0XFFFF;    //ȫ�������  
	if(DeviceCode==0X4531||DeviceCode==0X8989)return t;
	else return LCD_BGR2RGB(t);
}

void LCD_DisplayOn(void)
{					   
	LCD_WriteReg(R7, 0x0173); //26��ɫ��ʾ����
}	 

void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x0);//�ر���ʾ 
}   

__inline void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
#ifdef USE_HORIZONTAL
	if(DeviceCode==0X8989)
	{
		LCD_WriteReg(0X4E, Ypos);
		LCD_WriteReg(0X4F, 319-Xpos);
    }else
	{
		LCD_WriteReg(R32, Ypos);
		LCD_WriteReg(R33, 319-Xpos);
	}							   
#else
	if(DeviceCode==0X8989)
	{
		LCD_WriteReg(0X4E, Xpos);
		LCD_WriteReg(0X4F, Ypos);
    }else
	{
		LCD_WriteReg(R32, Xpos);  	 
		LCD_WriteReg(R33, Ypos);
	}						    
#endif
}  

void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WR_REG(R34);//��ʼд��GRAM
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 

 
void LCD_Clear(u16 Color)
{
	u32 index=0;	        
	LCD_SetCursor(0x00,0x0000);//���ù��λ�� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);   	  
	}
}  

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
#ifdef USE_HORIZONTAL
	xlen=yend-ysta+1;	   
	for(i=xsta;i<=xend;i++)
	{
	 	LCD_SetCursor(i,ysta);      //���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);//���ù��λ�� 	  
	}
#else
	xlen=xend-xsta+1;	   
	for(i=ysta;i<=yend;i++)
	{
	 	LCD_SetCursor(xsta,i);      //���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	}
#endif						  	    
}  

//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�
//x:0~234
//y:0~308
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  
#ifdef USE_HORIZONTAL
#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 232 
#else     
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 312
#endif 
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ2������
//x,y:�������
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;}
        LCD_ShowChar(x,y,*p,16,0);
        x+=8;
        p++;
    }  
}
