#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART1_REC_LEN 			800
#define USART_REC_LEN  			250  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���
extern u16 USART2_RX_STA;         		//����״̬���
extern char USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
extern char USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern unsigned char USART3_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.

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
void CLR_Buf1(void);     //�������1���ջ���

void Usart2_Init(u32); 
void USART2TxChar(int); 
void USART2TxData(unsigned char *pt);
void USART2TxData_hex(unsigned char *pt,unsigned char len);
void USART2TxStr(char *pt);
void CLR_Buf2(void);     //�������2���ջ���

void Usart3_Init(u32 bound);//����3��׼�ӿڳ�ʼ��
void Usart3_Remap_Init(u32 bound);//����3��ӳ��ӿڳ�ʼ�� 
static void USART3_GPIO_Configuration(void); 
static void USART3_Configuration(u32 bound); 
static void USART3_NVIC_Configuration(void); 
void USART3TxChar(int ch); 
void USART3TxData(unsigned char *pt);
void USART3TxData_hex(unsigned char *pt,unsigned char len);
void USART3TxStr(char *pt);
void CLR_Buf3(void);     //�������3���ջ���

#endif


