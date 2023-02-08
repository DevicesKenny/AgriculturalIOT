
extern char flag_time_reset;
extern char flag_time_wifi;
extern char flag_time_serv;
extern char number;

void Connect_wifi(void);
void wait_OK_WIFI(void);
void wait_OK(void);
unsigned char Query(char * src,char * des,unsigned int LEN);
