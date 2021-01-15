
/*
 *!
 * \file       :GarbageDetectionTask.c
 * \brief      : 
 * \version    : 
 * \date       : 2021/01/07
 * \author     : Ruikai Zhang
 *Last modified by Ruikai Zhang 2021/01/07
 *Copyright (c) 2021 by RUIKAIZHANG BIT. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "FreeRTOS_Tasks.h"   
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
   
/**
 * \}
 */
   
/* \addtogroup XXXX_Exported_Functions
 * \{
 */
void StartGarbageDetectionTask(void const * argument)
{
	uint32_t dist = 0;
	uint8_t t = 0;
	while(1)
	{		
		printf("garbage_detection_task RUNNING\r\n");
		printf("EventGroupHandler:%d\r\n",xEventGroupGetBits(EventGroupHandler));
		xEventGroupWaitBits(EventGroupHandler,
											 GARBAGE_DISPOSE_FINISH_EVENTBIT,
											 pdFALSE,
											 pdFALSE,
											 osWaitForever);
		HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
		__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);
		Trig_Send(0);
		while(dist==0)
		{
			dist = Get_Dist(0);
			vTaskDelay(20);//µÈ´ı·µ»ØÖµ
			++t;
			if(t > 4)
			{
				break;
			}
		}
		t = 0;
		printf("dist = %d\r\n", dist);
		if(dist>0 && dist < 120)
		{ 
			xEventGroupClearBits(EventGroupHandler,BIG_IDLE_EVENTBIT);
			xEventGroupSetBits(EventGroupHandler,CAMERA_EVENTBIT);
		}
		dist = 0;
		vTaskDelay(500);
	}
}
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

