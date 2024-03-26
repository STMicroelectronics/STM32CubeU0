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
#include <string.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define DATA_SIZE                        0x4000
/* Constants used by Serial Command Line Mode */
#define TX_TIMEOUT          ((uint32_t)100U)
#define RX_TIMEOUT          ((uint32_t)2000U)

void Error_Handler(void);

/* Definition for USARTx clock resources */
#define COM_INSTANCE                           USART2
#define COM_CLK_ENABLE()                       __HAL_RCC_USART2_CLK_ENABLE()
#define COM_TX_GPIO_PORT                       GPIOA
#define COM_TX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM_TX_PIN                             GPIO_PIN_2
#define COM_TX_AF                              GPIO_AF7_USART2

#define COM_RX_GPIO_PORT                       GPIOA
#define COM_RX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM_RX_PIN                             GPIO_PIN_3
#define COM_RX_AF                              GPIO_AF7_USART2
#endif /* __MAIN_H */