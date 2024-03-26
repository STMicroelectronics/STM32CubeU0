/**
  ******************************************************************************
  * @file    ux_device_dfu_media.h
  * @author  MCD Application Team
  * @brief   USBX Device DFU interface header file
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
#ifndef __UX_DEVICE_DFU_MEDIA_H__
#define __UX_DEVICE_DFU_MEDIA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"

/* Private includes ----------------------------------------------------------*/
#include "ux_device_class_dfu.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  ULONG wblock_num;
  ULONG wlength;
  UCHAR *data_ptr;
} ux_dfu_downloadInfotypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Special Commands with Download Request */
#define DFU_CMD_GETCOMMANDS               0x00U
#define DFU_CMD_SETADDRESSPOINTER         0x21U
#define DFU_CMD_ERASE                     0x41U
#define DFU_CMD_READ_PROTECT              0x91U
#define DFU_CMD_READ_UNPROTECT            0x92U
#define DFU_CMD_WRITE_PROTECT             0x81U
#define DFU_CMD_WRITE_UNPROTECT           0x82U

/* Private defines -----------------------------------------------------------*/
extern uint8_t JumpUsb;
/* Exported functions prototypes ---------------------------------------------*/
UINT DFU_GetStatus(void *dfu, ULONG *media_status);
UINT DFU_Notify(void *dfu, ULONG notification);
UINT DFU_Read(void *dfu, ULONG block_number, UCHAR *data_pointer, ULONG length, ULONG *media_status);
UINT DFU_Write(void *dfu, ULONG block_number, UCHAR *data_pointer, ULONG length, ULONG *media_status);

UINT DFU_Device_ConnectionCallback(ULONG Device_State);
void DFU_Activate(void *dfu_instance);
void DFU_Deactivate(void *dfu_instance);

#ifdef UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE
UINT DFU_Leave(void *dfu, UX_SLAVE_TRANSFER *transfer);
#endif
void DFU_Jump(void);
void USBX_DEVICE_DFU_DOWNLOAD_Task(void);

uint8_t DFU_CheckAddress(ULONG *Address);

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_DFU_MEDIA_H__ */
