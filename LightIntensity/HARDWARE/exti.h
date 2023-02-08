#ifndef __EXTI_H
#define __EXTI_H	 
#include "sys.h"
#define KEY_set   			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ��������
#define KEY_up  				GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ������
#define KEY_down  			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ������
#define KEY_left  			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ������ 
#define KEY_right  			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//��ȡ������
#define Buzzer_ON() 	GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define Buzzer_OFF() 	GPIO_SetBits(GPIOB,GPIO_Pin_9)
extern unsigned char KEY_value;
enum KEY_state
{
	key_free,
	up,
	down,
	left,
	right,
	set,
};
void EXTIX_Init(void);//�ⲿ�жϳ�ʼ��		 					    
#endif

