/**
  ******************************************************************************
  * @file    boot_hal_cfg.h
  * @author  MCD Application Team
  * @brief   File fixing configuration flag specific for STM32U0xx platform
  *
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
#ifndef BOOT_HAL_CFG_H
#define BOOT_HAL_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* RTC clock */
#define RTC_CLOCK_SOURCE_LSI
#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00F9
#endif
#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

/* Static protections */
#define WRP_PROTECT_ENABLE  /*!< Write Protection */
#define HDP_PROTECT_ENABLE /*!< HDP protection */

#define OEMIROT_OB_RDP_LEVEL_VALUE OB_RDP_LEVEL_0 /*!< RDP level.
                                                       This define is updated automatically by provisioning.
                                                       Do not edit. */

#define NO_TAMPER            (0)                /*!< No tamper activated */
#define INTERNAL_TAMPER_ONLY (1)                /*!< Only Internal tamper activated */
#define ALL_TAMPER           (2)                /*!< Internal and External tamper activated */
#define TAMPER_ENABLE INTERNAL_TAMPER_ONLY      /*!< TAMPER configuration flag */

#ifdef OEMIROT_DEV_MODE
#define OEMIROT_ERROR_HANDLER_STOP_EXEC /*!< Error handler stops execution (else it resets) */
#endif /* OEMIROT_DEV_MODE */

/* Run time protections */
#define OEMIROT_MPU_PROTECTION    /*!< OEMiROT_Boot uses MPU to prevent execution outside of OEMiROT_Boot code */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void) __NO_RETURN;

#endif /* BOOT_HAL_CFG_H */
