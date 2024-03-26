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
#include "stm32u0xx_nucleo.h"
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

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
/* Private define ------------------------------------------------------------*/
enum {
  IT_TRANSACTION_NONE,
  IT_TRANSACTION_COMPLETE,
  IT_TRANSACTION_ERROR
};

/* Private macro -------------------------------------------------------------*/
#define READ_COUNTER_COMMAND  0xFF
#define WRITE_COUNTER_COMMAND 0xBF

/* Private variables ---------------------------------------------------------*/
/* Buffer used for transmission */
uint8_t aTxBuffer[2];

/* Buffer used for reception */
uint8_t aRxBuffer[2];

uint8_t counter_write = 1, counter_read;

/* IT transaction state */
__IO uint32_t IT_Transcation;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  
  /* Configure LED4 */
  BSP_LED_Init(LED4);

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
  }
  BSP_LED_Off(LED4);

  do {

    /*## Execute the command write counter */
    aTxBuffer[0] = WRITE_COUNTER_COMMAND;
    IT_Transcation = IT_TRANSACTION_NONE;
    if (HAL_SPI_Transmit_IT(&hspi1, aTxBuffer, 1) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* Master Slave synchro */
    while(IT_Transcation != IT_TRANSACTION_COMPLETE);
    HAL_Delay(10);

    counter_write++;
    aTxBuffer[0] = counter_write;
    IT_Transcation = IT_TRANSACTION_NONE;
    if (HAL_SPI_Transmit_IT(&hspi1, aTxBuffer, 1) != HAL_OK)
    {
      Error_Handler();
    }
    while(IT_Transcation != IT_TRANSACTION_COMPLETE);
    
    /* Synchro between 2 transaction, to avoid SPI synchronuisation issue */
    HAL_Delay(10);
      
    /*## Execute the command read counter */
    aTxBuffer[0] = READ_COUNTER_COMMAND;
    IT_Transcation = IT_TRANSACTION_NONE;
    if (HAL_SPI_Transmit_IT(&hspi1, aTxBuffer, 1) != HAL_OK)
    {
      Error_Handler();
    }
    while(IT_Transcation != IT_TRANSACTION_COMPLETE);
    
   __HAL_SPI_DISABLE(&hspi1);
    /* Master Slave synchro */
    HAL_Delay(10); 

    IT_Transcation = IT_TRANSACTION_NONE;
    if (HAL_SPI_Receive_IT(&hspi1, aRxBuffer, 1) != HAL_OK)
    {
      Error_Handler();
    }
    
    while(IT_Transcation != IT_TRANSACTION_COMPLETE);
    counter_read = aRxBuffer[0]; 

    /* Check that counter write and counter read are equal */    
    if (counter_write != counter_read)
    {
      Error_Handler();
    }
    
    counter_write = counter_read;
    
    /* Synchro between 2 transaction, to avoid SPI synchronisation issue */
    HAL_Delay(10);
     
  } while(counter_write != 2);

  BSP_LED_On(LED4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Rx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of Interrupt Rx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  IT_Transcation = IT_TRANSACTION_COMPLETE;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of Interrupt Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  IT_Transcation = IT_TRANSACTION_COMPLETE;
}

/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  IT_Transcation = IT_TRANSACTION_ERROR;
  Error_Handler();
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
    while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
