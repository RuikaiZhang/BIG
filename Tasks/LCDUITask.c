
/*
 *!
 * \file       :LCDUITask.c
 * \brief      : 
 * \version    : 
 * \date       : 2021/01/07
 * \author     : Ruikai Zhang
 *Last modified by Ruikai Zhang 2021/01/07
 *Copyright (c) 2021 by RUIKAIZHANG BIT. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "FreeRTOS_Tasks.h"
#include "lcd.h"
#include "graph.h"
/* \defgroup LCDUITask_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup LCDUITask_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup LCDUITask_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup LCDUITask_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup LCDUITask_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup LCDUITask_Private_Functions
 * \{
 */
 /*******************************LCD用户函数定义区********************************/
void LCD_show_percentage(u16 x0,u16 color,uint16_t percentage)
{
	LCD_fill_halfcircle(1,4,x0,112,18,color);//圆11
	LCD_fill_halfcircle(2,4,x0,244,18,color);//圆12
	LCD_draw_vline(x0-17,112,132,4,color);
	LCD_draw_vline(x0+16,112,132,4,color);
	LCD_Fill(x0-18,(u16)(112+132*(1-(float)percentage/100)),x0+18,244,color);//区域填充1
}

void LCD_show_percentage_small(u16 x0,u16 y0)
{
	LCD_fill_halfcircle(1,2,x0-20,y0-12,6,BLUE);//圆11
	LCD_fill_halfcircle(2,2,x0-20,y0+11,6,BLUE);//圆12
	LCD_draw_vline(x0-25,y0-12,24,2,BLUE);
	LCD_draw_vline(x0-16,y0-12,24,2,BLUE);
	LCD_Fill(x0-25,(u16)(y0-12+24*(1-garbagePercentage.RecyclablePercentage)),x0-17,y0+12,BLUE);//区域填充1
	
	LCD_fill_halfcircle(1,2,x0-6,y0-12,6,GREEN);//圆11
	LCD_fill_halfcircle(2,2,x0-6,y0+11,6,GREEN);//圆12
	LCD_draw_vline(x0-11,y0-12,24,2,GREEN);
	LCD_draw_vline(x0-2,y0-12,24,2,GREEN);
	LCD_Fill(x0-11,(u16)(y0-12+24*(1-garbagePercentage.KitchenCondition)),x0-3,y0+12,GREEN);//区域填充1
	
	LCD_fill_halfcircle(1,2,x0+8,y0-12,6,RED);//圆11
	LCD_fill_halfcircle(2,2,x0+8,y0+11,6,RED);//圆12
	LCD_draw_vline(x0+3,y0-12,24,2,RED);
	LCD_draw_vline(x0+12,y0-12,24,2,RED);
	LCD_Fill(x0+3,(u16)(y0-12+24*(1-garbagePercentage.HarmfulCondition)),x0+11,y0+12,RED);//区域填充1
	
	LCD_fill_halfcircle(1,2,x0+22,y0-12,6,GRAY);//圆11
	LCD_fill_halfcircle(2,2,x0+22,y0+11,6,GRAY);//圆12
	LCD_draw_vline(x0+17,y0-12,24,2,GRAY);
	LCD_draw_vline(x0+26,y0-12,24,2,GRAY);
	LCD_Fill(x0+17,(u16)(y0-12+24*(1-garbagePercentage.RecyclablePercentage)),x0+25,y0+12,GRAY);//区域填充1
}

void LCD_show_pagefoot(void)
{
	LCD_ShowBMP(1,5,(u8 **)BIT_Intelligent_Garbage,24,253,1);//标题1
	LCD_ShowBMP(1,280,(u8 **)BIG,24,28,1);//标题2
//		LCD_ShowString(5,280,20,40,24,"BIG");
	LCD_ShowString(70,275,200,15,12,"IP:192.168.1.127");
	LCD_ShowString(70,290,200,15,12,"Port:8087");
	LCD_ShowString(70,305,200,15,12,"Status:IDLE");
	LCD_show_percentage_small(210,300);
}

void LCD_show_messagebox(u8* title,u8* message)
{
	LCD_Fill(20,45,220,270,DARKBLUE);
	LCD_Fill(23,85,217,267,LGRAY);
	BACK_COLOR = DARKBLUE;
	LCD_ShowString(25,53,150,25,24,title);
	BACK_COLOR = LGRAY;
	LCD_ShowString(40,166,160,30,12,message);
}
 
 
 
 
 /****************回调函数定义区********************/
 
void system_UI_callback(void)
{
	LCD_Clear(WHITE);
	
	POINT_COLOR = BLACK;
	LCD_ShowBMP(1,5,(u8 **)BIT_Intelligent_Garbage,24,253,1);//标题1
	
	POINT_COLOR = GRAYBLUE;
	LCD_Show_Graph(25,50,(u8*)recyclable_40,40,1);//可回收物图标
	POINT_COLOR = GREEN;
	LCD_Show_Graph(75,50,(u8*)foodwaste_40,40,1);//厨余垃圾图标
	POINT_COLOR = RED;
	LCD_Show_Graph(125,50,(u8*)hazardouswaste_40,40,1);//有害垃圾图标
	POINT_COLOR = GRAY;
	LCD_Show_Graph(175,50,(u8*)residualwaste_40,40,1);//其他垃圾图标

	LCD_ShowBMP(29,112,(u8 **)recyclable_32,132,68,1);//可回收物字
	LCD_ShowBMP(79,112,(u8 **)foodwaste_32,132,69,1);//厨余垃圾字
	LCD_ShowBMP(129,112,(u8 **)hazardouswaste_32,132,69,1);//有害垃圾字
	LCD_ShowBMP(179,112,(u8 **)residualwaste_32,132,68,1);//其他垃圾字

	LCD_show_percentage(45,GRAYBLUE,garbagePercentage.RecyclablePercentage);
	LCD_show_percentage(95,GREEN,garbagePercentage.KitchenCondition);
	LCD_show_percentage(145,RED,garbagePercentage.HarmfulCondition);
	LCD_show_percentage(195,GRAY,garbagePercentage.OtherCondition);
	
	LCD_ShowBMP(1,5,(u8 **)BIT_Intelligent_Garbage,24,253,1);//标题1
	LCD_ShowBMP(1,280,(u8 **)BIG,24,28,1);//标题2
//		LCD_ShowString(5,280,20,40,24,"BIG");
		LCD_ShowString(10,305,200,15,12,"IP:192.168.1.127");
		LCD_ShowString(120,305,200,15,12,"Port:8087");
		LCD_ShowString(80,280,200,30,16,"Status:IDLE");
	LCD_show_percentage_small(210,300);
};

void img_transfer_UI_callback(void)
{
	LCD_show_messagebox("img_transmitting","    image transmitting\n      progress:80%");
	LCD_show_percentage_small(210,300);
	POINT_COLOR = RED;BACK_COLOR = WHITE;
	LCD_ShowString(80,280,200,30,16,"Status:BUSY");
}

void monitor_UI_callback(void)
{
	
}


void litter_drop_UI_callback(char GarbageType)
{
	switch(GarbageType)
	{
		case(GARBAGE_TYPE_EVENT_RECYCLABLEBIT):
			LCD_show_messagebox("disposing garbage","    disposing garbage\n garbagetype:RECYCLABLE");
			LCD_show_percentage_small(210,300);
			POINT_COLOR = RED;BACK_COLOR = WHITE;
			LCD_ShowString(80,280,200,30,16,"Status:BUSY");
			break;
		case(GARBAGE_TYPE_EVENT_KITCHENBIT):
			LCD_show_messagebox("disposing garbage","    disposing garbage\ngarbagetype:KITCHEN GARBAGE");
			LCD_show_percentage_small(210,300);
			POINT_COLOR = RED;BACK_COLOR = WHITE;
			LCD_ShowString(80,280,200,30,16,"Status:BUSY");
			break;
		case(GARBAGE_TYPE_EVENT_HARMFULBIT):
			LCD_show_messagebox("disposing garbage","    disposing garbage\ngarbagetype:HARMFUL GARBAGE");
			LCD_show_percentage_small(210,300);
			POINT_COLOR = RED;BACK_COLOR = WHITE;
			LCD_ShowString(80,280,200,30,16,"Status:BUSY");
			break;
		case(GARBAGE_TYPE_EVENT_OTHERBIT):
			LCD_show_messagebox("disposing garbage","    disposing garbage\ngarbagetype:OTHER GARBAGE");
			LCD_show_percentage_small(210,300);
			POINT_COLOR = RED;BACK_COLOR = WHITE;
			LCD_ShowString(80,280,200,30,16,"Status:BUSY");
			break;	
	}
}

void err_UI_callback(void)
{
	POINT_COLOR = RED;
	LCD_show_messagebox("system ERROR","        system ERROR\n    please don't litter");
	LCD_show_percentage_small(210,300);
	BACK_COLOR = WHITE;
	LCD_ShowString(72,280,200,30,16,"Status:ERROR");
	POINT_COLOR = BLACK;
}

  
/**
 * \}
 */
   
/* \addtogroup LCDUITask_Exported_Functions
 * \{
 */


 




void StartLCDUITask(void const * argument)
{
	int flag = 1;
	EventBits_t CurrentEventGroupBit;
	EventBits_t currentProcess;
	printf("LCD_UI_Task_running\r\n");
 	LCD_Init();           //初始化LCD FSMC接口
	LCD_Clear(LBGREEN);

	while(1)
	{	
		CurrentEventGroupBit = xEventGroupWaitBits(EventGroupHandler,
																							 LCD_CHANGEUI_BITS,
																							 pdFALSE,
																							 pdFALSE,
																							 LCD_REFRESH_TICKS);
	  currentProcess = (CurrentEventGroupBit & LCD_CHANGEUI_BITS);
		switch(currentProcess)
		{
			case CAMERA_EVENTBIT:
				printf("camera\r\n");
				img_transfer_UI_callback();
				flag = 1;
				break;
			case GARBAGE_TYPE_EVENT_RECYCLABLEBIT:
				if(flag != 2)
				{
					printf("litter drop\r\n");
					litter_drop_UI_callback(GARBAGE_TYPE_EVENT_RECYCLABLEBIT);
					flag = 2;
				}
			  break;
			case GARBAGE_TYPE_EVENT_KITCHENBIT:
				if(flag != 2)
				{
					printf("litter drop\r\n");
					litter_drop_UI_callback(GARBAGE_TYPE_EVENT_KITCHENBIT);
					flag = 1;
				}
				break;
			case GARBAGE_TYPE_EVENT_HARMFULBIT:
				if(flag != 2)
				{
					printf("litter drop\r\n");
					litter_drop_UI_callback(GARBAGE_TYPE_EVENT_HARMFULBIT);
					flag = 2;
				}
				break;
			case GARBAGE_TYPE_EVENT_OTHERBIT:
				if(flag != 2)
				{
					printf("litter drop\r\n");
					litter_drop_UI_callback(GARBAGE_TYPE_EVENT_OTHERBIT);
					flag = 2;
				}
				break;
			case ERR_EVENTBIT:
				flag = 1;
				err_UI_callback();
				break;
			case MONITER_EVENTBIT:
				monitor_UI_callback();
				flag = 1;
				break;
			default:
				if(flag != 0)
				{
					printf("default\r\n");
					system_UI_callback();
					flag = 0;
				}
				break;
		}
	}
//		CurrentEventGroupBit = xEventGroupGetBits(EventGroupHandler);
//		if(CurrentEventGroupBit != PreviousEventGroupBit)
//		{
//			EventBits_t bitChanged = CurrentEventGroupBit ^ PreviousEventGroupBit;
//			PreviousEventGroupBit = CurrentEventGroupBit;
//			switch(bitChanged)
//				case()
//		}
}
/**
 * \}
 */
  
/************************ (C) COPYRIGHT RUIKAIZHANG BIT *****END OF FILE****/

