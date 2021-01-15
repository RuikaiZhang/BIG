
/*
 *!
 * \file       :LitterDropTask.c
 * \brief      : 
 * \version    : 
 * \date       : 2021/01/07
 * \author     : Ruikai Zhang
 *Last modified by Ruikai Zhang 2021/01/07
 *Copyright (c) 2021 by RUIKAIZHANG BIT. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "FreeRTOS_Tasks.h"
#include "stm32f4xx_hal_tim.h"
/* \defgroup XXXX_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Private_Functions
 * \{
 */
void MX996RDrive(u8 cur_pos,u8 des)
{
	int step;
	step = des - cur_pos;
	printf("step:%d",step);
	if(step > 0)
	{
		for(;cur_pos < des;cur_pos += 2)
		{
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,cur_pos);
			vTaskDelay(10);
		}
	}
	else
	{
		for(;cur_pos > des;cur_pos -= 2)
		{
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,cur_pos);
			vTaskDelay(10);
		}
	}
}
/**
 * \}
 */
   
/* \addtogroup XXXX_Exported_Functions
 * \{
 */
void StartLitterDropTask(void const * argument)
{
	uint32_t dist = 0;
	u8 t = 0;
	u8 cur_pos = 50;
	EventBits_t Eventbit;
	int i;
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,100);
	for(i = 50; i < 250; i += 5)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,i);
		vTaskDelay(5);
		printf("%d\r\n",i);
	}
		for(i = 250; i > 50; i -= 5)
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,i);
		vTaskDelay(5);
		printf("%d\r\n",i);
	}
	while(1)
	{
		printf("LitterDropTask正在运行\r\n");
		Eventbit = xEventGroupWaitBits(EventGroupHandler,
																	 GARBAGE_TYPE_EVENT_ALL,
																	 pdFALSE,
																	 pdFALSE,
																	 osWaitForever);
		switch(Eventbit & GARBAGE_TYPE_EVENT_ALL)
		{
			case (GARBAGE_TYPE_EVENT_RECYCLABLEBIT):
				MX996RDrive(cur_pos,50);
				cur_pos = 50;				
				break;
			case (GARBAGE_TYPE_EVENT_KITCHENBIT):
				MX996RDrive(cur_pos,100);
				cur_pos = 100;
				break;			
			case (GARBAGE_TYPE_EVENT_HARMFULBIT):
				MX996RDrive(cur_pos,150);
				cur_pos = 150;				
				break;
			case (GARBAGE_TYPE_EVENT_OTHERBIT):
				MX996RDrive(cur_pos,200);
				cur_pos = 200;				
				break;
		}
		printf("current_pos:%d\r\n",cur_pos);
		vTaskDelay(1000);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,115);
		vTaskDelay(3000);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,95);
		
		printf("getting trashbin condition\r\n");
		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_4);
		__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);

			Trig_Send(1);
			while(dist==0)
			{
				dist = Get_Dist(1);
				vTaskDelay(5);//等待返回值
				t++;
				if(t >4)
					break;
			}
			t = 0;
			printf("dist2:%d\r\n",dist);
			

	

		
		switch(Eventbit & GARBAGE_TYPE_EVENT_ALL)
		{
			case (GARBAGE_TYPE_EVENT_RECYCLABLEBIT):
				garbagePercentage.RecyclablePercentage = dist;
				break;
			case (GARBAGE_TYPE_EVENT_KITCHENBIT):
				garbagePercentage.KitchenCondition = dist;
				break;
			case (GARBAGE_TYPE_EVENT_HARMFULBIT):
				garbagePercentage.HarmfulCondition = dist;
				break;
			case (GARBAGE_TYPE_EVENT_OTHERBIT):
				garbagePercentage.OtherCondition = dist;
				break;
		}
		xEventGroupClearBits(EventGroupHandler,GARBAGE_TYPE_EVENT_ALL);
		xEventGroupSetBits(EventGroupHandler,CONDITION_RETURNBIT);
	}
}   
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

