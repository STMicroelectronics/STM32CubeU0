/**
  ******************************************************************************
  * @file    low_level_rng.c
  * @author  MCD Application Team
  * @brief   Low Level Interface module that uses RNG
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

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"
#include "low_level_rng.h"
#include "stm32u0xx_ll_bus.h"
#include "main.h"

/* exported variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define COMPILER_BARRIER() __ASM __IO("" : : : "memory")
#define RNG_NUMBER 10

/* Private variables ---------------------------------------------------------*/
static RNG_HandleTypeDef handle;
unsigned char seed_buf[RNG_NUMBER] = {0};
size_t index_seed_buf = 0;

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  The Random delay
  * @param  None
  * @retval None
  */
int RNG_Delay(void)
{
    uint8_t delay;
    int foo = 0;
    volatile int rc;

    delay = RNG_Get_Random();

    for (volatile int i = 0; i < delay; i++) {
        foo++;
    }

    rc = 1;
    return rc;
}

/**
  * @brief  Initializes random values
  * @param  None
  * @retval None
  */
void RNG_Delay_Init(void)
{
  size_t len = 0U;
  /* generate several random */
  RNG_GetBytes((unsigned char *)seed_buf, sizeof(seed_buf),(size_t *)&len);

}

/**
  * @brief  Get Random Value
  * @param  None
  * @retval Delay
  */
uint8_t RNG_Get_Random(void)
{
  uint8_t delay;

  delay = seed_buf[index_seed_buf];
  index_seed_buf++;

  if ( RNG_NUMBER == index_seed_buf )
  {
    RNG_Delay_Init();
    index_seed_buf = 0;
  }
  return delay;
}

/**
  * @brief  Random Number Generator configuration
  * @param  None
  * @retval None
  */
void RNG_Init(void)
{
  uint32_t dummy;

  /* Select RNG clock source */
  LL_RCC_SetRNGClockSource(LL_RCC_RNG_CLKSOURCE_MSI);

  /* RNG Peripheral clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_RNG);

  /* Initialize RNG instance */
  handle.Instance = RNG;
  handle.State = HAL_RNG_STATE_RESET;
  handle.Lock = HAL_UNLOCKED;

  if (HAL_RNG_Init(&handle) != HAL_OK)
  {
    Error_Handler();
  }

  /* first random number generated after setting the RNGEN bit should not be used */
  HAL_RNG_GenerateRandomNumber(&handle, &dummy);
}

/**
  * @brief  Generates a list of random numbers
  * @param  None
  * @retval None
  */
void RNG_GetBytes(uint8_t *output, size_t length, size_t *output_length)
{
  int32_t ret = 0;
  uint8_t try = 0;
  __IO uint8_t random[4];
  *output_length = 0;

  /* Get Random byte */
  while ((*output_length < length) && (ret == 0))
  {
    if (HAL_RNG_GenerateRandomNumber(&handle, (uint32_t *)random) != HAL_OK)
    {
      /* retry when random number generated are not immediately available */
      if (try < 3)
      {
        try++;
      }
      else
      {
        ret = -1;
      }
    }
    else
    {
      for (uint8_t i = 0; (i < 4) && (*output_length < length) ; i++)
      {
        *output++ = random[i];
        *output_length += 1;
        random[i] = 0;
      }
    }
  }
  /* Just be extra sure that we didn't do it wrong */
  if ((__HAL_RNG_GET_FLAG(&handle, (RNG_FLAG_CECS | RNG_FLAG_SECS))) != 0)
  {
    *output_length = 0;
  }
}


/**
  * @brief  DeInitializes the RNG peripheral
  * @param  None
  * @retval None
  */
void RNG_DeInit(void)
{
  /* RNG Peripheral clock disable */
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_RNG);
  LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_RNG);
  LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_RNG);
}
