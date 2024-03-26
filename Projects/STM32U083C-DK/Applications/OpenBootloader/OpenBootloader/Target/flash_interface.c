/**
  ******************************************************************************
  * @file    flash_interface.c
  * @author  MCD Application Team
  * @brief   Contains FLASH access functions
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
#include "main.h"
#include "platform.h"
#include "common_interface.h"

#include "openbl_mem.h"

#include "app_openbootloader.h"
#include "flash_interface.h"
#include "i2c_interface.h"
#include "optionbytes_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_FLASH_TIMEOUT_VALUE        1000U
#define FLASH_PAGE_MAX_NUMBER             ((uint8_t)0xFF)
#define FLASH_PROG_STEP_SIZE              ((uint8_t)0x8)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_ProcessTypeDef FlashProcess = {.Lock = HAL_UNLOCKED, \
                                     .ErrorCode = HAL_FLASH_ERROR_NONE, \
                                     .ProcedureOnGoing = FLASH_TYPENONE, \
                                     .Address = 0U, \
                                     .Page = 0U, \
                                     .NbPagesToErase = 0U
                                    };

uint32_t Flash_BusyState = FLASH_BUSY_STATE_DISABLED;

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_FLASH_Program(uint32_t address, uint32_t data);
static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *p_list_of_pages, uint32_t length);
static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void);

/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef FLASH_Descriptor =
{
  FLASH_START_ADDRESS,
  FLASH_END_ADDRESS,
  BL_FLASH_SIZE,
  FLASH_AREA,
  OPENBL_FLASH_Read,
  OPENBL_FLASH_Write,
  OPENBL_FLASH_SetReadOutProtectionLevel,
  OPENBL_FLASH_SetWriteProtection,
  OPENBL_FLASH_JumpToAddress,
  NULL,
  OPENBL_FLASH_Erase
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the FLASH control register access.
  * @retval None.
  */
void OPENBL_FLASH_Unlock(void)
{
  HAL_FLASH_Unlock();
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval None.
  */
void OPENBL_FLASH_Lock(void)
{
  HAL_FLASH_Lock();
}

/**
  * @brief  Unlock the FLASH Option Bytes Registers access.
  * @retval None.
  */
void OPENBL_FLASH_OB_Unlock(void)
{
  HAL_FLASH_Unlock();

  HAL_FLASH_OB_Unlock();
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_FLASH_Read(uint32_t address)
{
  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in FLASH memory.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_FLASH_Write(uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  __ALIGNED(4) uint8_t data[FLASH_PROG_STEP_SIZE] = {0x0U};
  uint8_t remaining;

  if ((p_data != NULL) && (data_length != 0U))
  {
    /* Unlock the flash memory for write operation */
    OPENBL_FLASH_Unlock();

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

    /* Program double-word by double-word (8 bytes) */
    while ((data_length >> 3U) > 0U)
    {
      for (index = 0U; index < FLASH_PROG_STEP_SIZE; index++)
      {
        data[index] = *(p_data + index);
      }

      OPENBL_FLASH_Program(address, (uint32_t)data);

      address    += FLASH_PROG_STEP_SIZE;
      p_data      += FLASH_PROG_STEP_SIZE;
      data_length -= FLASH_PROG_STEP_SIZE;
    }

    /* If remaining count, go back to fill the rest with 0xFF */
    if (data_length > 0U)
    {
      remaining = FLASH_PROG_STEP_SIZE - data_length;

      /* Copy the remaining bytes */
      for (index = 0U; index < data_length; index++)
      {
        data[index] = *(p_data + index);
      }

      /* Fill the upper bytes with 0xFF */
      for (index = 0U; index < remaining; index++)
      {
        data[index + data_length] = 0xFFU;
      }

      /* FLASH word program */
      OPENBL_FLASH_Program(address, (uint32_t)data);
    }

    /* Lock the Flash to disable the flash control register access */
    OPENBL_FLASH_Lock();
  }
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  address The address where the function will jump.
  * @retval None.
  */
void OPENBL_FLASH_JumpToAddress(uint32_t address)
{
  Function_Pointer jump_to_address;

  /* De-initialize all HW resources used by the Open Bootloader to their reset values */
  OPENBL_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) address);

  jump_to_address();
}

/**
  * @brief  Return the FLASH Read Protection level.
  * @retval The return value can be one of the following values:
  *         @arg OB_RDP_LEVEL_0: No protection
  *         @arg OB_RDP_LEVEL_1: Read protection of the memory
  *         @arg OB_RDP_LEVEL_2: Full chip protection
  */
uint32_t OPENBL_FLASH_GetReadOutProtectionLevel(void)
{
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Get the Option bytes configuration */
  HAL_FLASHEx_OBGetConfig(&flash_ob);

  return flash_ob.RDPLevel;
}

/**
  * @brief  Return the FLASH Read Protection level.
  * @param  level Can be one of these values:
  *         @arg OB_RDP_LEVEL_0: No protection
  *         @arg OB_RDP_LEVEL_1: Read protection of the memory
  *         @arg OB_RDP_LEVEL_2: Full chip protection
  * @retval None.
  */
void OPENBL_FLASH_SetReadOutProtectionLevel(uint32_t level)
{
  FLASH_OBProgramInitTypeDef flash_ob;

  if (level != OB_RDP_LEVEL2)
  {
    flash_ob.OptionType = OPTIONBYTE_RDP;
    flash_ob.RDPLevel   = level;

    /* Unlock the FLASH registers & Option Bytes registers access */
    OPENBL_FLASH_OB_Unlock();

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

    /* Change the RDP level */
    HAL_FLASHEx_OBProgram(&flash_ob);

    /* Register system reset callback */
    Common_SetPostProcessingCallback(OPENBL_OB_Launch);
  }
}

/**
  * @brief  This function is used to enable or disable write protection of the specified FLASH areas.
  * @param  state Can be one of these values:
  *         @arg DISABLE: Disable FLASH write protection
  *         @arg ENABLE: Enable FLASH write protection
  * @param  p_list_of_pages Contains the list of pages to be protected.
  * @param  length The length of the list of pages to be protected.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
ErrorStatus OPENBL_FLASH_SetWriteProtection(FunctionalState state, uint8_t *p_list_of_pages, uint32_t length)
{
  ErrorStatus status = SUCCESS;

  if (state == ENABLE)
  {
    OPENBL_FLASH_EnableWriteProtection(p_list_of_pages, length);

    /* Register system reset callback */
    Common_SetPostProcessingCallback(OPENBL_OB_Launch);
  }
  else if (state == DISABLE)
  {
    OPENBL_FLASH_DisableWriteProtection();

    /* Register system reset callback */
    Common_SetPostProcessingCallback(OPENBL_OB_Launch);
  }
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to start FLASH mass erase operation.
  * @param  p_data Pointer to the buffer that contains mass erase operation options.
  * @param  data_length Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Mass erase operation done
  *          - ERROR:   Mass erase operation failed or the value of one parameter is not ok
  */
ErrorStatus OPENBL_FLASH_MassErase(uint8_t *p_data, uint32_t data_length)
{
  uint32_t page_error;
  ErrorStatus status = SUCCESS;
  FLASH_EraseInitTypeDef erase_init_struct;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

  erase_init_struct.TypeErase = FLASH_TYPEERASE_MASS;

  if (data_length >= 2U)
  {
    if (status == SUCCESS)
    {
      if (HAL_FLASHEx_Erase(&erase_init_struct, &page_error) != HAL_OK)
      {
        status = ERROR;
      }
      else
      {
        status = SUCCESS;
      }
    }
  }
  else
  {
    status = ERROR;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();

  return status;
}

/**
  * @brief  This function is used to erase the specified FLASH pages.
  * @param  p_data Pointer to the buffer that contains erase operation options.
  * @param  data_length Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Erase operation done
  *          - ERROR:   Erase operation failed or the value of one parameter is not ok
  */
ErrorStatus OPENBL_FLASH_Erase(uint8_t *p_data, uint32_t data_length)
{
  uint32_t counter      = 0U;
  uint32_t pages_number = 0U;
  uint32_t page_error   = 0U;
  uint32_t errors       = 0U;
  ErrorStatus status    = SUCCESS;
  FLASH_EraseInitTypeDef erase_init_struct;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

  pages_number  = (uint32_t)(*(uint16_t *)(p_data));
  p_data       += 2U;

  erase_init_struct.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init_struct.NbPages = 1U;

  for (counter = 0U; ((counter < pages_number) && (counter < (data_length / 2U))) ; counter++)
  {
    erase_init_struct.Page = ((uint32_t)(*(uint16_t *)(p_data)));

    if (status != ERROR)
    {
      if (OPENBL_FLASH_ExtendedErase(&erase_init_struct, &page_error) != HAL_OK)
      {
        errors++;
      }
    }
    else
    {
      /* Reset the status for next erase operation */
      status = SUCCESS;
    }

    p_data += 2U;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();

  if (errors > 0U)
  {
    status = ERROR;
  }
  else
  {
    status = SUCCESS;
  }

  return status;
}

/**
  * @brief  Program double word at a specified FLASH address.
  * @param  address specifies the address to be programmed.
  * @param  data specifies the address of data to be programmed.
  * @retval None.
  */
static void OPENBL_FLASH_Program(uint32_t address, uint32_t data)
{
  /* Clear all FLASH errors flags before starting write operation */
  __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, (uint64_t)(*((uint64_t *)data)));
}

/**
  * @brief  This function is used to Set Flash busy state variable to activate busy state sending
  *         during flash operations
  * @retval None.
  */
void OPENBL_Enable_BusyState_Flag(void)
{
  /* Enable Flash busy state sending */
  Flash_BusyState = FLASH_BUSY_STATE_ENABLED;
}

/**
  * @brief  This function is used to disable the send of busy state in I2C non stretch mode.
  * @retval None.
  */
void OPENBL_Disable_BusyState_Flag(void)
{
  /* Disable Flash busy state sending */
  Flash_BusyState = FLASH_BUSY_STATE_DISABLED;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to enable write protection of the specified FLASH areas.
  * @param  p_list_of_pages Contains the list of pages to be protected.
  * @param  length The length of the list of pages to be protected.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *p_list_of_pages, uint32_t length)
{
  uint8_t wrp_start_offset = FLASH_PAGE_MAX_NUMBER;
  uint8_t wrp_end_offset   = 0x00U;
  ErrorStatus status       = SUCCESS;
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

  flash_ob.OptionType = OPTIONBYTE_WRP;

  /* Write protection of area 1 in WRPA1 */
  if (length >= 2U)
  {
    wrp_start_offset = *(p_list_of_pages);
    wrp_end_offset   = *(p_list_of_pages + 1U);

    flash_ob.WRPArea        = OB_WRPAREA_ZONE_A;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;

    status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
  }

  /* Write protection of area 1 in WRPA1 */
  if (length >= 4U)
  {
    wrp_start_offset = *(p_list_of_pages + 2U);
    wrp_end_offset   = *(p_list_of_pages + 3U);

    flash_ob.WRPArea        = OB_WRPAREA_ZONE_B;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;

    status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;
  }

  return status;
}

/**
  * @brief  This function is used to disable write protection.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void)
{
  uint8_t wrp_start_offset = FLASH_PAGE_MAX_NUMBER;
  uint8_t wrp_end_offset   = 0x00U;
  ErrorStatus status       = SUCCESS;
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_SR_ERRORS);

  flash_ob.OptionType = OPTIONBYTE_WRP;

  /* Disable area 1 and 2 write protection */
  flash_ob.WRPArea        = OB_WRPAREA_ZONE_A;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;
  status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;

  flash_ob.WRPArea        = OB_WRPAREA_ZONE_B;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;
  status = (HAL_FLASHEx_OBProgram(&flash_ob) != HAL_OK) ? ERROR : SUCCESS;

  return status;
}

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory pages.
  * @param[in]  p_erase_init pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * @param[out]  p_sector_error pointer to variable that contains the configuration
  *         information on faulty page in case of error (0xFFFFFFFF means that all
  *         the pages have been correctly erased).
  * @retval HAL_Status
  */
#if defined (__ICCARM__)
__ramfunc HAL_StatusTypeDef OPENBL_FLASH_ExtendedErase(FLASH_EraseInitTypeDef *p_erase_init,
                                                              uint32_t *p_sector_error)
#else
__attribute__((section(".ramfunc"))) HAL_StatusTypeDef OPENBL_FLASH_ExtendedErase(
  FLASH_EraseInitTypeDef *p_erase_init, uint32_t *p_sector_error)
#endif /* (__ICCARM__) */
{
  HAL_StatusTypeDef status;
  uint32_t tmp;

  /* Process Locked */
  __HAL_LOCK(&FlashProcess);

  /* Wait for last operation to be completed */
  status = OPENBL_FLASH_WaitForLastOperation(OPENBL_FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /*Initialization of SectorError variable*/
    *p_sector_error = 0xFFFFFFFFU;

    /* Start erase page */
    /* Get configuration register, then clear page number */
    tmp = (FLASH->CR & ~FLASH_CR_PNB);

    /* Set page number, Page Erase bit & Start bit */
    FLASH->CR = (tmp | (FLASH_CR_STRT | ((p_erase_init->Page) << FLASH_CR_PNB_Pos) | FLASH_CR_PER));

    if (Flash_BusyState == FLASH_BUSY_STATE_ENABLED)
    {
      /* Wait for last operation to be completed to send busy byte*/
      status = OPENBL_FLASH_SendBusyState(PROGRAM_TIMEOUT);
    }
    else
    {
      /* Wait for last operation to be completed */
      status = OPENBL_FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);
    }

    if (status != HAL_OK)
    {
      /* In case of error, stop erase procedure and return the faulty sector*/
      *p_sector_error = p_erase_init->Page;
    }

    /* If operation is completed or interrupted, disable the Page Erase Bit */
    CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&FlashProcess);

  /* return status */
  return status;
}

/**
  * @brief  Send a busy byte to the host while the flash is in a busy state.
  * @param  timeout maximum flash operation timeout.
  * @retval HAL_Status
  */
#if defined (__ICCARM__)
__ramfunc HAL_StatusTypeDef OPENBL_FLASH_WaitForLastOperation(uint32_t timeout)
#else
__attribute__((section(".ramfunc"))) HAL_StatusTypeDef OPENBL_FLASH_WaitForLastOperation(uint32_t timeout)
#endif /* (__ICCARM__) */
{
  uint32_t error;
  uint32_t i = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* Wait if any operation is ongoing */
  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0x00U)
  {
    if (i++ > timeout)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  /* check flash errors */
  error = (FLASH->SR & FLASH_SR_ERRORS);

  /* clear error flags */
  __HAL_FLASH_CLEAR_FLAG(error);

  if (error != 0x00U)
  {
    /*Save the error code*/
    pFlash.ErrorCode = error;

    status = HAL_ERROR;
  }

  /* Wait for control register to be written */
  i = 0U;

  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY) != 0x00U)
  {
    if (i++ > timeout)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  /* Return status */
  return status;
}

/**
  * @brief  Send a busy byte to the host while the flash is in a busy state.
  * @param  timeout maximum flash operation timeout.
  * @retval HAL_Status
  */
#if defined (__ICCARM__)
__ramfunc HAL_StatusTypeDef OPENBL_FLASH_SendBusyState(uint32_t timeout)
#else
__attribute__((section(".ramfunc"))) HAL_StatusTypeDef OPENBL_FLASH_SendBusyState(uint32_t timeout)
#endif /* (__ICCARM__) */
{
  uint32_t error;
  uint32_t i = 0;
  HAL_StatusTypeDef status = HAL_OK;

  /* Wait if any operation is ongoing */
  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0x00U)
  {
    if (i++ > timeout)
    {
      status = HAL_TIMEOUT;
      break;
    }
    else
    {
      /* Send busy byte */
      OPENBL_I2C_SendBusyByte();
    }
  }

  /* check flash errors */
  error = (FLASH->SR & FLASH_SR_ERRORS);

  /* clear error flags */
  __HAL_FLASH_CLEAR_FLAG(error);

  if (error != 0x00U)
  {
    /*Save the error code*/
    pFlash.ErrorCode = error;

    status = HAL_ERROR;
  }

  /* Wait for control register to be written */
  i = 0U;

  while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY) != 0x00U)
  {
    if (i++ > timeout)
    {
      status = HAL_TIMEOUT;
      break;
    }
    else
    {
      /* Send busy byte */
      OPENBL_I2C_SendBusyByte();
    }
  }

  /* Return status */
  return status;
}
