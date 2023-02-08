#ifndef __MOTOR_H
#define __MOTOR_H	

extern unsigned char Motor_run_time;
extern char motor_time_temp_str[3];
void Set_Motor_PWM(unsigned int value_Per);
void Motor_Mode_display(void);
void Stepper_motor_ctrl(unsigned char direction, unsigned char time);
#endif
