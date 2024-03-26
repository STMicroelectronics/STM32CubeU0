/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I2C/I2C_Sensor_Private_Command_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U0xx I2C HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          IT transfer.
  *          The communication is done using STM32U0xx Nucleo and
  *          X-NUCLEO-IKS01A3.
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

I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* Variable used for LSM6DSO configuration, and reading data raw */
uint8_t CTRL1_XL_Register   = 0x10;         /* Address of accelerometer control register */
uint8_t OUT_TEMP_L_Register = 0x20;         /* Address of temperature data output register */
uint8_t aLSM6DSO_Config[]   = {0x36, 0x62}; /* Enable Accelerometer then Gyroscope */

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Variable to display reading data */
uint32_t uwDisplayDelay   = 0U;
int16_t Temperature       = 0;
int16_t aGyroscope[3]     = {0};
int16_t aAccelerometer[3] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void DisplayValue(uint8_t *pPrivateBuffer);
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
extern void initialise_monitor_handles(void);
#endif

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
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
  initialise_monitor_handles();
  printf("Semihosting Test...\n\r");
#endif
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LED3 and LED5 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);


  /* Configure Joystick Sel push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /*##-STEP1-#- Configure the Accelerometer then Gyroscope ###########################################################*/
  if (HAL_I2C_Mem_Write_IT(&hi2c1, LSM6DSO_STA, CTRL1_XL_Register, REGISTER_SIZE,
                           aLSM6DSO_Config, COUNTOF(aLSM6DSO_Config)) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #############################################################################*/
  /* Before starting a new communication transfer, you need to check the current
  state of the peripheral; if it's busy you need to wait for the end of current
  transfer before starting a new one.
  For simplicity reasons, this example is just waiting till the end of the
  transfer, but application may perform other tasks while transfer operation
  is ongoing. */
  while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
  {
  }

  /* Wait for Joystick Sel push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
  }

  /* Delay to avoid that possible signal rebound is taken as button release */
  HAL_Delay(50);

  /* Wait for Joystick Sel push-button release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /*##- Start the read memory process ##############################################################################*/
    if (HAL_I2C_Mem_Read_IT(&hi2c1, LSM6DSO_STA, OUT_TEMP_L_Register, REGISTER_SIZE, aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
      /* Error_Handler() function is called when error occurs. */
      Error_Handler();
    }

    /*##- Wait for the end of the transfer ###########################################################################*/
    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if it's busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    /*##- Monitor the different value retrieve during Private communication */
    /*  At the end, of transfer, the application have retrieve all the data of
    the frame in an unique buffer, which must be unfill to retrieve the associated
    value */
    /* Display through external Terminal IO the value received */
    DisplayValue(&aRxBuffer[0]);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 6;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
  hi2c1.Init.Timing = 0x0030081C;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /** I2C Fast mode Plus enable
  */
  if (HAL_I2CEx_ConfigFastModePlus(&hi2c1, I2C_FASTMODEPLUS_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Memory Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED3: Transfer in transmission process is correct */
  BSP_LED_Toggle(LED3);
}

/**
  * @brief  Memory Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED3: Transfer in reception process is correct */
  BSP_LED_Toggle(LED3);
}

/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
  {
    /* Turn Off LED3 */
    BSP_LED_Off(LED3);

    /* Turn On LED5 */
    BSP_LED_On(LED5);
  }
}

/**
  * @brief  Display the different value retrieve during Communication.
  * @param  pPrivateBuffer Pointer to a receive Private buffer.
  * @retval None.
  */
static void DisplayValue(uint8_t *pPrivateBuffer)
{
  /* Temperature offset is 25 degree celsius, mean Temperature value 0 correspond to 25 Celsius degree */
#define TEMP_OFFSET 25

  /* Display the value measured each DISPLAY_REFRESH_DELAY */
  if (HAL_GetTick() >= (uwDisplayDelay + DISPLAY_REFRESH_DELAY))
  {
    Temperature = TEMP_OFFSET + (((int16_t)(((int16_t)aRxBuffer[1] * 256) + (int16_t)aRxBuffer[0])) / 100);

    printf("\n\r----------------LSM6DSO Measurement----------------\n\r");
    printf("Temperature (Celsius degree): %d \n\r", Temperature);

    aGyroscope[0] = (int16_t)aRxBuffer[3];
    aGyroscope[0] = (aGyroscope[0] * 256) + (int16_t)aRxBuffer[2];
    aGyroscope[1] = (int16_t)aRxBuffer[5];
    aGyroscope[1] = (aGyroscope[1] * 256) + (int16_t)aRxBuffer[4];
    aGyroscope[2] = (int16_t)aRxBuffer[7];
    aGyroscope[2] = (aGyroscope[2] * 256) + (int16_t)aRxBuffer[6];

    printf("GYR_X: %d, GYR_Y: %d, GYR_Z: %d\r\n", (int)aGyroscope[0], (int)aGyroscope[1], (int)aGyroscope[2]);

    aAccelerometer[0] = (int16_t)aRxBuffer[9];
    aAccelerometer[0] = (aAccelerometer[0] * 256) + (int16_t)aRxBuffer[8];
    aAccelerometer[1] = (int16_t)aRxBuffer[11];
    aAccelerometer[1] = (aAccelerometer[1] * 256) + (int16_t)aRxBuffer[10];
    aAccelerometer[2] = (int16_t)aRxBuffer[13];
    aAccelerometer[2] = (aAccelerometer[2] * 256) + (int16_t)aRxBuffer[12];

    printf("ACC_X: %d, ACC_Y: %d, ACC_Z: %d\r\n", (int)aAccelerometer[0], (int)aAccelerometer[1], (int)aAccelerometer[2]);

    uwDisplayDelay+=DISPLAY_REFRESH_DELAY;
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
  /* Turn LED5 on */
  BSP_LED_On(LED5);
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
  /* Infinite Loop */
  while(1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
