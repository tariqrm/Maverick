/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stdio.h"
#include "ssd1306.h"
#include "fonts.h"
#include "stdio.h"


#include "VL6180X.h"

#include "Pins.h"
#include "ZlgoVariables.h"
#include "Variables.h"
#include "Motors.h"
#include "Turns.h"
#include "WallPid.h"
#include "Speed.h"
#include "Moves.h"
#include "Zlgo.h"
#include "MazeSolve.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
VL6180X_Dev_t vl6180x_dev1;
VL6180X_Dev_t vl6180x_dev2;
VL6180X_Dev_t vl6180x_dev3;
VL6180X_Dev_t vl6180x_dev4;
VL6180X_Dev_t vl6180x_dev5;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENSOR1_ADDRESS  0x2B
#define SENSOR2_ADDRESS  0x2C
#define SENSOR3_ADDRESS  0x2D
#define SENSOR4_ADDRESS  0x2E
#define SENSOR5_ADDRESS  0x2F
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void VL6180X_SensorOn(GPIO_TypeDef* gpioPort, uint16_t gpioPin)
{
    HAL_GPIO_WritePin(gpioPort, gpioPin, GPIO_PIN_SET);
}


void VL6180X_Setup(){
	// Make sure all sensors are off initially.
	HAL_GPIO_WritePin(GPIO1_GPIO_Port, GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO2_GPIO_Port, GPIO2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO3_GPIO_Port, GPIO3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO4_GPIO_Port, GPIO4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO5_GPIO_Port, GPIO5_Pin, GPIO_PIN_RESET);

	// Power up each sensor in turn, assign unique address, init and configure.

	// --------------------- Sensor 1 --------------------
	VL6180X_SensorOn(GPIO1_GPIO_Port, GPIO1_Pin);
	vl6180x_dev1.hi2c = &hi2c1;
	vl6180x_dev1.i2c_address = VL6180X_ADDRESS_DEFAULT; // 0x29
	vl6180x_dev1.io_timeout = 10;
	vl6180x_dev1.did_timeout = false;

	VL6180X_Init(&vl6180x_dev1);
	VL6180X_ConfigureDefault(&vl6180x_dev1);

	// Keep sensor 1 at 0x29 or explicitly set it (redundant):
	// VL6180X_SetAddress(&vl6180x_dev1, SENSOR1_ADDRESS);

	// --------------------- Sensor 2 --------------------
	VL6180X_SensorOn(GPIO2_GPIO_Port, GPIO2_Pin);
	vl6180x_dev2.hi2c = &hi2c1;
	vl6180x_dev2.i2c_address = VL6180X_ADDRESS_DEFAULT; // 0x29 initially
	vl6180x_dev2.io_timeout = 10;
	vl6180x_dev2.did_timeout = false;

	VL6180X_Init(&vl6180x_dev2);
	VL6180X_ConfigureDefault(&vl6180x_dev2);
	// Set new address to 0x2A
	VL6180X_SetAddress(&vl6180x_dev2, SENSOR2_ADDRESS);

	// --------------------- Sensor 3 --------------------
	VL6180X_SensorOn(GPIO3_GPIO_Port, GPIO3_Pin);
	vl6180x_dev3.hi2c = &hi2c1;
	vl6180x_dev3.i2c_address = VL6180X_ADDRESS_DEFAULT;
	vl6180x_dev3.io_timeout = 10;
	vl6180x_dev3.did_timeout = false;

	VL6180X_Init(&vl6180x_dev3);
	VL6180X_ConfigureDefault(&vl6180x_dev3);
	VL6180X_SetAddress(&vl6180x_dev3, SENSOR3_ADDRESS);

	// --------------------- Sensor 4 --------------------
	VL6180X_SensorOn(GPIO4_GPIO_Port, GPIO4_Pin);
	vl6180x_dev4.hi2c = &hi2c1;
	vl6180x_dev4.i2c_address = VL6180X_ADDRESS_DEFAULT;
	vl6180x_dev4.io_timeout = 10;
	vl6180x_dev4.did_timeout = false;

	VL6180X_Init(&vl6180x_dev4);
	VL6180X_ConfigureDefault(&vl6180x_dev4);
	VL6180X_SetAddress(&vl6180x_dev4, SENSOR4_ADDRESS);

	// --------------------- Sensor 5 --------------------
	VL6180X_SensorOn(GPIO5_GPIO_Port, GPIO5_Pin);
	vl6180x_dev5.hi2c = &hi2c1;
	vl6180x_dev5.i2c_address = VL6180X_ADDRESS_DEFAULT;
	vl6180x_dev5.io_timeout = 10;
	vl6180x_dev5.did_timeout = false;

	VL6180X_Init(&vl6180x_dev5);
	VL6180X_ConfigureDefault(&vl6180x_dev5);
	VL6180X_SetAddress(&vl6180x_dev5, SENSOR5_ADDRESS);

	printf("All 5 VL6180X sensors are initialized.\r\n");
}

void VL6180X_readAll(){

	// Read range from each sensor
	uint8_t range1 = VL6180X_ReadRangeSingle(&vl6180x_dev1);
	uint8_t range2 = VL6180X_ReadRangeSingle(&vl6180x_dev2);
	uint8_t range3 = VL6180X_ReadRangeSingle(&vl6180x_dev3);
	uint8_t range4 = VL6180X_ReadRangeSingle(&vl6180x_dev4);
	uint8_t range5 = VL6180X_ReadRangeSingle(&vl6180x_dev5);

	// Optionally check each sensor's error code
	// (0 = OK; see your definitions for others)
	//uint8_t status1 = VL6180X_ReadRangeStatus(&vl6180x_dev1);
	//uint8_t status2 = VL6180X_ReadRangeStatus(&vl6180x_dev2);
	//uint8_t status3 = VL6180X_ReadRangeStatus(&vl6180x_dev3);
	//uint8_t status4 = VL6180X_ReadRangeStatus(&vl6180x_dev4);
	//uint8_t status5 = VL6180X_ReadRangeStatus(&vl6180x_dev5);


	// uint16_t mm1 = (uint16_t)range1 * vl6180x_dev1.scaling;
	//printf("S1=%3u (st=%u) | ", range1, status1);
	//printf("S2=%3u (st=%u) | ", range2, status2);
	//printf("S3=%3u (st=%u) | ", range3, status3);
	//printf("S4=%3u (st=%u) | ", range4, status4);
	//printf("S5=%3u (st=%u)\r\n", range5, status5);

	// Check for timeouts
	if (VL6180X_TimeoutOccurred(&vl6180x_dev1)) { printf("Sensor1 timeout.\r\n"); }
	if (VL6180X_TimeoutOccurred(&vl6180x_dev2)) { printf("Sensor2 timeout.\r\n"); }
	if (VL6180X_TimeoutOccurred(&vl6180x_dev3)) { printf("Sensor3 timeout.\r\n"); }
	if (VL6180X_TimeoutOccurred(&vl6180x_dev4)) { printf("Sensor4 timeout.\r\n"); }
	if (VL6180X_TimeoutOccurred(&vl6180x_dev5)) { printf("Sensor5 timeout.\r\n"); }
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  printf("Hello from STM32 via HC-05!\r\n");
	  HAL_Delay(1000);
	  searchStates();

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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  htim4.Init.Period = 255;
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
  huart2.Init.BaudRate = 9600;
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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(INT_LED_GPIO_Port, INT_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO2_Pin|GPIO1_Pin|GPIO5_Pin|GPIO3_Pin
                          |GPIO4_Pin|AIN2_Pin|STBY_Pin|AIN1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, BIN1_Pin|BIN2_Pin|BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : INT_LED_Pin */
  GPIO_InitStruct.Pin = INT_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(INT_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GPIO2_Pin GPIO1_Pin GPIO5_Pin GPIO3_Pin
                           GPIO4_Pin AIN2_Pin STBY_Pin AIN1_Pin */
  GPIO_InitStruct.Pin = GPIO2_Pin|GPIO1_Pin|GPIO5_Pin|GPIO3_Pin
                          |GPIO4_Pin|AIN2_Pin|STBY_Pin|AIN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BIN1_Pin BIN2_Pin BUZZER_Pin */
  GPIO_InitStruct.Pin = BIN1_Pin|BIN2_Pin|BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SWITCH_Pin */
  GPIO_InitStruct.Pin = SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SWITCH_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
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
