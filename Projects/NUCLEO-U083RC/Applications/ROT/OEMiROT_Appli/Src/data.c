/**
  ******************************************************************************
  * @file    data.c
  * @author  MCD Application Team
  * @brief   data module.
  *          This file provides examples of Data access.
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
#include <string.h>
#include "appli_flash_layout.h"
#include "data.h"
#include "com.h"

#if (DATA_IMAGE_NUMBER == 1)
/** @defgroup  Data_Private_Defines Private Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup  Data_Private_Functions Private Functions
  * @{
  */
static void DATA_Print_menu(void);
static void DATA_Display(void);

/**
  * @}
  */

/** @defgroup  Data_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Display the NS Data Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
void DATA_Run(void)
{
  uint8_t key = 0;
  uint8_t exit = 0;

  /* Print NS Data welcome message */
  DATA_Print_menu();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the user input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, COM_UART_TIMEOUT_MAX) == HAL_OK)
    {
      switch (key)
      {
        case '1' :
          DATA_Display();
          break;
        case 'x':
          exit = 1;
          break;
        default:
          printf("Invalid Number !\r");
          break;
      }
      DATA_Print_menu();
    }
  }
}
/**
  * @}
  */

/** @addtogroup  Data_Private_Functions
  * @{
  */

/**
  * @brief  Display the NS Data Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void DATA_Print_menu(void)
{
  printf("\r\n=============== Data Image Menu ===================\r\n\n");
  printf("  Display Data from Data Image ----------------------- 1\r\n\n");
  printf("  Previous Menu -------------------------------------- x\r\n\n");
}

/**
  * @brief  Display the NS Data on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void DATA_Display(void)
{
  uint8_t *data1;
  data1 = (uint8_t*)(FLASH_BASE + DATA_IMAGE_PRIMARY_PARTITION_OFFSET + DATA_HEADER_OFFSET);

  printf("  -- Data: %08lx%08lx..%08lx%08lx\r\n\n",
               *((unsigned long *)(&data1[0])),
               *((unsigned long *)(&data1[4])),
               *((unsigned long *)(&data1[DATA_SIZE - 8])),
               *((unsigned long *)(&data1[DATA_SIZE - 4]))
              );
}

/**
  * @}
  */

#endif /* (DATA_IMAGE_NUMBER == 1) */
