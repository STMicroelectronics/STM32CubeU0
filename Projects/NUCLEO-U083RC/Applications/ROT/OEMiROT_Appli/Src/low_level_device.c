/**
  ******************************************************************************
  * @file    low_level_device.c
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_device
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
#include "appli_flash_layout.h"
#include "low_level_flash.h"

static struct flash_range access_vect[] =
{
#if (PRIMARY_ONLY == 0)
  { IMAGE_SECONDARY_PARTITION_OFFSET, IMAGE_SECONDARY_PARTITION_OFFSET + IMAGE_PARTITION_SIZE - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { DATA_IMAGE_SECONDARY_PARTITION_OFFSET, DATA_IMAGE_SECONDARY_PARTITION_OFFSET + DATA_PARTITION_SIZE - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */
#else
  { IMAGE_PRIMARY_PARTITION_OFFSET, IMAGE_PRIMARY_PARTITION_OFFSET + IMAGE_PARTITION_SIZE - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { DATA_IMAGE_PRIMARY_PARTITION_OFFSET, DATA_IMAGE_PRIMARY_PARTITION_OFFSET + DATA_PARTITION_SIZE - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */
#endif /* (PRIMARY_ONLY == 0) */
};
#if (OVERWRITE != 0)
#define write_vect access_vect
#else
static struct flash_range write_vect[] =
{
  { IMAGE_SECONDARY_PARTITION_OFFSET, IMAGE_SECONDARY_PARTITION_OFFSET + IMAGE_PARTITION_SIZE - 1},
  { IMAGE_PRIMARY_PARTITION_OFFSET + IMAGE_PARTITION_SIZE - 32, IMAGE_PRIMARY_PARTITION_OFFSET + IMAGE_PARTITION_SIZE - 16 - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { DATA_IMAGE_SECONDARY_PARTITION_OFFSET, DATA_IMAGE_SECONDARY_PARTITION_OFFSET + DATA_PARTITION_SIZE - 1},
  { DATA_IMAGE_PRIMARY_PARTITION_OFFSET + DATA_PARTITION_SIZE - 32, DATA_IMAGE_PRIMARY_PARTITION_OFFSET + DATA_PARTITION_SIZE - 16 - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */
};
#endif /* (OVERWRITE != 0) */

struct low_level_device FLASH0_DEV =
{
  .erase = { .nb = sizeof(access_vect) / sizeof(struct flash_range), .range = access_vect},
  .write = { .nb = sizeof(write_vect) / sizeof(struct flash_range), .range = write_vect},
};
