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
	if(Flag_have_data_need_send&&flag_time_serv==0&&flag_time_reset==0&&flag_time_wifi==0)//�����������Ҫ����
		{
	Flag_have_data_need_send = 0;
	//ͨ��WIFI�������ݸ�������
	copy_str(AT_send_buf,"AT+CIPSEND=4,0",14);//�����ַ���
			x = Get_data_len(Data_Buf);//��ȡ�ַ���/������Ч���ݳ���
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
			USART1TxStr(AT_send_buf);//�����ַ�������	
			POINT_COLOR=YELLOW;	//����Ϊ��ɫ
			LCD_ShowString(120+16*6,40+18*6,"send len... ");	
			wait_dayuhao();
			if(flag_time_serv==0&&flag_time_reset==0&&flag_time_wifi==0)
			{
				Flag_wifi_receive_tosend = 1;
				USART2TxStr(Data_Buf);
				USART1TxStr(Data_Buf);//�����ַ���
				LCD_ShowString(120+16*6,40+18*6,"send data...");
				wait_OK();//�ȴ����յ�OK�ַ���
				CLR_Buf1();     //�������1���ջ���
				CLR_Buf2();     //�������2���ջ���
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
	while(!Flag_usart1_receive_OK)//�ȴ����յ�OK�ַ���
	{
		if(Count_Timer3_value_USART3_receive_timeout2==0)
		break;
	}
	Flag_usart1_receive_OK = 0;
	if( Query(USART1_RX_BUF,"Hwc",USART1_REC_LEN)&&Query(USART1_RX_BUF,"T",USART1_REC_LEN))
				{
					copy_str(USART1_RX_BUF_TEMP,USART1_RX_BUF,100);
				}
	CLR_Buf1();     //�������1���ջ���
}

void wait_OK_WIFI(void)
{
	Count_Timer3_value_USART3_receive_timeout2=70;
	while(!Flag_usart1_receive_OK)//�ȴ����յ�OK�ַ���
	{
		if(Count_Timer3_value_USART3_receive_timeout2==0)
		break;
	}
	Flag_usart1_receive_OK = 0;
	CLR_Buf1();     //�������1���ջ���
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
			{//�ȴ��յ����ں��ַ���
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
unsigned char Get_data_len(char* addr)//��ȡ�ַ���/������Ч���ݳ���
{
	unsigned len;
	for(len = 0;*(addr+len) != '\0';len ++);
	return len;
}
unsigned char Query(char * src,char * des,unsigned int LEN)
	//��ѯ�������ް������ַ��������򷵻�1�����򷵻�0
{
	unsigned int y= 0,strlen= 0,n= 0;
	unsigned char Result = 0;
	char * i;
	i = des;
	for(; *i != '\0';i ++,strlen ++){}// �ж���Ҫ�����ַ��ĳ���
	for(y = 0; y < LEN - strlen;y ++)
		//��ʼ��⣬����Ϊ�ܳ��ȼ�ȥ�ַ����ȵ��ֽ���
	{
		for(n = 0;n < strlen;n ++)
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
void Set_PWM(unsigned int value_Per)
{
	TIM_SetCompare1(TIM2,893 - 3.93 * value_Per);//����PWM��ռ�ձ�
}

void Connect_wifi(void){
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
					Num_wifi_times=0;
				}
			}
		if(flag_time_wifi)
			{
				//������֪WiFi
				LCD_ShowString(1,40+18,"                    ");
				LCD_ShowString(1,40+18*2,"                    ");
				LCD_ShowString(1,40+18,"Connect WIFI...");
				USART2TxStr("������֪WiFi...\r\n");
				USART1TxStr("AT+CWJAP=\"WIFI_99\",\"12345678\"\r\n");
				wait_OK_WIFI();//�ȴ����յ�OK�ַ���
				delay_ms(30);
				if(Count_Timer3_value_USART3_receive_timeout2!=0)
				{
					USART2TxStr("����wifi�ɹ�\r\n");
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
					Num_wifi_times=0;
					flag_time_serv=0;
					Flag_data_send_instart=1;
					LCD_ShowString(1,40+18*2,"Connect Serv success");
					USART2TxStr("���ӷ������ɹ�\r\n");
				}
				Num_wifi_times++;
				if(Num_wifi_times>3){
					flag_time_wifi=1;
					flag_time_serv=0;
				}
			}
		
}
