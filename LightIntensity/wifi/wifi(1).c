#include "wifi.h"
#include "time.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
//wifi
char flag_time_reset=1;
char flag_time_wifi=0;
char flag_time_serv=0;
char Num_wifi_times=0;
unsigned char Flag_wifi_receive_tosend = 1;
unsigned char Flag_have_data_need_send = 0;
unsigned char Flag_data_send_instart = 1;

char AT_send_buf[20];
char Data_Buf[100];


void Have_data_need_send(void){
	int x;
	if(Flag_have_data_need_send&&flag_time_serv==0&&flag_time_reset==0&&flag_time_wifi==0)//如果有数据需要发送
		{
	Flag_have_data_need_send = 0;
	//通过WIFI发送数据给服务器
	copy_str(AT_send_buf,"AT+CIPSEND=4,0",14);//拷贝字符串
			x = Get_data_len(Data_Buf);//获取字符串/数组有效数据长度
			if(x < 10)
			{
				AT_send_buf[13] = x % 10 + '0';
				AT_send_buf[14] = '\r';
				AT_send_buf[15] = '\n';
			}
			else
			{
				AT_send_buf[13] = x % 100 / 10 + '0';
				AT_send_buf[14] = x % 10  + '0';
				AT_send_buf[15] = '\r';
				AT_send_buf[16] = '\n';
			}
			USART2TxStr(AT_send_buf);
			USART1TxStr(AT_send_buf);//发送字符串长度	
			POINT_COLOR=YELLOW;	//画笔为黄色
			LCD_ShowString(120+16*6,40+18*6,"send len... ");	
			wait_dayuhao();
			if(flag_time_serv==0&&flag_time_reset==0&&flag_time_wifi==0)
			{
				Flag_wifi_receive_tosend = 1;
				USART2TxStr(Data_Buf);
				USART1TxStr(Data_Buf);//发送字符串
				LCD_ShowString(120+16*6,40+18*6,"send data...");
				wait_OK();//等待接收到OK字符串
				CLR_Buf1();     //清除串口1接收缓存
				CLR_Buf2();     //清除串口2接收缓存
				LCD_ShowString(120+16*6,40+18*6,"Done !      ");
				for(x = 0;x < 20;x ++)
					AT_send_buf[x] = 0;
				for(x = 0;x < 50;x ++)
					Data_Buf[x] = 0;
				//copy_str(Data_Buf,"HwdFF0T",7);
			}
		}
}


void wait_OK(void)
{
	Count_Timer3_value_USART3_receive_timeout2=30;
	while(!Flag_usart1_receive_OK)//等待接收到OK字符串
	{
		if(Count_Timer3_value_USART3_receive_timeout2==0)
		break;
	}
	Flag_usart1_receive_OK = 0;
	if( Query(USART1_RX_BUF,"Hwc",USART1_REC_LEN)&&Query(USART1_RX_BUF,"T",USART1_REC_LEN))
				{
					copy_str(USART1_RX_BUF_TEMP,USART1_RX_BUF,100);
				}
	CLR_Buf1();     //清除串口1接收缓存
}

void wait_OK_WIFI(void)
{
	Count_Timer3_value_USART3_receive_timeout2=70;
	while(!Flag_usart1_receive_OK)//等待接收到OK字符串
	{
		if(Count_Timer3_value_USART3_receive_timeout2==0)
		break;
	}
	Flag_usart1_receive_OK = 0;
	CLR_Buf1();     //清除串口1接收缓存
}

void wait_dayuhao(void)
{
	char x;
	Count_Timer3_value_USART3_receive_timeout2=50;
	while(Flag_wifi_receive_tosend)
	{
		for(x = 0;x < USART1_RX_STA;x ++)
		{
			if(USART1_RX_BUF[x] == '>')
			{//等待收到大于号字符串
				Flag_wifi_receive_tosend = 0;
				if( Query(USART1_RX_BUF,"Hwc",USART1_REC_LEN)&&Query(USART1_RX_BUF,"T",USART1_REC_LEN))
				{
					copy_str(USART1_RX_BUF_TEMP,USART1_RX_BUF,100);
				}
				CLR_Buf1();
				Flag_usart1_receive_OK = 0;
				break;
			}					
		}
		if(Count_Timer3_value_USART3_receive_timeout2==0)
			{
				flag_time_serv=1;
				CLR_Buf1();
				Flag_usart1_receive_OK = 0;
				break;
			}
	}
}

void copy_str(char* des,char* src,unsigned char len)
{
	unsigned char i;
	for(i = 0;i < len;i ++)
	{
		*(des+i) = *(src+i);
	}
}
unsigned char Get_data_len(char* addr)//获取字符串/数组有效数据长度
{
	unsigned len;
	for(len = 0;*(addr+len) != '\0';len ++);
	return len;
}
unsigned char Query(char * src,char * des,unsigned int LEN)
	//查询数组有无包含该字符串，有则返回1，无则返回0
{
	unsigned int y= 0,strlen= 0,n= 0;
	unsigned char Result = 0;
	char * i;
	i = des;
	for(; *i != '\0';i ++,strlen ++){}// 判断需要检测的字符的长度
	for(y = 0; y < LEN - strlen;y ++)
		//开始检测，次数为总长度减去字符长度的字节数
	{
		for(n = 0;n < strlen;n ++)
		{
			if(*(src + y + n) == *(des + n))
				//开始检测双方的第一个字节，如果相等则结果等于1，并且继续检测双方的第二个字节
			{
				Result = 1;				
			}
			else
			{
				Result = 0;	//不相等则结果等于0，并且退出此次循环，
										//开始检测数组的第二个字节和字符的第一个字节
				break;
			}
		}
		if(n == strlen)
		{
			return Result;
		}
	}
	return Result;
}
void Set_PWM(unsigned int value_Per)
{
	TIM_SetCompare1(TIM2,893 - 3.93 * value_Per);//设置PWM的占空比
}

void Connect_wifi(void){
	if(flag_time_reset)
			{
				//重启WiFi模块
				LCD_ShowString(1,40,"                    ");
				LCD_ShowString(1,40+18,"                    ");
				USART2TxStr("重启模块...\r\n"); 
				USART1TxStr("AT+RST\r\n");
				LCD_ShowString(1,40,"Reset wifi mode...");
				delay_ms(1000);//值最大为1864
				delay_ms(1000);//值最大为1864
				delay_ms(1000);//值最大为1864
				delay_ms(1000);//值最大为1864
				delay_ms(1000);//值最大为1864
				delay_ms(1000);//值最大为1864
				CLR_Buf1();     //清除串口1接收缓存
				Flag_usart1_receive_OK = 0;
				//设置工作模式
				USART2TxStr("设置工作模式...\r\n"); 
				USART1TxStr("AT+CWMODE=3\r\n");
				LCD_ShowString(1,40,"                    ");
				LCD_ShowString(1,40+18,"                    ");
				LCD_ShowString(1,40,"seting mode....");
				wait_OK();//等待接收到OK字符串
				if(Count_Timer3_value_USART3_receive_timeout2!=0)
				{
					flag_time_reset=0;
					flag_time_wifi=1;
					LCD_ShowString(1,40,"                    ");
					LCD_ShowString(1,40+18,"                    ");
					LCD_ShowString(1,40,"set mode success");
					USART2TxStr("设置模式成功\r\n");
					Num_wifi_times=0;
				}
			}
		if(flag_time_wifi)
			{
				//连接已知WiFi
				LCD_ShowString(1,40+18,"                    ");
				LCD_ShowString(1,40+18*2,"                    ");
				LCD_ShowString(1,40+18,"Connect WIFI...");
				USART2TxStr("连接已知WiFi...\r\n");
				USART1TxStr("AT+CWJAP=\"WIFI_99\",\"12345678\"\r\n");
				wait_OK_WIFI();//等待接收到OK字符串
				delay_ms(30);
				if(Count_Timer3_value_USART3_receive_timeout2!=0)
				{
					USART2TxStr("连接wifi成功\r\n");
					flag_time_wifi=0;
					LCD_ShowString(1,40+18*2,"                    ");
					LCD_ShowString(1,40+18,"Connect WiFi success");
					flag_time_serv=1;
					Num_wifi_times=0;
				}
				Num_wifi_times++;
				if(Num_wifi_times>33){
					flag_time_serv=0;
					flag_time_reset=1;
				}
			}
		if(flag_time_serv)
			{
				//启动多模块连接
				USART2TxStr("启动多模块连接...\r\n");
				USART1TxStr("AT+CIPMUX=1\r\n");
				wait_OK();//等待接收到OK字符串
				//连接服务器
				LCD_ShowString(1,40+18*2,"                    ");
				LCD_ShowString(1,40+18*2,"Connect Serv...");
				USART2TxStr("连接服务器...\r\n");
				USART1TxStr("AT+CIPSTART=4,\"TCP\",\"192.168.1.200\",6000\r\n");
				wait_OK();//等待接收到OK字符串
				if(Count_Timer3_value_USART3_receive_timeout2!=0)
				{
					Num_wifi_times=0;
					flag_time_serv=0;
					Flag_data_send_instart=1;
					LCD_ShowString(1,40+18*2,"Connect Serv success");
					USART2TxStr("连接服务器成功\r\n");
				}
				Num_wifi_times++;
				if(Num_wifi_times>3){
					flag_time_wifi=1;
					flag_time_serv=0;
				}
			}
		
}
