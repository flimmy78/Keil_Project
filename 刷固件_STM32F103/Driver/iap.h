#define GRAM_SIZE 8456
#include "tool.h"
#define PAGE_COUNT GRAM_SIZE/2048+1
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.
#define  FLASH_APP_ADDR		0x08006000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)

void iap_load_app(u32 appxaddr);

