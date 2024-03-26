/**
  ******************************************************************************
  * @file         stm32u0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
#include "interfaces_conf.h"

/**
  * @brief  Initialize the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/**
  * @brief PCD MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
  GPIO_InitTypeDef GPIO_InitStruct             = {0U};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0U};

  if (hpcd->Instance == USB_DRD_FS)
  {
    /** Initializes the peripherals clock */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /** USB GPIO Configuration
        PA11     ------> USB_DM
        PA12     ------> USB_DP
      */
    GPIO_InitStruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_USB;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_CLK_ENABLE();

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddUSB();

    /* USB_DRD_FS interrupt Init */
    HAL_NVIC_SetPriority(USB_DRD_FS_IRQn, 6U, 0U);
    HAL_NVIC_EnableIRQ(USB_DRD_FS_IRQn);

  }
}

/**
  * @brief PCD MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hpcd: PCD handle pointer
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
  if (hpcd->Instance == USB_DRD_FS)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USB_CLK_DISABLE();

    /** USB GPIO Configuration
        PA11     ------> USB_DM
        PA12     ------> USB_DP
      */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

    /* USB_DRD_FS interrupt DeInit */
    HAL_NVIC_DisableIRQ(USB_DRD_FS_IRQn);
  }
}
