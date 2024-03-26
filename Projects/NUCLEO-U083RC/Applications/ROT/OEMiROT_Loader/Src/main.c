/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main application file.
  *          This application demonstrates Firmware Update, protections
  *          and crypto testing functionalities.
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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "com.h"
#include "common.h"

#if defined(__ICCARM__)
#include <LowLevelIOInterface.h>
#endif /* __ICCARM__ */

#include "stm32u0xx_ll_rcc.h"
#include "stm32u0xx_ll_pwr.h"
#include "stm32u0xx_ll_system.h"
#include "stm32u0xx_ll_utils.h"

#include "fw_update_app.h"

/** @defgroup  USER_APP  exported variable
   * @{
  */

/** @addtogroup USER_APP User App Example
  * @{
  */


/** @addtogroup USER_APP_COMMON Common
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
int io_putchar(int ch);
#define PUTCHAR_PROTOTYPE int io_putchar(int ch)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int32_t uart_putc(int32_t ch)
#endif /* __ICCARM__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  */
PUTCHAR_PROTOTYPE
{
  COM_Transmit((uint8_t*)&ch, 1, TX_TIMEOUT);
  return ch;
}

/* Redirects printf to DRIVER_STDIO in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
FILE __stdout;

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
  /* Send byte to USART */
  io_putchar(ch);

  /* Return character written */
  return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to DRIVER_STDIO in case of GNUARM */
int _write(int fd, char *str, int len)
{
  int i;

  for (i = 0; i < len; i++)
  {
    /* Send byte to USART */
    uart_putc(str[i]);
  }

  /* Return the number of characters written */
  return len;
}
#elif defined(__ICCARM__)
size_t __write(int file, unsigned char const *ptr, size_t len)
{
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx = 0; idx < len; idx++)
  {
    iar_fputc((int)*pdata);
    pdata++;
  }
  return len;
}
#endif /*  __GNUC__ */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(int argc, char **argv)
{

  /* STM32U0xx HAL library initialization:
  - Systick timer is configured by default as source of time base, but user
  can eventually implement his proper time base source (a general purpose
  timer for example or other time source), keeping in mind that Time base
  duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
  handled in milliseconds basis.
  - Set NVIC Group Priority to 3
  - Low Level Initialization
  */
  HAL_Init();

  /* DeInitialize RCC to allow PLL reconfiguration when configuring system clock */
  HAL_RCC_DeInit();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure Communication module */
  COM_Init();

  printf("\r\n===========================================================");
  printf("\r\n=          (C) COPYRIGHT 2023 STMicroelectronics          =");
  printf("\r\n=                                                         =");
  printf("\r\n=                     LOCAL LOADER                        =");
  printf("\r\n===========================================================");
  printf("\r\n\r\n");

  /* User App firmware runs*/
  FW_UPDATE_Run();

  while (1U)
  {}

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            Flash Latency(WS)              = 1
  * @param  None
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
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
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
  /* User can add his own implementation to report the file name and line number,
   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1U)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */
