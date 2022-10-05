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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "time.h"
#include "display.h"
#include "dht11.h"
#include <stdio.h>
#include "user_uart.h"
#include "command.h"
#include "lock.h"
#include "motor.h"
#include "rain.h"
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
_Bool isAlarmOn = 0;
float ftemperature;
float fhumidity;
uint8_t *ptemperature;
uint8_t *phumidity;
uint8_t SendStr[20] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  uint8_t temp[1] = {ch};
  HAL_UART_Transmit(&huart1, temp, 1, 2); // huart1�??????要根据你的配置修�??????
  return ch;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  u8g2Init(&u8g2);
  if (!as608_init())
  {
    printf("AS608 Init Succeed\r\n");
  }
  else
  {
    printf("AS608 Init Failed\r\n");
  }
  RefreshScreen();
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  printf("start");
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_UART_Transmit(&huart2, "start", 10, 100);
  HAL_UART_Receive_IT(&huart1, (uint8_t *)UART1_temp, Rec_Long);
  HAL_UART_Receive_IT(&huart2, (uint8_t *)UART2_temp, Rec_Long);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (LOCK)
    {
      CHECK_FR();
    }
    HAL_Delay(700);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if (RAIN && !CURTAIN)
		CloseCurtain();
	if (!RAIN && CURTAIN)
		OpenCurtain();
    HAL_UART_Transmit(&huart1, UART1_Rx_Buf, UART1_Rx_cnt, 0xff);
    DHT11_Read_Data(&temperature, &humidity);
    if (UART1_Rx_flg)
      ParseCommand(&huart1);
    if (UART2_Rx_flg)
      ParseCommand(&huart2);
    if (!(CURRENT_TIME.second % 15))
    {
      /*
      ftemperature = (float)(temperature>>8) + (temperature & 0xff)/100.0;
      fhumidity = (float)(humidity>>8) + (humidity & 0xff)/100.0;
      ptemperature = (uint8_t *)&ftemperature;
      phumidity = (uint8_t *)&fhumidity;
      sprintf(SendStr, "\x80%c%c%c%c\x81%c%c%c%c", ptemperature[0], ptemperature[1], ptemperature[2], ptemperature[3], phumidity[0], phumidity[1], phumidity[2], phumidity[3]);
      HAL_UART_Transmit(&huart2, SendStr, 15, 0xff);
      */
      sprintf(SendStr, "SETTEMP %f\n", ((temperature>>8) + (temperature & 0xff)/100.0));
      HAL_UART_Transmit(&huart2, SendStr, 20, 0xff);
      sprintf(SendStr, "SETHUMI %f\n", ((humidity>>8) + (humidity & 0xff)/100.0));
      HAL_UART_Transmit(&huart2, SendStr, 20, 0xff);
    }
    HAL_UART_Transmit(&huart1, UART1_Rx_Buf, UART1_Rx_cnt, 0xff);
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

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim2.Instance)
  {
    Tick(&CURRENT_TIME);
    RefreshScreen();
    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
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
