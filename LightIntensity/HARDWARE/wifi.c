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
        //����WiFiģ��
        LCD_ShowString(1,40,"                    ");
        LCD_ShowString(1,40+18,"                    ");
        USART2TxStr("����ģ��...\r\n");
        USART1TxStr("AT+RST\r\n");
        LCD_ShowString(1,40,"Reset wifi mode...");
        delay_ms(1000);//ֵ���Ϊ1864
        delay_ms(1000);//ֵ���Ϊ1864
        delay_ms(1000);//ֵ���Ϊ1864
        delay_ms(1000);//ֵ���Ϊ1864
        delay_ms(1000);//ֵ���Ϊ1864
        delay_ms(1000);//ֵ���Ϊ1864
        CLR_Buf1();     //�������1���ջ���
        Flag_usart1_receive_OK = 0;
        //���ù���ģʽ
        USART2TxStr("���ù���ģʽ...\r\n");
        USART1TxStr("AT+CWMODE=3\r\n");
        LCD_ShowString(1,40,"                    ");
        LCD_ShowString(1,40+18,"                    ");
        LCD_ShowString(1,40,"seting mode....");
        wait_OK();//�ȴ����յ�OK�ַ���
        if(Count_Timer3_value_USART3_receive_timeout2!=0)
        {
            flag_time_reset=0;
            flag_time_wifi=1;
            LCD_ShowString(1,40,"                    ");
            LCD_ShowString(1,40+18,"                    ");
            LCD_ShowString(1,40,"set mode success");
            USART2TxStr("����ģʽ�ɹ�\r\n");
            number=0;
        }
    }
    if(flag_time_wifi)
    {
        //������֪WiFi
        LCD_ShowString(1,40+18,"                    ");
        LCD_ShowString(1,40+18*2,"                    ");
        LCD_ShowString(1,40+18,"Connect WIFI...");
        USART2TxStr("������֪WiFi...\r\n");
        USART1TxStr("AT+CWJAP=\"WIFI_100\",\"12345678\"\r\n");
        wait_OK_WIFI();//�ȴ����յ�OK�ַ���
        delay_ms(30);
        if(Count_Timer3_value_USART3_receive_timeout2!=0)
        {
            USART2TxStr("����wifi�ɹ�\r\n");
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
        //������ģ������
        USART2TxStr("������ģ������...\r\n");
        USART1TxStr("AT+CIPMUX=1\r\n");
        wait_OK();//�ȴ����յ�OK�ַ���
        //���ӷ�����
        LCD_ShowString(1,40+18*2,"                    ");
        LCD_ShowString(1,40+18*2,"Connect Serv...");
        USART2TxStr("���ӷ�����...\r\n");
        USART1TxStr("AT+CIPSTART=4,\"TCP\",\"192.168.1.200\",6000\r\n");
        wait_OK();//�ȴ����յ�OK�ַ���
        if(Count_Timer3_value_USART3_receive_timeout2!=0)
        {
            number=0;
            flag_time_serv=0;
            //	Flag_data_send_instart=1;
            LCD_ShowString(1,40+18*2,"Connect Serv success");
            USART2TxStr("���ӷ������ɹ�\r\n");
        }
        number++;
        if(number>3) {
            flag_time_wifi=1;
            flag_time_serv=0;
        }
    }

}

unsigned char Query(char * src,char * des,unsigned int LEN)
//��ѯ�������ް������ַ��������򷵻�1�����򷵻�0
{
    unsigned int y= 0,strlen= 0,n= 0;
    unsigned char Result = 0;
    char * i;
    i = des;
    for(; *i != '\0'; i ++,strlen ++) {} // �ж���Ҫ�����ַ��ĳ���
    for(y = 0; y < LEN - strlen; y ++)
        //��ʼ��⣬����Ϊ�ܳ��ȼ�ȥ�ַ����ȵ��ֽ���
    {
        for(n = 0; n < strlen; n ++)
        {
            if(*(src + y + n) == *(des + n))
                //��ʼ���˫���ĵ�һ���ֽڣ���������������1�����Ҽ������˫���ĵڶ����ֽ�
            {
                Result = 1;
            }
            else
            {
                Result = 0;	//�������������0�������˳��˴�ѭ����
                //��ʼ�������ĵڶ����ֽں��ַ��ĵ�һ���ֽ�
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
    while(!Flag_usart1_receive_OK)//�ȴ����յ�OK�ַ���
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
    CLR_Buf1();     //�������1���ջ���
}

void wait_OK_WIFI(void)
{
    Count_Timer3_value_USART3_receive_timeout2=70;
    while(!Flag_usart1_receive_OK)//�ȴ����յ�OK�ַ���
    {
			stm32flash_stroage();
			nowifi_Light();
        if(Count_Timer3_value_USART3_receive_timeout2==0)
            break;
    }
    Flag_usart1_receive_OK = 0;
    CLR_Buf1();     //�������1���ջ���
}
