
/*
 *!
 * \file       :CommunicateTask.c
 * \brief      : 
 * \version    : 
 * \date       : 2021/01/07
 * \author     : Ruikai Zhang
 *Last modified by Ruikai Zhang 2021/01/07
 *Copyright (c) 2021 by RUIKAIZHANG BIT. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "FreeRTOS_Tasks.h"
#include "malloc.h"
/* \defgroup CommunicateTask_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup CommunicateTask_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup CommunicateTask_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup CommunicateTask_Private_Variables
 * \{
 */
u8 RX_BUF[RX_BUFFER_LEN];   
/**
 * \}
 */
   
/* \defgroup CommunicateTask_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup CommunicateTask_Private_Functions
 * \{
 */
   
/**
 * \}
 */
   
/* \addtogroup CommunicateTask_Exported_Functions
 * \{
 */
void StartCommunicateTask(void const * argument)
{
	struct transfertype* transferType;
	printf("StartCommunicateTask\r\n");
	while(1)
	{
		printf("communicate taskÕýÔÚÔËÐÐ\r\n");
		xEventGroupWaitBits(EventGroupHandler,
																	 COMMUNICATE_EVENTBIT,
																	 pdTRUE,
																	 pdTRUE,
																	 osWaitForever);
		transferType = (struct transfertype*)RX_BUF;
		printf("controlch:%d\r\n",controlch);
		if(controlch == 'a' || controlch =='b' || controlch =='c' || controlch =='d')
		{
			switch(controlch)
			{
				case('a'):
				{
					printf("garbageTypeRecyclable");
					xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_RECYCLABLEBIT);
					break;
				}
				case('b'):
				{
					printf("garbageTypeKitchen");
					xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_KITCHENBIT);
					break;
				}
				case('c'):
				{
					printf("_HARMFUL");
					xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_HARMFULBIT);
					break;
				}
				case('d'):
				{
					printf("OTHER");
					xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_OTHERBIT);
					break;
				} 
			}
			controlch = 0;
		}
		else
		{
			switch(transferType->frameHead.funCode)
			{
				case(monitorRequest)://¼à¿ØÇëÇó
				{
					xEventGroupSetBits(EventGroupHandler,MONITER_EVENTBIT);
					vTaskResume(MonitorTaskHandle);
					break;
				}
				
				case(garbageTypeReturn)://À¬»øÀàÐÍ·µ»Ø
				{
					switch(transferType->frameHead.funCode)
					{
						case(garbageTypeRecyclable):
						{
							xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_RECYCLABLEBIT);
							break;
						}
						case(garbageTypeKitchen):
						{
							xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_KITCHENBIT);
							break;
						}
						case(garbageTypeHarmful):
						{
							xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_HARMFULBIT);
							break;
						}
						case(garbageTypeOther):
						{
							xEventGroupSetBits(EventGroupHandler,GARBAGE_TYPE_EVENT_OTHERBIT);
							break;
						}
					}
					break;
				}	
				case(monitorEndRequest)://¼à¿ØÍ£Ö¹ÇëÇó
				{
					xEventGroupClearBits(EventGroupHandler,MONITER_EVENTBIT);
					vTaskSuspend(MonitorTaskHandle);
					break;
				}
				case(conditionReturn):
				{
					xEventGroupSetBits(EventGroupHandler,CONDITION_RETURNBIT);
				}
				case(conditionRequest)://À¬»øÍ°×´Ì¬ÇëÇó
				{
					xEventGroupSetBits(EventGroupHandler,CONDITION_REQUESTBIT);
					break;
				}
			}
		}
	}
}   
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

