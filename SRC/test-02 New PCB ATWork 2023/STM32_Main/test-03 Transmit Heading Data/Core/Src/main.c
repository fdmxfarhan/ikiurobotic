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
#include "math.h"

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
 TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define RX1_Size 9
#define RX2_Size 8
uint8_t Rx1_Buff[RX1_Size];
uint8_t Rx2_Buff[RX2_Size];

int v = 20;
int l1, l2, r2, r1;
int16_t Pitch=0, Roll=0, Heading=0;
float thetaD = 0;
float time = 0;
float Heading_f = 0.0, Last_Heading = 0.0;
int counter = 0;
int correction = 0;
float K_P, K_I, K_D;
int Correction_EN = 0;
char look_direction = 'N';
uint32_t Front_Dist=0, Back_Dist=0, Right_Dist=0, Left_Dist=0, srf_cnt=0;

uint8_t GY_A5[] = {0xA5};
uint8_t GY_54[] = {0x54};
uint8_t GY_51[] = {0x51};
uint8_t GY_55[] = {0x55};
uint8_t GY_Init_Command[]    = {0xA5, 0x54, 0xA5, 0x51};
uint8_t GY_Request_Command[] = {0xA5, 0x51};
uint8_t GY_Set_Command[] = {0xA5, 0x55};

uint8_t tx_data[4];
uint32_t Last_Time = 0;
uint16_t oldPos=0, newPos=0;
int beginTime;
void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}

void motor(int L1, int L2, int R2, int R1){
	if(L1 > 15 && L1 <-15) L1 = 0;
	if(L2 > 15 && L2 <-15) L2 = 0;
	if(R2 > 15 && R2 <-15) R2 = 0;
	if(R1 > 15 && R1 <-15) R1 = 0;

	L1 = L1*255;
	L2 = L2*255;
	R2 = R2*255;
	R1 = R1*255*1.35;

	if(L1 > 65535) L1 = 65535;
	if(L2 > 65535) L2 = 65535;
	if(R2 > 65535) R2 = 65535;
	if(R1 > 65535) R1 = 65535;

	if(L1 < -65535) L1 = -65535;
	if(L2 < -65535) L2 = -65535;
	if(R2 < -65535) R2 = -65535;
	if(R1 < -65535) R1 = -65535;

	// ------------------ L1
	if(L2 == 0){
		HAL_GPIO_WritePin(INA1_GPIO_Port, INA1_Pin, 0);
		HAL_GPIO_WritePin(INB1_GPIO_Port, INB1_Pin, 0);
		TIM4->CCR1 = 65535;
	}
	else if(L2 > 0){
		HAL_GPIO_WritePin(INA1_GPIO_Port, INA1_Pin, 1);
		HAL_GPIO_WritePin(INB1_GPIO_Port, INB1_Pin, 0);
		TIM4->CCR1 = L2;
	}else{
		HAL_GPIO_WritePin(INA1_GPIO_Port, INA1_Pin, 0);
		HAL_GPIO_WritePin(INB1_GPIO_Port, INB1_Pin, 1);
		TIM4->CCR1 = -L2;
	}
	// ------------------ L2
	if(L1 == 0){
		HAL_GPIO_WritePin(INA2_GPIO_Port, INA2_Pin, 0);
		HAL_GPIO_WritePin(INB2_GPIO_Port, INB2_Pin, 0);
		TIM4->CCR2 = 65535;
	}
	else if(L1 > 0){
		HAL_GPIO_WritePin(INA2_GPIO_Port, INA2_Pin, 1);
		HAL_GPIO_WritePin(INB2_GPIO_Port, INB2_Pin, 0);
		TIM4->CCR2 = L1;
	}else{
		HAL_GPIO_WritePin(INA2_GPIO_Port, INA2_Pin, 0);
		HAL_GPIO_WritePin(INB2_GPIO_Port, INB2_Pin, 1);
		TIM4->CCR2 = -L1;
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
	HAL_UART_Transmit(&huart2, GY_A5, 1, PHY_FULLDUPLEX_10M);
	HAL_UART_Transmit(&huart2, GY_54, 1, PHY_FULLDUPLEX_10M);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, GY_A5, 1, PHY_FULLDUPLEX_10M);
	HAL_UART_Transmit(&huart2, GY_51, 1, PHY_FULLDUPLEX_10M);
	HAL_Delay(500);
	HAL_UART_Transmit(&huart2, GY_A5, 1, PHY_FULLDUPLEX_10M);
	HAL_UART_Transmit(&huart2, GY_55, 1, PHY_FULLDUPLEX_10M);
	HAL_Delay(500);
}
void delay(int t){
	for(register int i=0; i<t; i++);
}
void pinMode(int state){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(state == 0) // INPUT
	{
		GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	else if(state == 1) // OUTPUT
	{
		GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
void getDistances(){
	pinMode(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
	pinMode(0);
	srf_cnt = 0;
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1) srf_cnt++;
	Front_Dist = srf_cnt/52;

	pinMode(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	pinMode(0);
	srf_cnt = 0;
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 0);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 1) srf_cnt++;
	Right_Dist = srf_cnt/52;

	pinMode(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	pinMode(0);
	srf_cnt = 0;
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 0);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1) srf_cnt++;
	Back_Dist = srf_cnt/52;

	pinMode(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	pinMode(0);
	srf_cnt = 0;
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 0);
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 1) srf_cnt++;
	Left_Dist = srf_cnt/52;
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance == USART1){
		RED_ON;
		if(Rx1_Buff[0] == 'M') {
		  if(Rx1_Buff[1]=='-') 	r1 =-(int)Rx1_Buff[2];
		  else  				r1 = (int)Rx1_Buff[2];
		  ////////////////
		  if(Rx1_Buff[3]=='-') 	r2 =-(int)Rx1_Buff[4];
		  else  				r2 = (int)Rx1_Buff[4];
		  ////////////////
		  if(Rx1_Buff[5]=='-') 	l2 =-(int)Rx1_Buff[6];
		  else  				l2 = (int)Rx1_Buff[6];
		  ////////////////
		  if(Rx1_Buff[7]=='-') 	l1 =-(int)Rx1_Buff[8];
		  else  				l1 = (int)Rx1_Buff[8];
		}
		else if(Rx1_Buff[0] == 'S') {
			HAL_UART_Transmit(&huart2, GY_Set_Command, 2, PHY_FULLDUPLEX_10M);
			delay(100000);
			HAL_UART_Transmit(&huart2, GY_Set_Command, 2, PHY_FULLDUPLEX_10M);
			delay(100000);
		}
		else if(Rx1_Buff[0] == 'C') {
			if(Rx1_Buff[1] == 'T'){
				if(Correction_EN == 1) Correction_EN = 0;
				else				   Correction_EN = 1;
			}if(Rx1_Buff[1] == '1'){
				Correction_EN = 1;
			}if(Rx1_Buff[1] == '0'){
				Correction_EN = 0;
			}
			time = 0;
			beginTime = HAL_GetTick();
		}
		else if(Rx1_Buff[0] == 'D') {
			if(Rx1_Buff[1] == 'A' && Rx1_Buff[2] == 'L' && Rx1_Buff[3] == 'L'){
				getDistances();
				uint8_t tx_data[] = {(uint8_t) Front_Dist >> 8,(uint8_t) Front_Dist & 0xFF,
									 (uint8_t) Right_Dist >> 8,(uint8_t) Right_Dist & 0xFF,
									 (uint8_t) Back_Dist  >> 8,(uint8_t) Back_Dist  & 0xFF,
									 (uint8_t) Left_Dist  >> 8,(uint8_t) Left_Dist  & 0xFF};
				HAL_UART_Transmit(&huart1, tx_data, 8, PHY_FULLDUPLEX_10M);
			}
		}
		else if(Rx1_Buff[0] == 'P') {
			look_direction = Rx1_Buff[1];
		}
		else if(Rx1_Buff[0] == 'H') {
			tx_data[0] = (uint8_t) (Heading + 360) >> 8;
			tx_data[1] = (uint8_t) (Heading + 360) & 0xFF;
			tx_data[2] = (uint8_t) (thetaD + 360) >> 8;
			tx_data[3] = (uint8_t) (thetaD + 360) & 0xFF;
			HAL_UART_Transmit(&huart1, tx_data, 4, PHY_FULLDUPLEX_10M);
		}

		RED_OFF;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Rx1_Buff, RX1_Size);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
	else if(huart->Instance == USART2){
		GREEN_ON;
		for(int i=0; i<RX2_Size; i++){
			if(Rx2_Buff[i] == 0xAA){
				Heading_f = (int16_t)(Rx2_Buff[(i+1)%8]<<8 | Rx2_Buff[(i+2)%8])/100.00;
				Heading = (int16_t)(Rx2_Buff[(i+1)%8]<<8 | Rx2_Buff[(i+2)%8])/100.00;
				Pitch = (int16_t)(Rx2_Buff[(i+3)%8]<<8 | Rx2_Buff[(i+4)%8])/100.00;
				Roll = (int16_t)(Rx2_Buff[(i+5)%8]<<8 | Rx2_Buff[(i+6)%8])/100.00;

				if(look_direction == 'W'){
					Heading -= 90;
				}
				if(look_direction == 'E'){
					Heading += 90;
				}
				if(look_direction == 'S'){
					if(Heading > 0) Heading = Heading - 180;
					else		    Heading = Heading + 180;
				}
				if(Heading > 180) Heading -= 180;
				if(Heading <-180) Heading += 180;
			}
		}
		GREEN_OFF;
		HAL_UART_Transmit(&huart2, GY_Request_Command, 2, PHY_FULLDUPLEX_10M);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, Rx2_Buff, RX2_Size);
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
}
float u(float t){
	if(t > 0)	return 1;
	return 0;
}
double chirp(double t) {
	double s = 0;
	for(int i=1; i<100; i++){
		s += (1.0/(i*M_PI))*cos(2.0 * i * M_PI * t + M_PI/2.0);
	}
	return 0.5 + s;
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  HAL_TIM_Base_Start(&htim1);

  HAL_GPIO_WritePin(MOTORS_EN_GPIO_Port, MOTORS_EN_Pin, 1);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1); // Red LED
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, 0); // Red LED
  for(int i=0; i<5; i++){
	  HAL_Delay(100);
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14); // Red LED
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Green LED
  }
  RED_OFF;
  GREEN_OFF;

  initGY();

  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Rx1_Buff, RX1_Size);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

  HAL_UART_Transmit(&huart2, GY_Request_Command, 2, PHY_FULLDUPLEX_10M);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, Rx2_Buff, RX2_Size);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);


  Last_Time = HAL_GetTick();
  beginTime = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  time = (HAL_GetTick() - beginTime)/1000.0;

	  thetaD = 50 * sin(time*0.2);
//	  thetaD = 50 * chirp(time * 0.1);

	  if(thetaD >  180) thetaD = thetaD - 360;
	  if(thetaD < -180) thetaD = thetaD + 360;

	  K_P = 4 * (Heading_f - thetaD);
	  if(HAL_GetTick() - Last_Time >= 100){
		  if(K_P == 0) {
			  K_I = 0;
			  K_D = 0;
		  }else{
			  K_I = ((K_P) * (0.1))+K_I ;
			  K_D = ((K_P) - Last_Heading)/0.1;
		  }
		  Last_Time = HAL_GetTick();
		  Last_Heading = K_P;
	  }
	   correction = 1.46 * K_P + 17.5 * K_I;// - 0.00883 * K_D;
//	  correction = 4 * (Heading - thetaD);//1 * K_P + 100 * K_I + 1 * K_D;


	  if(Correction_EN == 1)
		  motor(l1 + correction, l2 + correction, r2 + correction, r1 + correction);
	  else
		  motor(l1, l2, r2, r1);



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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 64-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|INA3_Pin
                          |INB3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, INA2_Pin|INB2_Pin|INA1_Pin|INB1_Pin
                          |MOTORS_EN_Pin|INA4_Pin|INB4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 INA3_Pin
                           INB3_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|INA3_Pin
                          |INB3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INA2_Pin INB2_Pin INA1_Pin INB1_Pin
                           MOTORS_EN_Pin INA4_Pin INB4_Pin */
  GPIO_InitStruct.Pin = INA2_Pin|INB2_Pin|INA1_Pin|INB1_Pin
                          |MOTORS_EN_Pin|INA4_Pin|INB4_Pin;
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
