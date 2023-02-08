#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART1_REC_LEN 			800
#define USART_REC_LEN  			250  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记
extern u16 USART2_RX_STA;         		//接收状态标记
extern char USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
extern char USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern unsigned char USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.

extern unsigned char Flag_Usart1_Receive;
extern unsigned char Flag_Usart2_Receive;
extern unsigned char Flag_Usart3_Receive;
extern unsigned char Flag_usart1_receive_OK;
extern unsigned char Count_Timer3_value_USART3_receive_timeout;
extern unsigned char Count_Timer3_value_USART3_receive_timeout2;

void Usart1_Init(u32 bound);
void Usart1_Remap_Init(u32 bound);
void USART1TxChar(char ch);
void USART1TxStr(char *pt);
void CLR_Buf1(void);     //清除串口1接收缓存

void Usart2_Init(u32); 
void USART2TxChar(int); 
void USART2TxData(unsigned char *pt);
void USART2TxData_hex(unsigned char *pt,unsigned char len);
void USART2TxStr(char *pt);
void CLR_Buf2(void);     //清除串口2接收缓存

void Usart3_Init(u32 bound);//串口3标准接口初始化
void Usart3_Remap_Init(u32 bound);//串口3重映射接口初始化 
static void USART3_GPIO_Configuration(void); 
static void USART3_Configuration(u32 bound); 
static void USART3_NVIC_Configuration(void); 
void USART3TxChar(int ch); 
void USART3TxData(unsigned char *pt);
void USART3TxData_hex(unsigned char *pt,unsigned char len);
void USART3TxStr(char *pt);
void CLR_Buf3(void);     //清除串口3接收缓存

#endif


