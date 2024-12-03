/**
  ******************************************************************************
  * @file    app_usbx_device.c
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

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"
#include "platform.h"
#include "usb_interface.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBX_APP_STACK_SIZE     1024U
#define USBX_MEMORY_SIZE        (4U * 1024U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UX_SYSTEM_SLAVE                 *ux_system;

UX_SLAVE_CLASS_DFU_PARAMETER    dfu_parameter;

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN static UCHAR ux_device_memory_buffer[USBX_DEVICE_APP_MEMORY_BUFFER_SIZE] __ALIGN_END;


static ULONG dfu_interface_number;
static ULONG dfu_configuration_number;

/* Private function prototypes -----------------------------------------------*/
void  usbx_app_thread_entry(ULONG arg);

/**
  * @brief  Application USBX Device Initialization.
  * @param none
  * @retval int
  */
UINT MX_USBX_Device_Init(void)
{
  UINT ret = UX_SUCCESS;
  UCHAR *device_framework_high_speed;
  UCHAR *device_framework_full_speed;
  ULONG device_framework_hs_length;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG languge_id_framework_length;
  UCHAR *string_framework;
  UCHAR *language_id_framework;
  UCHAR *pointer;

  pointer = ux_device_memory_buffer;

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_DEVICE_MEMORY_STACK_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get Device Framework High Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED,
                                                                &device_framework_hs_length);

  /* Get Device Framework Full Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                                                &device_framework_fs_length);

  /* Get String Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get Language Id Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

  /* Install the device portion of USBX */
  if (ux_device_stack_initialize(device_framework_high_speed,
                                 device_framework_hs_length,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 languge_id_framework_length,
                                 DFU_Device_ConnectionCallback) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the dfu class parameters for the device */
  dfu_parameter.ux_slave_class_dfu_parameter_instance_activate   = DFU_Activate;
  dfu_parameter.ux_slave_class_dfu_parameter_instance_deactivate = DFU_Deactivate;
  dfu_parameter.ux_slave_class_dfu_parameter_get_status          = DFU_GetStatus;
  dfu_parameter.ux_slave_class_dfu_parameter_read                = DFU_Read;
  dfu_parameter.ux_slave_class_dfu_parameter_write               = DFU_Write;
  dfu_parameter.ux_slave_class_dfu_parameter_notify              = DFU_Notify;

#ifdef UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE
  dfu_parameter.ux_device_class_dfu_parameter_custom_request     = DFU_Leave;
#endif /* UX_DEVICE_CLASS_DFU_CUSTOM_REQUEST_ENABLE */
  dfu_parameter.ux_slave_class_dfu_parameter_framework           = device_framework_full_speed;
  dfu_parameter.ux_slave_class_dfu_parameter_framework_length    = device_framework_fs_length;

  /* Get dfu configuration number */
  dfu_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_DFU, 0);

  /* Find dfu interface number */
  dfu_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_DFU, 0);

  /* Initialize the device dfu class */
  if (ux_device_stack_class_register(_ux_system_slave_class_dfu_name,
                                     ux_device_class_dfu_entry,
                                     dfu_configuration_number,
                                     dfu_interface_number,
                                     &dfu_parameter) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  return ret;
}

/**
  * @brief  Function implementing usbx_app_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_app_thread_entry(ULONG arg)
{
  /* Initialization of USB device */
  MX_USB_Device_Init();

  HAL_PCD_Start(&USB_Handle_Fs);
}

/**
  * @brief  _ux_utility_interrupt_disable
  *         USB utility interrupt disable.
  * @param  none
  * @retval none
  */
ALIGN_TYPE _ux_utility_interrupt_disable(VOID)
{
  UINT interrupt_save;

  interrupt_save = __get_PRIMASK();
  __disable_irq();

  return interrupt_save;
}

/**
  * @brief  _ux_utility_interrupt_restore
  *         USB utility interrupt restore.
  * @param  flags
  * @retval none
  */
VOID _ux_utility_interrupt_restore(ALIGN_TYPE flags)
{
  __set_PRIMASK(flags);
}

/**
  * @brief  _ux_utility_time_get
  *         Get Time Tick for host timing.
  * @param  none
  * @retval time tick
  */
ULONG _ux_utility_time_get(VOID)
{
  ULONG time_tick = 0U;

  time_tick = HAL_GetTick();

  return time_tick;
}


/**
  * @brief  Run USBX state machine.
  * @param  arg: not used
  * @retval none
  */
void USBX_Device_Process(void *arg)
{
  ux_device_stack_tasks_run();

  /* Check if there is a jump request to application from USB interface */
  if (JumpUsb == 1U)
  {
    DFU_Jump();
  }
}

/**
  * @brief MX_USB_Device_Init
  *        Initialization of USB device.
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{
  /* USB_Device_Init_PreTreatment */
  HAL_PCDEx_PMAConfig(&USB_Handle_Fs, 0x00U, PCD_SNG_BUF, 0x0CU);
  HAL_PCDEx_PMAConfig(&USB_Handle_Fs, 0x80U, PCD_SNG_BUF, 0x4CU);

  /* Initialize the device controller driver*/
  _ux_dcd_stm32_initialize((ULONG)USB_DRD_FS, (ULONG)&USB_Handle_Fs);

}
