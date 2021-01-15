/**
  ******************************************************************************
  * @file    dcmi.h
  * @brief   This file contains all the function prototypes for
  *          the dcmi.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCMI_H__
#define __DCMI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;
typedef __IO uint16_t vu16;
typedef __IO uint32_t vu32;
/* USER CODE END Includes */

extern DCMI_HandleTypeDef hdcmi;

/* USER CODE BEGIN Private defines */

#define jpeg_dma_bufsize	3*1024

/* USER CODE END Private defines */

void MX_DCMI_Init(void);

/* USER CODE BEGIN Prototypes */
extern void (*dcmi_rx_callback)(void);//DCMI DMA接收回调函数
extern void jpeg_data_process(void);
extern u32 *jpeg_buf1;							//JPEG数据缓存buf,通过malloc申请内存
extern u32 *jpeg_buf0;							//JPEG数据缓存buf,通过malloc申请内存
extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma_dcmi;

void DCMI_Start(void);
void DCMI_Stop(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __DCMI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
