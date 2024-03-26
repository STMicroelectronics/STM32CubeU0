/**
******************************************************************************
* @file    loader_flash_layout.h
* @author  MCD Application Team
* @brief   Loader configuration file
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
#ifndef __LOADER_FLASH_LAYOUT_H__
#define __LOADER_FLASH_LAYOUT_H__

#define PRIMARY_ONLY                          0x0        /* 1: No secondary (download) slot(s),
                                                               only primary slot(s) for each image.
                                                            0: Primary and secondary slot(s) for each image. */

#define DATA_IMAGE_NUMBER                     0x1        /* 1: Data image for application.
                                                            0: No data image. */

#define IMAGE_PARTITION_OFFSET                0x30000    /* Code image offset */
#define DATA_IMAGE_PARTITION_OFFSET           0x1E000    /* Data image offset */

#define IMAGE_PARTITION_SIZE                  0x10000    /* Code image size */
#define DATA_PARTITION_SIZE                   0x2000     /* Data image size */

#define LOADER_FLASH_DEV_NAME Driver_FLASH0

#endif /* __LOADER_FLASH_LAYOUT_H__ */
