/**
  ******************************************************************************
  * @file    stm32u0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32u0xx_it.h"
#include "spi_interface.h"
#include "usb_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                        */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @retval None
  */
void NMI_Handler(void)
{
  while (true)
  {
  }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @retval None
  */
void HardFault_Handler(void)
{
  while (true)
  {
  }
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}


/******************************************************************************/
/*                 STM32U0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32u0xx.s).                                               */
/******************************************************************************/


/**
  * @brief  This function handles USB FS global interrupt request.
  * @retval None.
  */
void USB_DRD_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&USB_Handle_Fs);
}

/**
  * @brief This function handles SPIx global interrupt.
  * @retval None.
  */
void SPI1_IRQHandler(void)
{
  OPENBL_SPI_IRQHandler();
}
