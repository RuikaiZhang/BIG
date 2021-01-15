/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "dcmi.h"
#include "dma.h"
#include "lwip.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcp.h"
#include "ip_addr.h"
#include "sockets.h"
#include "api.h"
#include "portmacro.h"
#include "HC_SR04.h"
#include "tim.h"
#include "dcmi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern u8 TIM2CH3_CAPTURE_STA; //输入捕获状态
extern u32 TIM2CH3_CAPTURE_VAL;//输入捕获值(TIM2/TIM5 是 32 位)
extern u8 TIM2CH4_CAPTURE_STA; //输入捕获状态
extern u32 TIM2CH4_CAPTURE_VAL;//输入捕获值(TIM2/TIM5 是 32 位)
int serverSocket = -1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int c,FILE* pf)
{
	uint8_t ch = (uint8_t)(c & 0xff);
	HAL_UART_Transmit(&huart1, &ch, 1, 0xff);
	return c;
}

//err_t LWIP_TCP_ReceiveCallback(void *arg, struct tcp_pcb *tpcb,struct pbuf *p, err_t err)
//{
//	if(p != NULL)
//	{
//		tcp_write(tpcb,p->payload,p->len,TCP_WRITE_FLAG_COPY);
//		tcp_recved(tpcb,p->tot_len);
//		pbuf_free(p);
//	}
//	else
//	{
//		printf("close\r\n");
//		tcp_close(tpcb);
//	}
//	return ERR_OK;
//}
//err_t LWIP_TCP_AcceptCallback(void *arg, struct tcp_pcb *newpcb, err_t err)
//{
//	tcp_recv(newpcb, LWIP_TCP_ReceiveCallback);
//	return ERR_OK;
//}


//err_t LWIP_TCP_ConnectedCallback(void *arg, struct tcp_pcb *tpcb, err_t err)
//{
//	if(tpcb == NULL)
//	{
//		printf("connect error...%d\r\n",err);
//	}
//	else
//	{
//		tcp_recv(tpcb,LWIP_TCP_ReceiveCallback);
//		printf("connect\r\n");
//	}
//	return ERR_OK;
//}

//int LWIP_TCP_Server(uint16_t port)
//{
//	struct tcp_pcb *bTCP;
//	err_t retval;
//	bTCP = tcp_new();
//	if(bTCP == NULL)
//	{
//		printf("error");
//		return -1;
//	}
//	retval = tcp_bind(bTCP, IP_ADDR_ANY,port);
//	if(retval == ERR_OK)
//	{
//		bTCP = tcp_listen(bTCP);
//		if(bTCP == NULL)
//		{
//			printf("error");
//			return -1;
//		}
//		printf("...\r\n");
//		tcp_accept(bTCP,LWIP_TCP_AcceptCallback);
//		return 0;
//	}
//	memp_free(MEMP_TCP_PCB,bTCP);
//	printf("error");
//	return -1;
//}


//int LWIP_TCP_Client()
//{
//	struct tcp_pcb *bTCP;
////	err_t retval;
//	ip_addr_t tcp_server_ip;
//	bTCP = tcp_new();
//	if(bTCP == NULL)
//	{
//		return -1;
//	}
//	IP_ADDR4(&tcp_server_ip,192,168,1,150);
//	tcp_connect(bTCP,&tcp_server_ip,667,LWIP_TCP_ConnectedCallback);
//	return 0;
//}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  delay_init(168);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
//	delay_init(168);  //初始化延时函数
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_DCMI_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

//  LWIP_TCP_Server(666);
//  LWIP_TCP_Client();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	MX_LWIP_Process();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

	
	
if(htim->Instance == TIM2)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM2CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
					{
						TIM2CH3_CAPTURE_STA|=0X80; //标记成功捕获了一次
						TIM2CH3_CAPTURE_VAL=0XFFFFFFFF;
					}else TIM2CH3_CAPTURE_STA++;
				}
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			if((TIM2CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM2CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
					{
						TIM2CH4_CAPTURE_STA|=0X80; //标记成功捕获了一次
						TIM2CH4_CAPTURE_VAL=0XFFFFFFFF;
					}else TIM2CH4_CAPTURE_STA++;
				}
			}
		}
	}
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
