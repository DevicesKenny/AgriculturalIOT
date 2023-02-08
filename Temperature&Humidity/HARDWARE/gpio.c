#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"

void GPIO_Configuration(void)//GPIO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*初始化结构体*/   
	GPIO_StructInit(&GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);//使能PC端口时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通推挽输出 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //初始化PC.15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通推挽输出 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //初始化PC.3,PC.2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通推挽输出 
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化PA.0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通推挽输出 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //初始化PC.6
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通推挽输出 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //初始化PC.0
	
}
