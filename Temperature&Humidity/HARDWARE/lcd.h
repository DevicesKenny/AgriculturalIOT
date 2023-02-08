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
//-----------------OLED端口定义----------------  					   
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

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
extern  u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色
void Lcd_Init(void); 
void LCD_WR_DATA8(char da); //发送数据-8位参数
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);
void LCD_Clear(u16 Color);//横屏清屏函数，就是在竖屏的基础上横纵坐标改一下
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	
void LCD_DrawPoint(u16 x,u16 y);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);
void LCD_ShowString(u16 x,u16 y,char *p);		 //显示一个字符串,16字体
void LCD_Show_One_Chinese16x16(unsigned int x,unsigned int y,const char *index);
void LCD_Show_Chinese16x16(unsigned int x,unsigned int y,char *index);

//画笔颜色
#define WHITE      	0xFFFF	//白色    //1
#define BLACK      	0x0000	//黑色    //2
#define BLUE       	0x001F  //蓝色		//3
#define BRED       	0XF81F  //亚粉红	//4
#define GRED 			 	0XFFE0  //土黄色	//5
#define GBLUE			 	0X07FF  //浅蓝白	//6
#define RED        	0xF800  //红色		//7
#define MAGENTA    	0xF81F  //粉红		//8
#define GREEN      	0x07E0  //绿色		//9
#define CYAN       	0x7FFF  //蓝白		//10
#define YELLOW     	0xFFE0  //鲜黄色	//11
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      	 	0X7D7C	//浅蓝色  
#define GRAYBLUE       	 	0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN 				0X841F //浅绿色
#define LGRAY 						0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           	0X2B12 //浅棕蓝色(选择条目的反色)
				  		 
#endif 
