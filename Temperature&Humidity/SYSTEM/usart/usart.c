#include "sys.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif

char USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
char USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
unsigned char USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 USART1_RX_STA=0;       //接收状态标记	  
u16 USART2_RX_STA=0;       //接收状态标记
u16 USART3_RX_STA=0;       //接收状态标记 
unsigned char Count_Timer3_value_USART3_receive_timeout = 0;
unsigned char Count_Timer3_value_USART3_receive_timeout2 = 0;
unsigned char Flag_Usart1_Receive;
unsigned char Flag_Usart1_Remap_Receive;
unsigned char Flag_Usart2_Receive;
unsigned char Flag_Usart3_Receive;
unsigned char Flag_Usart1_Remap;
unsigned char Flag_Usart3_Remap;
unsigned char Flag_Usart3_Receive_Complete;
unsigned char Flag_usart1_receive_OK = 0;
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void Usart1_Init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟以及复用功能时钟
  //GPIO_PinRemapConfig(GPIO_Remap_USART1,DISABLE);//重映射关闭	
	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
    //USART1_TX   PB.6	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB.6 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART1_RX	  PB.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB.7 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0//优先级别最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART1, ENABLE);                    //使能串口 		
}
void Usart1_Remap_Init(u32 bound)
{
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟以及复用功能时钟
  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//重映射开启

	 //USART1_TX   PB.6	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB.6 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    //USART1_RX	  PB.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB.7 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
   
   //Usart1 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0//优先级别最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 
}
/* USART1发送一个字符 */ 
void USART1TxChar(char ch) 
{ 
	USART_SendData(USART1,(u8) ch);         
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{}
}
/* USART3发送一个数组数据（十六进制） */
void USART1TxData(unsigned char *pt)
{
	while(*pt != '\0')   
	{
		USART1TxChar(*pt);
		pt++;
	}
}
void USART1TxData_hex(unsigned char *pt,unsigned char len)
{
  while(len)   
  {
    USART1TxChar(*pt);
    pt++;
    len --; 
  }
}
/* USART1发送一个字符串 */ 
void USART1TxStr(char *pt) 
{
	while(*pt != '\0')   
	{
		USART1TxChar(*pt);
		pt++;
	}
}
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Flag_Usart1_Receive = 1;
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		USART1_RX_BUF[USART1_RX_STA]=Res;
		USART2TxChar(Res);
		if(USART1_RX_BUF[USART1_RX_STA] == 'K')
		{
			if(USART1_RX_BUF[USART1_RX_STA - 1] == 'O')
			{
				Flag_usart1_receive_OK = 1;
			}
		}				
		USART1_RX_STA++;
		if(USART1_RX_STA > (USART1_REC_LEN - 1))
			USART1_RX_STA = 0;	
	}
} 
void CLR_Buf1(void)     //清除串口1接收缓存
{
	unsigned int y=0;
	for( y= 0;y < USART1_REC_LEN;y ++)
	{
		USART1_RX_BUF[y] = '\0';
	}
	USART1_RX_STA =0;
} 
///////////////////////////串口2初始化/////////////////////////////////////////////   
static void USART2_RCC_Configuration(void)//初始化USART3的时钟  
{  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM3 , ENABLE); 
}
static void USART2_GPIO_Configuration(void) 
{ 
	/*定义GPIO初始化结构体*/  
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*初始化结构体*/   
	GPIO_StructInit(&GPIO_InitStructure);
	/*配置USART3的接收端口*/   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;     //PA.3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);     //初始化PA.3 
	/*配置USART3的发送端口*/   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;       //PA.2 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化PA.2 
}
//配置USART2的工作参数  
static void USART2_Configuration(u32 bound) 
{
	USART_InitTypeDef USART_InitStructure; 
	/*USART相关时钟初始化配置*/  
	USART2_RCC_Configuration();
	/*USART相关GPIO初始化配置*/   
	USART2_GPIO_Configuration();
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	/*配置USART2的异步双工串行工作模式*/  
	USART_Init(USART2, &USART_InitStructure); 
	/*使能USART2的接收中断 */   
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/*使能USART2*/   
	USART_Cmd(USART2, ENABLE);
}

//配置中断优先级函数  
static void USART2_NVIC_Configuration(void) 
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* USART3中断优先级*/   
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级优先级别为第三
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
}
void Usart2_Init(u32 bound) 
{
	USART2_RCC_Configuration();  
	USART2_GPIO_Configuration();  
	USART2_Configuration(bound);   
	USART2_NVIC_Configuration();
}
/* USART2发送一个字符 */ 
void USART2TxChar(int ch) 
{ 
	USART_SendData(USART2,(u8) ch);         
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}
}
/* USART2发送一个数组数据（十六进制） */
void USART2TxData(unsigned char *pt)
{
	while(*pt != '\0')   
	{
		USART2TxChar(*pt);
		pt++;
	}
}
void USART2TxData_hex(unsigned char *pt,unsigned char len)
{
  while(len)   
  {
    USART2TxChar(*pt);
    pt++;
    len --; 
  }
}
/* USART2发送一个字符串 */ 
void USART2TxStr(char *pt) 
{
	while(*pt != '\0')   
	{
		USART2TxChar(*pt);
		pt++;
	}
}
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		USART2_RX_BUF[USART2_RX_STA]=Res;
		//USART1TxChar(Res);
		if((USART2_RX_BUF[USART2_RX_STA] == 0x0a) && (USART2_RX_BUF[USART2_RX_STA - 1] == 0x0d))
			Flag_Usart2_Receive = 1;
		USART2_RX_STA++;
		if(USART2_RX_STA > (USART_REC_LEN - 1))
			USART2_RX_STA = 0;
	}
}
void CLR_Buf2(void)     //清除串口2接收缓存
{
	unsigned int y=0;
#ifdef Debug_Mode
	USART2TxStr("Erase usart2 rxbuf\r\n");
#endif
	for( y= 0;y < USART_REC_LEN;y ++)
	{
		USART2_RX_BUF[y] = '\0';
	}
	USART2_RX_STA =0;
}

///////////////////////////串口3初始化/////////////////////////////////////////////   
void Usart3_Init(u32 bound)//串口3标准接口初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM3 , ENABLE);  
	USART3_GPIO_Configuration();  
	USART3_Configuration(bound);   
	USART3_NVIC_Configuration();
}
void Usart3_Remap_Init(u32 bound)//串口3重映射接口初始化 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM3 , ENABLE);  
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);//部分重映射
	USART3_GPIO_Configuration();  
	USART3_Configuration(bound);   
	USART3_NVIC_Configuration();
}
static void USART3_GPIO_Configuration(void) 
{ 
	/*定义GPIO初始化结构体*/  
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*初始化结构体*/   
	GPIO_StructInit(&GPIO_InitStructure);
	/*配置USART3的接收端口*/   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;     //Px.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);     //初始化PC.11 //重映射后的引脚
	/*配置USART3的发送端口*/   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       //PC.10 //重映射后的引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //初始化PC.10 
	
	/*配置USART3的接收端口*/   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;     //PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);     //初始化PB.11 //重映射后的引脚
	/*配置USART3的发送端口*/   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       //PB.10 //重映射后的引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);      //初始化PB.10 
}
//配置USART3的工作参数  
static void USART3_Configuration(u32 bound) 
{
	USART_InitTypeDef USART_InitStructure;   
	/*USART相关GPIO初始化配置*/   
	USART3_GPIO_Configuration();
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	/*配置USART3的异步双工串行工作模式*/  
	USART_Init(USART3, &USART_InitStructure); 
	/*使能USART3的接收中断 */   
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	/*使能USART3*/   
	USART_Cmd(USART3, ENABLE);
}

//配置中断优先级函数  
static void USART3_NVIC_Configuration(void) 
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* USART3中断优先级*/   
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级优先级别为第二
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
}

/* USART3发送一个字符 */ 
void USART3TxChar(int ch) 
{ 
	//LED3_ON();
	USART_SendData(USART3,(u8) ch);         
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{}
	//LED3_OFF();
}
/* USART3发送一个数组数据（十六进制） */
void USART3TxData(unsigned char *pt)
{
	while(*pt != '\0')   
	{
		USART3TxChar(*pt);
		pt++;
	}
}
void USART3TxData_hex(unsigned char *pt,unsigned char len)
{
  while(len)   
  {
    USART3TxChar(*pt);
    pt++;
    len --; 
  }
}
/* USART3发送一个字符串 */ 
void USART3TxStr(char *pt) 
{
	while(*pt != '\0')   
	{
		USART3TxChar(*pt);
		pt++;
	}
}
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{		
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res;
		USART3_RX_STA++;
		if(USART3_RX_STA > (USART_REC_LEN - 1))
			USART3_RX_STA = 0;
		Flag_Usart3_Receive = 1;
		Count_Timer3_value_USART3_receive_timeout = 2;//定时200ms
	}
}
void CLR_Buf3(void)     //清除串口3接收缓存
{
	unsigned int y=0;
#ifdef Debug_Mode
	USART2TxStr("Erase usart3 rxbuf\r\n");
#endif
	for( y= 0;y < USART_REC_LEN;y ++)
	{
		USART3_RX_BUF[y] = '\0';
	}
	USART3_RX_STA =0;
}
#endif	

