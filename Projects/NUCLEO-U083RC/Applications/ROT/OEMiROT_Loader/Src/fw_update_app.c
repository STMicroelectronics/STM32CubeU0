/**
  ******************************************************************************
  * @file    fw_update_app.c
  * @author  MCD Application Team
  * @brief   Firmware Update module.
  *          This file provides set of firmware functions to manage Firmware
  *          Update functionalities.
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
#include "string.h"
#include "stm32u0xx_hal.h"
#include "loader_flash_layout.h"
#include "com.h"
#include "common.h"
#include "ymodem.h"
#include "Driver_Flash.h"
#include "fw_update_app.h"

/** @addtogroup USER_APP User App Example
  * @{
  */
extern ARM_DRIVER_FLASH LOADER_FLASH_DEV_NAME;
/** @addtogroup  FW_UPDATE Firmware Update Example
  * @{
  */
/** @defgroup  FW_UPDATE_Private_Variables Private Variables
  * @{
  */
static uint32_t m_uFileSizeYmodem = 0U;    /* !< Ymodem File size*/
static uint32_t m_uNbrBlocksYmodem = 0U;   /* !< Ymodem Number of blocks*/
static uint32_t m_uPacketsReceived = 0U;   /* !< Ymodem packets received*/
static uint32_t m_uFlashSectorSize = 0U;   /* !< Flash Sector Size */
static uint32_t m_uFlashMinWriteSize = 0U; /* !< FLash Min Write access*/
#if (PRIMARY_ONLY == 0)
/** @defgroup  FW_UPDATE_Private_Const Private Const
  * @{
  */
const uint32_t MagicTrailerValue[] =
{
  0xf395c277,
  0x7fefd260,
  0x0f505235,
  0x8079b62c,
};
#endif /* (PRIMARY_ONLY == 0) */

/**
  * @}
  */

/** @defgroup  FW_UPDATE_Private_Functions Private Functions
  * @{
  */
static void FW_UPDATE_PrintWelcome(void);
static HAL_StatusTypeDef FW_UPDATE_DownloadNewFirmware(SFU_FwImageFlashTypeDef *pFwImageDwlArea);
static HAL_StatusTypeDef FW_UPDATE_APP_IMAGE(void);
#if (DATA_IMAGE_NUMBER == 1)
static HAL_StatusTypeDef FW_UPDATE_DATA_IMAGE(void);
#endif /* (DATA_IMAGE_NUMBER == 1) */
/**
  * @}
  */
/** @defgroup  FW_UPDATE_Exported_Functions Exported Functions
  * @{
  */

/** @defgroup  FW_UPDATE_Control_Functions Control Functions
  * @{
   */
void FW_UPDATE_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;

  /* Print Firmware Update welcome message */
  FW_UPDATE_PrintWelcome();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      switch (key)
      {
      case '1' :
          printf("  -- Install image : reboot\r\n\n");
          NVIC_SystemReset();
          break;
      case '2' :
          FW_UPDATE_APP_IMAGE();
          break;
#if (DATA_IMAGE_NUMBER == 1)
      case '3' :
          FW_UPDATE_DATA_IMAGE();
          break;
#endif /* (DATA_IMAGE_NUMBER == 1) */
      default:
          printf("Invalid Number !\r");
          break;
      }
      /* Print Main Menu message */
      FW_UPDATE_PrintWelcome();
    }
  }
}

/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_APP_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
  printf("Download App Image\r\n");

  /* Get Info about the download area */
  fw_image_dwl_area.DownloadAddr = IMAGE_PARTITION_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = IMAGE_PARTITION_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
    printf("  -- App Image correctly downloaded \r\n\n");
    HAL_Delay(1000U);
  }

  return ret;
}

#if (DATA_IMAGE_NUMBER == 1)
/**
  * @brief  Run FW Update process.
  * @param  None
  * @retval HAL Status.
  */
static HAL_StatusTypeDef FW_UPDATE_DATA_IMAGE(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  SFU_FwImageFlashTypeDef fw_image_dwl_area;
  ARM_FLASH_INFO *data = LOADER_FLASH_DEV_NAME.GetInfo();
  /* Print Firmware Update welcome message */
  printf("Download Data Image\r\n");
  /* Get Info about the download area */
  fw_image_dwl_area.DownloadAddr = DATA_IMAGE_PARTITION_OFFSET;
  fw_image_dwl_area.MaxSizeInBytes = DATA_PARTITION_SIZE;
  fw_image_dwl_area.ImageOffsetInBytes = 0x0;
  m_uFlashSectorSize = data->sector_size;
  m_uFlashMinWriteSize = data->program_unit;
  /* Download new firmware image*/
  ret = FW_UPDATE_DownloadNewFirmware(&fw_image_dwl_area);

  if (HAL_OK == ret)
  {
    printf("  -- Data Image correctly downloaded \r\n\n");
    HAL_Delay(1000U);
  }

  return ret;
}

#endif /* (DATA_IMAGE_NUMBER == 1) */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup  FW_UPDATE_Private_Functions
  * @{
  */

/**
  * @brief  Display the FW_UPDATE Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_UPDATE_PrintWelcome(void)
{
  printf("\r\n=================== New Firmware images ===================\r\n\n");
  printf("  Reset to trigger Installation ------------------------- 1\r\n\n");
  printf("  Download App Image ------------------------------------ 2\r\n\n");
#if (DATA_IMAGE_NUMBER == 1)
  printf("  Download Data Image ----------------------------------- 3\r\n\n");
#endif /* (DATA_IMAGE_NUMBER == 1) */
}

/**
  * @brief Download a new Firmware from the host.
  * @retval HAL status
  */
static HAL_StatusTypeDef FW_UPDATE_DownloadNewFirmware(SFU_FwImageFlashTypeDef *pFwImageDwlArea)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  COM_StatusTypeDef e_result;
  int32_t ret_arm;
  uint32_t u_fw_size = pFwImageDwlArea->MaxSizeInBytes ;
  uint32_t sector_address;

  /* Clear download area */
  printf("  -- Erasing download area \r\n\n");

  for (sector_address = pFwImageDwlArea->DownloadAddr;
       sector_address < pFwImageDwlArea->DownloadAddr + pFwImageDwlArea->MaxSizeInBytes;
       sector_address += m_uFlashSectorSize)
  {
    ret_arm = LOADER_FLASH_DEV_NAME.EraseSector(sector_address);
    if (ret_arm < 0)
    {
      return HAL_ERROR;
    }
  }

  printf("  -- Send Firmware \r\n\n");

  /* Download binary */
  printf("  -- -- File> Transfer> YMODEM> Send \t\n");

  /*Init of Ymodem*/
  Ymodem_Init();

  /*Receive through Ymodem*/
  e_result = Ymodem_Receive(&u_fw_size, pFwImageDwlArea->DownloadAddr);
  printf("\r\n\n");

  if (e_result == COM_OK)
  {
    printf("  -- -- Programming Completed Successfully!\r\n\n");
    printf("  -- -- Bytes: %lu\r\n\n", (unsigned long int)u_fw_size);
    ret = HAL_OK;
#if (PRIMARY_ONLY == 0)
    if (u_fw_size <= (pFwImageDwlArea->MaxSizeInBytes - sizeof(MagicTrailerValue)))
    {
      uint32_t MagicAddress =
        pFwImageDwlArea->DownloadAddr + (pFwImageDwlArea->MaxSizeInBytes - sizeof(MagicTrailerValue));
      /* Write the magic to trigger installation at next reset */
      printf("  Write Magic Trailer at %lx\r\n\n", (unsigned long int)MagicAddress);
      if (LOADER_FLASH_DEV_NAME.ProgramData(MagicAddress, MagicTrailerValue, sizeof(MagicTrailerValue)) != ARM_DRIVER_OK)
      {
        ret = HAL_ERROR;
      }
    }
#endif /* (PRIMARY_ONLY == 0) */
  }
  else if (e_result == COM_ABORT)
  {
    printf("  -- -- !!Aborted by user!!\r\n\n");
    COM_Flush();
    ret = HAL_ERROR;
  }
  else
  {
    printf("  -- -- !!Error during file download!!\r\n\n");
    ret = HAL_ERROR;
    HAL_Delay(500U);
    COM_Flush();
  }

  return ret;
}


/**
  * @}
  */

/** @defgroup FW_UPDATE_Callback_Functions Callback Functions
  * @{
  */

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  uFileSize Dimension of the file that will be received (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize)
{
  /*Reset of the ymodem variables */
  m_uFileSizeYmodem = 0U;
  m_uPacketsReceived = 0U;
  m_uNbrBlocksYmodem = 0U;

  /*Filesize information is stored*/
  m_uFileSizeYmodem = uFileSize;

  /* compute the number of 1K blocks */
  m_uNbrBlocksYmodem = (m_uFileSizeYmodem + (PACKET_1K_SIZE - 1U)) / PACKET_1K_SIZE;

  /* NOTE : delay inserted for Ymodem protocol*/
  HAL_Delay(1000);
  return HAL_OK;
}

extern uint32_t total_size_received;
/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  uSize Packet dimension (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize)
{
  int32_t ret;
  m_uPacketsReceived++;

  /*Increase the number of received packets*/
  if (m_uPacketsReceived == m_uNbrBlocksYmodem) /*Last Packet*/
  {
    /*Extracting actual payload from last packet*/
    if (0 == (m_uFileSizeYmodem % PACKET_1K_SIZE))
    {
      /* The last packet must be fully considered */
      uSize = PACKET_1K_SIZE;
    }
    else
    {
      /* The last packet is not full, drop the extra bytes */
      uSize = m_uFileSizeYmodem - ((uint32_t)(m_uFileSizeYmodem / PACKET_1K_SIZE) * PACKET_1K_SIZE);
    }

    m_uPacketsReceived = 0U;
  }
  /*Adjust dimension to 64-bit length */
  if (uSize %  m_uFlashMinWriteSize != 0U)
  {
    memset(&pData[uSize], 0xff, (m_uFlashMinWriteSize - (uSize %  m_uFlashMinWriteSize)));
    uSize += (m_uFlashMinWriteSize - (uSize %  m_uFlashMinWriteSize));
  }
  /* Write Data in Flash - size has to be 64-bit aligned */
  ret = LOADER_FLASH_DEV_NAME.ProgramData(uFlashDestination, pData, uSize);
  if (ret != ARM_DRIVER_OK)
  {
    /*Reset of the ymodem variables */
    m_uFileSizeYmodem = 0U;
    m_uPacketsReceived = 0U;
    m_uNbrBlocksYmodem = 0U;
    return HAL_ERROR;
  }
  else
    return HAL_OK;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
