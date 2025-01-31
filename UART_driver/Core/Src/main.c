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

/* USER CODE BEGIN PV */
uint8_t buffer[1] = {'s'}; //01000001 for 65
uint8_t RX_buffer[1] = {'a'}; // null character until received

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

	//Peripheral Clock enable APB1 with it USART
	RCC -> APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Enable GPIOA

	//Pin PA2 Setup,TX
	GPIOA -> MODER &= ~(GPIO_MODER_MODER2); //Clear PA2
	GPIOA -> MODER |= (1<<5); // 10 for Alternate Function mode
	GPIOA -> AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos); //AF7 for TX
	//Pin PA3 Setup,RX
	GPIOA -> MODER &= ~(GPIO_MODER_MODER3); //Clear PA3, already 00 for input mode
	GPIOA -> MODER |= (2 << GPIO_MODER_MODE3_Pos); // 10 for Alternate Function mode
	GPIOA -> AFR[0] |= (7 << GPIO_AFRL_AFSEL3_Pos); //AF7 mode for RX

	//Setting up UART
	USART2 -> CR1 = 0x00000000; //Reset
	// USART2 -> CR1 = (1 << 28); //1 Start Bit, 8 data bit, n stop bits
	uint16_t uartdiv = 4000000 / 115200;
	USART2 -> BRR = (((uartdiv / 16) << USART_BRR_DIV_MANTISSA_Pos) | ((uartdiv % 16) << USART_BRR_DIV_FRACTION_Pos));


	//Enable Transmitter
	USART2 -> CR1 |= USART_CR1_TE;
	//Enable Receiver
	USART2 -> CR1 |= USART_CR1_RE;
	//Enable UART
	USART2 -> CR1 |= USART_CR1_UE;

	/*
	 * 	while (!(USART2->ISR & USART_ISR_TXE)) {}
	USART2 -> TDR = buffer[0];
	while (!(USART2->ISR & USART_ISR_TC)) {}

	while (!(USART2->ISR & USART_ISR_TXE)) {}
	USART2 -> TDR = RX_buffer[0];
	while (!(USART2->ISR & USART_ISR_TC)) {}
	 *
	 *
	 */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  USART2->TDR = 'h';
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  while (!(USART2 -> ISR & USART_ISR_RXNE));

	  RX_buffer[0] = USART2 -> RDR;

	  while (!(USART2->ISR & USART_ISR_TXE)) {}
	  USART2 -> TDR = RX_buffer[0];
	  while (!(USART2->ISR & USART_ISR_TC)) {}

	  HAL_Delay(100);

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
