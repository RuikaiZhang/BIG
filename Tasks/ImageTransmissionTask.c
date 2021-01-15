
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
extern u32 *jpeg_buf0;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
extern u32 *jpeg_buf1;							//JPEG���ݻ���buf,ͨ��malloc�����ڴ�
extern u32 *jpeg_data_buf;						//JPEG���ݻ���buf,ͨ��malloc�����ڴ�



u8 ov2640_mode = 1;						//����ģʽ:0,RGB565ģʽ;1,JPEGģʽ

volatile u32 jpeg_data_len = 0; 			//buf�е�JPEG��Ч���ݳ���(*4�ֽ�)
volatile u8 jpeg_data_ok;				//JPEG���ݲɼ���ɱ�־ 
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
	u16 rlen;//ʣ�����ݳ���
	u32 *pbuf;
	if(ov2640_mode)//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{
			__HAL_DMA_DISABLE(&hdma_dcmi);	//ֹͣ��ǰ����
			printf("jpegdataok == 0\r\n");
			while(DMA2_Stream1->CR&0X01);	//�ȴ�DMA2_Stream1������   
			rlen=jpeg_dma_bufsize-__HAL_DMA_GET_COUNTER(&hdma_dcmi);//�õ�ʣ�����ݳ���	
			pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ,�������
			if(DMA2_Stream1->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf1[i];//��ȡbuf1�����ʣ������
			else for(i=0;i<rlen;i++)pbuf[i]=jpeg_buf0[i];//��ȡbuf0�����ʣ������ 
			jpeg_data_len+=rlen;			//����ʣ�೤��
			jpeg_data_ok=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{ 
			printf("jpegdataok == 2\r\n");
			__HAL_DMA_SET_COUNTER(&hdma_dcmi,jpeg_dma_bufsize);//���䳤��Ϊjpeg_dma_bufsize*4�ֽ�
			__HAL_DMA_ENABLE(&hdma_dcmi); //��DMA
			jpeg_data_ok=0;					//�������δ�ɼ�
			jpeg_data_len=0;				//�������¿�ʼ
		}
	}
}
 
 
 
//jpeg���ݽ��ջص�����
void jpeg_dcmi_rx_callback(void)
{ 
	u16 i;
	u32 *pbuf;

	pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ
	
	if(DMA2_Stream1->CR&(1<<19))//buf0����,��������buf1
	{ 
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i]=jpeg_buf0[i];//��ȡbuf0���������
		jpeg_data_len+=jpeg_dma_bufsize;//ƫ��
	}else //buf1����,��������buf0
	{
		for(i=0;i<jpeg_dma_bufsize;i++)pbuf[i]=jpeg_buf1[i];//��ȡbuf1���������
		jpeg_data_len+=jpeg_dma_bufsize;//ƫ�� 
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
	dcmi_rx_callback=jpeg_dcmi_rx_callback;//�ص�����
	while(OV2640_Init())//��ʼ��OV2640
	{
		printf("OV2640 Initing...");
	}
	printf("Ov2640 Initialized\r\n");
	OV2640_JPEG_Mode();		//�л�ΪJPEGģʽ 
	//OV2640_ImageWin_Set(0,0,1600,1200);			
	OV2640_ImageWin_Set(0,0,1600,1200);			
	//OV2640_OutSize_Set(1600,1200);//���ճߴ�Ϊ1600*1200
	OV2640_OutSize_Set(1600,1200);//���ճߴ�Ϊ1600*1200
	while(1)
	{
		xEventGroupWaitBits(EventGroupHandler,CAMERA_EVENTBIT,pdFALSE,pdTRUE,osWaitForever);
		printf("EventGroupHandler:%d\r\n",xEventGroupGetBits(EventGroupHandler));
		DCMI_Start(); 			//�������� 
		for(j=0; j<10; j++)
		{
			printf("image_transmissioin_task��������\r\n");
			//printf("%d\r\n", jpeg_buf0);
			jpeg_data_ok=2;	//���jpeg���ݴ�������,������DMAȥ�ɼ���һ֡��.
			while(jpeg_data_ok!=1){vTaskDelay(1);};				//�ȴ���һ֡ͼƬ�ɼ���
			//while(jpeg_data_ok!=1)	//�ȴ��ڶ�֡ͼƬ�ɼ���
		//jpeg_data_ok=2;			//���Ա�֡ͼƬ,������һ֡�ɼ�		
			printf("��%d�� jpeg data size:%d\r\n",j, jpeg_data_len*4);//���ڴ�ӡJPEG�ļ���
			xEventGroupSetBits(EventGroupHandler,IMG_TRANSFERBIT);
			printf("Waiting for UDP...\r\n");
			xEventGroupWaitBits(EventGroupHandler,IMG_SENT_ONE_PICTUREBIT,pdTRUE,pdFALSE,osWaitForever);
				printf("Send Complete.Eventbit:%x\r\n",xEventGroupGetBits(EventGroupHandler));
//			for(i=0;i<jpeg_data_len*4;i++)		//dma����1�ε���4�ֽ�,���Գ���4.
//			{
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);	//ѭ������,ֱ���������  		
//				USART_SendData(USART2,pbuf[i]); 
//			}
//			LCD_ShowString(30,210,210,16,16,"Send data complete!!");//��ʾ����������� 
			jpeg_data_len=0;
			//vTaskDelay(20);
		}
		DCMI_Stop(); 			//ֹͣDMA����
		xEventGroupClearBits(EventGroupHandler,CAMERA_EVENTBIT);
	}
}	
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

