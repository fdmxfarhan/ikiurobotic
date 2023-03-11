/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
 TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int v = 20;
uint8_t buffer[9], s;
int l1, l2, r2, r1;
uint8_t buff[8];
int16_t Pitch=0, Roll=0, Heading=0;
int counter = 0;

uint8_t GY_A5[] = {0xA5};
uint8_t GY_54[] = {0x54};
uint8_t GY_51[] = {0x51};
uint8_t GY_55[] = {0x55};
uint8_t GY_Init_Command[]    = {0xA5, 0x54, 0xA5, 0x51};
uint8_t GY_Request_Command[] = {0xA5, 0x51};



void motor(int L1, int L2, int R2, int R1){
	L1 = L1*255;
	L2 = L2*255;
	R2 = R2*255;
	R1 = R1*255;

	if(L1 > 65535) L1 = 65535;
	if(L2 > 65535) L2 = 65535;
	if(R2 > 65535) R2 = 65535;
	if(R1 > 65535) R1 = 65535;

	if(L1 < -65535) L1 = -65535;
	if(L2 < -65535) L2 = -65535;
	if(R2 < -65535) R2 = -65535;
	if(R1 < -65535) R1 = -65535;

	// ------------------ L1
	if(L1 == 0){
		HAL_GPIO_WritePin(INA1_GPIO_Port, INA1_Pin, 0);
		HAL_GPIO_WritePin(INB1_GPIO_Port, INB1_Pin, 0);
		TIM4->CCR1 = 65535;
	}
	else if(L1 > 0){
		HAL_GPIO_WritePin(INA1_GPIO_Port, INA1_Pin, 1);
		HAL_GPIO_WritePin(INB1_GPIO_Port, INB1_Pin, 0);
		TIM4->CCR1 = L1;
	}else{
		HAL_GPIO_WritePin(INA1_GPIO_Port, INA1_Pin, 0);
		HAL_GPIO_WritePin(INB1_GPIO_Port, INB1_Pin, 1);
		TIM4->CCR1 = -L1;
	}
	// ------------------ L2
	if(L2 == 0){
		HAL_GPIO_WritePin(INA2_GPIO_Port, INA2_Pin, 0);
		HAL_GPIO_WritePin(INB2_GPIO_Port, INB2_Pin, 0);
		TIM4->CCR2 = 65535;
	}
	else if(L2 > 0){
		HAL_GPIO_WritePin(INA2_GPIO_Port, INA2_Pin, 1);
		HAL_GPIO_WritePin(INB2_GPIO_Port, INB2_Pin, 0);
		TIM4->CCR2 = L2;
	}else{
		HAL_GPIO_WritePin(INA2_GPIO_Port, INA2_Pin, 0);
		HAL_GPIO_WritePin(INB2_GPIO_Port, INB2_Pin, 1);
		TIM4->CCR2 = -L2;
	}
	// ------------------ R2
	if(R2 == 0){
		HAL_GPIO_WritePin(INA3_GPIO_Port, INA3_Pin, 0);
		HAL_GPIO_WritePin(INB3_GPIO_Port, INB3_Pin, 0);
		TIM4->CCR3 = 65535;
	}
	else if(R2 > 0){
		HAL_GPIO_WritePin(INA3_GPIO_Port, INA3_Pin, 1);
		HAL_GPIO_WritePin(INB3_GPIO_Port, INB3_Pin, 0);
		TIM4->CCR3 = R2;
	}else{
		HAL_GPIO_WritePin(INA3_GPIO_Port, INA3_Pin, 0);
		HAL_GPIO_WritePin(INB3_GPIO_Port, INB3_Pin, 1);
		TIM4->CCR3 = -R2;
	}
	// ------------------ R1
	if(R1 == 0){
		HAL_GPIO_WritePin(INA4_GPIO_Port, INA4_Pin, 0);
		HAL_GPIO_WritePin(INB4_GPIO_Port, INB4_Pin, 0);
		TIM4->CCR4 = 65535;
	}
	else if(R1 > 0){
		HAL_GPIO_WritePin(INA4_GPIO_Port, INA4_Pin, 1);
		HAL_GPIO_WritePin(INB4_GPIO_Port, INB4_Pin, 0);
		TIM4->CCR4 = R1;
	}else{
		HAL_GPIO_WritePin(INA4_GPIO_Port, INA4_Pin, 0);
		HAL_GPIO_WritePin(INB4_GPIO_Port, INB4_Pin, 1);
		TIM4->CCR4 = -R1;
	}
}
void stop(){
	motor(0,0,0,0);
}
void move(int direction){
	if(direction == 0)  motor( v,  v, -v, -v);
	if(direction == 2)  motor( v,  0, -v,  0);
	if(direction == 4)  motor( v, -v, -v,  v);
	if(direction == 6)  motor( 0, -v,  0,  v);
	if(direction == 8)  motor(-v, -v,  v,  v);
	if(direction == 10) motor(-v,  0,  v,  0);
	if(direction == 12) motor(-v,  v,  v, -v);
	if(direction == 14) motor( 0,  v,  0, -v);
}
void initGY(){
	HAL_Delay(500);
	HAL_UART_Transmit_IT(&huart2, GY_A5, 1);
	HAL_UART_Transmit_IT(&huart2, GY_54, 1);
	HAL_Delay(500);
	HAL_UART_Transmit_IT(&huart2, GY_A5, 1);
	HAL_UART_Transmit_IT(&huart2, GY_51, 1);
	HAL_Delay(500);
	HAL_UART_Transmit_IT(&huart2, GY_A5, 1);
	HAL_UART_Transmit_IT(&huart2, GY_55, 1);
}
void readGY(){
	HAL_UART_Transmit_IT(&huart2, GY_A5, 1);
	HAL_UART_Transmit_IT(&huart2, GY_51, 1);
//	HAL_UART_Receive(&huart2, buff, 8, PHY_FULLDUPLEX_100M);
//	if(buff[0] != 0x55 || buff[1] != 0xAA) return;
//	Heading = (int16_t)(buff[2]<<8 | buff[3])/100.00;
//	Pitch = (int16_t)(buff[4]<<8 | buff[5])/100.00;
//	Roll = (int16_t)(buff[6]<<8 | buff[7])/100.00;
}
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1); // Red LED
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0); // Red LED
  for(int i=0; i<5; i++){
	  HAL_Delay(100);
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14); // Red LED
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Green LED
  }
  RED_OFF;
  GREEN_OFF;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1); // Motors Enable
  HAL_UART_Transmit_DMA(&huart2, GY_Init_Command, sizeof(GY_Init_Command));
  HAL_UART_Receive_DMA(&huart2, buff, sizeof(buff));
//  initGY();
//  HAL_UART_Receive_IT(&huart1, buffer, 9);
//  HAL_UART_Receive_IT(&huart2, buff, 8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	readGY();
	HAL_UART_Transmit_DMA(&huart2, GY_Request_Command, sizeof(GY_Request_Command));
	motor(l1+Heading, l2+Heading, r2+Heading, r1+Heading);
//	motor(v+Heading, v+Heading, -v+Heading, -v+Heading);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  	__HAL_UART_ENABLE_IT(&huart1, UART_IT_TC);
  	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_TC);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, INA2_Pin|INB2_Pin|INA1_Pin|INB1_Pin
                          |MOTORS_EN_Pin|INA4_Pin|INB4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, INA3_Pin|INB3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : INA2_Pin INB2_Pin INA1_Pin INB1_Pin
                           MOTORS_EN_Pin INA4_Pin INB4_Pin */
  GPIO_InitStruct.Pin = INA2_Pin|INB2_Pin|INA1_Pin|INB1_Pin
                          |MOTORS_EN_Pin|INA4_Pin|INB4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : INA3_Pin INB3_Pin */
  GPIO_InitStruct.Pin = INA3_Pin|INB3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	if(huart == &huart1){
//	RED_ON;
//		HAL_UART_Receive_IT(&huart1, buffer, 9);
//		if(buffer[0] == 'M') {
//		  if(buffer[1]=='-') 	r1 =-(int)buffer[2];
//		  else  				r1 = (int)buffer[2];
//		  ////////////////
//		  if(buffer[3]=='-') 	r2 =-(int)buffer[4];
//		  else  				r2 = (int)buffer[4];
//		  ////////////////
//		  if(buffer[5]=='-') 	l2 =-(int)buffer[6];
//		  else  				l2 = (int)buffer[6];
//		  ////////////////
//		  if(buffer[7]=='-') 	l1 =-(int)buffer[8];
//		  else  				l1 = (int)buffer[8];
//		}
//		RED_OFF;
//	}
	if(huart == &huart2){
		GREEN_ON;
		HAL_UART_Receive_DMA(&huart2, buff, sizeof(buff));
		if(buff[0] == 0x55){
			Heading = (int16_t)(buff[2]<<8 | buff[3])/100.00;
			Pitch = (int16_t)(buff[4]<<8 | buff[5])/100.00;
			Roll = (int16_t)(buff[6]<<8 | buff[7])/100.00;
		}else if(buff[0] == 0xAA){
			Heading = (int16_t)(buff[1]<<8 | buff[2])/100.00;
			Pitch = (int16_t)(buff[3]<<8 | buff[4])/100.00;
			Roll = (int16_t)(buff[5]<<8 | buff[6])/100.00;
		}
		GREEN_OFF;
	}
	__NOP();
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();
}


/* USER CODE END 4 */

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
