/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_1touchkey_LowPower\Src\main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "touchsensing.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tsl.h"
#include "stm32u083c_discovery.h"
#include "stm32u083c_discovery_glass_lcd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* TSC variable and define */
#define TKEY_DET(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_DETECT)
#define TKEY_PRX(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_PROX)
#define TKEY_REL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_RELEASE)
#define TKEY_CAL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_CALIB)

extern uint32_t idx_bank_it;
extern uint32_t config_done_it;
extern uint32_t acq_done_it;
extern __IO TSL_tTick_ms_T ECSLastTick;
extern TSL_TouchKeyParam_T MyTKeys_Param[];
extern COMP_HandleTypeDef hcomp2;

__IO uint32_t uwTSC_WakeupDetected = 1;

/* Select to use or not the LCD (0=No, 1=Yes) */
#define USE_LCD (1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef hcomp2;

RTC_HandleTypeDef hrtc;

TSC_HandleTypeDef htsc;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_COMP2_Init(void);
static void MX_RTC_Init(void);
static void MX_TSC_Init(void);
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
  uint32_t count_nb_acq = 2000;

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
  MX_COMP2_Init();
  MX_RTC_Init();
  MX_TSC_Init();
  MX_TOUCHSENSING_Init();
  /* USER CODE BEGIN 2 */

  /* LEDs Initialisation */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);

  /* Configure LCD */
#if USE_LCD > 0
  BSP_LCD_GLASS_Init();

  BSP_LCD_GLASS_Clear();
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" STM32U083xx  ", 1, SCROLL_SPEED_MEDIUM);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" TouchSensing Low Power ", 1, SCROLL_SPEED_MEDIUM);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" Application  ",  1, SCROLL_SPEED_MEDIUM);
#endif

  /* ShutDown the debugger to reduce current consumption */
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();
  __HAL_RCC_DBGMCU_CLK_DISABLE();

  /* Display "RUN MODE" at startup */
#if USE_LCD > 0
  BSP_LCD_GLASS_Clear();
#endif
  BSP_LED_On(LED4);
  HAL_Delay(500);
#if USE_LCD > 0
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" RUN MODE", 2, SCROLL_SPEED_MEDIUM);
#endif

  /* Start Comparator for TSC detection */
  HAL_COMP_Start(&hcomp2);

  /* Let TSC and TSL run for 5s to set up ECS */
  while(count_nb_acq)
  {
    if (tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY)
    {
      /* Restart TSLPRM_TOTAL_BANKS banks acquisition */
      idx_bank_it = 0;
      acq_done_it = 0;
      TSL_acq_BankConfig(idx_bank_it);
      TSL_acq_BankStartAcq_IT();
      count_nb_acq--;
    }
    else
    {
      HAL_Delay(1);
    }
  }

  /* Reduce the discharge time of the sampling capacitor */
  TSL_Globals.DelayDischarge = 0;

  /* Start RTC Timer for 250ms interval */
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 500, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0x0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Run TSC acquisition in blocking mode after wake up */
    if (tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY)
    {
        if (!TKEY_CAL(0))
        {
          if (TKEY_DET(0))
          {
            BSP_LED_On(LED4);
#if USE_LCD > 0
            BSP_LCD_GLASS_Clear();
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)"WAKEUP TOUCH DETECTED", 1, SCROLL_SPEED_MEDIUM);
#endif
            HAL_Delay(500);
            uwTSC_WakeupDetected = 1;
          }
          else if (TKEY_REL(0) && (uwTSC_WakeupDetected == 1))
          {
#if USE_LCD > 0
            BSP_LCD_GLASS_Clear();
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)"STOP2 MODE ENTERING", 1, SCROLL_SPEED_MEDIUM);
#endif
            BSP_LED_Off(LED4);
            uwTSC_WakeupDetected = 0;
          }
        }

      /* Go into stop 2 mode */
      HAL_SuspendTick();
      HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
      HAL_ResumeTick();

      /* Clear Wakeup flag */
      SET_BIT(RTC->SCR,RTC_SCR_CWUTF);

      /* Wakeup and Restart TSLPRM_TOTAL_BANKS banks acquisition */
      idx_bank_it = 0;
      acq_done_it = 0;
      TSL_acq_BankConfig(idx_bank_it);
      TSL_acq_BankStartAcq_IT();
    }
    else
    {
      /* SuspendTick to reduce current consumption BUT ECS WILL NOT WORK */
      HAL_SuspendTick();
      /* Enter the CPU to SLEEP mode */
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      HAL_ResumeTick();
    }
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
  * @brief COMP2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_COMP2_Init(void)
{

  /* USER CODE BEGIN COMP2_Init 0 */

  /* USER CODE END COMP2_Init 0 */

  /* USER CODE BEGIN COMP2_Init 1 */

  /* USER CODE END COMP2_Init 1 */
  hcomp2.Instance = COMP2;
  hcomp2.Init.InputPlus = COMP_INPUT_PLUS_IO4;
  hcomp2.Init.InputMinus = COMP_INPUT_MINUS_VREFINT;
  hcomp2.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  hcomp2.Init.WindowOutput = COMP_WINDOWOUTPUT_EACH_COMP;
  hcomp2.Init.Hysteresis = COMP_HYSTERESIS_HIGH;
  hcomp2.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
  hcomp2.Init.Mode = COMP_POWERMODE_ULTRALOWPOWER;
  hcomp2.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  hcomp2.Init.TriggerMode = COMP_TRIGGERMODE_NONE;
  if (HAL_COMP_Init(&hcomp2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN COMP2_Init 2 */

  /* USER CODE END COMP2_Init 2 */

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
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 249;
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

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 499, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TSC Initialization Function
  * @param None
  * @retval None
  */
static void MX_TSC_Init(void)
{

  /* USER CODE BEGIN TSC_Init 0 */

  /* USER CODE END TSC_Init 0 */

  /* USER CODE BEGIN TSC_Init 1 */

  /* USER CODE END TSC_Init 1 */

  /** Configure the TSC peripheral
  */
  htsc.Instance = TSC;
  htsc.Init.CTPulseHighLength = TSC_CTPH_6CYCLES;
  htsc.Init.CTPulseLowLength = TSC_CTPL_6CYCLES;
  htsc.Init.SpreadSpectrum = DISABLE;
  htsc.Init.SpreadSpectrumDeviation = 1;
  htsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  htsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  htsc.Init.MaxCountValue = TSC_MCV_16383;
  htsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  htsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  htsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  htsc.Init.MaxCountInterrupt = DISABLE;
  htsc.Init.ShieldIOs = TSC_GROUP1_IO1;
  htsc.Init.ChannelIOs = TSC_GROUP6_IO2;
  htsc.Init.SamplingIOs = TSC_GROUP1_IO2|TSC_GROUP6_IO1;
  if (HAL_TSC_Init(&htsc) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_SYSCFG_SetTSCComparatorModeIO(SYSCFG_COMP_MODE_GRP6_IO1);
  HAL_SYSCFG_EnableTSCComparatorMode();
  /* USER CODE BEGIN TSC_Init 2 */

  /* USER CODE END TSC_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  while (1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(200);
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
