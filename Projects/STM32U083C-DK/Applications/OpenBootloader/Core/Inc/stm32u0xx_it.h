/**
  ******************************************************************************
  * @file    stm32u0xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef STM32U0xx_IT_H
#define STM32U0xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx.h"
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_DRD_FS;

/* Exported functions ------------------------------------------------------- */
void NMI_Handler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);
void SPI1_IRQHandler(void);
void USB_DRD_FS_IRQHandler(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32U0xx_IT_H */
