#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "gpio.h"
#include "stmflash.h"
#include "exti.h"
#include "string.h"
#include "Motor.h"
#include "wifi.h"

#define	Illuminance						1//���ն�
#define Light
#define Motor

#define FLASH_SAVE_ADDR  0X0807F000	
#define FLASH_SAVE_ADDR1 0X0807F100	

unsigned char Read_Illuminance_CMD[8]	={0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};//��ȡ���ն����ݵ�����
unsigned char Flag_LED3_ONOFF = 0;
unsigned long Light_value = 0,temp_u32 = 0;
unsigned int Motor_Flag = 2;
unsigned int Left_Flag = 0;
unsigned int Temp_Temporarily_value;
unsigned int Temp1_Temporarily_value;
float Temp_currentTemp;
int cur_mode = 0;
int flag = 0;
int flag_connectwifi = 1;
int Mode_Flag = 0;
uint8_t value = 0; 
uint8_t value1 = 0;
uint8_t count = 0;
uint8_t count1 = 0;
int nowifi = 1;
unsigned char x;
int i = 0;
char Temp_value_str[7];
char Hum_value_str[6];
char Gas_value_str[7];
char AT_send_buf[100];
char Light_value_str[7];
char Light_value_test[7];
char Light1_value_test[7];

char FAN1_status[7];
char FAN2_status[7];
char Light_status[8];
char Motor_status[8];

char Light_data[7] = {'0','0','0','0','0','0',0};
char Light1_data[7];
char Light_storage_data[7] = {'0','0','0','0','0','0',0};
char Light1_storage_data[7] = {'0','0','0','0','0','0',0};
char Data_Buf[50];
void copy_str(char* des,char* src,unsigned char len);
void wait_OK(void);
void stm32flash_stroage();		//�洢flash����
void esp8266_init();		//wifi��ʼ��
void copy_str(char* des,char* src,unsigned char len);
void wait_OK(void);
void wait1_OK(void);
unsigned char Query(char * src,char * des,unsigned int LEN)	;
unsigned char Get_data_len(char* addr);
void nowifi_Light();
void senddata();
int main(void)
{
	GPIO_Configuration();//GPIO��ʼ��
	delay_init();	    	 			//��ʱ������ʼ��
	Lcd_Init();					//��ʼ��LCD
	EXTIX_Init();					//�����ж��¼�
	Usart1_Init(115200);			//����1��ʼ��
	Usart2_Init(115200);			//����2��ʼ��
	Usart3_Remap_Init(9600);//����3��ӳ��ӿڳ�ʼ�� 
	NVIC_Configuration(); 		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��	
	TIM3_Int_Init(999,7199);	//������ʱ��3������100ms
	TIM2_PWM_Init(2249,80);		//PWM��ʼ��
	BACK_COLOR=BLACK;		//����Ϊ��ɫ
	POINT_COLOR=YELLOW;	//����Ϊ��ɫ
	#ifdef Illuminance
	LCD_Show_Chinese16x16(90,10,"���ռ��");
	#endif
	POINT_COLOR=GREEN;//��ɫ	
	LCD_DrawRectangle(0, 40, 320, 240);//�����ο�	
	
	//esp8266_init();
	
	LCD_Show_Chinese16x16(128-4*16,180,"���նȣ�");
	LCD_ShowString(128+8*7,180,"LUX");
	
	POINT_COLOR=YELLOW;	//����Ϊ��ɫ
	copy_str(Data_Buf,"Hwdie0206      T",16);
	
	//��ȡ��ֵ����ʾ
	STMFLASH_Read(FLASH_SAVE_ADDR,(unsigned int*)Light_data,6);
	Light_data[6] = 0;
	LCD_ShowString(120,200,Light_data);
	LCD_Show_Chinese16x16(120-5*16,200,"��ǰ��ֵ��");
	POINT_COLOR=GREEN;
	LCD_ShowString(120+6*8,200," LUX");
	
	copy_str(Light_status,"lightof",7);
	copy_str(Motor_status,"motorof",7);

	while(1)
	{
		Connect_wifi();
		//nowifi = 0;
		if(Flag_timer_2S)
		{//2Sʱ�䵽
			Flag_timer_2S = 0;
			if(Flag_LED3_ONOFF)
			{
				Flag_LED3_ONOFF = 0;
				LED3_ON();
				LCD_Fill(180+16,60,180+32,60+16,YELLOW);
			}
			else
			{
				Flag_LED3_ONOFF = 1;
				LED3_OFF();
				LCD_Fill(180+16,60,180+32,60+16,BLACK);
			}				
			//���Ͳɼ���������������
			USART3TxData_hex(Read_Illuminance_CMD,8);
		}
		
		stm32flash_stroage();	//�洢flash����
		//��������������*********************************
		if(flag_time_reset==1 || flag_time_serv==1 || flag_time_wifi == 1 || Query(USART1_RX_BUF,"ERROR",USART1_REC_LEN))
		{
			nowifi = 1;
		}
		else
			nowifi = 0;	
			
		//***********************************************	
		
		if(Query(USART1_RX_BUF,"Hwcig0207lightonT",USART1_REC_LEN))
		{
			copy_str(Light_status,"lighton",7);
			PCout(6) = 1;
			TIM_SetCompare2(TIM2,1000);
			
		}
		else if(Query(USART1_RX_BUF,"Hwcig0207lightofT",USART1_REC_LEN))
		{
			PCout(6) = 0;
			TIM_SetCompare2(TIM2,0);
			copy_str(Light_status,"lightof",7);
		}
		else if(Query(USART1_RX_BUF,"Hwcig0207motoronT",USART1_REC_LEN))
		{
			copy_str(Motor_status,"motoron",7);
			PCout(2) = 0;
			Set_Motor_PWM(1125);
			Mode_Flag = 0;
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			Set_Motor_PWM(0);
		}
		else if(Query(USART1_RX_BUF,"Hwcig0207motorofT",USART1_REC_LEN))
		{
			copy_str(Motor_status,"motorof",7);
			PCout(2) = 1;
			Set_Motor_PWM(1125);
			Mode_Flag = 1;
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			Set_Motor_PWM(0);
		}
		
			
		//�����ϴ���ֵ
		if(Query(USART1_RX_BUF,"Hwcef0206",USART1_REC_LEN))
		{

			for(i = 0;i < USART1_REC_LEN ; i++)
			{
				if(USART1_RX_BUF[i] == 'c'&&USART1_RX_BUF[i+1] == 'e'&&USART1_RX_BUF[i+2] == 'f')
				{
//					PCout(6) = 1;
//					TIM_SetCompare2(TIM2,1000);
					
					Light_data[0] = USART1_RX_BUF[i+7];
					Light_data[1] = USART1_RX_BUF[i+8];
					Light_data[2] = USART1_RX_BUF[i+9];
					Light_data[3] = USART1_RX_BUF[i+10];
					Light_data[4] = USART1_RX_BUF[i+11];
					Light_data[5] = USART1_RX_BUF[i+12];
					LCD_ShowString(120,200,(char*)Light_data);
					STMFLASH_Write(FLASH_SAVE_ADDR,(unsigned int*)Light_data,6);
				
					break;
				}
			}
		}

		if(Flag_Usart3_Receive//�������3�����ݽ���
			&&(!Count_Timer3_value_USART3_receive_timeout))//����������ճ�ʱ
		{
			Flag_Usart3_Receive = 0;			
			//��ʾ���������ݵ�LCD��
				
	/*
				��ȡ���նȴ������������01 03 00 00 00 02 C4 0B������ʪ�ȵ�һ����
				���ص����ݸ�ʽΪ��  01 03 04 00 00 10 54 F6 0C 
				*/
			
			Light_value = USART3_RX_BUF[4];
			Light_value <<= 8;
			Light_value |= USART3_RX_BUF[5];
			//Light_value = USART3_RX_BUF[5];
			Light_value <<= 8;
			Light_value |= USART3_RX_BUF[6];
			CLR_Buf3();
					
			Light_value_str[0]	= (char)(Light_value /100000    + '0');
			Light_value_str[1]	=	(char)(Light_value /10000%10 + '0');
			Light_value_str[2]	=	(char)(Light_value /1000%10 + '0');
			Light_value_str[3]	=	(char)(Light_value /100%10 + '0');
			Light_value_str[4]	=	(char)(Light_value /10%10 + '0');
			Light_value_str[5]  = (char)(Light_value   %10 + '0');
			Light_value_str[6] = 0;		
				
			POINT_COLOR=YELLOW;	//����Ϊ��ɫ
			LCD_ShowString(80+16*3,180,Light_value_str);
			
			nowifi_Light();
				
			copy_str(Data_Buf,"Hwdie0206                    T",30);
			copy_str(&Data_Buf[9],Light_value_str,6);
			copy_str(&Data_Buf[15],Light_status,7);
			copy_str(&Data_Buf[22],Motor_status,7);

			if(Query(USART1_RX_BUF,"ERROR",USART1_REC_LEN)||Query(USART1_RX_BUF,"CLOSED",USART1_REC_LEN))
			{
				flag_time_serv = 1;
			}

			//ͨ��WIFI�������ݸ�������
			if((flag_time_serv == 0 && flag_time_wifi == 0 ))
			{
				senddata();
			}
			
		}
	}
}

void stm32flash_stroage()
{
		
		if(count == 0)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,216,"-");
			LCD_ShowString(120+1*8,216," ");
			LCD_ShowString(120+2*8,216," ");
			LCD_ShowString(120+3*8,216," ");
			LCD_ShowString(120+4*8,216," ");
			LCD_ShowString(120+5*8,216," ");
		}	
		else if(count == 1)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,216," ");
			LCD_ShowString(120+1*8,216,"-");
			LCD_ShowString(120+2*8,216," ");
			LCD_ShowString(120+3*8,216," ");
			LCD_ShowString(120+4*8,216," ");
			LCD_ShowString(120+5*8,216," ");
		}
		else if(count == 2)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,216," ");
			LCD_ShowString(120+1*8,216," ");
			LCD_ShowString(120+2*8,216,"-");
			LCD_ShowString(120+3*8,216," ");
			LCD_ShowString(120+4*8,216," ");
			LCD_ShowString(120+5*8,216," ");
		}
		else if(count == 3)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,216," ");
			LCD_ShowString(120+1*8,216," ");
			LCD_ShowString(120+2*8,216," ");
			LCD_ShowString(120+3*8,216,"-");
			LCD_ShowString(120+4*8,216," ");
			LCD_ShowString(120+5*8,216," ");
		}
		else if(count == 4)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,216," ");
			LCD_ShowString(120+1*8,216," ");
			LCD_ShowString(120+2*8,216," ");
			LCD_ShowString(120+3*8,216," ");
			LCD_ShowString(120+4*8,216,"-");
			LCD_ShowString(120+5*8,216," ");
		}
		else if(count == 5)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,216," ");
			LCD_ShowString(120+1*8,216," ");
			LCD_ShowString(120+2*8,216," ");
			LCD_ShowString(120+3*8,216," ");
			LCD_ShowString(120+4*8,216," ");
			LCD_ShowString(120+5*8,216,"-");
		}
			
		#ifdef 	Humiture
		if(count1 == 0)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,180,"-");
			LCD_ShowString(120+1*8,180," ");
			LCD_ShowString(120+2*8,180," ");
			LCD_ShowString(120+3*8,180," ");
			LCD_ShowString(120+4*8,180," ");
			LCD_ShowString(120+5*8,180," ");
		}	
		else if(count1 == 1)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,180," ");
			LCD_ShowString(120+1*8,180,"-");
			LCD_ShowString(120+2*8,180," ");
			LCD_ShowString(120+3*8,180," ");
			LCD_ShowString(120+4*8,180," ");
			LCD_ShowString(120+5*8,180," ");
		}
		else if(count1 == 2)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,180," ");
			LCD_ShowString(120+1*8,180," ");
			LCD_ShowString(120+2*8,180,"-");
			LCD_ShowString(120+3*8,180," ");
			LCD_ShowString(120+4*8,180," ");
			LCD_ShowString(120+5*8,180," ");
		}
		else if(count1 == 3)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,180," ");
			LCD_ShowString(120+1*8,180," ");
			LCD_ShowString(120+2*8,180," ");
			LCD_ShowString(120+3*8,180,"-");
			LCD_ShowString(120+4*8,180," ");
			LCD_ShowString(120+5*8,180," ");
		}
		else if(count1 == 4)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,180," ");
			LCD_ShowString(120+1*8,180," ");
			LCD_ShowString(120+2*8,180," ");
			LCD_ShowString(120+3*8,180," ");
			LCD_ShowString(120+4*8,180,"-");
			LCD_ShowString(120+5*8,180," ");
		}
		else if(count1 == 5)
		{
			POINT_COLOR=GREEN;
			LCD_ShowString(120+0*8,180," ");
			LCD_ShowString(120+1*8,180," ");
			LCD_ShowString(120+2*8,180," ");
			LCD_ShowString(120+3*8,180," ");
			LCD_ShowString(120+4*8,180," ");
			LCD_ShowString(120+5*8,180,"-");
		}
		#endif
		
		//LCD_ShowString(80,120+16,(char*)temp_data1);
		if(KEY_value == up)
		{//����ϰ���������
			KEY_value = key_free;
			#ifdef 	Humiture
			if(Left_Flag == 0)
			{
				delay_ms(200);
				value = (int)(Light_storage_data[count] - '0');
				if(value >= 0 && value < 9 )
					value ++;
	//			if(count == 5)
	//				value++;
				Light_storage_data[count] = value + '0';
				POINT_COLOR=YELLOW;
				//LCD_ShowString(120,200,(char*)Light_storage_data);
			}
			else 
			{
				delay_ms(200);
				value1 = (int)(Light1_storage_data[count1] - '0');
				if(value1 >= 0 && value1 < 9 )
					value1 ++;
	//			if(count == 5)
	//				value++;
				Light1_storage_data[count1] = value1 + '0';
				POINT_COLOR=YELLOW;
				//LCD_ShowString(88,180,(char*)Light1_storage_data);
			}
			LCD_ShowString(120,200,(char*)Light_storage_data);
			LCD_ShowString(120,180-16,(char*)Light1_storage_data);
			#endif
			#ifdef Illuminance
				delay_ms(200);
				value = (int)(Light_data[count] - '0');
				if(value >= 0 && value < 9 )
					value ++;
	//			if(count == 5)
	//				value++;
				Light_data[count] = value + '0';
				POINT_COLOR=YELLOW;
				LCD_ShowString(120,200,(char*)Light_data);
			#endif
			
			//LCD_ShowString(80,120+16,(char*)temp_data1);
			
			//LCD_ShowString(16*7,16*9,"AA 00 03 01 BB");
		}
		else if(KEY_value == down)
		{//����°���������
			KEY_value = key_free;
			#ifdef 	Humiture
			if(Left_Flag == 0)
			{
				delay_ms(200);
				value = (int)(Light_storage_data[count] - '0'); 
				if(value > 0 && value <= 9 )
					value --;
				
				Light_storage_data[count] = value + '0';
				POINT_COLOR=YELLOW;
				//LCD_ShowString(120,200,(char*)Light_storage_data);
				//LCD_ShowString(80,120+16,(char*)temp_data1);
			}
			//alue = temp_str[count] - '0'; 
			else
			{
				delay_ms(200);
				value1 = (int)(Light1_storage_data[count1] - '0');
				if(value1 > 0 && value1 <= 9 )
					value1 --;
	//			if(count == 5)
	//				value++;
				Light1_storage_data[count1] = value1 + '0';
				POINT_COLOR=YELLOW;
			//LCD_ShowString(88,180,(char*)Light1_storage_data);
			//LCD_ShowString(80,120+16,(char*)temp_data1);
			}
			LCD_ShowString(120,200,(char*)Light_storage_data);
			LCD_ShowString(120,180-16,(char*)Light1_storage_data);
			#endif
			#ifdef Illuminance
				delay_ms(200);
				value = (int)(Light_data[count] - '0');
				if(value > 0 && value <= 9 )
					value --;
	//			if(count == 5)
	//				value++;
				Light_data[count] = value + '0';
				POINT_COLOR=YELLOW;
				LCD_ShowString(120,200,(char*)Light_data);
			#endif
		}
		
		else if(KEY_value == left)
		{//����󰴼�������
			KEY_value = key_free;
			#ifdef 	Humiture
			delay_ms(200);
			Left_Flag = !Left_Flag;
				
			LCD_ShowString(120,200,(char*)Light_storage_data);
			LCD_ShowString(120,180-16,(char*)Light1_storage_data);
			#endif
			#ifdef Illuminance
				delay_ms(200);
				count--;
//				value = (int)(Light_storage_data[count] - '0'); 		
//				Light_storage_data[count] = value + '0';
				if(count < 0)
					count = 0;
				POINT_COLOR=YELLOW;
				LCD_ShowString(120,200,(char*)Light_data);
				#endif
				//LCD_ShowString(120,200,(char*)Light_storage_data);
				//LCD_ShowString(80,120+16,(char*)temp_data1);
		}
		else if(KEY_value == right)
		{//����Ұ���������
			KEY_value = key_free;
			#ifdef 	Humiture
			if(Left_Flag == 0)
			{
				delay_ms(200);
				count++;
//				value = (int)(Light_storage_data[count] - '0'); 		
//				Light_storage_data[count] = value + '0';
				if(count > 5)
					count = 0;
				POINT_COLOR=YELLOW;
				//LCD_ShowString(120,200,(char*)Light_storage_data);
				//LCD_ShowString(80,120+16,(char*)temp_data1);
			}
			else
			{
				delay_ms(200);
				count1++;
//				value1 = (int)(Light1_storage_data[count1] - '0'); 		
//				Light1_storage_data[count1] = value1 + '0';
				if(count1 > 5)
					count1 = 0;
				POINT_COLOR=YELLOW;
				//LCD_ShowString(88,180,(char*)Light1_storage_data);
				//LCD_ShowString(80,120+16,(char*)temp_data1);
			}
			LCD_ShowString(120,200,(char*)Light_storage_data);
			LCD_ShowString(120,180-16,(char*)Light1_storage_data);
			#endif
			#ifdef Illuminance
				delay_ms(200);
				count++;
//				value = (int)(Light_storage_data[count] - '0'); 		
//				Light_storage_data[count] = value + '0';
				if(count > 5)
					count = 5;
				POINT_COLOR=YELLOW;
				LCD_ShowString(120,200,(char*)Light_data);
			#endif
		
		}
		else if(KEY_value == set)
		{//������ð���������
			KEY_value = key_free;
			#ifdef 	Humiture
			delay_ms(200);
			//value = (int)(Light_data[count] - '0'); 
			//ת�����ݸ�ʽ
//		for(i = 0;i < 6; i++)
//			temp_data[i] = temp_str[i] - '0';
			//Light_data[count] = value + '0';
				STMFLASH_Write(FLASH_SAVE_ADDR,(unsigned int*)Light_storage_data,6);
				STMFLASH_Write(FLASH_SAVE_ADDR1,(unsigned int*)Light1_storage_data,6);
			
			//delay_ms(100);
			//strcpy(temp_str,temp1_str);
			POINT_COLOR=YELLOW;
			LCD_ShowString(120,200,(char*)Light_storage_data);
			LCD_ShowString(120,180-16,(char*)Light1_storage_data);
			#endif
			#ifdef Illuminance
				delay_ms(200);
				STMFLASH_Write(FLASH_SAVE_ADDR,(unsigned int*)Light_data,6);
				Mode_Flag = 0;
				LCD_ShowString(120,200,(char*)Light_data);
			#endif
			//LCD_ShowString(80,120+16,(char*)temp_data1);
		}
}

void senddata()
{
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
	Count_Timer3_value_USART3_receive_timeout2 = 30;
	POINT_COLOR=YELLOW;	//����Ϊ��ɫ
	//LCD_ShowString(120+16*6,40+18*6,"send len... ");			
	while(flag)
	{
		for(x = 0;x < USART1_RX_STA;x ++)
		{
			if(USART1_RX_BUF[x] == '>')
			{//�ȴ��յ����ں��ַ���
				flag = 0;
				break;
			}					
		}
		if(Count_Timer3_value_USART3_receive_timeout2 == 0)
		{
			flag_time_serv = 1;
			flag = 0;
			CLR_Buf1();
				break;
		}	
	}
	if(flag_time_serv == 0 && flag_time_wifi == 0)
	{
		flag = 1;
		USART2TxStr(Data_Buf);
		USART1TxStr(Data_Buf);//�����ַ���
	//			LCD_ShowString(120+16*6,40+18*6,"send data...");
		wait_OK();//�ȴ����յ�OK�ַ���
	//			LCD_ShowString(120+16*6,40+18*6,"Done !      ");
		CLR_Buf1();     //�������1���ջ���
		CLR_Buf2();     //�������2���ջ���
		for(x = 0;x < 20;x ++)
			AT_send_buf[x] = 0;
		for(x = 0;x < 50;x ++)
			Data_Buf[x] = 0;
		}
}

void esp8266_init()
{
	//����WiFiģ��
	LCD_ShowString(0,50,"Connecting wifi...");
	USART2TxStr("����ģ��...\r\n"); 
	USART1TxStr("AT+RST\r\n");
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
	wait_OK();//�ȴ����յ�OK�ַ���
	LCD_ShowString(0,70,"AT+CWMODE=3");
	//������֪WiFi
	USART2TxStr("������֪WiFi...\r\n");
	USART1TxStr("AT+CWJAP=\"WIFI_100\",\"12345678\"\r\n");
	wait_OK();//�ȴ����յ�OK�ַ���
	LCD_ShowString(0,90,"AT+CWJAP=\"WIFI_100\",\"12345678\"");
	//������ģ������
	USART2TxStr("������ģ������...\r\n");
	USART1TxStr("AT+CIPMUX=1\r\n");
	wait_OK();//�ȴ����յ�OK�ַ���
	LCD_ShowString(0,110,"AT+CIPMUX=1");
	//���ӷ�����
	USART2TxStr("���ӷ�����...\r\n");
	USART1TxStr("AT+CIPSTART=4,\"TCP\",\"192.168.1.200\",6000\r\n");
	wait_OK();//�ȴ����յ�OK�ַ���
  LCD_ShowString(0,130,"AT+CIPSTART=4,\"TCP\",\"192.168.1.200\",6000");	
	USART2TxStr("�뷢��Ӣ���ַ������Իس�������"); 
}


//void wait_OK(void)
//{
////	while(!Flag_usart1_receive_OK);//�ȴ����յ�OK�ַ���
////	Flag_usart1_receive_OK = 0;
////	CLR_Buf1();     //�������1���ջ���
//	
//	//**************���wifi�ϵ�����************************
//		while(!Flag_usart1_receive_OK)//�ȴ����յ�OK�ַ���
//		{
//			if(Flag_timer_15S)
//			{
//				Flag_timer_15S = 0;
//				break;
//			}
//		}

//		if(Flag_usart1_receive_OK)
//		{
//			 nowifi = 0;
//			 Flag_usart1_receive_OK = 0;
//			 CLR_Buf1(); 
//		}
//		else
//		{
//			CLR_Buf1(); 
//			nowifi = 1;
//			esp8266_init();
//		}
//	
//	//******************************************************
//		
//}

void wait1_OK()
{
	while(!Flag_usart1_receive_OK)//�ȴ����յ�OK�ַ���
		{
			if(Flag_timer_15S)
			{
				Flag_timer_15S = 0;
				break;
			}
		}

		if(Flag_usart1_receive_OK)
		{
			 nowifi = 0;
			 Flag_usart1_receive_OK = 0;
			 CLR_Buf1(); 
		}
		else
		{
			CLR_Buf1(); 
			nowifi = 1;
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

void nowifi_Light()
{
	if(flag_time_serv==1||nowifi||Query(USART1_RX_BUF,"Hwctr0104autoT",USART1_REC_LEN))
	{
		//�������
		if(Mode_Flag == 0)
		{
			if(strcmp(Light_value_str,Light_data) < 0)
			{
				PCout(2) = 0;
				Set_Motor_PWM(1125);
				Mode_Flag = 1;
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
			}
			else{
				PCout(2) = 1;
				Set_Motor_PWM(1125);
				Mode_Flag = 1;
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
				delay_ms(1000);
			}
			if(Mode_Flag == 1)
				Set_Motor_PWM(0);
		}
		
	
		//�ɵ�������
		if(strcmp(Light_value_str,Light_data) < 0)
		{
			PCout(6) = 1;
			TIM_SetCompare2(TIM2,1000);
		}
		else
		{
			PCout(6) = 0;
			TIM_SetCompare2(TIM2,0);
		}
	}
}



