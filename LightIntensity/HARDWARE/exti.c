#include "stm32f10x.h"
#include "exti.h"
#include "sys.h"
#include "delay.h"
#include "timer.h"
unsigned char KEY_value = 0;
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
}
void EXTIX_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;	
  KEY_Init();	 //	按键端口初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	
	//中断线以及中断初始化配置 下降沿触发
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

//GPIOA.1	  中断线以及中断初始化配置 下降沿触发 //KEY_set
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);
 
//GPIOA.4	  中断线以及中断初始化配置 下降沿触发 //KEY_set
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_Init(&EXTI_InitStructure);

//GPIOA.5	  中断线以及中断初始化配置 下降沿触发 //KEY_left
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOA.6	  中断线以及中断初始化配置  下降沿触发	//KEY_down
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIOA.7	  中断线以及中断初始化配置  下降沿触发	//KEY_right
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键KEY_up所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY_set所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//使能按键KEY_left,KEY_right,KEY_down所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级为第四 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI1_IRQHandler(void)//外部中断1服务程序
{
	delay_ms(20);//消抖
	if(KEY_up == 0)
	{
		Buzzer_ON();
		KEY_value = up;
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位
	delay_ms(2);
	Buzzer_OFF();
}
void EXTI4_IRQHandler(void)//外部中断4服务程序
{
	delay_ms(20);//消抖
	if(KEY_set == 0)
	{
		Buzzer_ON();
		KEY_value = set;
	}
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位
	delay_ms(2);
	Buzzer_OFF();
}
void EXTI9_5_IRQHandler(void)//外部中断5，6，7服务程序 
{	
	delay_ms(20);//消抖	
	if(KEY_down == 0)	 	 //KEY_down按键  PA6端口
	{				 
    Buzzer_ON();
		KEY_value = down;			
	}
	if(KEY_left == 0)	 	 //KEY_lift按键  PA5端口
	{				 
    Buzzer_ON();
		KEY_value = left;	
	}
	if(KEY_right == 0)	 	 //KEY_right按键 PA7端口
	{				 
		Buzzer_ON();
		KEY_value = right;		
	}
	EXTI_ClearITPendingBit(EXTI_Line6); //清除LINE6上的中断标志位
	EXTI_ClearITPendingBit(EXTI_Line5); //清除LINE5上的中断标志位
	EXTI_ClearITPendingBit(EXTI_Line7); //清除LINE7上的中断标志位
	delay_ms(2);
	Buzzer_OFF();	
}

