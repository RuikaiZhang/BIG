#ifndef PWM_H
#define PWM_H



#include "stm32f4xx.h" //这个头文件一定要留

typedef uint32_t    u32;

void TIM3_PWM_Init(u32 arr,u32 psc);

#endif
