/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HC_SR04_ECHO01_Pin GPIO_PIN_3
#define HC_SR04_ECHO01_GPIO_Port GPIOA
#define HC_SR04_ECHO02_Pin GPIO_PIN_10
#define HC_SR04_ECHO02_GPIO_Port GPIOB
#define LCD_BackLight_Pin GPIO_PIN_15
#define LCD_BackLight_GPIO_Port GPIOB
#define HC_SR04_TRIG2_Pin GPIO_PIN_7
#define HC_SR04_TRIG2_GPIO_Port GPIOG
#define HC_SR04_TRIG1_Pin GPIO_PIN_8
#define HC_SR04_TRIG1_GPIO_Port GPIOG
#define DCMI_XCLK_Pin GPIO_PIN_8
#define DCMI_XCLK_GPIO_Port GPIOA
#define ETH_RESET_Pin GPIO_PIN_3
#define ETH_RESET_GPIO_Port GPIOD
#define DCMI_SCL_Pin GPIO_PIN_6
#define DCMI_SCL_GPIO_Port GPIOD
#define DCMI_SDA_Pin GPIO_PIN_7
#define DCMI_SDA_GPIO_Port GPIOD
#define DCMI_PWDN_Pin GPIO_PIN_9
#define DCMI_PWDN_GPIO_Port GPIOG
#define DCMI_RST_Pin GPIO_PIN_15
#define DCMI_RST_GPIO_Port GPIOG
#define MX996R_Pin GPIO_PIN_4
#define MX996R_GPIO_Port GPIOB
#define SG90_Pin GPIO_PIN_5
#define SG90_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
