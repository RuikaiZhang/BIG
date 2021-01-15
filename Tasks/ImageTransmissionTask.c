
/*
 *!
 * \file       :ImageTransmissionTask.c
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
extern u32 *jpeg_buf0;							//JPEG数据缓存buf,通过malloc申请内存
extern u32 *jpeg_buf1;							//JPEG数据缓存buf,通过malloc申请内存
extern u32 *jpeg_data_buf;						//JPEG数据缓存buf,通过malloc申请内存



u8 ov2640_mode = 1;						//工作模式:0,RGB565模式;1,JPEG模式

volatile u32 jpeg_data_len = 0; 			//buf中的JPEG有效数据长度(*4字节)
volatile u8 jpeg_data_ok;				//JPEG数据采集完成标志 
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
void jpeg_data_process(void)
{
	u16 i;
	u16 rlen;//剩余数据长度
	u32 *pbuf;
	if(ov2640_mode)//只有在JPEG格式下,才需要做处理.
	{
		if(jpeg_data_ok==0)	//jpeg数据还未采集完?
		{
			__HAL_DMA_DISABLE(&hdma_dcmi);	//停止当前传输
			printf("jpegdataok == 0\r\n");
			while(DMA2_Stream1->CR&0X01);	//等待DMA2_Stream1可配置   
			rlen=jpeg_dma_bufsize-__HAL_DMA_GET_COUNTER(&hdma_dcmi);//得到剩余数据长度	
			pbuf=jpeg_data_buf+jpeg_data_len;//偏移到有效数据末尾,继续添加
			if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf1[i];//读取buf1里面的剩余数据
			else for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf0[i];//读取buf0里面的剩余数据 
			jpeg_data_len+=rlen;			//加上剩余长度
			jpeg_data_ok=1; 				//标记JPEG数据采集完按成,等待其他函数处理
		}
		if(jpeg_data_ok==2)	//上一次的jpeg数据已经被处理了
		{ 
			printf("jpegdataok == 2\r\n");
			__HAL_DMA_SET_COUNTER(&hdma_dcmi,jpeg_dma_bufsize);//传输长度为jpeg_dma_bufsize*4字节
			__HAL_DMA_ENABLE(&hdma_dcmi); //打开DMA
			jpeg_data_ok=0;					//标记数据未采集
			jpeg_data_len=0;				//数据重新开始
		}
	}
}
 
 
 
//jpeg数据接收回调函数
void jpeg_dcmi_rx_callback(void)
{ 
	u16 i;
	u32 *pbuf;

	pbuf=jpeg_data_buf+jpeg_data_len;//偏移到有效数据末尾
	
	if(DMA2_Stream1->CR&(1<<19))//buf0已满,正常处理buf1
	{ 
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i]=jpeg_buf0[i];//读取buf0里面的数据
		jpeg_data_len+=jpeg_dma_bufsize;//偏移
	}else //buf1已满,正常处理buf0
	{
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i]=jpeg_buf1[i];//读取buf1里面的数据
		jpeg_data_len+=jpeg_dma_bufsize;//偏移 
	} 	
}   
/**
 * \}
 */
   
/* \addtogroup XXXX_Exported_Functions
 * \{
 */
void StartImageTransmissionTask(void const * argument)
{		
	printf("StartImageTransmissionTask\r\n");
		
	u8 j;
	u8* pbuf;
	ov2640_mode=1;
	dcmi_rx_callback=jpeg_dcmi_rx_callback;//回调函数
	while(OV2640_Init())//初始化OV2640
	{
		printf("OV2640 Initing...");
	}
	printf("Ov2640 Initialized\r\n");
	OV2640_JPEG_Mode();		//切换为JPEG模式 
	//OV2640_ImageWin_Set(0,0,1600,1200);			
	OV2640_ImageWin_Set(0,0,1600,1200);			
	//OV2640_OutSize_Set(1600,1200);//拍照尺寸为1600*1200
	OV2640_OutSize_Set(1600,1200);//拍照尺寸为1600*1200
	while(1)
	{
		xEventGroupWaitBits(EventGroupHandler,CAMERA_EVENTBIT,pdFALSE,pdTRUE,osWaitForever);
		printf("EventGroupHandler:%d\r\n",xEventGroupGetBits(EventGroupHandler));
		DCMI_Start(); 			//启动传输 
		for(j=0; j<10; j++)
		{
			printf("image_transmissioin_task正在运行\r\n");
			//printf("%d\r\n", jpeg_buf0);
			jpeg_data_ok=2;	//标记jpeg数据处理完了,可以让DMA去采集下一帧了.
			while(jpeg_data_ok!=1){vTaskDelay(1);};				//等待第一帧图片采集完
			//while(jpeg_data_ok!=1)	//等待第二帧图片采集完
		//jpeg_data_ok=2;			//忽略本帧图片,启动下一帧采集		
			printf("第%d次 jpeg data size:%d\r\n",j, jpeg_data_len*4);//串口打印JPEG文件大
			xEventGroupSetBits(EventGroupHandler,IMG_TRANSFERBIT);
			printf("Waiting for UDP...\r\n");
			xEventGroupWaitBits(EventGroupHandler,IMG_SENT_ONE_PICTUREBIT,pdTRUE,pdFALSE,osWaitForever);
				printf("Send Complete.Eventbit:%x\r\n",xEventGroupGetBits(EventGroupHandler));
//			for(i=0;i<jpeg_data_len*4;i++)		//dma传输1次等于4字节,所以乘以4.
//			{
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);	//循环发送,直到发送完毕  		
//				USART_SendData(USART2,pbuf[i]); 
//			}
//			LCD_ShowString(30,210,210,16,16,"Send data complete!!");//提示传输结束设置 
			jpeg_data_len=0;
			//vTaskDelay(20);
		}
		DCMI_Stop(); 			//停止DMA搬运
		xEventGroupClearBits(EventGroupHandler,CAMERA_EVENTBIT);
	}
}	
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

