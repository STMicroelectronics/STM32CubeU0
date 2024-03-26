/**
  ******************************************************************************
  * @file    interfaces_conf.h
  * @author  MCD Application Team
  * @brief   Contains Interfaces configuration
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
#ifndef INTERFACES_CONF_H
#define INTERFACES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_ll_usart.h"
#include "stm32u0xx_ll_i2c.h"
#include "stm32u0xx_ll_spi.h"
#include "stm32u0xx_ll_gpio.h"
#include "stm32u0xx_ll_rcc.h"


#define MEMORIES_SUPPORTED                8U

/*-------------------------- Definitions for USART ---------------------------*/
#define USARTx                            USART2
#define USARTx_CLK_ENABLE()               __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_CLK_DISABLE()              __HAL_RCC_USART2_CLK_DISABLE()
#define USARTx_GPIO_CLK_TX_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_GPIO_CLK_RX_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_DEINIT()                   LL_USART_DeInit(USARTx)

#define USARTx_TX_PIN                     GPIO_PIN_2
#define USARTx_TX_GPIO_PORT               GPIOA
#define USARTx_RX_PIN                     GPIO_PIN_3
#define USARTx_RX_GPIO_PORT               GPIOA
#define USARTx_ALTERNATE                  LL_GPIO_AF_7

/*-------------------------- Definitions for I2C -----------------------------*/
#define I2Cx                              I2C1
#define I2Cx_CLK_ENABLE()                 __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_CLK_DISABLE()                __HAL_RCC_I2C1_CLK_DISABLE()
#define I2Cx_GPIO_CLK_SCL_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_GPIO_CLK_SDA_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_DEINIT()                     LL_I2C_DeInit(I2Cx)

#define I2Cx_SCL_PIN                      GPIO_PIN_6
#define I2Cx_SCL_PIN_PORT                 GPIOB
#define I2Cx_SDA_PIN                      GPIO_PIN_7
#define I2Cx_SDA_PIN_PORT                 GPIOB
#define I2Cx_ALTERNATE                    LL_GPIO_AF_4
#define I2C_ADDRESS                       (0x000000D4U)
#define OPENBL_I2C_TIMEOUT                (2000U * 0x000005CCU)
#define I2C_TIMING                        0x00800000U

/*--------------------------- Definitions for SPI ----------------------------*/
#define SPIx                              SPI1
#define SPIx_CLK_ENABLE()                 __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_CLK_DISABLE()                __HAL_RCC_SPI1_CLK_DISABLE()
#define SPIx_GPIO_CLK_SCK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_GPIO_CLK_MISO_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_GPIO_CLK_MOSI_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_GPIO_CLK_NSS_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_DEINIT()                     LL_SPI_DeInit(SPIx)
#define SPIx_IRQ                          SPI1_IRQn

#define SPIx_MOSI_PIN                     GPIO_PIN_7
#define SPIx_MOSI_PIN_PORT                GPIOA
#define SPIx_MISO_PIN                     GPIO_PIN_6
#define SPIx_MISO_PIN_PORT                GPIOA
#define SPIx_SCK_PIN                      GPIO_PIN_5
#define SPIx_SCK_PIN_PORT                 GPIOA
#define SPIx_NSS_PIN                      GPIO_PIN_4
#define SPIx_NSS_PIN_PORT                 GPIOA
#define SPIx_ALTERNATE                    LL_GPIO_AF_5

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERFACES_CONF_H */
