/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    RTC/RTC_InternalTimeStamp/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U0xx RTC HAL API to configure
  *          internal time stamp.
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

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */

uint8_t LcdTimeStr[16] = "hh:ms:ss";
uint8_t LcdDateStr[16] = "dd-mm-yyyy";

__IO uint8_t RTCStatus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TAMP_RTC_Init(void);
/* USER CODE BEGIN PFP */
static void RTC_CalendarShow(void);
static void RTC_TimeStampShow(void);
void Convert_IntegerIntoChar(float number, uint16_t *p_tab);
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
  /* STM32U0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Configure LED3, LED4, LED5 and LED5 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* Turn on LED3 */
  BSP_LED_On(LED3);

  /* Initialize LCD */
  BSP_LCD_GLASS_Init();

  /* Display LCD messages */
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"  RTC Internal ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" TimeStamp Example ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" Current Time-Date ", 1, SCROLL_SPEED_HIGH);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TAMP_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* Set Date and Time (if not already done before) */
  /* Check whether calendar year field has been set */
  if (__HAL_RTC_GET_FLAG(&hrtc, RTC_FLAG_INITS) != 0x00u)
  {
    /* Check if the Power On Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST) != RESET)
    {
      /* Turn on LED4: Power on reset occurred */
      BSP_LED_On(LED4);
    }
    /* Check if Pin Reset flag is set */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      /* Turn on LED5: External reset occurred */
      BSP_LED_On(LED5);
    }
    /* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /* Check whether internal time stamp event has been generated */
    if (__HAL_RTC_INTERNAL_TIMESTAMP_GET_FLAG(&hrtc, RTC_FLAG_ITSF) == SET)
    {
      /* Display time stamp Time and Date */
      RTC_TimeStampShow();

      /* Clear Internal TimeStamp flag */
      __HAL_RTC_INTERNAL_TIMESTAMP_CLEAR_FLAG(&hrtc, RTC_FLAG_ITSF);
    }
    else
    {
      BSP_LCD_GLASS_ScrollSentence((uint8_t *)"No Internal TimeStamp event", 1, SCROLL_SPEED_MEDIUM);
    }
  }
  RTCStatus = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Display the updated Time and Date */
    RTC_CalendarShow();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  hrtc.Init.BinMode = RTC_BINARY_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

    /* Enable internal time stamp */
    if (HAL_RTCEx_SetInternalTimeStamp(&hrtc) !=  HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TAMP Initialization Function
  * @param None
  * @retval None
  */
static void MX_TAMP_RTC_Init(void)
{

  /* USER CODE BEGIN TAMP_Init 0 */

  /* USER CODE END TAMP_Init 0 */

  RTC_TamperTypeDef sTamper = {0};

  /* USER CODE BEGIN TAMP_Init 1 */

  /* USER CODE END TAMP_Init 1 */

  /** Enable the RTC Tamper 1
  */
  sTamper.Tamper = RTC_TAMPER_1;
  sTamper.Trigger = RTC_TAMPERTRIGGER_RISINGEDGE;
  sTamper.NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
  sTamper.MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE;
  sTamper.Filter = RTC_TAMPERFILTER_DISABLE;
  sTamper.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
  sTamper.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  sTamper.TamperPullUp = RTC_TAMPER_PULLUP_ENABLE;
  sTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
  if (HAL_RTCEx_SetTamper(&hrtc, &sTamper) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TAMP_Init 2 */

  /* USER CODE END TAMP_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
static void RTC_CalendarShow(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  uint16_t tabTime[6];
  uint16_t tabS[6];
  uint16_t tabM[6];
  uint16_t tabH[6];

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  Convert_IntegerIntoChar((float)stimestructureget.Seconds, tabS);
  Convert_IntegerIntoChar((float)stimestructureget.Minutes, tabM);
  Convert_IntegerIntoChar((float)stimestructureget.Hours, tabH);
  tabTime[0]=tabH[4];
  tabTime[1]=tabH[5];
  tabTime[2]=tabM[4];
  tabTime[3]=tabM[5];
  tabTime[4]=tabS[4];
  tabTime[5]=tabS[5];
  tabTime[1]|= DOUBLE_DOT;
  tabTime[3]|= DOUBLE_DOT;
  BSP_LCD_GLASS_DisplayStrDeci((uint16_t *) tabTime);
  HAL_Delay(500);
  /* Display time hh:ms:ss and date mm-dd-yyyy Format */
  sprintf((char *)LcdTimeStr, "%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)LcdTimeStr, 1, SCROLL_SPEED_MEDIUM);

  sprintf((char *)LcdDateStr, "%02d-%02d-%02d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)LcdDateStr, 1, SCROLL_SPEED_MEDIUM);

  /* Display time hh:ms:ss and date Format mm-dd-yyyy Format */
}

/**
  * @brief  Display the time stamp
  * @param  None
  * @retval None
  */
void RTC_TimeStampShow(void)
{
  RTC_TimeTypeDef sTimeStampTime;
  RTC_DateTypeDef sTimeStampDate;
  uint8_t LcdTimeStr[16] = "hh:ms:ss";
  uint8_t LcdDateStr[16] = "dd-mm-yyyy";

  HAL_RTCEx_GetTimeStamp(&hrtc, &sTimeStampTime, &sTimeStampDate, RTC_FORMAT_BIN);

  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"Internal TimeStamp event at :", 1, SCROLL_SPEED_MEDIUM);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)LcdTimeStr, "%02d:%02d:%02d", sTimeStampTime.Hours, sTimeStampTime.Minutes, sTimeStampTime.Seconds);
  BSP_LCD_GLASS_DisplayString((uint8_t *) LcdTimeStr);
  HAL_Delay(500);
  /* Display date Format : mm-dd-yyyy */
  sprintf((char *)LcdDateStr, "%02d-%02d-%02d", sTimeStampDate.Month, sTimeStampDate.Date, 2024);
  BSP_LCD_GLASS_ScrollSentence( (uint8_t *)LcdDateStr, 1, SCROLL_SPEED_MEDIUM);
}


/**
  * @brief converts a 32bit unsined int into ASCII
  * @caller several callers for display values
  * @param number digit to displays
  * @param p_tab values in array in ASCII
  * @retval None
  */
void Convert_IntegerIntoChar(float number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, tenthousand=0, hundredthousand=0;

  uint32_t number1 = (uint32_t) number;

  units = ((((number1%100000)%10000)%1000)%100)%10;
  tens = (((((number1-units)/10)%10000)%1000)%100)%10;
  hundreds = ((((number1-tens-units)/100)%1000)%100)%10;
  thousands = (((number1-hundreds-tens-units)/1000)%100)%10;
  tenthousand = ((number1-thousands-hundreds-tens-units)/10000)%10;
  hundredthousand = ((number1-tenthousand-thousands-hundreds-tens-units)/100000);

  *(p_tab+5) = units + 0x30;
  *(p_tab+4) = tens + 0x30;
  *(p_tab+3) = hundreds + 0x30;
  *(p_tab+2) = thousands + 0x30;
  *(p_tab+1) = tenthousand + 0x30;
  *(p_tab+0) = hundredthousand + 0x30;
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
  /* Turn LED5 on */
  RTCStatus = 0xE;
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

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
