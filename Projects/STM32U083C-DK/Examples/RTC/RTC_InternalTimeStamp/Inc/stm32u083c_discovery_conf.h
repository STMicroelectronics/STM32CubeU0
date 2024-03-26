/**
  ******************************************************************************
  * @file    stm32u083c_discovery_conf_template.h
  * @author  MCD Application Team
  * @brief   configuration file.
  *          This file should be copied to the application folder and renamed
  *          to stm32u083c_dk_conf.h
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
#ifndef STM32U083C_DK_CONF_H
#define STM32U083C_DK_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U083C_DK
  * @{
  */

/** @defgroup STM32U083C_DK_CONFIG Config
  * @{
  */

/** @defgroup STM32U083C_DK_CONFIG_Exported_Constants Exported Constants
  * @{
  */

/* Environmental Sensors usage */
#define USE_ENV_SENSOR_STTS22H_0             0U

/* COM port usage */
#if defined(USE_VCP_CONNECTION)
#define USE_BSP_COM_FEATURE                  0U
#define USE_COM_LOG                          0U
#else
#define USE_BSP_COM_FEATURE                  0U
#define USE_COM_LOG                          0U
#endif /* USE_VCP_CONNECTION */

/* JOY define */
#define USE_BSP_JOY_FEATURE                  0U

/* IRQ priorities */
#define BSP_BUTTON_USER_IT_PRIORITY          15U

/* I2C1 Frequency in Hz  */
#define BUS_I2C1_FREQUENCY                   100000UL /* Frequency of I2C1 = 100 KHz*/

#define STM32U083C_DK_I2C_Init BSP_I2C1_Init
#define STM32U083C_DK_I2C_DeInit BSP_I2C1_DeInit
#define STM32U083C_DK_I2C_ReadReg BSP_I2C1_ReadReg
#define STM32U083C_DK_I2C_WriteReg BSP_I2C1_WriteReg
  
#define STM32U083C_DK_GetTick BSP_GetTick


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U083C_DK_CONF_H */
