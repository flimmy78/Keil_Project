#ifndef __24L01__H__INCLUDED__
#define __24L01__H__INCLUDED__
#include "stm32f10x.h"
/*NRF24L01说明

*/
//NRF24L01寄存器操作命令
#define NRF_READ_REG 	0x00 //读配置寄存器,低5位为寄存器地址
#define NRF_WRITE_REG 	0x20 //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD 0x61 //读RX有效数据,1~32字节
#define WR_TX_PLOAD 0xA0 //写TX有效数据,1~32字节
#define FLUSH_TX 	0xE1 //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX 	0xE2 //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL 0xE3 //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP 		0xFF //空操作,可以用来读状态寄存器
//********************************************************************************************************************// 
//SPI(NRF24L01)寄存器地址
#define CONFIG 		0x00 //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
						 //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA 		0x01 //使能自动应答功能 bit0~5,对应通道0~5
#define EN_RXADDR 	0x02 //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW 	0x03 //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR 	0x04 //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH 		0x05 //RF通道,bit6:0,工作通道频率;
#define RF_SETUP 	0x06 //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS 		0x07 //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
						 //bit5:数据发送完成中断;bit6:接收数据中断;
#define OBSERVE_TX 	0x08 //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD 			0x09 //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0 	0x0A //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1 	0x0B //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2 	0x0C //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3 	0x0D //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4 	0x0E //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5 	0x0F //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR 	0x10 //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0 	0x11 //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1 	0x12 //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2 	0x13 //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3 	0x14 //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4 	0x15 //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5 	0x16 //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define FIFO_STATUS 0x17 //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
						 //bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;
#define MAX_TX 		0x10 //达到最大发送次数中断
#define TX_OK 		0x20 //TX发送完成中断
#define RX_OK 		0x40 //接收到数据中断
//定义接收通道
#define Ch0	0x00
#define Ch1	0x02
#define Ch2	0x04
#define Ch3	0x06
#define Ch4	0x08
#define Ch5	0x0a

#define ADR_WIDTH    5 
#define TX_PLOAD_WIDTH  32 //数据缓冲区大小
#define RX_PLOAD_WIDTH  32 //数据缓冲区大小
//SPI接口定义
//CE收发方向--PD12
//CSN片选--PB12
//IRQ--PA3
/*#define CE_H  GPIOC->BSRR=GPIO_Pin_5   //PD5置1
#define CE_L  GPIOC->BRR=GPIO_Pin_5   	//PD5置0
#define CSN_H GPIOC->BSRR=GPIO_Pin_6	//PB6置1
#define CSN_L GPIOC->BRR=GPIO_Pin_6	//PB6置0*/

typedef struct _NRF24L01_T NRF24L01_T;
struct _NRF24L01_T{
	GPIO_TypeDef* CE_GPIO;
	uint16_t CE_GPIO_Pin;
	GPIO_TypeDef* IRQ_GPIO;
	uint16_t IRQ_GPIO_Pin;
	GPIO_TypeDef* CSN_GPIO;
	uint16_t CSN_GPIO_Pin;
	SPI_TypeDef* SPI;
	
	void(*CE_Set)(NRF24L01_T* nrf);
	void(*CE_Reset)(NRF24L01_T* nrf);
	void(*CSN_Set)(NRF24L01_T* nrf);
	void(*CSN_Reset)(NRF24L01_T* nrf);
	u8(*IRQ_Get)(NRF24L01_T* nrf);
	
	void (*RF_Write_Reg)(NRF24L01_T* nrf,u8 reg, u8 value);
	u8 (*RF_Write_Buf)(NRF24L01_T* nrf,u8 reg, u8 *pBuf,u8 bytes);
	u8 (*RF_Read_Reg)(NRF24L01_T* nrf,u8 reg);
	u8 (*RF_Read_Buf)(NRF24L01_T* nrf,u8 reg, u8 *pBuf, u8 bytes);
	u8 (*nRF24L01_TxPacket)(NRF24L01_T* nrf,u8 *tx_buf);
	u8 (*nRF24L01_RxPacket)(NRF24L01_T* nrf,u8 *rx_buf);
	void (*Set_TX_Mode)(NRF24L01_T* nrf);
	void (*Set_RX_Mode)(NRF24L01_T* nrf);
};

//void RF_SPI_Init(void);
//void Receive(void);
void NRF24L01_Init(NRF24L01_T* nrf);
//void RX_Mode(u8 channel);
//u8 SPIx_ReadWriteByte(u8 data);
//void  RF_Write_Reg(u8 reg, u8 value);
//u8 RF_Read_Reg(u8 reg);
// u8 RF_Read_Buf(u8 reg, u8 *pBuf, u8 bytes);
// u8 RF_Write_Buf(u8 reg, u8 *pBuf,u8 bytes);
// u8 nRF24L01_RxPacket(u8 *rx_buf);
// u8 nRF24L01_TxPacket(u8 *tx_buf);
extern NRF24L01_T* nrf1;
#endif
