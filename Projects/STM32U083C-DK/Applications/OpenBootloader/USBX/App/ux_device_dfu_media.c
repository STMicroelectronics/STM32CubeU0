/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_dfu_media.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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
/* USER CODE END  */

/* Includes ------------------------------------------------------------------*/

#include "ux_device_dfu_media.h"

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "app_usbx_device.h"
#include "openbl_usb_cmd.h"
#include "platform.h"
#include "interfaces_conf.h"
#include "openbl_core.h"
#include "openbl_mem.h"
#include "usb_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBD_DFU_APP_DEFAULT_ADDRESS      0x08020000U

/* Private macro -------------------------------------------------------------*/
#define DFU_MEDIA_ERASE_TIME             (uint16_t)500
#define DFU_MEDIA_PROGRAM_TIME           (uint16_t)500

/* Private variables ---------------------------------------------------------*/
ux_dfu_downloadInfotypeDef       ux_dfu_download;

__IO ULONG   dfu_status = 0U;
ULONG   Address_ptr;
UCHAR   RX_Data[1024];
uint8_t JumpUsb = 0U;

/* Private function prototypes -----------------------------------------------*/
static UINT DFU_WriteMemory(uint32_t Address, uint8_t *pDest, uint32_t Len);
static UINT DFU_ReadMemory(uint32_t Address, uint8_t *pDest, uint32_t Len);

static UINT DFU_WriteProtect(uint8_t *pListOfSectors, uint32_t Length);
static UINT DFU_WriteUnprotect(void);
static UINT DFU_Erase(uint32_t Address);
static UINT DFU_ReadProtect(void);
static UINT DFU_ReadUnprotect(void);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  DFU_Activate
  *         This function is called when insertion of a DFU device.
  * @param  dfu: Pointer to the dfu class instance.
  * @retval none
  */
VOID DFU_Activate(VOID *dfu)
{
  /* Save the DFU instance */
  dfu = (UX_SLAVE_CLASS_DFU *) dfu;

  /* Unlock the internal flash */
  HAL_FLASH_Unlock();

  return;
}

/**
  * @brief  DFU_Deactivate
  *         This function is called when extraction of a DFU device.
  * @param  dfu: Pointer to the dfu class instance.
  * @retval none
  */
VOID DFU_Deactivate(VOID *dfu)
{
  UX_PARAMETER_NOT_USED(dfu);

  /* Reset the DFU instance */
  dfu = UX_NULL;

  /* Lock the internal flash */
  HAL_FLASH_Lock();

  return;
}

/**
  * @brief  Get status routine.
  * @param  dfu: dfu Instance.
  * @param  media_status : dfu media status.
  * @retval UX_SUCCESS.
  */
UINT DFU_GetStatus(void *dfu, ULONG *media_status)
{
  *media_status = dfu_status;

  return (UX_SUCCESS);
}

/**
  * @brief  Inform application when a begin and end of transfer of the firmware
occur.
  * @param  dfu: dfu Instance.
  * @param  notification: unused.
  * @retval UX_SUCCESS.
  */
UINT DFU_Notify(void *dfu, ULONG notification)
{
  UNUSED(notification);

  return (UX_SUCCESS);
}

/**
  * @brief  Memory read routine.
  * @param  dfu: dfu Instance
  * @param  block_number: block number.
  * @param  data_pointer: Pointer to the Source buffer.
  * @param  length: Number of data to be read (in bytes).
  * @retval Status.
  */
UINT DFU_Read(void *dfu, ULONG block_number, UCHAR *data_pointer,
              ULONG length, ULONG *media_status)
{
  UINT   Status      = UX_SUCCESS;
  ULONG  Address_src = 0U;

  if (block_number == 0U)
  {
    /* Store the values of all supported commands */
    *data_pointer       = DFU_CMD_GETCOMMANDS;
    *(data_pointer + 1U) = DFU_CMD_SETADDRESSPOINTER;
    *(data_pointer + 2U) = DFU_CMD_ERASE ;
    *(data_pointer + 3U) = 0U;
    *media_status = length;

  }
  else if (block_number > 0U)
  {
    /* Return the physical address from which the host requests to read data */
    Address_src = ((block_number - 2U) * UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH) + Address_ptr;

    /* Read Memory */
    if (DFU_ReadMemory(Address_src, data_pointer, length) != UX_SUCCESS)
    {
      /* Set DFU media Status Error */
      dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
      dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_WRITE << 4U;

      /* syncs the USB DFU device state */
      ux_device_class_dfu_state_sync(dfu);
    }
    else
    {
      /* Set DFU Status OK */
      dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
      dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;

      /* syncs the USB DFU device state */
      ux_device_class_dfu_state_sync(dfu);
    }

    *media_status = length;
  }
  else
  {
    Status = UX_ERROR;
  }

  return (Status);
}

/**
  * @brief  Memory write routine.
  * @param  dfu: dfu Instance.
  * @param  block_number: block number
  * @param  data_pointer: Pointer to the Source buffer.
  * @param  length: Number of data to be read (in bytes).
  * @param  media_status: Not used.
  * @retval status.
  */
UINT DFU_Write(void *dfu, ULONG block_number, UCHAR *data_pointer,
               ULONG length, ULONG *media_status)
{
  UINT  status  = 0U;
  ULONG dfu_polltimeout = 0U;

  /* store ux_dfu_download info*/
  ux_dfu_download.wlength = length;
  ux_dfu_download.data_ptr = RX_Data;
  ux_dfu_download.wblock_num = block_number;

  ux_utility_memory_copy(ux_dfu_download.data_ptr, data_pointer, length);

  if ((block_number == 0U) && (*data_pointer == DFU_CMD_ERASE))
  {
    /* set the time necessary for an erase operation*/
    dfu_polltimeout = DFU_MEDIA_ERASE_TIME;

    /* Set DFU media status Busy, dfu polltimeout in erase phase */
    dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_BUSY;
    dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;
    dfu_status += (uint8_t)(dfu_polltimeout) << 8U;
  }
  else
  {
    /* set the time necessary for a program operation*/
    dfu_polltimeout = DFU_MEDIA_PROGRAM_TIME;

    /* Set DFU media status Busy, dfu polltimeout in program phase */
    dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_BUSY;
    dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;
    dfu_status += (uint8_t)(dfu_polltimeout) << 8U;
  }

  USBX_DEVICE_DFU_DOWNLOAD_Task();

  return status;
}

#ifdef UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE
/**
  * @brief  Handles the sub-protocol DFU leave DFU mode request (leaves DFU mode
  *         and resets device to jump to user loaded code).
  * @param  dfu: dfu Instance.
  * @param  transfer: transfer request.
  * @retval None.
  */
UINT DFU_Leave(void *dfu, UX_SLAVE_TRANSFER *transfer)
{
  UCHAR *setup;
  UCHAR dfu_state;
  UINT  status = UX_ERROR;

  /* Get DFU state */
  dfu_state = _ux_device_class_dfu_state_get((UX_SLAVE_CLASS_DFU *)dfu);

  setup  = transfer->ux_slave_transfer_request_setup;

  if ((dfu_state == UX_SYSTEM_DFU_STATE_DFU_IDLE) ||
      (dfu_state == UX_SYSTEM_DFU_STATE_DFU_DNLOAD_IDLE))
  {

    if (setup[UX_SETUP_REQUEST] == UX_SLAVE_CLASS_DFU_COMMAND_DOWNLOAD)
    {

      if ((setup[UX_SETUP_LENGTH] == 0U) && (setup[UX_SETUP_LENGTH + 1U] == 0U))
      {
        /* Set USB jump token */
        JumpUsb = 1;

        status = UX_SUCCESS;
      }
    }
  }

  return (status);
}
#endif /* UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE */

/**
  * @brief  USBX_DEVICE_DFU_DOWNLOAD_Task
  *         Function implementing Download process.
  * @param  thread_input: Not used.
  * @retval none
  */
void USBX_DEVICE_DFU_DOWNLOAD_Task(void)
{
  UINT Command;
  ULONG Length;
  ULONG Address_dest;
  UX_SLAVE_CLASS_DFU *dfu = NULL;
  uint8_t status;

  if (ux_dfu_download.wblock_num == 0)
  {
    Command = *(ux_dfu_download.data_ptr);

    /* Decode the Special Command */
    switch (Command)
    {

      case DFU_CMD_SETADDRESSPOINTER:
        /* Get address pointer value used for computing the start address for Read and Write memory operations */
        Address_ptr =  *(ux_dfu_download.data_ptr + 1) ;
        Address_ptr += *(ux_dfu_download.data_ptr + 2) << 8 ;
        Address_ptr += *(ux_dfu_download.data_ptr + 3) << 16 ;
        Address_ptr += *(ux_dfu_download.data_ptr + 4) << 24 ;

        /* Check if the address is supported or not */
        status = DFU_CheckAddress(&Address_ptr);

        if (status != ACK_BYTE)
        {
          /* Set DFU media Status Error */
          dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
          dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_ADDRESS << 4U;
        }
        else
        {
          /* Set DFU Status OK */
          dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
          dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;
        }

        /* Update USB DFU state machine */
        ux_device_class_dfu_state_sync(dfu);
        break;

      case DFU_CMD_ERASE:
        /* Get address pointer value to erase one page of the internal media memory */
        Address_ptr =  *(ux_dfu_download.data_ptr + 1);
        Address_ptr += *(ux_dfu_download.data_ptr + 2) << 8;
        Address_ptr += *(ux_dfu_download.data_ptr + 3) << 16;
        Address_ptr += *(ux_dfu_download.data_ptr + 4) << 24;

        /* Check if the address is supported or not */
        status = DFU_CheckAddress(&Address_ptr);

        if (status != ACK_BYTE)
        {
          /* Set DFU media Status Error */
          dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
          dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_ADDRESS << 4U;
        }
        else
        {
          /* Set DFU Status OK */
          dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
          dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;

          /* Erase memory */
          if (DFU_Erase(Address_ptr) != UX_SUCCESS)
          {
            dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_ERASE << 4;
          }
          else
          {
            /* Set DFU status OK */
            dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
            dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;
          }
        }

        /* Update USB DFU state machine */
        ux_device_class_dfu_state_sync(dfu);
        break;

      case DFU_CMD_WRITE_PROTECT:
        /* Get the write protect length */
        Length = *(ux_dfu_download.data_ptr + 1U);

        /* Write protect command */
        DFU_WriteProtect((ux_dfu_download.data_ptr + 2U), Length);

        /* Set DFU Status OK */
        dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
        dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;

        /* syncs the USB DFU device state */
        ux_device_class_dfu_state_sync(dfu);
        break;

      case DFU_CMD_WRITE_UNPROTECT:
        /* Write unprotect command */
        DFU_WriteUnprotect();

        /* Set DFU Status OK */
        dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
        dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;

        /* syncs the USB DFU device state */
        ux_device_class_dfu_state_sync(dfu);
        break;

      case DFU_CMD_READ_PROTECT:
        /* Read protect command */
        DFU_ReadProtect();

        /* Set DFU Status OK */
        dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
        dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

        /* syncs the USB DFU device state */
        ux_device_class_dfu_state_sync(dfu);
        break;

      case DFU_CMD_READ_UNPROTECT:
        /* Read unprotect command */
        DFU_ReadUnprotect();

        /* Set DFU Status OK */
        dfu_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
        dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;

        /* syncs the USB DFU device state */
        ux_device_class_dfu_state_sync(dfu);
        break;

      default:
        break;
    }
  }
  /* Regular Download Command */
  else
  {
    /* Decode the required address to which the host requests to write data */
    Address_dest = ((ux_dfu_download.wblock_num - 2U) * UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH) +  Address_ptr;

    /* Write Memory */
    if (DFU_WriteMemory(Address_dest, ux_dfu_download.data_ptr, ux_dfu_download.wlength) != UX_SUCCESS)
    {
      /* Flash content doesn't match SRAM content, Set DFU status Error */
      dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
      dfu_status += UX_SLAVE_CLASS_DFU_STATUS_ERROR_WRITE << 4;
    }
    else
    {
      /* Set DFU Status OK */
      dfu_status =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
      dfu_status += UX_SLAVE_CLASS_DFU_STATUS_OK << 4;
    }

    /* Update USB DFU state machine */
    ux_device_class_dfu_state_sync(dfu);
  }
}

/**
  * @brief  Write to flash memory.
  * @param  Address: Address to read from.
  * @param  pDest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_ReadMemory(uint32_t Address, uint8_t *pDest, uint32_t Len)
{
  OPENBL_USB_ReadMemory((uint8_t *)(&Address), pDest, Len);
  return (UX_SUCCESS);
}

/**
  * @brief  Write to flash memory.
  * @param  Address: Address to be written to.
  * @param  pDest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_WriteMemory(uint32_t Address, uint8_t *pDest, uint32_t Len)
{
  OPENBL_USB_WriteMemory(pDest, (uint8_t *)(&Address), Len);
  return (UX_SUCCESS);
}

/**
  * @brief  Jump to user application
  * @retval None
  */
void DFU_Jump(void)
{
  /* Disable the USB */
  HAL_PCD_Stop(&USB_Handle_Fs);

  /* Disconnect USBX stack driver,  */
  ux_device_stack_disconnect();

  /* Jump to address */
  if (Address_ptr != 0U)
  {
    OPENBL_USB_Jump(Address_ptr);
  }
  else
  {
    OPENBL_USB_Jump(USBD_DFU_APP_DEFAULT_ADDRESS);
  }
}

/**
  * @brief  Erase sector.
  * @param  Address: Address of sector to be erased.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_Erase(uint32_t Address)
{
  UX_INTERRUPT_SAVE_AREA

  /* Disable interrupts to prevent the kernel disruption */
  UX_DISABLE

  OPENBL_USB_EraseMemory(Address);

  /* Restore interrupts */
  UX_RESTORE

  return (UX_SUCCESS);
}

/**
  * @brief  Write protect.
  * @param  pListOfSectors: List of sectors to be protected.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_WriteProtect(uint8_t *pListOfSectors, uint32_t Length)
{
  OPENBL_USB_WriteProtect(pListOfSectors, Length);
  return (UX_SUCCESS);
}

/**
  * @brief  Write unprotect.
  * @param  pListOfSectors: List of sectors to be protected.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_WriteUnprotect(void)
{
  OPENBL_USB_WriteUnprotect();
  return (UX_SUCCESS);
}

/**
  * @brief  Read protect.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_ReadProtect(void)
{
  OPENBL_USB_ReadProtect();
  return (UX_SUCCESS);
}

/**
  * @brief  Read unprotect.
  * @retval UX_SUCCESS if operation is successful, MAL_FAIL else.
  */
static UINT DFU_ReadUnprotect(void)
{
  OPENBL_USB_ReadUnprotect();
  return (UX_SUCCESS);
}
/**
  * @brief DFU device connection callback.
  * @param  Device_State: dfu Instance.
  * @retval status.
  */
UINT DFU_Device_ConnectionCallback(ULONG Device_State)
{

  if (Device_State == UX_DEVICE_REMOVED)
  {
    if (_ux_system_slave -> ux_system_slave_device_dfu_mode ==  UX_DEVICE_CLASS_DFU_MODE_DFU)
    {
    }
  }
  else if (Device_State == UX_DEVICE_ATTACHED)
  {
    /* Notify the OpenBL the detection of a USB interface */
    UsbSofDetected = 1;
  }
  else
  {
  }

  return UX_SUCCESS;
}

/**
  * @brief  This function is used to check a valid address or not.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
uint8_t DFU_CheckAddress(ULONG *Address)
{
  uint8_t  status;
  uint32_t *add = (uint32_t *)Address;
  /* Check if received address is valid or not */
  if (OPENBL_MEM_GetAddressArea(*add) == AREA_ERROR)
  {
    status = NACK_BYTE;
  }
  else
  {
    status = ACK_BYTE;
  }

  return status;
}
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
