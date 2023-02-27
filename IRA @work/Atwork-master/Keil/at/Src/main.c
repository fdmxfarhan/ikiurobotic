/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

uint8_t buffer,buffer2[3],s,k,d,distance;
int32_t l1,l2,r1,r2;
uint8_t data[];
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t read_adc(int index)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,(index/8)%2); //D
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,(index/4)%2); //C
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,(index/2)%2); //B
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0 ,(index/1)%2); //A
	
	int chanel = 1;
	uint16_t val;
	ADC_ChannelConfTypeDef sconfig;
	sconfig.Channel = chanel;
	sconfig.Rank = 1;
	HAL_ADC_ConfigChannel(&hadc1,&sconfig);
	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1,100);
	val = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return val;
}

void step(int32_t st)
	{
		d=20;
		if(st==0){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
			HAL_Delay(50);
		}
		if(st==1){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
			HAL_Delay(d);
		}
		if(st==2){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
			HAL_Delay(d);
		}
		if(st==3){		
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
			HAL_Delay(d);
		}
		if(st==4){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
			HAL_Delay(d);
		}
		if(st==5){	 
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1);
			HAL_Delay(d);
		}
		if(st==6){		
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1);
			HAL_Delay(d);
		}
		if(st==7){	 
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1);
			HAL_Delay(d);
		}
		if(st==8){
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
		}
	}
void ARM(int32_t arm1,int32_t arm2)
	{
			if(arm1==1)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
			}
			else if(arm1==-1)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
			}
				else if(arm1==0)
					{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
			}
			///////////////////
			if(arm2==1)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
			}
			else if(arm2==-1)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
			}
				else if(arm2==0)
					{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
			}
			
		}
void motor(int32_t mr1,int32_t mr2,int32_t ml2,int32_t ml1)
	{
		if(mr1>255)  		 mr1=255;
		if(mr2>255)  		 mr2=255;
		if(ml1>255)  		 ml1=255;
		if(ml2>255)  		 ml2=255;
	
		if(mr1<-255)  		 mr1=-255;
		if(mr2<-255)  		 mr2=-255;
		if(ml1<-255)  		 ml1=-255;
		if(ml2<-255)  		 ml2=-255;
		mr1 *= 65535/255;
		mr2 *= 65535/255;
		ml1 *= 65535/255;
		ml2 *= 65535/255;
		
		//////////////////////////////////
		if(mr1>0)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
			TIM4->CCR4=mr1;
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
			TIM4->CCR4=65535+mr1;
		}
		
		//////////////////////////////
		if(mr2>0)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
			TIM4->CCR3=mr2;
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
			TIM4->CCR3=65535+mr2;
		}
		
		///////////////////
		if(ml2>0)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
			TIM4->CCR2=ml2;
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,1);
			TIM4->CCR2=65535+ml2;
		}
		///////////////////
		if(ml1>0)
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
			TIM4->CCR1=ml1;
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
			TIM4->CCR1=65535+ml1;
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
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
		for(int i = 0; i < 5; i++)
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,0);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,1);
			HAL_Delay(100);
		}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*
		data[0] = (read_adc(1)/1000)%10 + '0';
		data[1] = (read_adc(1)/100)%10 + '0';
		data[2] = (read_adc(1)/10)%10 + '0';
		data[3] = (read_adc(1)/1)%10 + '0';
		data[4] = '\n';
		
		HAL_UART_Transmit(&huart1,data,5,100);
		HAL_Delay(1000);*/
		
		
		HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
		
		if(buffer == 'M'){
			HAL_UART_Receive(&huart2,&s,1,PHY_FULLDUPLEX_100M);
			HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
			if(s=='-') 			r1=-buffer;
			else  					r1=buffer;
		////////////////
			HAL_UART_Receive(&huart2,&s,1,PHY_FULLDUPLEX_100M);
			HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
			if(s=='-') 			r2=-buffer;
			else  					r2=buffer;
		////////////////
			HAL_UART_Receive(&huart2,&s,1,PHY_FULLDUPLEX_100M);
			HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
			if(s=='-') 			l2=-buffer;
			else  					l2=buffer;
		////////////////
			HAL_UART_Receive(&huart2,&s,1,PHY_FULLDUPLEX_100M);
			HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
			if(s=='-') 			l1=-buffer;
			else  					l1=buffer;
			HAL_UART_Receive(&huart2,&distance,1,PHY_FULLDUPLEX_100M);
			motor(r1,r2,l2,l1);
			if(distance != 0)
			{
				distance /= 3;
				int cnt = 0;
				while(cnt != distance)
				{
					while(read_adc(1) <1000);
					cnt++;
					if(cnt == distance) break;
					while(read_adc(1) >=1000);
					cnt++;
				}
				motor(0,0,0,0);
			}
			data[0] = 'N';
			HAL_UART_Transmit(&huart1,data,1,100);
		}
		else if(buffer == 'A'){
			HAL_UART_Receive(&huart2,&k,1,PHY_FULLDUPLEX_100M);
			if(k=='1')
			{
					HAL_UART_Receive(&huart2,&s,1,PHY_FULLDUPLEX_100M);
					HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
					if(s=='+')
						{
							for(int i=0;i<=buffer;i++) 	
							step(i%7);
						}
					else if(s=='-')
						{
							for(int i=buffer;i>=0;i--) 	
							step(i%7);
						}
					step(8);
			}
			//////////////////////////////
			
			else if(k=='2')  
			{				
					HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
					if(buffer=='L') 
					{
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
					}
					else if(buffer=='R') 
					{
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
					}
					else if(buffer=='S') 
					{
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
					}
			}
			//////////////////
			else if(k=='3')  
			{				
					HAL_UART_Receive(&huart2,&buffer,1,PHY_FULLDUPLEX_100M);
					if(buffer=='L') 
					{
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
					}
					else if(buffer=='R') 
					{
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
					}
					else if(buffer=='S') 
					{
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
						HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
					}
			}

		}
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  hadc1.Init.ContinuousConvMode = ENABLE;
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
  sConfig.Channel = ADC_CHANNEL_1;
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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
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
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7|GPIO_PIN_11|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA4 PA5 PA6 
                           PA7 PA11 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7|GPIO_PIN_11|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 
                           PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
