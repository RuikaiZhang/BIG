/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS_Tasks.h"
#include "malloc.h"
#include "tcp.h"
#include "string.h"
#include "api.h"
#include "sockets.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
struct garbagePercentageType garbagePercentage;
EventGroupHandle_t EventGroupHandler;
BaseType_t HigherPriorityTaskWoken;

u8 UDP_FLAG;
u32 *jpeg_buf0;							//JPEG数据缓存buf,通过malloc申请内存
u32 *jpeg_buf1;							//JPEG数据缓存buf,通过malloc申请内存
u32 *jpeg_data_buf;						//JPEG数据缓存buf,通过malloc申请内存

int sock;                          //UDP Socket变量
u8 *TX_buf;                  //发送缓冲区
struct sockaddr_in udp_addr,seraddr;    //UDP Socket、UDP服务器描述符
socklen_t addrlen;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ImageTransmissionTaskHandle;
osThreadId LCDUITaskHandle;
osThreadId CommunicateTaskHandle;
osThreadId LitterDropTaskHandle;
osThreadId MonitorTaskHandle;
osThreadId GarbageDetectionTaskHandle;
osThreadId UDPTXTaskHandle;
osThreadId UDPRXTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
extern void StartImageTransmissionTask(void const * argument);
extern void StartLCDUITask(void const * argument);
extern void StartCommunicateTask(void const * argument);
extern void StartLitterDropTask(void const * argument);
extern void StartMonitorTask(void const * argument);
extern void StartGarbageDetectionTask(void const * argument);
extern void StartUDPTXTask(void const * argument);
extern void StartUDPRXTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	EventGroupHandler = xEventGroupCreate();//创建事件标志组
	xEventGroupSetBits(EventGroupHandler,GARBAGE_DISPOSE_FINISH_EVENTBIT);
	my_mem_init(SRAMIN);			//初始化内部内存池
	my_mem_init(SRAMEX);			//初始化外部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
	printf("sram 初始化完成\r\n");
	TX_buf = pvPortMalloc(FRAME_SIZE + sizeof(struct transfertype));

	
	printf("sramin:buf0:%x,buf1:%x\r\n",jpeg_buf0,jpeg_buf1);
	printf("%d",TX_buf);
	jpeg_data_buf = mymalloc(SRAMEX,300*1024);		//为jpeg文件申请内存(最大300KB)
	printf("sramex:%d\r\n",jpeg_data_buf);
	UDP_FLAG = 0;
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ImageTransmissionTask */
  osThreadDef(ImageTransmissionTask, StartImageTransmissionTask, osPriorityAboveNormal, 0, 128);
  ImageTransmissionTaskHandle = osThreadCreate(osThread(ImageTransmissionTask), NULL);

  /* definition and creation of LCDUITask */
  osThreadDef(LCDUITask, StartLCDUITask, osPriorityLow, 0, 128);
  LCDUITaskHandle = osThreadCreate(osThread(LCDUITask), NULL);

  /* definition and creation of CommunicateTask */
  osThreadDef(CommunicateTask, StartCommunicateTask, osPriorityHigh, 0, 128);
  CommunicateTaskHandle = osThreadCreate(osThread(CommunicateTask), NULL);

  /* definition and creation of LitterDropTask */
  osThreadDef(LitterDropTask, StartLitterDropTask, osPriorityNormal, 0, 128);
  LitterDropTaskHandle = osThreadCreate(osThread(LitterDropTask), NULL);

  /* definition and creation of MonitorTask */
  osThreadDef(MonitorTask, StartMonitorTask, osPriorityBelowNormal, 0, 128);
  MonitorTaskHandle = osThreadCreate(osThread(MonitorTask), NULL);

  /* definition and creation of GarbageDetectionTask */
  osThreadDef(GarbageDetectionTask, StartGarbageDetectionTask, osPriorityBelowNormal, 0, 128);
  GarbageDetectionTaskHandle = osThreadCreate(osThread(GarbageDetectionTask), NULL);

  /* definition and creation of UDPTXTask */
  osThreadDef(UDPTXTask, StartUDPTXTask, osPriorityAboveNormal, 0, 256);
  UDPTXTaskHandle = osThreadCreate(osThread(UDPTXTask), NULL);

  /* definition and creation of UDPRXTask */
  osThreadDef(UDPRXTask, StartUDPRXTask, osPriorityAboveNormal, 0, 256);
  UDPRXTaskHandle = osThreadCreate(osThread(UDPRXTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	jpeg_buf0 = pvPortMalloc(jpeg_dma_bufsize*4);	//为jpeg dma接收申请内存	
	jpeg_buf1 = pvPortMalloc(jpeg_dma_bufsize*4);	//为jpeg dma接收申请内存
	int recv_data_len;
	EventBits_t CurrentEventGroupBit;
	memset(jpeg_buf0,3,jpeg_dma_bufsize*4);
	memset(jpeg_buf1,3,jpeg_dma_bufsize*4);
	printf("buf:%d\r\n",*jpeg_buf0);
	sock = -1;
	do{
		sock = socket(AF_INET,SOCK_DGRAM,0);
		printf("Socket error\r\n");
	}while(sock < 0);
	
	printf("Socket created!\r\n");
	
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_addr.s_addr = INADDR_ANY;
	udp_addr.sin_port = htons(668);
	memset(&(udp_addr.sin_zero),0,sizeof(udp_addr.sin_zero));
	
	seraddr.sin_family = AF_INET;
	addrlen = sizeof(struct sockaddr_in);
	printf("sock:%d\r\n",sock);
	printf("addrlen:%d\r\n",addrlen);
	while(bind(sock,(struct sockaddr*)&udp_addr,sizeof(struct sockaddr)) == -1)
	{
		printf("Unable to bind\r\n");
	}
	printf("Bind Successful!\r\n");
	while(1)
	{	
		if(!UDP_FLAG)
		{
			CurrentEventGroupBit = xEventGroupGetBits(EventGroupHandler);
			xEventGroupClearBits(EventGroupHandler,CurrentEventGroupBit);
			printf("defaulttask:sock:%d\r\n",sock);
			printf("defaulttask:addrlen:%d\r\n",addrlen);
			printf("default:%d",xEventGroupGetBits(EventGroupHandler));
			printf("default task:UDP initing...\r\n");
			recv_data_len = recvfrom(sock,RX_BUF,1024,0,(struct sockaddr*)&seraddr,&addrlen);
			printf("recv_data_len:%d\r\n",recv_data_len);
			if(recv_data_len > 0)
			{
				printf("received init sig\r\n");
				sendto(sock,RX_BUF,recv_data_len,0,(struct sockaddr*)&seraddr,addrlen);
				UDP_FLAG = 1;
				printf("UDP Init successful\r\n");
				xEventGroupSetBits(EventGroupHandler,CurrentEventGroupBit);
			}
		}
		else 
		{
			printf("Default task: Nothing todo\r\n");
			vTaskDelay(1000);
		}
	}
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
