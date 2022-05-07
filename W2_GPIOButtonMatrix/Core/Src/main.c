/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
	//------------------------------------------------------------------------------------------Values
uint32_t ADCData[1]={0};

uint16_t ButtonState = 0; //store 4x4

static int CurrentDegree = 0;
static int InputDegree = 0;

int count100 = 0;
int count10 = 0;
int count1 = 0;
int count_100 = 0;
int count_10 = 0;
int count_1 = 0;

static int checktoggle[2] = {0};


//static uint16_t DataAns[2] = {0};
//static uint16_t DataAns[11] = {0};

//int i = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void ButtonMatrixRead();
void KEYDEG();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  //-------------------------------------------------------------------------------------------------Start reading ADC by DMA
  HAL_ADC_Start_DMA(&hadc1, ADCData, 1);	//(1channel form ADCData)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //-------------------------------------------------------------------------------------------------------------------------------------------------- Competency
	  ButtonMatrixRead();
	  KEYDEG();

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 99;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|L4_Pin|L1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : L4_Pin L1_Pin */
  GPIO_InitStruct.Pin = L4_Pin|L1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : L2_Pin */
  GPIO_InitStruct.Pin = L2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : R1_Pin */
  GPIO_InitStruct.Pin = R1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(R1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : R2_Pin R4_Pin R3_Pin */
  GPIO_InitStruct.Pin = R2_Pin|R4_Pin|R3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : L3_Pin */
  GPIO_InitStruct.Pin = L3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void KEYDEG(){
	static uint32_t timeStamp = 0;

	  if(InputDegree > 360){
		  InputDegree -= 360;
	  }
	  if(InputDegree < 0){
		  InputDegree *= -1;
	  }

	  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);

	  if(HAL_GetTick() - timeStamp >= 100){
		  timeStamp = HAL_GetTick();

		  //-----------------------------------------------------------------------------------------------------------Push Some Toggle SW
		  //-------------------------------------------------------------------+100 deg
		  if(ButtonState == 1){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count100 += 1;
				  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
			  }
		  }

		  //-------------------------------------------------------------------+10 deg
		  else if(ButtonState == 2){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count10 += 1;
				  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
			  }
		  }

		  //-------------------------------------------------------------------+1 deg
		  else if(ButtonState == 4){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count1 += 1;
				  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
			  }
		  }

		  //-------------------------------------------------------------------- -100 deg
		  else if(ButtonState == 16){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count_100 += 1;
				  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
			  }
		  }

		  //-------------------------------------------------------------------- -10 deg
		  else if(ButtonState == 32){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count_10 += 1;
				  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
			  }
		  }

		  //-------------------------------------------------------------------- -1 deg
		  else if(ButtonState == 64){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count_1 += 1;
				  CurrentDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
			  }
		  }

		  //-----------------------------------------------------------------------------------------------------------Enter SW
		  else if(ButtonState == 4096){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  InputDegree = (count100*100)+(count10*10)+(count1*1)+(count_100*-100)+(count_10*-10)+(count_1*-1);
				  CurrentDegree = InputDegree;
				  count100 = 0;
				  count10 = 0;
				  count1 = 0;
				  count_100 = 0;
				  count_10 = 0;
				  count_1 = 0;
			  }
		  }

		  //-----------------------------------------------------------------------------------------------------------SET Zero SW
		  else if(ButtonState == 8192){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  InputDegree = 0;
				  CurrentDegree = 0;
			  }
		  }

		  //-----------------------------------------------------------------------------------------------------------Reset Input
		  else if(ButtonState == 16384){
			  checktoggle[0] = 1;
			  if(checktoggle[1] == 0 && checktoggle[0] == 1){
				  count100 = 0;
				  count10 = 0;
				  count1 = 0;
				  count_100 = 0;
				  count_10 = 0;
				  count_1 = 0;
			  }
		  }

		  //-----------------------------------------------------------------------------------------------------------No push any Toggle SW
		  if(ButtonState == 0){
			  checktoggle[0] = 0;
		  }
		  checktoggle[1] = checktoggle[0];

		  //-----------------------------------------------------------------------------------------------------------No push any Toggle SW

	  }
}

//------------------------------------------------------------------------------------------------------------------------------------------- KEY PAD 4x4
//Read button state form 4x4 button
GPIO_TypeDef* ButtonMatrixPortR[4] = {R1_GPIO_Port,R2_GPIO_Port,R3_GPIO_Port,R4_GPIO_Port};
uint16_t ButtonMatrixPinR[4] = {R1_Pin,R2_Pin,R3_Pin,R4_Pin};

GPIO_TypeDef* ButtonMatrixPortL[4] = {L1_GPIO_Port,L2_GPIO_Port,L3_GPIO_Port,L4_GPIO_Port};
uint16_t ButtonMatrixPinL[4] = {L1_Pin,L2_Pin,L3_Pin,L4_Pin};


void ButtonMatrixRead()
{
	static uint32_t timeStamp = 0;
	static uint8_t CurrentL = 0;
	//call reader every 100ms
	if (HAL_GetTick() - timeStamp >= 100)
	{
		timeStamp = HAL_GetTick();

		for(int i = 0 ; i < 4; i++)
		{
			if ( HAL_GPIO_ReadPin(ButtonMatrixPortR[i], ButtonMatrixPinR[i]) == GPIO_PIN_RESET) // Button press
			{

				//set bit i to 1
				ButtonState |= 1 << (i + (CurrentL*4));
			}
			else
			{
				//set bit i to 0
				ButtonState &= ~(1 << (i + (CurrentL*4)));

			}

		}


		HAL_GPIO_WritePin(ButtonMatrixPortL[CurrentL], ButtonMatrixPinL[CurrentL], GPIO_PIN_SET);

		uint8_t nextL = ( CurrentL+1 ) % 4;
		HAL_GPIO_WritePin(ButtonMatrixPortL[nextL], ButtonMatrixPinL[nextL], GPIO_PIN_RESET);
		CurrentL = nextL;
	}

}

//------------------------------------------------------------------------------------------------------------------------------------------- Interupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_13)
	{
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	}
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
