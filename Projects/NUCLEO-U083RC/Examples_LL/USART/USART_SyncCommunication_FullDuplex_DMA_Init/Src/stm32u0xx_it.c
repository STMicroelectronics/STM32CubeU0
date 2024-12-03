/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/USART/USART_SyncCommunication_FullDuplex_DMA_Init/Src/stm32u0xx_it.c
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32u0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SVC instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32U0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32u0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */

  /* USER CODE END EXTI4_15_IRQn 0 */
  if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_13);
    /* USER CODE BEGIN LL_EXTI_LINE_13_FALLING */

    /* Handle User push-button press in dedicated function */
    UserButton_Callback();

    /* USER CODE END LL_EXTI_LINE_13_FALLING */
  }
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */
  if (LL_DMA_IsActiveFlag_TC2(DMA1))
  {
    LL_DMA_ClearFlag_TC2(DMA1);
    /* Call function Transmission complete Callback */
    SPI1_DMA1_TransmitComplete_Callback();
  }
  else if (LL_DMA_IsActiveFlag_TE2(DMA1))
  {
    /* Call Error function */
    SPI_TransferError_Callback();
  }
  if (LL_DMA_IsActiveFlag_TC3(DMA1))
  {
    LL_DMA_ClearFlag_TC3(DMA1);
    /* Call function Reception complete Callback */
    SPI1_DMA1_ReceiveComplete_Callback();
  }
  else if (LL_DMA_IsActiveFlag_TE3(DMA1))
  {
    /* Call Error function */
    SPI_TransferError_Callback();
  }
  /* USER CODE END DMA1_Channel2_3_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMAMUX_OVR_IT + DMA1 channel 4 to 7 + DMA2 channel 1 to 5.
  */
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQn 0 */
  if (LL_DMA_IsActiveFlag_TC4(DMA1))
  {
    LL_DMA_ClearFlag_TC4(DMA1);
    /* Call function Transmission complete Callback */
    USART1_DMA1_TransmitComplete_Callback();
  }
  else if (LL_DMA_IsActiveFlag_TE4(DMA1))
  {
    /* Call Error function */
    USART_TransferError_Callback();
  }
  if (LL_DMA_IsActiveFlag_TC5(DMA1))
  {
    LL_DMA_ClearFlag_TC5(DMA1);
//    LL_DMA_ClearFlag_GI5(DMA1);
    /* Call function Reception complete Callback */
    USART1_DMA1_ReceiveComplete_Callback();
  }
  else if (LL_DMA_IsActiveFlag_TE5(DMA1))
  {
    /* Call Error function */
    USART_TransferError_Callback();
  }
  /* USER CODE END DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQn 0 */

  /* USER CODE BEGIN DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX_OVR_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
