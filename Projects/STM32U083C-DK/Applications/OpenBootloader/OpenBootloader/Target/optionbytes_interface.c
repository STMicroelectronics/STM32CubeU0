/**
  ******************************************************************************
  * @file    optionbytes_interface.c
  * @author  MCD Application Team
  * @brief   Contains Option Bytes access functions
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
#include "main.h"
#include "platform.h"
#include "common_interface.h"

#include "openbl_mem.h"

#include "app_openbootloader.h"
#include "flash_interface.h"
#include "optionbytes_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef OB_Descriptor =
{
  OB_START_ADDRESS,
  OB_END_ADDRESS,
  OB_SIZE,
  OB_AREA,
  OPENBL_OB_Read,
  OPENBL_OB_Write,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Launch the option byte loading.
  * @retval None.
  */
void OPENBL_OB_Launch(void)
{
  /* Set the option start bit */
  HAL_FLASH_OB_Launch();
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_OB_Read(uint32_t address)
{
  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in Option bytes.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_OB_Write(uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  /* Unlock the FLASH & Option Bytes Registers access */
  HAL_FLASH_Unlock();
  HAL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

  /* Wait for last operation to be completed */
  FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if ((data_length != 0U) && (*p_data != RDP_LEVEL_2))
  {
    /* Write the FLASH option bytes registers */

    /* Write OPTR option bytes */
    if (data_length > 0U)
    {
      FLASH->OPTR = *(uint32_t *)(uint32_t)&p_data[0U];
    }

    /* Write WRP1AR option bytes */
    if (data_length > 12U)
    {
      FLASH->WRP1AR = *(uint32_t *)(uint32_t)&p_data[12U];
    }

    /* Write WRP1BR option bytes */
    if (data_length > 16U)
    {
      FLASH->WRP1BR = *(uint32_t *)(uint32_t)&p_data[16U];
    }

    /* Write SECR option bytes */
    if (data_length > 96U)
    {
      FLASH->SECR = *(uint32_t *)(uint32_t)&p_data[96U];
    }

    /* Write OEM1KEYW0R option bytes */
    if (data_length > 104U)
    {
      FLASH->OEM1KEYW0R = *(uint32_t *)(uint32_t)&p_data[104U];
    }

    /* Write OEM1KEYW1R option bytes */
    if (data_length > 108U)
    {
      FLASH->OEM1KEYW1R = *(uint32_t *)(uint32_t)&p_data[108U];
    }

    /* Write OEM1KEYW2R option bytes */
    if (data_length > 112U)
    {
      FLASH->OEM1KEYW2R = *(uint32_t *)(uint32_t)&p_data[112U];
    }

    /* Write OEM1KEYW3R option bytes */
    if (data_length > 116U)
    {
      FLASH->OEM1KEYW3R = *(uint32_t *)(uint32_t)&p_data[116U];
    }

    /* Write OEM2KEYW0R option bytes */
    if (data_length > 120U)
    {
      FLASH->OEM2KEYW0R = *(uint32_t *)(uint32_t)&p_data[120U];
    }

    /* Write OEM2KEYW1R option bytes */
    if (data_length > 124U)
    {
      FLASH->OEM2KEYW1R = *(uint32_t *)(uint32_t)&p_data[124U];
    }

    /* Write OEM2KEYW2R option bytes */
    if (data_length > 128U)
    {
      FLASH->OEM2KEYW2R = *(uint32_t *)(uint32_t)&p_data[128U];
    }

    /* Write OEM2KEYW3R option bytes */
    if (data_length > 132U)
    {
      FLASH->OEM2KEYW3R = *(uint32_t *)(uint32_t)&p_data[132U];
    }
  }

  SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);

  /* Wait for last operation to be completed */
  FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  /* Register system reset callback */
  Common_SetPostProcessingCallback(OPENBL_OB_Launch);
}
