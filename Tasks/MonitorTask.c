
/*
 *!
 * \file       :MonitorTask.c
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
void StartMonitorTask(void const * argument)
{
	while(1)
	{
		printf("StartMonitorTask\r\n");
		xEventGroupWaitBits(EventGroupHandler,MONITER_EVENTBIT,pdFALSE,pdTRUE,osWaitForever);
		vTaskDelay(500);
	}
}  
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

