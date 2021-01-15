
/*
 *!
 * \file       :UDPTXTask.c
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
   
/**
 * \}
 */
   
/* \defgroup XXXX_Private_Defines
 * \{
 */
#define FRAME_SIZE 1300   
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
void StartUDPTXTask(void const * argument)
{
	printf("startUDP_TX_Task\r\n");
	long int send_size,data_size,leftsize,danci;
	EventBits_t EventBit = 0;
	u8* curr_position;
	struct transfertype transferStruct;
	while(1)
	{
		while(!UDP_FLAG)
		{
			printf("无法发送，UDP初始化中\r\n");
			printf("UDP:sock:%d",sock);
			printf("UDP:addrlen:%d",addrlen);
			vTaskDelay(500);
		}
		printf("UDPTXTask正在运行\r\n");
		EventBit = xEventGroupWaitBits(EventGroupHandler,TX_ENABLEBIT,pdFALSE,pdFALSE,UDP_TXTimeout);
		printf("UDP:Eventbit:%x\r\n",EventBit);
		memset(&transferStruct,0,sizeof(transferStruct));
		curr_position = (u8*)jpeg_data_buf;
		printf("Eventbit:%d",EventBit&TX_ENABLEBIT);
		if((EventBit&TX_ENABLEBIT) == 0x0 )
		{
			printf("UDPTXTimeOut\r\n");
			sendto(sock,"Hello",sizeof("Hello"),0,(struct sockaddr*)&seraddr,addrlen);
			continue;
		}
		else
		{
			switch(EventBit&TX_ENABLEBIT)
			{
				case CONDITION_RETURNBIT:
				{
					printf("发送condition\r\n");
					transferStruct.frameHead.funCode = conditionReturn;
					transferStruct.data.conditionRet = garbagePercentage;
					leftsize = 0;
					danci = sendto(sock,&transferStruct,sizeof(struct transfertype),0,(struct sockaddr*)&seraddr,addrlen);
					xEventGroupClearBits(EventGroupHandler,TX_ENABLEBIT);
					xEventGroupSetBits(EventGroupHandler,GARBAGE_DISPOSE_FINISH_EVENTBIT);
					break;
				}
				
				case IMG_TRANSFERBIT:
				{
					printf("发送图片\r\n");
					transferStruct.frameHead.funCode = garbageTypeRequest;
					transferStruct.data.fileData.totalFrame = jpeg_data_len*4/FRAME_SIZE;
					transferStruct.data.fileData.fileSize = jpeg_data_len*4;
					transferStruct.data.fileData.currentFrame = 0;
					leftsize = jpeg_data_len*4;
					while(leftsize > 0)
					{
						data_size=FRAME_SIZE>leftsize?leftsize:FRAME_SIZE;
						printf("jpeg_data_len:%d\r\n",jpeg_data_len*4);
						printf("datasize:%ld\r\n",data_size);
						send_size = data_size + sizeof(struct transfertype);
						transferStruct.data.fileData.currentFrameLength = data_size;
						printf("transfertype:%d\r\n",sizeof(struct transfertype));
						memcpy(TX_buf,&transferStruct,sizeof(struct transfertype));
						memcpy(TX_buf + sizeof(struct transfertype),curr_position,data_size);
						
						printf("fasong\r\n");
						printf("TX_buf:%d\r\n",TX_buf);
						printf("jpeg_data_buf:%d\r\n",jpeg_data_buf);
						danci = sendto(sock,TX_buf,send_size,0,(struct sockaddr*)&seraddr,addrlen);
						printf("发送%ld数据到服务器...\r\n",danci);
						printf("剩余数据量：%ld",leftsize);
						printf("curr_positionbefore:%d\r\n",curr_position);
						curr_position += data_size;
						printf("curr_positionafter:%d\r\n",curr_position);
						leftsize -= data_size;
						transferStruct.data.fileData.currentFrame += 1;
					}
					xEventGroupClearBits(EventGroupHandler,TX_ENABLEBIT);
					xEventGroupSetBits(EventGroupHandler, IMG_SENT_ONE_PICTUREBIT);
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

