#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "stdlib.h"
#include "lcdfont.h"
#include "spi.h"

u8 ref=0;//刷新显示
u16 vx=15542,vy=11165;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx=140,chy=146;//默认像素点坐标为0时的AD起始值

u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色
void LCD_Writ_Bus(char dat)   //串行数据写入
{	
	SPI2_ReadWriteByte(dat);
}
void LCD_WR_DATA8(char da) //发送数据-8位参数
{	
  OLED_DC_Set();
	LCD_Writ_Bus(da);  
}  
 void LCD_WR_DATA(int da)
{
  OLED_DC_Set();
	LCD_Writ_Bus(da>>8);
  LCD_Writ_Bus(da);
}	  
void LCD_WR_REG(char da)	 
{
  OLED_DC_Clr();
	LCD_Writ_Bus(da);
}
 void LCD_WR_REG_DATA(int reg,int da)
{
  LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(x1>>8);
	LCD_WR_DATA8(x1);
	LCD_WR_DATA8(x2>>8);
	LCD_WR_DATA8(x2);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(y1>>8);
	LCD_WR_DATA8(y1);
	LCD_WR_DATA8(y2>>8);
	LCD_WR_DATA8(y2);
	LCD_WR_REG(0x2C);					 						 
}
void Lcd_Init(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 ;//设置控制LCD的DC、RES、BLK信号引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
 	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉
	
	SPI2_Init();		   	//初始化SPI
	
	OLED_RST_Clr();
	delay_ms(20);
	OLED_RST_Set();
	delay_ms(20);
	OLED_BLK_Set();	
//************* Start Initial Sequence **********//
	
	LCD_WR_REG(0xCF); 	
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0xD9); 
	LCD_WR_DATA8(0X30); 
	 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA8(0x64); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0X12); 
	LCD_WR_DATA8(0X81); 
	 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA8(0x85); 
	LCD_WR_DATA8(0x10); 
	LCD_WR_DATA8(0x78); 
	 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x34); 
	LCD_WR_DATA8(0x02); 
	 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA8(0x20); 
	 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA8(0x21);   //VRH[5:0] 
	 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA8(0x12);   //SAP[2:0];BT[3:0] 
	 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA8(0x32); 
	LCD_WR_DATA8(0x3C); 
	 
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA8(0XC1); 
	 
	LCD_WR_REG(0x36);    // Memory Access Control 
	//LCD_WR_DATA8(0x08); //竖屏
	LCD_WR_DATA8(0xB8); //横屏 1011 1000  左右颠倒改第一个位，上下颠倒改第二个位

	LCD_WR_REG(0x3A);   
	LCD_WR_DATA8(0x55); 

	LCD_WR_REG(0xB1);   
	LCD_WR_DATA8(0x00);   
	LCD_WR_DATA8(0x18); 
	 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0xA2); 
 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA8(0x01); 
	 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA8(0x0F); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x1E); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x0B); 
	LCD_WR_DATA8(0x50); 
	LCD_WR_DATA8(0XBA); 
	LCD_WR_DATA8(0x44); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x14); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x21); 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x19); 
	LCD_WR_DATA8(0x19); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x3F); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x25); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x0F); 
	 
	LCD_WR_REG(0x11);    //Exit Sleep 
	delay_ms(120); 
	LCD_WR_REG(0x29);    //Display on 

	LCD_Clear(BLACK); //清屏 
}
void LCD_Clear(u16 Color)//横屏清屏函数，就是在竖屏的基础上横纵坐标改一下
{
	u16 i,j;  	
	Address_set(0,0,LCD_H-1,LCD_W-1);
  for(i=0;i<LCD_H;i++)
	{
	  for (j=0;j<LCD_W;j++)
		{
				LCD_WR_DATA(Color);	 			 
		}
	}
}
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
} 
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
} 
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
  u8 temp;
  u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
	if(x>LCD_H-16||y>LCD_W-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	Address_set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
} 
void LCD_ShowString(u16 x,u16 y,char *p)
{//显示一个字符串         
	while(*p!='\0')
	{       
		if(x>LCD_H-16){x=0;y+=16;}
		if(y>LCD_W-16){y=x=0;LCD_Clear(RED);}
		LCD_ShowChar(x,y,*p,0);
		x+=8;
		p++;
	}  
}
void LCD_Show_One_Chinese16x16(unsigned int x,unsigned int y,const char *index)
{
	unsigned char *temp;
	unsigned char j,i;
	unsigned int k,num,num1;
	Address_set(x,y,x+15,y+15); //设置区域
	num = sizeof(codeGB_16x16)/sizeof(codeGB_16x16[0]);//
	num1 = sizeof(codeGB_16x16[0].Msk);
	for(k = 0;k < num;k ++)
	{
		if((codeGB_16x16[k].Index[0] == *index) && (codeGB_16x16[k].Index[1] == *(index+1)))
		{
			temp = (unsigned char*)&codeGB_16x16[k].Msk[0];
			
			for(j=0;j<num1;j++)
			{
				for(i=0;i<8;i++)
				{ 		     
					if((*temp&(1<<i))!=0)
					{
						LCD_WR_DATA(POINT_COLOR);
					} 
					else
					{
						LCD_WR_DATA(BACK_COLOR);
					}   
				}
				temp++;
			}
			break;
		}		
	}
}
void LCD_Show_Chinese16x16(unsigned int x,unsigned int y,char *index)
{
	unsigned char len = 0;
	while(*index)
	{
		LCD_Show_One_Chinese16x16(x+len*16,y,index);
		index += 2;
		len += 1;
	}		
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}



