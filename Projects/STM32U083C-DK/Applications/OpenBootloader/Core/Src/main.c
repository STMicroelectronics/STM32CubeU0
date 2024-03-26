/**
  ******************************************************************************
  * @file    main.c
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_openbootloader.h"
#include "interfaces_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/**
  * @brief  Main program
  * @retval None
  */
int main(void)
{
  /* STM32U0xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Low Level Initialization
  */
  HAL_Init();

  /* Configure the System clock to have a frequency of 24 MHz */
  SystemClock_Config();

  /* Initialize the OpenBootloader */
  OpenBootloader_Init();

  while (true)
  {
    OpenBootloader_ProtocolDetection();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 24000000
  *            HCLK(Hz)                       = 24000000
  *            AHB Prescaler                  = 0
  *            APB1 Prescaler                 = 0
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 2
  *            PLL_N                          = 24
  *            PLL_P                          = 8
  *            PLL_Q                          = 8
  *            PLL_R                          = 8
  *            Flash Latency(WS)              = 0
  *            Voltage range                  = 1
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0U};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0U};

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0U};

  /* HSI Oscillator enabled at reset (16Mhz), activate PLL with HSI as source */
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN            = 24U;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV8;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV8;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV8;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* To avoid undershoot due to maximum frequency, select PLL as system clock source
     with AHB prescaler divider 0 as first step */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is used to de-initialized the clock.
  * @retval None
  */
void System_DeInit(void)
{
  HAL_RCC_DeInit();

  /* Disable GPIOx */
  LL_GPIO_DeInit(GPIOA);
  LL_GPIO_DeInit(GPIOB);

  NVIC_DisableIRQ(SysTick_IRQn);

  /* Disable interfaces */
  USARTx_DEINIT();
  I2Cx_DEINIT();
  SPIx_DEINIT();

  /* Disable USB interrupt */
  HAL_NVIC_DisableIRQ(USB_DRD_FS_IRQn);

  /* Disable SPI interrupt */
  HAL_NVIC_DisableIRQ(SPIx_IRQ);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  while (true)
  {
  }
}

#ifdef USE_FULL_ASSERT
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
  while (true)
  {
  }
}
#endif /* USE_FULL_ASSERT */
