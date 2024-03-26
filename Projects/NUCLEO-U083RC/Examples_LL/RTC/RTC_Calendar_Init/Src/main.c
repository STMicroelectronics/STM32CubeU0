/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Calendar_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U0xx RTC LL API to configure
  *          Time and Date.
  *          Peripheral initialization done using LL unitary services functions.
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

/* Oscillator time-out values */
#define LSI_TIMEOUT_VALUE          ((uint32_t)2)     /* 2 ms */
#define RTC_TIMEOUT_VALUE          ((uint32_t)1000)  /* 1 s */

/* Defines related to Clock configuration */
/* Uncomment to enable the adequate Clock Source */
#define RTC_CLOCK_SOURCE_LSI
//#define RTC_CLOCK_SOURCE_LSE

#ifdef RTC_CLOCK_SOURCE_LSI
/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=32 kHz RC */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00F9)
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq=32768Hz */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00FF)
#endif

/* Define used to indicate date/time updated */
#define RTC_BKP_DATE_TIME_UPDTATED ((uint32_t)0x32F2)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Buffers used for displaying Time and Date */
uint8_t aShowTime[16] = "hh:ms:ss";
uint8_t aShowDate[16] = "mm-dd-yyyy";

#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
uint32_t Enter_RTC_InitMode(void);
uint32_t Exit_RTC_InitMode(void);
uint32_t WaitForSynchro_RTC(void);
void     Show_RTC_Calendar(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

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
   LL_PWR_EnableBkUpAccess();
   
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* Enables the PWR Clock and Enables access to the backup domain #######*/
  /* To change the source clock of the RTC feature (LSE, LSI), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_RCC_EnableRTC();




  /* Configure LSE/LSI as RTC clock source ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() == 0)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }
#endif /* USE_TIMEOUT */
    }
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  }
#elif defined(RTC_CLOCK_SOURCE_LSI)
  /* Enable LSI */
  LL_RCC_LSI_Enable();
#if (USE_TIMEOUT == 1)
  Timeout = LSI_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_LSI_IsReady() != 1)
  {
#if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      Timeout --;
    }
    if (Timeout == 0)
    {
      /* LSI activation error */
      LED_Blinking(LED_BLINK_ERROR);
    }
#endif /* USE_TIMEOUT */
  }
  /* Reset backup domain only if LSI is not yet selected as RTC clock source */
  if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }

#else
#error "configure clock for RTC"
#endif


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* Turn-on LED4 to indicate that calendar has been well configured */
  LED_On();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* Display the updated Time and Date */
    Show_RTC_Calendar();

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
  LL_PWR_EnableBkUpAccess();
  while (LL_PWR_IsEnabledBkUpAccess () == 0U)
  {
  }

  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};
  LL_RTC_TimeTypeDef RTC_TimeStruct = {0};
  LL_RTC_DateTypeDef RTC_DateStruct = {0};

  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTCAPB);

  /* USER CODE BEGIN RTC_Init 1 */

  /* Check if Data stored in BackUp register1: No Need to reconfigure RTC */
  /* Read the Back Up Register 1 Data */
  if (LL_RTC_BKP_GetRegister((TAMP_TypeDef*)RTC, LL_RTC_BKP_DR1) != RTC_BKP_DATE_TIME_UPDTATED)
  {

    /* Disable RTC registers write protection */
    LL_RTC_DisableWriteProtection(RTC);

    /* Enter in initialization mode */
    if (Enter_RTC_InitMode() != RTC_ERROR_NONE)
    {
      /* Initialization Error */
      LED_Blinking(LED_BLINK_ERROR);
    }
  /* USER CODE END RTC_Init 1 */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_AMPM;
  RTC_InitStruct.AsynchPrescaler = RTC_ASYNCH_PREDIV;
  RTC_InitStruct.SynchPrescaler = RTC_SYNCH_PREDIV;
  LL_RTC_Init(RTC, &RTC_InitStruct);

  /** Initialize RTC and set the Time and Date
  */
  RTC_TimeStruct.TimeFormat = LL_RTC_TIME_FORMAT_AM_OR_24;
  RTC_TimeStruct.Hours = 0x2;
  RTC_TimeStruct.Minutes = 0x0;
  RTC_TimeStruct.Seconds = 0x0;

  LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_TimeStruct);
  RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_FRIDAY;
  RTC_DateStruct.Month = LL_RTC_MONTH_DECEMBER;
  RTC_DateStruct.Day = 0x12;
  RTC_DateStruct.Year = 0x23;

  LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_DateStruct);
  /* USER CODE BEGIN RTC_Init 2 */

    /* Exit of initialization mode */
    if (Exit_RTC_InitMode() != RTC_ERROR_NONE)
    {
      /* Initialization Error */
      LED_Blinking(LED_BLINK_ERROR);
    }

    /* Enable RTC registers write protection */
    LL_RTC_EnableWriteProtection(RTC);

    /* Writes a data in a RTC Backup data Register1 */
    LL_RTC_BKP_SetRegister((TAMP_TypeDef*)RTC, LL_RTC_BKP_DR1, RTC_BKP_DATE_TIME_UPDTATED);

  }
  /* USER CODE END RTC_Init 2 */

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
  LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);

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

/**
  * @brief  Enter in initialization mode
  * @note In this mode, the calendar counter is stopped and its value can be updated
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t Enter_RTC_InitMode(void)
{
  /* Set Initialization mode */
  LL_RTC_EnableInitMode(RTC);

#if (USE_TIMEOUT == 1)
  Timeout = RTC_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */

  /* Check if the Initialization mode is set */
  while (LL_RTC_IsActiveFlag_INIT(RTC) != 1)
  {
#if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      Timeout --;
    }
    if (Timeout == 0)
    {
      return RTC_ERROR_TIMEOUT;
    }
#endif /* USE_TIMEOUT */
  }

  return RTC_ERROR_NONE;
}

/**
  * @brief  Exit Initialization mode
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t Exit_RTC_InitMode(void)
{
  LL_RTC_DisableInitMode(RTC);

  /* Wait for synchro */
  /* Note: Needed only if Shadow registers is enabled           */
  /*       LL_RTC_IsShadowRegBypassEnabled function can be used */
  return (WaitForSynchro_RTC());
}

/**
  * @brief  Wait until the RTC Time and Date registers (RTC_TR and RTC_DR) are
  *         synchronized with RTC APB clock.
  * @param  None
  * @retval RTC_ERROR_NONE if no error (RTC_ERROR_TIMEOUT will occur if RTC is
  *         not synchronized)
  */
uint32_t WaitForSynchro_RTC(void)
{
  /* Clear RSF flag */
  LL_RTC_ClearFlag_RS(RTC);

#if (USE_TIMEOUT == 1)
  Timeout = RTC_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */

  /* Wait the registers to be synchronised */
  while (LL_RTC_IsActiveFlag_RS(RTC) != 1)
  {
#if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      Timeout --;
    }
    if (Timeout == 0)
    {
      return RTC_ERROR_TIMEOUT;
    }
#endif /* USE_TIMEOUT */
  }
  return RTC_ERROR_NONE;
}

/**
  * @brief  Display the current time and date.
  * @param  None
  * @retval None
  */
void Show_RTC_Calendar(void)
{
  /* Note: need to convert in decimal value in using __LL_RTC_CONVERT_BCD2BIN helper macro */
  /* Display time Format : hh:mm:ss */
  sprintf((char *)aShowTime, "%.2d:%.2d:%.2d", __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC)),
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC)),
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC)));
  /* Display date Format : mm-dd-yyyy */
  sprintf((char *)aShowDate, "%.2d-%.2d-%.2d", __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC)),
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC)),
          2000 + __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC)));
}

/**
  * @brief  Turn-on LED4.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED4 on */
  LL_GPIO_SetOutputPin(LED4_GPIO_Port, LED4_Pin);
}

/**
  * @brief  Set LED4 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
    LL_mDelay(Period);
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
