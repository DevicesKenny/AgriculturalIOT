#include "wifi.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "lcd.h"

char flag_time_reset=1;
char flag_time_wifi=0;
char flag_time_serv=0;
char number = 0;

void wait_OK_WIFI(void);
void wait_OK(void);
unsigned char Query(char * src,char * des,unsigned int LEN);
extern void stm32flash_stroage();
extern void nowifi_Light();

void Connect_wifi(void) {
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
            number=0;
        }
    }
    if(flag_time_wifi)
    {
        //连接已知WiFi
        LCD_ShowString(1,40+18,"                    ");
        LCD_ShowString(1,40+18*2,"                    ");
        LCD_ShowString(1,40+18,"Connect WIFI...");
        USART2TxStr("连接已知WiFi...\r\n");
        USART1TxStr("AT+CWJAP=\"WIFI_100\",\"12345678\"\r\n");
        wait_OK_WIFI();//等待接收到OK字符串
        delay_ms(30);
        if(Count_Timer3_value_USART3_receive_timeout2!=0)
        {
            USART2TxStr("连接wifi成功\r\n");
            flag_time_wifi=0;
            LCD_ShowString(1,40+18*2,"                    ");
            LCD_ShowString(1,40+18,"Connect WiFi success");
            flag_time_serv=1;
            number=0;
        }
        number++;
        if(number>33) {
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
            number=0;
            flag_time_serv=0;
            //	Flag_data_send_instart=1;
            LCD_ShowString(1,40+18*2,"Connect Serv success");
            USART2TxStr("连接服务器成功\r\n");
        }
        number++;
        if(number>3) {
            flag_time_wifi=1;
            flag_time_serv=0;
        }
    }

}

unsigned char Query(char * src,char * des,unsigned int LEN)
//查询数组有无包含该字符串，有则返回1，无则返回0
{
    unsigned int y= 0,strlen= 0,n= 0;
    unsigned char Result = 0;
    char * i;
    i = des;
    for(; *i != '\0'; i ++,strlen ++) {} // 判断需要检测的字符的长度
    for(y = 0; y < LEN - strlen; y ++)
        //开始检测，次数为总长度减去字符长度的字节数
    {
        for(n = 0; n < strlen; n ++)
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
void wait_OK(void)
{
    Count_Timer3_value_USART3_receive_timeout2=30;
    while(!Flag_usart1_receive_OK)//等待接收到OK字符串
    {
			stm32flash_stroage();
			nowifi_Light();
        if(Count_Timer3_value_USART3_receive_timeout2==0)
            break;
    }
    Flag_usart1_receive_OK = 0;
//	if( Query(USART1_RX_BUF,"Hwc",USART1_REC_LEN)&&Query(USART1_RX_BUF,"T",USART1_REC_LEN))
//				{
//					copy_str(USART1_RX_BUF_TEMP,USART1_RX_BUF,100);
//				}
    CLR_Buf1();     //清除串口1接收缓存
}

void wait_OK_WIFI(void)
{
    Count_Timer3_value_USART3_receive_timeout2=70;
    while(!Flag_usart1_receive_OK)//等待接收到OK字符串
    {
			stm32flash_stroage();
			nowifi_Light();
        if(Count_Timer3_value_USART3_receive_timeout2==0)
            break;
    }
    Flag_usart1_receive_OK = 0;
    CLR_Buf1();     //清除串口1接收缓存
}
