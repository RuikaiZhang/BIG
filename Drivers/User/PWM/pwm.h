#ifndef PWM_H
#define PWM_H



#include "stm32f4xx.h" //���ͷ�ļ�һ��Ҫ��

typedef uint32_t    u32;

void TIM3_PWM_Init(u32 arr,u32 psc);

#endif
