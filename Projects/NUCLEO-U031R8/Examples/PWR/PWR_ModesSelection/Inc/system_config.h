/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/system_config.h
  * @author  MCD Application Team
  * @brief   Header for system_config.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/** @defgroup  System Status Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_OK    = 0x00, /*!< System status OK             */
  SYSTEM_ERROR = 0x01  /*!< System status ERROR          */

} System_StatusTypeDef;

/** @defgroup  System Low Power Mode Scenario Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_LOWPOWER_S0        , /*!< System low power selection mode 0.   */
  SYSTEM_LOWPOWER_S1        , /*!< System low power selection mode 1.   */
  SYSTEM_LOWPOWER_S2        , /*!< System low power selection mode 2.   */
  SYSTEM_LOWPOWER_S3        , /*!< System low power selection mode 3.   */
  SYSTEM_LOWPOWER_S4        , /*!< System low power selection mode 4.   */
  SYSTEM_LOWPOWER_S5        , /*!< System low power selection mode 5.   */
  SYSTEM_LOWPOWER_S6        , /*!< System low power selection mode 6.   */
  SYSTEM_LOWPOWER_S7        , /*!< System low power selection mode 7.   */
  SYSTEM_LOWPOWER_S8        , /*!< System low power selection mode 8.   */
  SYSTEM_LOWPOWER_S9        , /*!< System low power selection mode 9.   */
  SYSTEM_LOWPOWER_S10       , /*!< System low power selection mode 10.  */
  SYSTEM_LOWPOWER_S11       , /*!< System low power selection mode 11.  */
  SYSTEM_LOWPOWER_S12       , /*!< System low power selection mode 12.  */
  SYSTEM_LOWPOWER_S13         /*!< System low power selection mode 13.  */
} System_LowPowerModeScenarioTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void system_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_lowpower_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_rtc_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_clock_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_ram_retention_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_regulator_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_flash_config(System_LowPowerModeScenarioTypeDef scenario);

#endif /* SYSTEM_CONFIG_H */
