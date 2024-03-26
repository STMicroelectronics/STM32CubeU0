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

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* Step Index */
 __IO uint32_t uwStep = 0;

/* Authorize TIM COM event generation */
__IO uint32_t uwAuthorizeTimComEvent = 0;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef              sPWMConfig1, sPWMConfig2, sPWMConfig3;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LED4 */
  BSP_LED_Init(LED4);

  /* Initialise structures */
  sPWMConfig1.OCMode = TIM_OCMODE_TOGGLE;
  sPWMConfig1.Pulse = 2047;
  sPWMConfig1.OCPolarity = TIM_OCPOLARITY_HIGH;
  sPWMConfig1.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sPWMConfig1.OCFastMode = TIM_OCFAST_DISABLE;
  sPWMConfig1.OCIdleState = TIM_OCIDLESTATE_SET;
  sPWMConfig1.OCNIdleState = TIM_OCNIDLESTATE_SET;

  sPWMConfig2 = sPWMConfig1;
  sPWMConfig2.Pulse = 1023;

  sPWMConfig3 = sPWMConfig1;
  sPWMConfig3.Pulse = 511;

 /*## Configure the commutation event: software event ####################*/
	HAL_TIMEx_ConfigCommutEvent_IT(&htim1, TIM_TS_NONE, TIM_COMMUTATION_SOFTWARE);
  /*## Start signals generation ###########################################*/
  /*--------------------------------------------------------------------------*/
  /* Start channel 1 */
  if(HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 1N */
  if(HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /*--------------------------------------------------------------------------*/

  
  /*--------------------------------------------------------------------------*/
  /* Start channel 2 */
  if(HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 2N */
  if(HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /*--------------------------------------------------------------------------*/

  
  /*--------------------------------------------------------------------------*/
  /* Start channel 3 */
  if(HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 3N */
  if(HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  /* Authorize TIM COM event generation */
  uwAuthorizeTimComEvent = 1;


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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 4095;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 2047;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 1023;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 511;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 1;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Commutation event callback in non blocking mode
  * @param  htim : Timer handle
  * @retval None
  */

void HAL_TIMEx_CommutCallback(TIM_HandleTypeDef *htim)
{
  /* Entry state */
  if (uwStep == 0)
  {
    /* Next step: Step 1 Configuration -------------------------------------- */
    sPWMConfig1.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_1);

    /*  Channel3 configuration */
    sPWMConfig3.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig3, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

    /*  Channel2 configuration */
    HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_2);
    uwStep = 1;
  }

  if (uwStep == 1)
  {
    /* Next step: Step 2 Configuration -------------------------------------- */
    /*  Channel1 configuration */
    /* Same configuration as the previous step */

    /*  Channel2 configuration */
    sPWMConfig2.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig2, TIM_CHANNEL_2);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_2);

    /*  Channel3 configuration */
    HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_3);

    uwStep++;
  }

  else if (uwStep == 2)
  {
    /* Next step: Step 3 Configuration -------------------------------------- */
    /*  Channel2 configuration */
    /* Same configuration as the previous step */

    /*  Channel3 configuration */
    sPWMConfig3.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

    /*  Channel1 configuration */
    HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_1);

    uwStep++;
  }

  else if (uwStep == 3)
  {
    /* Next step: Step 4 Configuration -------------------------------------- */
    /*  Channel3 configuration */
    /* Same configuration as the previous step */

    /*  Channel2 configuration */
    HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_2);

    /*  Channel1 configuration */
    sPWMConfig1.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig1, TIM_CHANNEL_1);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_1);

    uwStep++;
  }
  else if (uwStep == 4)
  {
    /* Next step: Step 5 Configuration -------------------------------------- */
    /*  Channel3 configuration */
    HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_3);

    /*  Channel1 configuration */
    /* Same configuration as the previous step */

    /*  Channel2 configuration */
    sPWMConfig2.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

    uwStep++;
  }

  else if (uwStep == 5)
  {
    /* Next step: Step 6 Configuration -------------------------------------- */
    /*  Channel3 configuration */
    sPWMConfig3.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig3, TIM_CHANNEL_3);
    HAL_TIMEx_OCN_Start(&htim1, TIM_CHANNEL_3);

    /*  Channel1 configuration */
    HAL_TIMEx_OCN_Stop(&htim1, TIM_CHANNEL_1);

    /*  Channel2 configuration */
    /* Same configuration as the previous step */

    uwStep++;
  }

  else
  {
    /* Next step: Step 1 Configuration -------------------------------------- */
    /*  Channel1 configuration */
    sPWMConfig1.OCMode     = TIM_OCMODE_PWM1;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sPWMConfig1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    /*  Channel3 configuration */
    /* Same configuration as the previous step */

    /*  Channel2 configuration */
    HAL_TIM_OC_Stop(&htim1, TIM_CHANNEL_2);

    uwStep = 1;
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
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1) 
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
