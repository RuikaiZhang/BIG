#ifndef FREERTOS_TASK_H
#define FREERTOS_TASK_H
/*
 *!
 * \file       : FreeRTOS_Tasks.h
 * \brief      : 
 * \version    : 
 * \date       : 2021/01/07
 * \author     : Ruikai Zhang
 * Last modified by Ruikai Zhang 2021/01/07
 * Copyright (c) 2021 by RUIKAIZHANG BIT. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "HC_SR04.h"
#include "lcd.h"
#include "ov2640.h"
#include "sram.h"
#include "sccb.h"
#include "f4_sys.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "transfer_type.h"
#include "f4_sys.h"
#include "stdio.h"
#include "dcmi.h"
#include "event_groups.h"


#include "sockets.h"
#include "tcp.h"
#include "string.h"
#include "api.h"
/* \defgroup FreeRTOS_Tasks_Exported_TypesDefinitions
 * \{
 */
 
/**
 * \}
 */
   
/* \defgroup FreeRTOS_Tasks_Exported_Defines
 * \{
 */
#define MAX_TICKS_TO_WAIT 0xffffffff
#define LCD_REFRESH_TICKS 0xffffffff
#define TICKS_WAITFOR_TYPERETURN 0xffffffff

//事件位
#define COMMUNICATE_EVENTBIT	(1<<0)											 //上位机接收事件
#define CAMERA_EVENTBIT	(1<<1)														 //拍照事件
#define IMAGE_TRANSFER_COMPLETEBIT	(1<<2)								 //图像传输完成事件
//垃圾类别事件
#define GARBAGE_TYPE_EVENT_RECYCLABLEBIT	(1<<3)
#define GARBAGE_TYPE_EVENT_KITCHENBIT  (1<<4)
#define GARBAGE_TYPE_EVENT_HARMFULBIT  (1<<5)
#define GARBAGE_TYPE_EVENT_OTHERBIT	 (1<<6)
#define GARBAGE_TYPE_EVENT_ALL  (GARBAGE_TYPE_EVENT_RECYCLABLEBIT|GARBAGE_TYPE_EVENT_KITCHENBIT|GARBAGE_TYPE_EVENT_HARMFULBIT|GARBAGE_TYPE_EVENT_OTHERBIT)
#define GARBAGE_TYPE_EVENT_ANY  (GARBAGE_TYPE_EVENT_RECYCLABLEBIT||GARBAGE_TYPE_EVENT_KITCHENBIT||GARBAGE_TYPE_EVENT_HARMFULBIT||GARBAGE_TYPE_EVENT_OTHERBIT)

#define GARBAGE_DISPOSE_FINISH_EVENTBIT  (1<<7)							 //垃圾处理完成事件
#define BIG_IDLE_EVENTBIT		GARBAGE_DISPOSE_FINISH_EVENTBIT
#define MONITER_EVENTBIT  (1<<8)													 //监控事件
#define ERR_EVENTBIT  (1<<9)															 //错误事件
#define CONDITION_REQUESTBIT  (1<<10)                      //垃圾桶状态请求事件
#define CONDITION_RETURNBIT  (1<<11)                       //UDP发送垃圾桶情况事件
#define IMG_TRANSFERBIT  (1<<12)                           //UDP传送图片事件
#define IMG_SENT_ONE_PICTUREBIT  (1<<13)                   //传输一张图片完成事件
//#define UDP_RX_ENBIT  (1<<14)                              //UDP接收使能

#define TX_ENABLEBIT  (CONDITION_RETURNBIT|IMG_TRANSFERBIT)//串口任务判断发送事件

#define LCD_CHANGEUI_BITS  (CAMERA_EVENTBIT|IMAGE_TRANSFER_COMPLETEBIT|GARBAGE_TYPE_EVENT_ALL|ERR_EVENTBIT|MONITER_EVENTBIT|BIG_IDLE_EVENTBIT)

#define UDP_TXTimeout     5000


#define RX_BUFFER_LEN  1024
#define FRAME_SIZE 1300  
/**
 * \}
 */
   
/* \defgroup FreeRTOS_Tasks_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FreeRTOS_Tasks_Exported_Variables
 * \{
 */
extern struct garbagePercentageType garbagePercentage;   
extern EventGroupHandle_t EventGroupHandler;
extern BaseType_t HigherPriorityTaskWoken;

extern u8 RX_BUF[RX_BUFFER_LEN];

extern osThreadId MonitorTaskHandle;

extern u8 ov2640_mode;						//工作模式:0,RGB565模式;1,JPEG模式

extern volatile u32 jpeg_data_len; 			//buf中的JPEG有效数据长度(*4字节)
extern volatile u8 jpeg_data_ok;				//JPEG数据采集完成标志 

extern u32 *jpeg_buf0;							//JPEG数据缓存buf,通过malloc申请内存
extern u32 *jpeg_buf1;							//JPEG数据缓存buf,通过malloc申请内存
extern u32 *jpeg_data_buf;						//JPEG数据缓存buf,通过malloc申请内存

extern int sock;                          //UDP Socket变量
extern u8 *TX_buf;                  //发送缓冲区
extern struct sockaddr_in udp_addr,seraddr;    //UDP Socket、UDP服务器描述符
extern socklen_t addrlen;
extern u8 UDP_FLAG;
extern u8 controlch;

extern void jpeg_data_process(void);
/**
 * \}
 */
   
/* \defgroup FreeRTOS_Tasks_Exported_Functions
 * \{
 */
/****************task定义*************/
extern void StartImageTransmissionTask(void const * argument);
extern void StartLCDUITask(void const * argument);
extern void StartCommunicateTask(void const * argument);
extern void StartLitterDropTask(void const * argument);
extern void StartMonitorTask(void const * argument);
extern void StartGarbageDetectionTask(void const * argument);
extern void StartUDPRXTask(void const * argument);
extern void StartUDPTXTask(void const * argument);

/************LCD应用层函数***********/
void LCD_show_percentage(u16 x0,u16 color,uint16_t percentage);
void LCD_show_percentage_small(u16 x0,u16 y0);
void LCD_show_pagefoot(void);
void LCD_show_messagebox(u8* title,u8* message);

/************LCD回调函数**************/
void system_UI_callback(void);
void img_transfer_UI_callback(void);
void litter_drop_UI_callback(char GarbageType);
void err_UI_callback(void);
void monitor_UI_callback(void);
/**
 * \}
 */
#endif  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

