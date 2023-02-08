#include "Motor.h"
#include "sys.h"
#include "delay.h"
unsigned char Motor_run_time = 5;
extern unsigned char count_time_motor;
char motor_time_temp_str[3] = {0,0,0};

//direction 1--正转，0--反转
void Stepper_motor_ctrl(unsigned char direction, unsigned char time)
{
	if(direction != 2)
	{
		if(direction == 0)
			PCout(2) = 1;//PC2输出高电平
		else if(direction == 1)
			 PCout(2) = 0;//PC2输出低电平
		count_time_motor = time * 10;
		Set_Motor_PWM(1125);
	}
	else
		Set_Motor_PWM(0);
}

void Set_Motor_PWM(unsigned int value_Per)
{	
	TIM_SetCompare1(TIM2,value_Per);//设置PWM的占空比
}
void Motor_Mode_display(void)
{
//	POINT_COLOR=YELLOW;
//	LCD_Show_Chinese(Display_addr_D,32*0+10,"  步进电机");
//	if(Device_State == Set_Mode)
//	{
//		LCD_ShowString(16*6,16*12-8,"                     ");	
//		POINT_COLOR=GREEN;
//		LCD_Fill(16*0+5,16*13-8,318,238,BLACK);
//		LCD_Show_Chinese16x16(16*0+5,16*13,"控制端口：脉冲    方向    ");	
//		LCD_ShowString(16*7+5,16*13,"PA0");
//		LCD_ShowString(16*11+5,16*13,"PC2");
//		LCD_ShowString(16*6,16*12-8,"run time:    S");
//		POINT_COLOR=YELLOW;
//	}
//	LCD_Show_Chinese16x16(16*14,16*13,"←");
//	LCD_Show_Chinese16x16(16*18,16*13,"→");
//	if(Flag_Motor_onoffstop == 0)
//		LCD_Show_Chinese16x16(16*15+5,16*13,"正转");
//	else if(Flag_Motor_onoffstop == 1)
//		LCD_Show_Chinese16x16(16*15+5,16*13,"反转");
//	else if(Flag_Motor_onoffstop == 2)
//		LCD_Show_Chinese16x16(16*15+5,16*13,"停止");
	
//	motor_time_temp_str[0] = Motor_run_time / 10 + '0';
//	motor_time_temp_str[1] = Motor_run_time % 10 + '0';	
//	LCD_ShowString(16*11,16*12-8,motor_time_temp_str);
}
