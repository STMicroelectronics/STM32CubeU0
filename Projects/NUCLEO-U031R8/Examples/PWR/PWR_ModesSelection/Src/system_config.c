/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/system_config.c
  * @author  MCD Application Team
  * @brief   This file provides system configuration.
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

/* Includes ------------------------------------------------------------------*/
#include "system_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USER_MSI_CALIBRATION  0U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
static System_StatusTypeDef SystemClock_16MHz(void);
static System_StatusTypeDef SystemClock_48MHz(void);
static System_StatusTypeDef SystemClock_2MHz(void);
static System_StatusTypeDef RTC_Enable(void);
static System_StatusTypeDef RTC_Disable(void);

extern UART_HandleTypeDef huart1;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Optimize system power consumption configuration.
  * @param  None.
  * @retval System Status.
  */
void system_config(System_LowPowerModeScenarioTypeDef scenario)
{
  GPIO_InitTypeDef   GPIO_InitStruct   = {0};

  /* De-Initialization of UART1 */
  HAL_UART_DeInit(&huart1);

  /* Configure the GPIO in low-frequency */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
#if defined (GPIOE)
  __HAL_RCC_GPIOE_CLK_ENABLE();
#endif /* GPIOE */
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Set parameters to be configured */
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Pin   = GPIO_PIN_ALL;

  /* Initialize all GPIO pins */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
#if defined (GPIOE)
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
#endif /* GPIOE */
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* Disable all GPIO clocks */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
#if defined (GPIOE)
  __HAL_RCC_GPIOE_CLK_DISABLE();
#endif /* GPIOE */
  __HAL_RCC_GPIOF_CLK_DISABLE();

  /* Enforcing true STOP and STANDBY modes, by disabling their emulation. */
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();

  /* Clock disable for APB1 Bus */
  CLEAR_REG(RCC->APBENR1);

  /* Clock disable for IO port ins Sleep modes */
  if( (scenario == SYSTEM_LOWPOWER_S10) ||
      (scenario == SYSTEM_LOWPOWER_S11) ||
      (scenario == SYSTEM_LOWPOWER_S8) )
  {
    CLEAR_REG(RCC->IOPSMENR);
  }

  /* HSI16 disable for LP-Sleep/Run modes */
    if( (scenario == SYSTEM_LOWPOWER_S8) || (scenario == SYSTEM_LOWPOWER_S9))
    {
      __HAL_RCC_HSI_DISABLE();
    }

    /* HSI16 disable for LP-Sleep/Run modes */
    if( (scenario == SYSTEM_LOWPOWER_S4) || (scenario == SYSTEM_LOWPOWER_S5))
    {
      CLEAR_REG(RCC->IOPSMENR);
      __HAL_RCC_HSI_DISABLE();
      __HAL_RCC_MSI_DISABLE();
      __HAL_RCC_SYSCFG_CLK_DISABLE();
    }

}

/**
  * @brief  Configure rtc state.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_rtc_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  switch (scenario)
  {
    /* Check if rtc state is DISABLE */
    case SYSTEM_LOWPOWER_S2:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      __HAL_RCC_RTCAPB_CLK_DISABLE();
      /* Disable Default MSI because LSE has been enabled by the RTC Init */
      __HAL_RCC_MSI_DISABLE();
      break;
    }
    case SYSTEM_LOWPOWER_S3:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      __HAL_RCC_RTCAPB_CLK_DISABLE();
      /* Disable Default MSI because LSE has been enabled by the RTC Init */
      __HAL_RCC_MSI_DISABLE();
      break;
    }
    case SYSTEM_LOWPOWER_S5:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }

      __HAL_RCC_RTCAPB_CLK_DISABLE();
      /* Disable Default MSI because LSE has been enabled by the RTC Init */
      __HAL_RCC_MSI_DISABLE();
      break;
    }
    case SYSTEM_LOWPOWER_S7:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      __HAL_RCC_RTCAPB_CLK_DISABLE();
      /* Disable Default MSI */
      __HAL_RCC_MSI_DISABLE();
      break;
    }
    default:
    {
      if (RTC_Disable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }

      /* Disable RTC Clk */
      __HAL_RCC_RTCAPB_CLK_DISABLE();
      __HAL_RCC_RTC_DISABLE();
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system clock frequency.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_clock_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Disable all peripherals clocks in Sleep and Stop modes */
  CLEAR_REG(RCC->AHBSMENR);
  CLEAR_REG(RCC->APBSMENR1);
  CLEAR_REG(RCC->APBSMENR2);

  switch (scenario)
  {
    case SYSTEM_LOWPOWER_S8:
    {
      if (SystemClock_2MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
    case SYSTEM_LOWPOWER_S9:
    {
      if (SystemClock_2MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
    case SYSTEM_LOWPOWER_S10:
    {
      if (SystemClock_16MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
    case SYSTEM_LOWPOWER_S11:
    {
      if (SystemClock_48MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
    case SYSTEM_LOWPOWER_S12:
    {
      if (SystemClock_16MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
    case SYSTEM_LOWPOWER_S13:
    {
      if (SystemClock_48MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
    default:
    {
      /* Default System clock */
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure SRAM retention.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_ram_retention_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  switch (scenario)
  {
    /* Check if system low power scenario is S3 */
    case SYSTEM_LOWPOWER_S3:
    {
      HAL_PWREx_EnableSRAMContentRetention();
      break; /* Do nothing, this power mode does not support memory retention */
    }
    /* Disable SRAM Retention if mode selection is not SYSTEM_LOWPOWER_S3 */
    default:
    {
      HAL_PWREx_DisableSRAMContentRetention();
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system clock frequency to 16MHz (MSI range 8).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_16MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = USER_MSI_CALIBRATION;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_8;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_NONE ;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2; /* 24MHz */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure system clock frequency to 48MHz (MSI range 11).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_48MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = USER_MSI_CALIBRATION;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_NONE ;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2; /* 24MHz */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 );
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure system clock frequency to 2MHz (MSI range 5).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_2MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  RCC_OscInitStruct.MSICalibrationValue = USER_MSI_CALIBRATION;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_5; /* MSI = 2MHz */
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_NONE;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2; /* 24MHz */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 );
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure the RTC peripheral and Initialize it.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Enable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Set parameter to be configured */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv   = 0x7FU;
  RTCHandle.Init.SynchPrediv    = 0xF9U;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  RTCHandle.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;

  /* Initialize the RTC peripheral */
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Set Calendar Ultra-Low power mode */
  if (HAL_RTCEx_SetLowPowerCalib(&RTCHandle, RTC_LPCAL_SET) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  De-Initialize the RTC peripheral.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Disable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Check RTC state */
  if (HAL_RTC_GetState(&RTCHandle) != HAL_RTC_STATE_RESET)
  {
    /* De-Initialize the RTC peripheral */
    if (HAL_RTC_DeInit(&RTCHandle) != HAL_OK)
    {
      return SYSTEM_ERROR;
    }
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure system low power mode.
  * @param  scenario : Specifies system low-power mode secanrio from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_lowpower_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* SuspenTick to prevent pending interruption from blocking the switch to stop1 and stop2 modes */
  HAL_SuspendTick();

  switch (scenario)
  {
    /* Check if system low power mode is Shutdown */
    case SYSTEM_LOWPOWER_S0:
    {
      /* Enter the system to SHUTDOWN mode */
      HAL_PWR_EnterSHUTDOWNMode();
      break;
    }

    /* Check if system low power is Standby */
    case SYSTEM_LOWPOWER_S1:
    {
      /* Clear StandBy Flag if returned from StandBy mode */
      if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB))
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
      }
      /* Enter the system to STANDBY mode */
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Check if system low power is Standby */
    case SYSTEM_LOWPOWER_S2:
    {
      /* Clear StandBy Flag if returned from StandBy mode */
      if ( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) )
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
      }
      /* Enter the system to STANDBY mode */
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Check if system low power is Standby */
    case SYSTEM_LOWPOWER_S3:
    {
      /* Clear StandBy Flag if returned from StandBy mode */
      if ( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) )
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
      }
      /* Enter the system to STANDBY mode */
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Check if system low power is Stop2 */
    case SYSTEM_LOWPOWER_S4:
    {
      if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOP2))
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP2);
      }
      /* Enter the system to STOP 2 mode */
      HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
      break;
    }

    /* Check if system low power is Stop2 */
    case SYSTEM_LOWPOWER_S5:
    {
      if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOP2))
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP2);
      }
      /* Enter the system to STOP 2 mode */
      HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
      break;
    }

    /* Check if system low power is Stop1 */
    case SYSTEM_LOWPOWER_S6:
    {
      if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOP1))
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP1);
      }
      /* Enter the system to STOP 1 mode */
      HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFE);
      break;
    }

    /* Check if system low power is Stop1 */
    case SYSTEM_LOWPOWER_S7:
    {
      if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOP1))
      {
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP1);
      }
      /* Enter the system to STOP 1 mode */
      HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFE);
      break;
    }

    /* Check if system low power mode is LP-Sleep */
    case SYSTEM_LOWPOWER_S8:
    {
      /* Enter the system into LP-Sleep mode */
      HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Check if system low power mode is LP-Run */
    case SYSTEM_LOWPOWER_S9:
    {
      /* Enter the system into LP-RUn mode */
      HAL_PWREx_EnableLowPowerRunMode();
      break;
    }

    /* Check if system low power mode is Sleep */
    case SYSTEM_LOWPOWER_S10:
    {
      /* Enter the system into Sleep mode */
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Check if system low power mode is Sleep */
    case SYSTEM_LOWPOWER_S11:
    {
      /* Enter the system into Sleep mode */
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    }

    default:
    {
      /* Run mode */
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system regulator.
  * @param  scenario : Specifies system low-power mode secanrio from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_regulator_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  switch (scenario)
  {
    /* Check if system regulator is in low power mode STOP1 with MR Off */
    case SYSTEM_LOWPOWER_S4:
    {
      HAL_PWREx_EnableLowPowerRunMode();
      HAL_PWREx_EnableUltraLowPowerMode();
      break;
    }
    case SYSTEM_LOWPOWER_S5:
    {
      HAL_PWREx_EnableLowPowerRunMode();
      HAL_PWREx_EnableUltraLowPowerMode();
      break;
    }
    case SYSTEM_LOWPOWER_S6:
    {
      HAL_PWREx_EnableLowPowerRunMode();
      break;
    }
    /* Check if system regulator is in low power mode STOP1 with MR Off */
    case SYSTEM_LOWPOWER_S7:
    {
      HAL_PWREx_EnableLowPowerRunMode();
      break;
    }
    case SYSTEM_LOWPOWER_S8:
    {
      HAL_PWREx_EnableLowPowerRunMode();
      break;
    }
    case SYSTEM_LOWPOWER_S9:
    {
      HAL_PWREx_EnableLowPowerRunMode();
      break;
    }
    default:
    {
      HAL_PWREx_DisableLowPowerRunMode();
      HAL_PWREx_DisableUltraLowPowerMode();
      /* Do notthing */
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system flash power donw mode.
  * @param  scenario : Specifies system low-power mode secanrio from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_flash_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;
    switch (scenario)
  {
    case SYSTEM_LOWPOWER_S6:
    {
      HAL_PWREx_EnableFlashPowerDown(PWR_FLASHPD_STOP);
      break;
    }
    case SYSTEM_LOWPOWER_S7:
    {
      HAL_PWREx_EnableFlashPowerDown(PWR_FLASHPD_STOP);
      break;
    }
    case SYSTEM_LOWPOWER_S8:
    {
      HAL_PWREx_EnableFlashPowerDown(PWR_FLASHPD_LPSLEEP);
      break;
    }
    case SYSTEM_LOWPOWER_S9:
    {
      HAL_PWREx_DisableFlashPowerDown(PWR_FLASHPD_LPRUN);
      __HAL_RCC_FLASH_CLK_SLEEP_DISABLE();
      break;
    }
    default:
    {
      /* Do notthing */
      break;
    }
  }

  return status;
}
