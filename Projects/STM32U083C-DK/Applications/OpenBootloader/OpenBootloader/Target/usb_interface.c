/**
  ******************************************************************************
  * @file    usb_interface.c
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
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
#include "openbl_core.h"

#include "usb_interface.h"

#include "app_usbx_device.h"
#include "ux_device_dfu_media.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t UsbDetected = 0U;

/* Exported variables --------------------------------------------------------*/
uint8_t UsbSofDetected = 0U;

PCD_HandleTypeDef USB_Handle_Fs;

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure USB pins and then initialize the used USB instance.
  * @retval None.
  */
void OPENBL_USB_Configuration(void)
{

  MX_USBX_Device_Init();

  USB_Handle_Fs.Instance                      = USB_DRD_FS;
  USB_Handle_Fs.Init.dev_endpoints            = 6U;
  USB_Handle_Fs.Init.speed                    = USBD_FS_SPEED;
  USB_Handle_Fs.Init.phy_itface               = PCD_PHY_EMBEDDED;
  USB_Handle_Fs.Init.Sof_enable               = ENABLE;
  USB_Handle_Fs.Init.low_power_enable         = DISABLE;
  USB_Handle_Fs.Init.lpm_enable               = DISABLE;
  USB_Handle_Fs.Init.battery_charging_enable  = DISABLE;

  if (HAL_PCD_Init(&USB_Handle_Fs) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialization of USB device */
  MX_USB_Device_Init();

  HAL_PCD_Start(&USB_Handle_Fs);
}

/**
  * @brief  This function is used to detect if there is any activity on USB protocol.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_USB_ProtocolDetection(void)
{
  if (UsbSofDetected == 1U)
  {
    UsbDetected = 1U;
  }
  else
  {
    UsbDetected = 0U;
  }

  return UsbDetected;
}

/**
  * @brief  This function is used to De-initialize the I2C pins and instance.
  * @retval None.
  */
void OPENBL_USB_DeInit(void)
{
  /* Only de-initialize the USB if it is not the current detected interface */
  if (UsbDetected == 0U)
  {
    __HAL_RCC_USB_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(USB_DRD_FS_IRQn);
    __HAL_RCC_USB_FORCE_RESET();
    __HAL_RCC_USB_RELEASE_RESET();
    HAL_PCD_DeInit(&USB_Handle_Fs);
  }
}

/**
  * @brief  Gets the page of a given address
  * @param  address Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t OPENBL_USB_GetPage(uint32_t address)
{
  uint32_t page = 0U;

  if (address < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE + 128U;
  }

  return page;
}
