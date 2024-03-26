/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"
#include <stdio.h>
#include "stm32u0xx_ll_rcc.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/
extern void Error_Handler(void);
/* Definition for USARTx clock resources */

#define DATA_SIZE                        0x4000
#define DATA_MAX_SIZE                    0x4000
#define DATA_MPU_SUB_REG                 0x0

#endif /* __MAIN_H */

