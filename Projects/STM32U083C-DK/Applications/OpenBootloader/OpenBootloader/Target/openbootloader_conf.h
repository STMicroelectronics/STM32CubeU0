/**
  ******************************************************************************
  * @file    openbootloader_conf.h
  * @author  MCD Application Team
  * @brief   Contains Open Bootloader configuration
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
#ifndef OPENBOOTLOADER_CONF_H
#define OPENBOOTLOADER_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* -------------------------------- Device ID --------------------------------*/
#define DEVICE_ID_MSB                     0x04U  /* MSB byte of device ID */
#define DEVICE_ID_LSB                     0x89U  /* LSB byte of device ID */

/* -------------------------- Definitions for Memories -----------------------*/
#define BL_FLASH_SIZE                     (256U * 1024U)                        /* Size of FLASH 256 KByte */
#define FLASH_START_ADDRESS               FLASH_BASE                            /* Start of Flash */
#define FLASH_END_ADDRESS                 (FLASH_START_ADDRESS + BL_FLASH_SIZE) /* End of Flash */

#define RAM_SIZE                          (40U * 1024U)                         /* Size of RAM 40 KByte */
#define RAM_START_ADDRESS                 0x20000000U                           /* Start of SRAM */
#define RAM_END_ADDRESS                   (RAM_START_ADDRESS + RAM_SIZE)        /* End of SRAM */

#define OB_SIZE                           136U                                  /* Size of OB 136 Bytes */
#define OB_START_ADDRESS                  0x40022020U                           /* Option bytes start address */
#define OB_END_ADDRESS                    (OB_START_ADDRESS + OB_SIZE)          /* Option bytes end address */

#define OTP_SIZE                          1024U                                 /* Size of OTP 1024 Bytes */
#define OTP_START_ADDRESS                 0x1FFF6800U                           /* OTP start address */
#define OTP_END_ADDRESS                   (OTP_START_ADDRESS + OTP_SIZE)        /* OTP end address */

#define ICP_SIZE                          (26U * 1024U)                         /* Size of ICP 26 KByte */
#define ICP_START_ADDRESS                 0x1FFF0000U                           /* System memory start address */
#define ICP_END_ADDRESS                   (ICP_START_ADDRESS + ICP_SIZE)        /* System memory end address */

#define EB_SIZE                           1024U                                 /* Size of EB 1024 Bytes */
#define EB_START_ADDRESS                  0x1FFF6C00U                           /* Engi bytes start address */
#define EB_END_ADDRESS                    (EB_START_ADDRESS + EB_SIZE)          /* Engi bytes end address */

#define OPENBL_RAM_SIZE                   0x00003400U         /* RAM used by the Open Bootloader 13 KByte */

#define OPENBL_DEFAULT_MEM                FLASH_START_ADDRESS /* Address used for the Erase, Writep and readp command */

#define RDP_LEVEL_0                       OB_RDP_LEVEL_0
#define RDP_LEVEL_1                       OB_RDP_LEVEL_1
#define RDP_LEVEL_2                       OB_RDP_LEVEL_2

#define AREA_ERROR                        0x0U  /* Error Address Area */
#define FLASH_AREA                        0x1U  /* Flash Address Area */
#define RAM_AREA                          0x2U  /* RAM Address area */
#define OB_AREA                           0x3U  /* Option bytes Address area */
#define OTP_AREA                          0x4U  /* OTP Address area */
#define ICP_AREA                          0x5U  /* System memory area */
#define EB_AREA                           0x6U  /* Engi bytes Address area */

#define FLASH_MASS_ERASE                  0xFFFFU
#define FLASH_BANK1_ERASE                 0xFFFEU
#define FLASH_BANK2_ERASE                 0xFFFDU

#define INTERFACES_SUPPORTED              5U

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBOOTLOADER_CONF_H */
