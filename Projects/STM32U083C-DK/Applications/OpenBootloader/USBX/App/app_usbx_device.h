/**
  ******************************************************************************
  * @file    app_usbx_device.h
  * @author  MCD Application Team
  * @brief   USBX Device applicative header file
  ******************************************************************************
   * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_USBX_DEVICE_H__
#define __APP_USBX_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_stack.h"
#include "ux_dcd_stm32.h"
#include "ux_device_descriptors.h"
#include "ux_device_dfu_media.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USBX_DEVICE_APP_MEMORY_BUFFER_SIZE  3*1024
#define USBX_DEVICE_MEMORY_STACK_SIZE       3*1024
/* Exported macro ------------------------------------------------------------*/
#define APP_QUEUE_SIZE          5U

/* Exported functions prototypes ---------------------------------------------*/
UINT MX_USBX_Device_Init(void);

void MX_USB_Device_Init(void);
void USBX_Device_Process(void *arg);


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  STOP_USB_DEVICE = 1,
  START_USB_DEVICE,
} USB_MODE_STATE;

#ifdef __cplusplus
}
#endif
#endif /* __APP_USBX_DEVICE_H__ */
