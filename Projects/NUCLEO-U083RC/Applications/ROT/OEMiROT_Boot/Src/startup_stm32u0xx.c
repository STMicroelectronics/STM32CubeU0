/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 * Copyright (c) 2024 STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.6.0 startup_ARMv81MML.c
 * Git SHA: b5f0603d6a584d1724d952fd8b0737458b90d62b
 */

#include "stm32u0xx.h"
/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern void __PROGRAM_START(void) __NO_RETURN;

extern void Error_Handler(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler  (void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
#define DEFAULT_IRQ_HANDLER(handler_name)  \
void handler_name(void); \
__WEAK void handler_name(void) { \
    while(1); \
}

/* Exceptions */
DEFAULT_IRQ_HANDLER(NMI_Handler)
DEFAULT_IRQ_HANDLER(HardFault_Handler)
DEFAULT_IRQ_HANDLER(SVC_Handler)
DEFAULT_IRQ_HANDLER(PendSV_Handler)
DEFAULT_IRQ_HANDLER(SysTick_Handler)

DEFAULT_IRQ_HANDLER(WWDG_IWDG_IRQHandler)
DEFAULT_IRQ_HANDLER(PVD_PVM_IRQHandler)
DEFAULT_IRQ_HANDLER(RTC_TAMP_IRQHandler)
DEFAULT_IRQ_HANDLER(FLASH_ECC_IRQHandler)
DEFAULT_IRQ_HANDLER(RCC_CRS_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI0_1_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI2_3_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI4_15_IRQHandler)
DEFAULT_IRQ_HANDLER(USB_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel2_3_IRQHandler)
DEFAULT_IRQ_HANDLER(DMAMUX_IRQHandler)
DEFAULT_IRQ_HANDLER(ADC_COMP1_2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_CC_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM6_DAC_LPTIM1_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM7_LPTIM2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM15_LPTIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM16_IRQHandler)
DEFAULT_IRQ_HANDLER(TSC_IRQHandler)
DEFAULT_IRQ_HANDLER(LCD_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C1_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C2_3_4_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI2_3_IRQHandler)
DEFAULT_IRQ_HANDLER(USART1_IRQHandler)
DEFAULT_IRQ_HANDLER(USART2_LPUART2_IRQHandler)
DEFAULT_IRQ_HANDLER(USART3_LPUART1_IRQHandler)
DEFAULT_IRQ_HANDLER(USART4_LPUART3_IRQHandler)
DEFAULT_IRQ_HANDLER(RNG_CRYP_IRQHandler)

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const pFunc __VECTOR_TABLE[];
       const pFunc __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (pFunc)(&__INITIAL_SP),           /*      Initial Stack Pointer */
  Reset_Handler,                    /*      Reset Handler */
  NMI_Handler,                      /* -14: NMI Handler */
  Error_Handler,                    /* -13: Hard Fault Handler */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  SVC_Handler,                      /*  -5: SVCall Handler */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  PendSV_Handler,                   /*  -2: PendSV Handler */
  SysTick_Handler,                  /*  -1: SysTick Handler */
  WWDG_IWDG_IRQHandler,             /*   0: Window WatchDog / Independent Watchdog Interrupt */
  PVD_PVM_IRQHandler,               /*   1: PVD through EXTI line detection */
  RTC_TAMP_IRQHandler,              /*   2: RTC/TAMP through the EXTI line */
  FLASH_ECC_IRQHandler,             /*  11: FLASH / ECC */
  RCC_CRS_IRQHandler,               /*  12: RCC / CRS */
  EXTI0_1_IRQHandler,               /*  13: EXTI Line[0:1] */
  EXTI2_3_IRQHandler,               /*  14: EXTI Line[2:3] */
  EXTI4_15_IRQHandler,              /*  15: EXTI Line[4:15] */
  USB_IRQHandler,                   /*  17: USB global interrupt through EXTI line detection */
  DMA1_Channel1_IRQHandler,         /*  18: DMA1 Channel1 interrupt */
  DMA1_Channel2_3_IRQHandler,       /*  19: DMA1 Channel2/Channel3 interrupt */
  DMAMUX_IRQHandler,                /*  20: DMAMUX_OVR_IT global interrupt */
  ADC_COMP1_2_IRQHandler,           /*  21: ADC and COMP1/COMP2 interrupts */
  TIM1_IRQHandler,                  /*  22: TIM1 break, update, trigger, commutation, error, direction change  and index interrupts */
  TIM1_CC_IRQHandler,               /*  22: TIM1 Capture Compare interrupt */
  TIM2_IRQHandler,                  /*  23: TIM2 */
  TIM3_IRQHandler,                  /*  24: TIM3 */
  TIM6_DAC_LPTIM1_IRQHandler,       /*  25: TIM6 / DAC1&3 / LPTIM1 global interrupt */
  TIM7_LPTIM2_IRQHandler,           /*  26: TIM7 / LPTIM2 global interrupt */
  TIM15_LPTIM3_IRQHandler,          /*  27: TIM15 / LPTIM3 global interrupt */
  TIM16_IRQHandler,                 /*  28: TIM16 */
  TSC_IRQHandler,                   /*  29: TSC global interrupt */
  LCD_IRQHandler,                   /*  30: LCD global interrupt */
  I2C1_IRQHandler,                  /*  31: I2C1 global interrupt */
  I2C2_3_4_IRQHandler,              /*  32: I2C2 / I2C3 / I2C4 global interrupt */
  SPI1_IRQHandler,                  /*  33: SPI1 global interrupt */
  SPI2_3_IRQHandler,                /*  34: SPI2 /SPI3 global interrupt */
  USART1_IRQHandler,                /*  35: USART1 global interrupt */
  USART2_LPUART2_IRQHandler,        /*  36: USART2, LPUART 2 global interrupt */
  USART3_LPUART1_IRQHandler,        /*  37: USART3, LPUART 1 global interrupt */
  USART4_LPUART3_IRQHandler,        /*  38: USART4, LPUART 3 global interrupt */
  RNG_CRYP_IRQHandler,              /*  39: RNG, CRYPTO global interrupt 1 */
};
#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

#ifdef OEMIROT_DEV_MODE
#ifdef __ICCARM__
__no_init volatile uint32_t TamperEventCleared;
#else
  volatile uint32_t TamperEventCleared  __attribute__((section(".bss.NoInit")));
#endif /* __ICCARM__ */
#endif /* OEMIROT_DEV_MODE */

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
  /* CMSIS System Initialization */
  SystemInit();

  /* active access to tamper register */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_RTC_ENABLE();
  __HAL_RCC_RTCAPB_CLK_ENABLE();
  /* Release reset of back-up domain in case it is set, to avoid blocking the device (system reset
  does not release it) */
  __HAL_RCC_BACKUPRESET_RELEASE();

   /* Get tamper status */
  if (READ_REG(TAMP->SR))
  {
#ifdef OEMIROT_DEV_MODE
/* avoid several re-boot in DEV_MODE with Tamper active, clean tamper configuration*/
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();
    /* wait for event being cleared*/
    while(READ_REG(TAMP->SR));

    /* memorize that event has been cleared */
    TamperEventCleared=1;
#else
    /* VBAT and VDD must be set to zero to allow board to restart */
    Error_Handler();
#endif /* OEMIROT_DEV_MODE */
  }
  /*  Enable TAMP IRQ, to catch tamper interrupt in RTC_TAMP_IRQHandler */
  /*  else a stack in BKPSRAM2 is cleaned and a HardFault can occur, at every pop of
   *  function */
#ifndef OEMIROT_DEV_MODE
  HAL_NVIC_EnableIRQ(RTC_TAMP_IRQn);
#endif /* OEMIROT_DEV_MODE */

  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
