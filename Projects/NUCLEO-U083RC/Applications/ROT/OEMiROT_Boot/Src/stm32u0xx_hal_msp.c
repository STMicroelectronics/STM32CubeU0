/**
  ******************************************************************************
  * @file    stm32u0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
#include "boot_hal_cfg.h"

/** @addtogroup STM32U0xx_HAL_Examples
  * @{
  */

/** @defgroup RTC_ActiveTamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RCC_LSI_TIMEOUT_VALUE           ((uint32_t)2U)    /* 2 ms (minimum Tick + 1) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constant ----------------------------------------------------------*/
/** @defgroup HAL_MSP_Private_Constant
  * @{
  */
const  RCC_OscInitTypeDef RCC_OscInitStruct_RTC =
{
    .OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE,
    .HSEState = 0,
#if defined(RTC_CLOCK_SOURCE_LSE)
    .LSEState = RCC_LSE_ON,
    .LSIState = RCC_LSI_OFF,
#elif defined (RTC_CLOCK_SOURCE_LSI)
    .LSEState = RCC_LSE_OFF,
    .LSIState = RCC_LSI_ON,
#else
#error
#endif /* defined (RTC_CLOCK_SOURCE_LSI) */
    .HSIState = 0,
    .HSICalibrationValue = 0,
    .HSI48State = 0,
    .PLL = {
        .PLLState = RCC_PLL_NONE,
        .PLLSource = 0,
        .PLLM = 0,
        .PLLN = 0,
        .PLLP = 0,
        .PLLQ = 0,
        .PLLR = 0,
    }
};

const RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct_RTC =
{
    .PeriphClockSelection = RCC_PERIPHCLK_RTC,
#if defined(RCC_CR_PLL3ON)
    .PLL3 = {0},
#endif /* RCC_CR_PLL3ON */
#if defined(UART4)
    .Uart4ClockSelection = 0,
#endif /* UART4 */
#if defined(UART5)
    .Uart5ClockSelection = 0,
#endif /* UART5 */
#if defined(USART6)
    .Usart6ClockSelection = 0,
#endif /* UART6 */
#if defined(UART7)
    .Uart7ClockSelection = 0,
#endif /* UART7 */
#if defined(UART8)
    .Uart8ClockSelection = 0,
#endif /* UART8 */
#if defined(UART9)
    .Uart9ClockSelection = 0,
#endif /* UART9 */
#if defined(USART10)
    .Usart10ClockSelection = 0,
#endif /* UART10 */
#if defined(USART11)
    .Usart11ClockSelection = 0,
#endif /* UART11 */
#if defined(USART12)
    .Uart12ClockSelection = 0,
#endif /* UART12 */
#if defined(I2C3)
#endif /* I2C3 */
#if defined(I2C4)
#endif /* I2C4 */
#if defined(I3C2)
#endif /* I3C2 */
#if defined(LPTIM3)
#endif /* LPTIM3 */
#if defined(LPTIM4)
    .Lptim4ClockSelection = 0,
#endif /* LPTIM4 */
#if defined(LPTIM5)
    .Lptim5ClockSelection = 0,
#endif /* LPTIM5 */
#if defined(LPTIM6)
    .Lptim6ClockSelection = 0,
#endif /* LPTIM6 */
#if defined(SAI1)
    .Sai1ClockSelection = 0,
#endif /* SAI1 */
#if defined(SAI2)
    .Sai2ClockSelection = 0,
#endif /* SAI2 */
#if defined(SDMMC1)
    .Sdmmc1ClockSelection = 0,
#endif /* SDMMC1 */
#if defined(SDMMC2)
    .Sdmmc2ClockSelection = 0,
#endif /* SDMMC2 */
#if defined(OCTOSPI1)
    .OspiClockSelection = 0,
#endif
#if defined(SPI4)
    .Spi4ClockSelection = 0,
#endif /* SPI4 */
#if defined(SPI5)
    .Spi5ClockSelection = 0,
#endif /* SPI5 */
#if defined(SPI6)
    .Spi6ClockSelection = 0,
#endif /* SPI6 */
#if defined(RTC_CLOCK_SOURCE_LSE)
    .RTCClockSelection = RCC_RTCCLKSOURCE_LSE,
#elif defined (RTC_CLOCK_SOURCE_LSI)
    .RTCClockSelection = RCC_RTCCLKSOURCE_LSI,
#else
#error
#endif /* RTC_CLOCK_SOURCE_LSE */
#if defined(CEC)
    .CecClockSelection = 0,
#endif /* CEC */
};
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    /*  not required to be removed */
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
    /* Reset the whole backup domain, RTC included */
    if (HAL_RCC_OscConfig((RCC_OscInitTypeDef *)&RCC_OscInitStruct_RTC) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_RCCEx_PeriphCLKConfig((RCC_PeriphCLKInitTypeDef *)&PeriphClkInitStruct_RTC) != HAL_OK)
    {
        Error_Handler();
    }
    /* Enable RTC */
    __HAL_RCC_RTC_ENABLE();

    /* Enable RTC clock  */
    __HAL_RCC_RTCAPB_CLK_ENABLE();

    HAL_NVIC_SetPriority(RTC_TAMP_IRQn, 0x4, 0);
    HAL_NVIC_EnableIRQ(RTC_TAMP_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    /*##-1- Reset peripherals ##################################################*/
    __HAL_RCC_RTC_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
