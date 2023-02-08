#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	   
#define LCD_W 240
#define LCD_H 320
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)		     
#define OLED_BLK_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_12)//CS
#define OLED_BLK_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
extern  u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ
void Lcd_Init(void); 
void LCD_WR_DATA8(char da); //��������-8λ����
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);
void LCD_Clear(u16 Color);//�������������������������Ļ����Ϻ��������һ��
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	
void LCD_DrawPoint(u16 x,u16 y);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);
void LCD_ShowString(u16 x,u16 y,char *p);		 //��ʾһ���ַ���,16����
void LCD_Show_One_Chinese16x16(unsigned int x,unsigned int y,const char *index);
void LCD_Show_Chinese16x16(unsigned int x,unsigned int y,char *index);

//������ɫ
#define WHITE      	0xFFFF	//��ɫ    //1
#define BLACK      	0x0000	//��ɫ    //2
#define BLUE       	0x001F  //��ɫ		//3
#define BRED       	0XF81F  //�Ƿۺ�	//4
#define GRED 			 	0XFFE0  //����ɫ	//5
#define GBLUE			 	0X07FF  //ǳ����	//6
#define RED        	0xF800  //��ɫ		//7
#define MAGENTA    	0xF81F  //�ۺ�		//8
#define GREEN      	0x07E0  //��ɫ		//9
#define CYAN       	0x7FFF  //����		//10
#define YELLOW     	0xFFE0  //�ʻ�ɫ	//11
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE      	 	0X01CF	//����ɫ
#define LIGHTBLUE      	 	0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 	0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
#define LIGHTGREEN 				0X841F //ǳ��ɫ
#define LGRAY 						0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           	0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
				  		 
#endif 
