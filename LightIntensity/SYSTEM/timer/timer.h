#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h" 

extern unsigned char Count_timer;
extern unsigned char Flag_timer_2S;
extern unsigned char count_time_motor;
extern unsigned char Flag_timer_15S;
extern unsigned char Flag_timer_7S;
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
#endif
