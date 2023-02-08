#ifndef	wifi_h
#define	wifi_h

extern char flag_time_reset;
extern char flag_time_wifi;
extern char flag_time_serv;
extern char Num_wifi_times;
extern unsigned char Flag_wifi_receive_tosend;
extern unsigned char Flag_have_data_need_send ;
extern unsigned char Flag_data_send_instart ;

extern char AT_send_buf[20];
extern char Data_Buf[100];

void wait_OK_WIFI(void);
void wait_OK(void);
void wait_dayuhao(void);
void Set_PWM(unsigned int value_Per);
void Have_data_need_send(void);
void Connect_wifi(void);
void copy_str(char* des,char* src,unsigned char len);
unsigned char Get_data_len(char* addr);//获取字符串/数组有效数据长度
unsigned char Query(char * src,char * des,unsigned int LEN);

#endif
