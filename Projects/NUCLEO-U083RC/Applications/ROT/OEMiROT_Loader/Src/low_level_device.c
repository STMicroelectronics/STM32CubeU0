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
#include "loader_flash_layout.h"
#include "low_level_flash.h"

static struct flash_range access_vect[] =
{
  { IMAGE_PARTITION_OFFSET, IMAGE_PARTITION_OFFSET + IMAGE_PARTITION_SIZE - 1},
#if (DATA_IMAGE_NUMBER == 1)
  { DATA_IMAGE_PARTITION_OFFSET, DATA_IMAGE_PARTITION_OFFSET + DATA_PARTITION_SIZE - 1},
#endif /* (DATA_IMAGE_NUMBER == 1) */
};

struct low_level_device FLASH0_DEV =
{
  .erase = { .nb = sizeof(access_vect) / sizeof(struct flash_range), .range = access_vect},
  .write = { .nb = sizeof(access_vect) / sizeof(struct flash_range), .range = access_vect},
};
