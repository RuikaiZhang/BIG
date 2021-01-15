
/*
 *!
 * \file       :UDPRXTask.c
 * \brief      : 
 * \version    : 
 * \date       : 2021/01/08
 * \author     : Ruikai Zhang
 *Last modified by Ruikai Zhang 2021/01/08
 *Copyright (c) 2021 by RUIKAIZHANG BIT. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "FreeRTOS_Tasks.h"
/* \defgroup XXXX_Private_TypesDefinitions
 * \{
 */
extern u8 RX_BUF[RX_BUFFER_LEN];
u8 controlch;
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
void StartUDPRXTask(void const * argument)
{
	printf("startUDP_RX_Task\r\n");
	int recv_data_len = 0;
	while(1)
	{
		while(!UDP_FLAG)
		{
			printf("无法接收，UDP初始化中\r\n");
			vTaskDelay(500);
		}
		printf("UDP_RX_Task正在运行\r\n");
		recv_data_len = recvfrom(sock,RX_BUF,1024,0,(struct sockaddr*)&seraddr,&addrlen);
		if(recv_data_len == sizeof(struct transfertype) || recv_data_len == 1)
		{
			controlch = *RX_BUF;
			printf("controlch:%d",controlch);
			xEventGroupSetBits(EventGroupHandler,COMMUNICATE_EVENTBIT);
			printf("UDP_RX_Task\r\n");
		}
		else {printf("%d\r\n",recv_data_len);vTaskDelay(5);}
		recv_data_len = 0;
	}
}
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

