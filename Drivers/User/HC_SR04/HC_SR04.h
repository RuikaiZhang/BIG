#ifndef __HC_SR04_H
#define __HC_SR04_H

//ͷ�ļ�
#include "stm32f4xx.h" //���ͷ�ļ�һ��Ҫ��
#include "tim.h"
#include "delay.h"
#include "stdio.h"


//#include "sysTick.h" //�δ�ʱ��
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;

//��������
void HC_SR04_Init(u32 arr,u16 psc);
void Trig_Send(u8 i);
uint16_t Get_Dist(u8 i);

#endif
