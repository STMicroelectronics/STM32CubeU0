/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u083c_discovery.h"
#include "stm32u083c_discovery_env_sensors.h"
#include "stm32u083c_discovery_glass_lcd.h"
#include "stm32u083c_discovery_bus.h"  
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* User can use this section to tailor TSCx/TSCx instance used and associated
   resources */
/* Definition for TSCx clock resources */
#define TSCx                                        TSC
#define TSCx_CLK_ENABLE()                           __HAL_RCC_TSC_CLK_ENABLE()
#define TSCx_TS1_IO_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define TSCx_SHIELD_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define TSCx_TS1_CAPACITOR_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()

#define TSCx_SHIELD_CAPACITOR_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()

#define TSCx_FORCE_RESET()                          __HAL_RCC_TSC_FORCE_RESET()
#define TSCx_RELEASE_RESET()                        __HAL_RCC_TSC_RELEASE_RESET()

/* Definition for TSCx IO Pins */
#define TSCx_TS1_IO_PIN                             GPIO_PIN_11
#define TSCx_TS1_IO_GPIO_PORT                       GPIOD
#define TSCx_TS1_IO_AF                              GPIO_AF9_TSC


#define TSCx_SHIELD_IO_PIN                          GPIO_PIN_12
#define TSCx_SHIELD_IO_GPIO_PORT                    GPIOB
#define TSCx_SHIELD_IO_AF                           GPIO_AF9_TSC

/* Definition for TSCx Sampling Capacitor Pins */
#define TSCx_TS1_SAMPLING_PIN                       GPIO_PIN_10
#define TSCx_TS1_SAMPLING_GPIO_PORT                 GPIOD
#define TSCx_TS1_SAMPLING_AF                        GPIO_AF9_TSC


#define TSCx_SHIELD_SAMPLING_PIN                    GPIO_PIN_13
#define TSCx_SHIELD_SAMPLING_GPIO_PORT              GPIOB
#define TSCx_SHIELD_SAMPLING_AF                     GPIO_AF9_TSC

/* Definition for TSCx's NVIC */
#define TSC_IRQn                              TSC_IRQn
#define TSC_IRQHandler                        TSC_IRQHandler

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#define nReset_Pin GPIO_PIN_10
#define nReset_GPIO_Port GPIOB
#define nINT_Pin GPIO_PIN_4
#define nINT_GPIO_Port GPIOB
#if defined (USE_STM32U083C_DISCO_REVB) || defined (USE_STM32U083C_DISCO_REVC)
#define NWAKE_Pin GPIO_PIN_15
#define NWAKE_GPIO_Port GPIOA
#elif defined (USE_STM32U083C_DISCO_REVA)
#define NWAKE_Pin GPIO_PIN_6
#define NWAKE_GPIO_Port GPIOB
#endif /* USE_STM32U083C_DISCO_REVB || USE_STM32U083C_DISCO_REVC */
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
