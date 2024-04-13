/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
 ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM4_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define LED_ON    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0)
#define LED_OFF   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1)

#define MAX_SPEED 40000

#define RX2_Size 9
uint8_t Rx2_Buff[RX2_Size];

uint32_t Motor1_FB = 0;
uint32_t Motor2_FB = 0;
uint32_t Motor3_FB = 0;
int Motor4_FB = 0;
int Motor1_SP = 500;
int Motor2_SP = 2400;
int Motor3_SP = 2400;
int Motor4_SP = 0;
int Motor5_SP = 100;
int Motor6_SP = 100;
int error1 = 0;
int error2 = 0;
int error3 = 0;
int error4 = 0;
int start_time;
int count_grip = 0;

uint32_t read_adc(uint32_t channel){
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint32_t val = HAL_ADC_GetValue(&hadc1);
	return val;
}

void motor1(int v){
	if(v >  MAX_SPEED)  v =  MAX_SPEED;
	if(v < -MAX_SPEED)  v = -MAX_SPEED;
	// ------------- Motor L1
	if(v >= 0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
		TIM4->CCR3 = v;
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
		TIM4->CCR3 = v+65535;
	}
}
void motor2(int v){
	if(v >  MAX_SPEED)  v =  MAX_SPEED;
	if(v < -MAX_SPEED)  v = -MAX_SPEED;
	// ------------- Motor L1
	if(v >= 0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
		TIM4->CCR4 = v;
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
		TIM4->CCR4 = v+65535;
	}
}

void motor3(int v){
	if(v >  MAX_SPEED)  v =  MAX_SPEED;
	if(v < -MAX_SPEED)  v = -MAX_SPEED;
	// ------------- Motor L1
	if(v >= 0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
		TIM4->CCR2 = v;
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
		TIM4->CCR2 = v+65535;
	}
}
void motor4(int sp){
	if(sp < Motor4_FB){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // ENA
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0); // DIR
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // PUL
		HAL_Delay(5);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // PUL
		HAL_Delay(5);
		Motor4_FB--;
	}else if(sp > Motor4_FB){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // ENA
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // DIR
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // PUL
		HAL_Delay(5);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // PUL
		HAL_Delay(5);
		Motor4_FB++;
	}else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // DIR
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1); // ENA
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // PUL
	}
}
void motor5(int val){
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, val);
}
void motor6(int val){
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, val);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0) {
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){
		  //motor1(20000);
		  Motor1_SP = 1000;
	  }
	  else {
		  Motor1_SP = 2000;
//		  motor1(0);
	  }
  } else {
	  if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)){
		  motor1(-20000);
	  }
	  else motor1(0);
  }
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	LED_ON;
	if(huart->Instance == USART2){
		if(Rx2_Buff[0] == 'A') {
		  Motor1_SP = Rx2_Buff[1]*255 + Rx2_Buff[2];
		  ////////////////
		  Motor2_SP = Rx2_Buff[3]*255 + Rx2_Buff[4];
		  ////////////////
		  Motor3_SP = Rx2_Buff[5]*255 + Rx2_Buff[6];
		  ////////////////
		  Motor4_SP = Rx2_Buff[7]*255 + Rx2_Buff[8];
		}
		if(Rx2_Buff[0] == 'S') {
			if(Rx2_Buff[1] == '1') {
				Motor5_SP = Rx2_Buff[2]*255 + Rx2_Buff[3];
			}
			if(Rx2_Buff[1] == '2') {
				Motor6_SP = Rx2_Buff[2]*255 + Rx2_Buff[3];
			}
		}
		if(Rx2_Buff[0] == 'R') {
			int8_t tx_data[] = {
				(uint8_t) Motor1_FB  >> 8, (uint8_t) Motor1_FB  & 0xFF,
				(uint8_t) Motor2_FB  >> 8, (uint8_t) Motor2_FB  & 0xFF,
				(uint8_t) Motor3_FB  >> 8, (uint8_t) Motor3_FB  & 0xFF,
				(uint8_t) Motor4_FB  >> 8, (uint8_t) Motor4_FB  & 0xFF
			};
			HAL_UART_Transmit(&huart2, tx_data, 8, PHY_FULLDUPLEX_10M);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, Rx2_Buff, RX2_Size);
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
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
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	HAL_ADC_Start(&hadc1);
	motor1(0);
	motor2(0);
	start_time = HAL_GetTick();

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 250);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 250);
//	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 750);
//	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 750);

	///////////////////// default
	Motor1_SP = 3800;
	Motor2_SP = 3500;
	Motor3_SP = 2000;

	///////////////////// Grab
//	Motor1_SP = 1600;
//	Motor2_SP = 2100;
//	Motor3_SP = 1800;

//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0); // DIR
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); // ENA
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // PUL
//	for(int j = 0; j<10; j++){
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0); // DIR
//		for(int i=0; i<50; i++){
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // PUL
//			HAL_Delay(5);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // PUL
//			HAL_Delay(5);
//		}
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // DIR
//		for(int i=0; i<50; i++){
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // PUL
//			HAL_Delay(5);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0); // PUL
//			HAL_Delay(5);
//		}
//	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1); // DIR
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1); // ENA
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1); // PUL
	motor1(0);
	motor2(0);
	motor3(0);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, Rx2_Buff, RX2_Size);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 750);
//	  HAL_Delay(3000);
//	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 250);
//	  HAL_Delay(3000);

//	  if(HAL_GetTick() - start_time > 20000){
//		  ///////////////////// default
//			Motor1_SP = 3800;
//			Motor2_SP = 3500;
//			Motor3_SP = 2000;
//		  start_time = HAL_GetTick();
//		  count_grip++;
//	  }
//	  else if(HAL_GetTick() - start_time > 7000){
//		  ///////////////////// Grab
//		  	Motor1_SP = 1600;
//		  	Motor2_SP = 2100;
//		  	Motor3_SP = 1800;
//		  if(HAL_GetTick() - start_time > 12000){
//			if(count_grip%2 == 0){
//				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 750);
//			}else{
//				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 250);
//			}
//		  }
//	  }
	Motor1_FB = read_adc(6);
	Motor2_FB = read_adc(5);
	Motor3_FB = read_adc(4);
	error1 = (Motor1_FB - Motor1_SP)*80;
	if(error1 > -15000 && error1 < 15000) error1 = 0;
	motor1(error1);

	error2 = -(Motor2_FB - Motor2_SP)*90;
	if(error2 > -15000 && error2 < 15000) error2 = 0;
	motor2(error2);

	error3 = (Motor3_FB - Motor3_SP)*90;
	if(error3 > -15000 && error3 < 15000) error3 = 0;
	motor3(error3);

	motor4(Motor4_SP);
	motor5(Motor5_SP);
	motor6(Motor6_SP);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 63;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB3
                           PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
