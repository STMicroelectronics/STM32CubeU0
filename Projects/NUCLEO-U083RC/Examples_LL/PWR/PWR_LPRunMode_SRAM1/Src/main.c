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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USE_LED
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t ubExecuteRunMode = 1;
#ifdef USE_LED  
__IO uint16_t uhLedBlinkSpeed = LED_BLINK_FAST;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void     Configure_PWR(void);
#ifdef USE_LED
void     LED_Init(void);
void     LED_Blinking(void);
#endif
void     UserButton_Init(void);
void     EnterRunMode_LowPower_100KHz(void);
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
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
#ifdef USE_LED
  /* Initialize LED4 */
  LED_Init();
#endif

  /* Initialize User push-button */
  UserButton_Init();

  /* Configure Power Peripheral */
  Configure_PWR();

  
  while(ubExecuteRunMode)
  {
#ifdef USE_LED   
    /* Led blinking until User push-button action */
    LED_Blinking();
#endif
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#ifdef USE_LED   
    /* Led blinking in infinite loop*/
    LED_Blinking();
#endif
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() != 0)
  {
  }
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {
  }

  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_11);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(48000000);

  LL_SetSystemCoreClock(48000000);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);

  /**/
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#ifdef USE_LED
/**
  * @brief  Initialize LED4.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED4 Clock */
  LED4_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED4 */
  LL_GPIO_SetPinMode(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_MODE_OUTPUT);

}

/**
  * @brief  Set LED4 to Blinking mode (Shall be call in a Loop).
  * @param  None
  * @retval None
  */
void LED_Blinking(void)
{
  /* Toggle IO. Shall be call in a loop to toggle */
  LL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN);
  LL_mDelay(uhLedBlinkSpeed);
}
#endif

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_UP);

  /* Connect External Line to the GPIO*/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTC, LL_EXTI_CONFIG_LINE13);

  /* Enable a falling trigger EXTI_Line13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
}

/**
  * @brief  Function to configure and initialize PWR Peripheral.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

}

/**
  * @brief  Function to configure system at 100 KHZ in Low Power Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_LowPower_100KHz(void)
{
  

  /* 1 - Set Frequency to 100 KHz to activate Low Power Run Mode */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI); 
  /* Set systick to 1ms in using frequency set to 100 KHz */
  LL_Init1msTick(100*1000);
  /* Update CMSIS variable */
  SystemCoreClock = 100*1000;

  /* 2 - Set Voltage scaling to Range 2  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);   

  /* 3 - Activate Low Power Run Mode */
  LL_PWR_EnterLowPowerRunMode();
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage BUTTON IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Decrease core frequency and voltage. Enter Low Power Run mode
   * Frequency: 48 MHz -> 100KHz
   * Voltage Scaling Range 2
   * LowPowerRunMode activated
   */
  EnterRunMode_LowPower_100KHz();
#ifdef USE_LED  
  uhLedBlinkSpeed = LED_BLINK_SLOW;
#endif
  /* Exit Test */
  ubExecuteRunMode = 0;
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
