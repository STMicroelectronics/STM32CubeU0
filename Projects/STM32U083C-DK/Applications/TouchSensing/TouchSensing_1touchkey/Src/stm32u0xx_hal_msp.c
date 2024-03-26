/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32u0xx_hal_msp.c
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief TSC MSP Initialization
* This function configures the hardware resources used in this example
* @param htsc: TSC handle pointer
* @retval None
*/
void HAL_TSC_MspInit(TSC_HandleTypeDef* htsc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htsc->Instance==TSC)
  {
  /* USER CODE BEGIN TSC_MspInit 0 */

  /* USER CODE END TSC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TSC_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**TSC GPIO Configuration
    PB12     ------> TSC_G1_IO1
    PB13     ------> TSC_G1_IO2
    PD10     ------> TSC_G6_IO1
    PD11     ------> TSC_G6_IO2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TSC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TSC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TSC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TSC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* TSC interrupt Init */
    HAL_NVIC_SetPriority(TSC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TSC_IRQn);
  /* USER CODE BEGIN TSC_MspInit 1 */

  /* USER CODE END TSC_MspInit 1 */
  }

}

/**
* @brief TSC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htsc: TSC handle pointer
* @retval None
*/
void HAL_TSC_MspDeInit(TSC_HandleTypeDef* htsc)
{
  if(htsc->Instance==TSC)
  {
  /* USER CODE BEGIN TSC_MspDeInit 0 */

  /* USER CODE END TSC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TSC_CLK_DISABLE();

    /**TSC GPIO Configuration
    PB12     ------> TSC_G1_IO1
    PB13     ------> TSC_G1_IO2
    PD10     ------> TSC_G6_IO1
    PD11     ------> TSC_G6_IO2
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_10|GPIO_PIN_11);

    /* TSC interrupt DeInit */
    HAL_NVIC_DisableIRQ(TSC_IRQn);
  /* USER CODE BEGIN TSC_MspDeInit 1 */

  /* USER CODE END TSC_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
