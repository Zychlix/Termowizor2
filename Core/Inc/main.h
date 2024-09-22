/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

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
#define LED_ORANGE_Pin GPIO_PIN_6
#define LED_ORANGE_GPIO_Port GPIOI
#define LED_GREEN_Pin GPIO_PIN_4
#define LED_GREEN_GPIO_Port GPIOI
#define LCD_AUX_1_Pin GPIO_PIN_5
#define LCD_AUX_1_GPIO_Port GPIOB
#define LCD_PWM_Pin GPIO_PIN_6
#define LCD_PWM_GPIO_Port GPIOB
#define DES_LOCK_Pin GPIO_PIN_1
#define DES_LOCK_GPIO_Port GPIOA
#define DES_RCLK_Pin GPIO_PIN_0
#define DES_RCLK_GPIO_Port GPIOA
#define SD_DET_A_Pin GPIO_PIN_11
#define SD_DET_A_GPIO_Port GPIOD
#define IO_AUX_Pin GPIO_PIN_5
#define IO_AUX_GPIO_Port GPIOA
#define SW_MODE_Pin GPIO_PIN_12
#define SW_MODE_GPIO_Port GPIOB
#define SW_PICTURE_Pin GPIO_PIN_13
#define SW_PICTURE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */