/**
******************************************************************************
* @file    appli_flash_layout.h
* @author  MCD Application Team
* @brief   Application configuration file
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

/* ########### HEADER GENERATED AUTOMATICALLY, DO NOT EDIT ########### */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLI_FLASH_LAYOUT_H__
#define __APPLI_FLASH_LAYOUT_H__

#define PRIMARY_ONLY                          0x0        /* 1: No secondary (download) slot(s),
                                                               only primary slot(s) for each image.
                                                            0: Primary and secondary slot(s) for each image. */

#define OVERWRITE_ONLY                        0x0        /* 1: the FW installation uses overwrite method.
                                                            0: The FW installation uses swap mode. */

#define DATA_IMAGE_NUMBER                     0x1        /* 1: Data image for application.
                                                            0: No data image. */

#define IMAGE_PRIMARY_PARTITION_OFFSET        0x20000    /* Primary code image offset */
#define IMAGE_SECONDARY_PARTITION_OFFSET      0x30000    /* Secondary code image offset */
#define DATA_IMAGE_PRIMARY_PARTITION_OFFSET   0x1C000    /* Primary data image offset */
#define DATA_IMAGE_SECONDARY_PARTITION_OFFSET 0x1E000    /* Secondary data image offset */

#define IMAGE_PARTITION_SIZE                  0x10000    /* Code image size */
#define DATA_PARTITION_SIZE                   0x2000     /* Data image size */

#define DATA_HEADER_OFFSET                    0x20       /* Data payload offset */
#define DATA_SIZE                             0x20       /* Data payload size */

#define LOADER_FLASH_DEV_NAME Driver_FLASH0

#endif /* __APPLI_FLASH_LAYOUT_H__ */
