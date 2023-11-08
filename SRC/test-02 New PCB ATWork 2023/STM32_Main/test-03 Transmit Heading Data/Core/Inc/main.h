/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INA2_Pin GPIO_PIN_12
#define INA2_GPIO_Port GPIOB
#define INB2_Pin GPIO_PIN_13
#define INB2_GPIO_Port GPIOB
#define INA1_Pin GPIO_PIN_14
#define INA1_GPIO_Port GPIOB
#define INB1_Pin GPIO_PIN_15
#define INB1_GPIO_Port GPIOB
#define INA3_Pin GPIO_PIN_11
#define INA3_GPIO_Port GPIOA
#define INB3_Pin GPIO_PIN_12
#define INB3_GPIO_Port GPIOA
#define MOTORS_EN_Pin GPIO_PIN_3
#define MOTORS_EN_GPIO_Port GPIOB
#define INA4_Pin GPIO_PIN_4
#define INA4_GPIO_Port GPIOB
#define INB4_Pin GPIO_PIN_5
#define INB4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define GREEN_ON  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1) // Green LED
#define GREEN_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0) // Green LED
#define RED_ON    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 1) // Red LED
#define RED_OFF   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0) // Red LED
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
