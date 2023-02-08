#include "stm32f10x.h"
#include "exti.h"
#include "sys.h"
#include "delay.h"
#include "timer.h"
unsigned char KEY_value = 0;
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTAʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
}
void EXTIX_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;	
  KEY_Init();	 //	�����˿ڳ�ʼ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	//�ж����Լ��жϳ�ʼ������ �½��ش���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

//GPIOA.1	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY_set
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);
 
//GPIOA.4	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY_set
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_Init(&EXTI_InitStructure);

//GPIOA.5	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY_left
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//GPIOA.6	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY_down
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIOA.7	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY_right
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ���KEY_up���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY_set���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//ʹ�ܰ���KEY_left,KEY_right,KEY_down���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�Ϊ���� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI1_IRQHandler(void)//�ⲿ�ж�1�������
{
	delay_ms(20);//����
	if(KEY_up == 0)
	{
		Buzzer_ON();
		KEY_value = up;
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE1�ϵ��жϱ�־λ
	delay_ms(2);
	Buzzer_OFF();
}
void EXTI4_IRQHandler(void)//�ⲿ�ж�4�������
{
	delay_ms(20);//����
	if(KEY_set == 0)
	{
		Buzzer_ON();
		KEY_value = set;
	}
	EXTI_ClearITPendingBit(EXTI_Line4); //���LINE4�ϵ��жϱ�־λ
	delay_ms(2);
	Buzzer_OFF();
}
void EXTI9_5_IRQHandler(void)//�ⲿ�ж�5��6��7������� 
{	
	delay_ms(20);//����	
	if(KEY_down == 0)	 	 //KEY_down����  PA6�˿�
	{				 
    Buzzer_ON();
		KEY_value = down;			
	}
	if(KEY_left == 0)	 	 //KEY_lift����  PA5�˿�
	{				 
    Buzzer_ON();
		KEY_value = left;	
	}
	if(KEY_right == 0)	 	 //KEY_right���� PA7�˿�
	{				 
		Buzzer_ON();
		KEY_value = right;		
	}
	EXTI_ClearITPendingBit(EXTI_Line6); //���LINE6�ϵ��жϱ�־λ
	EXTI_ClearITPendingBit(EXTI_Line5); //���LINE5�ϵ��жϱ�־λ
	EXTI_ClearITPendingBit(EXTI_Line7); //���LINE7�ϵ��жϱ�־λ
	delay_ms(2);
	Buzzer_OFF();	
}

