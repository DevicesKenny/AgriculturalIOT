#ifndef __GPIO_H
#define __GPIO_H
#define Buzzer_ON() 	GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define Buzzer_OFF() 	GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LED1_ON()			GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED1_OFF()		GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED2_ON()			GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define LED2_OFF()		GPIO_SetBits(GPIOC,GPIO_Pin_14)
#define LED3_ON()			GPIO_ResetBits(GPIOC,GPIO_Pin_15)
#define LED3_OFF()		GPIO_SetBits(GPIOC,GPIO_Pin_15)

void GPIO_Configuration(void);

#endif
