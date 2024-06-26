/**
  ******************************************************************************
  * @file    spi_interface.c
  * @author  MCD Application Team
  * @brief   Contains SPI HW configuration
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
#include "interfaces_conf.h"

#include "openbl_spi_cmd.h"

#include "app_openbootloader.h"
#include "spi_interface.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_DUMMY_BYTE                    0x00U  /* Dummy byte */
#define SPI_SYNC_BYTE                     0x5AU  /* Synchronization byte */
#define SPI_BUSY_BYTE                     0xA5U  /* Busy byte */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint8_t SpiRxNotEmpty = 0U;
static uint8_t SpiDetected        = 0U;
static uint8_t BusyState;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_SPI_Init(void);
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_ClearFlag_OVR(void);
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_ClearFlag_OVR(void);
#endif /* (__ICCARM__) */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the SPI peripheral
  * @retval None.
  */
static void OPENBL_SPI_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0U};

  /* Configure the SPIx IP:
      - Mode               : Slave.
      - Transfer Direction : Full Duplex.
      - Data width         : 8-bit.
      - Bit Order          : MSB first.
      - Polarity           : CPOL Low, CPHA Low.
      - NSS                : Software.
  */
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode              = LL_SPI_MODE_SLAVE;
  SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly           = 7U;

  LL_SPI_Init(SPIx, &SPI_InitStruct);
  LL_SPI_SetRxFIFOThreshold(SPIx, LL_SPI_RX_FIFO_TH_QUARTER);

  HAL_NVIC_SetPriority(SPIx_IRQ, 0U, 0U);
  HAL_NVIC_EnableIRQ(SPIx_IRQ);

  LL_SPI_Enable(SPIx);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure SPI pins and then initialize the used SPI instance.
  * @retval None.
  */
void OPENBL_SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0U};

  /* Enable all resources clocks ---------------------------------------------*/
  /* Enable used GPIOx clocks */
  SPIx_GPIO_CLK_SCK_ENABLE();
  SPIx_GPIO_CLK_MISO_ENABLE();
  SPIx_GPIO_CLK_MOSI_ENABLE();
  SPIx_GPIO_CLK_NSS_ENABLE();

  /* Enable SPI clock */
  SPIx_CLK_ENABLE();

  /* SPI1 pins configuration -------------------------------------------------*/
  /*
           +-------------+
           |   SPI1      |
     +-----+-------------+
     | MOSI|     PA7     |
     +-----+-------------+
     | MISO|     PA6     |
     +-----+-------------+
     | SCK |     PA5     |
     +-----+-------------+
     | NSS |     PA4     |
     +-----+-------------+ */

  /* Common configuration for SPIx PINs:
      Mode        : Alternate function.
      Output type : Push-Pull (No pull).
      Speed       : High speed.
  */
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = SPIx_ALTERNATE;

  /* SPI MOSI pin configuration */
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
  HAL_GPIO_Init(SPIx_MOSI_PIN_PORT, &GPIO_InitStruct);

  /* SPI MISO pin configuration */
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;
  HAL_GPIO_Init(SPIx_MISO_PIN_PORT, &GPIO_InitStruct);

  /* SPI SCK pin configuration */
  GPIO_InitStruct.Pin = SPIx_SCK_PIN;
  HAL_GPIO_Init(SPIx_SCK_PIN_PORT, &GPIO_InitStruct);

  /* SPI NSS pin configuration */
  GPIO_InitStruct.Pin = SPIx_NSS_PIN;
  HAL_GPIO_Init(SPIx_NSS_PIN_PORT, &GPIO_InitStruct);

  OPENBL_SPI_Init();
}

/**
  * @brief  This function is used to De-initialize the SPI pins and instance.
  * @retval None.
  */
void OPENBL_SPI_DeInit(void)
{
  /* Only de-initialize the SPI if it is not the current detected interface */
  if (SpiDetected == 0U)
  {
    LL_SPI_Disable(SPIx);

    SPIx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on SPI protocol.
  * @retval None.
  */
uint8_t OPENBL_SPI_ProtocolDetection(void)
{
  /* Check if there is any activity on SPI */
  if (LL_SPI_IsActiveFlag_RXNE(SPIx) != 0U)
  {
    /* Check that Synchronization byte has been received on SPI */
    if (LL_SPI_ReceiveData8(SPIx) == SPI_SYNC_BYTE)
    {
      SpiDetected = 1U;

      /* Enable the interrupt of Rx not empty buffer */
      LL_SPI_EnableIT_RXNE(SPIx);

      /* Send synchronization byte */
      OPENBL_SPI_SendByte(SYNC_BYTE);

      /* Send acknowledgment */
      OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);
    }
    else
    {
      SpiDetected = 0U;
    }
  }
  else
  {
    SpiDetected = 0U;
  }

  return SpiDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command.
  */
uint8_t OPENBL_SPI_GetCommandOpcode(void)
{
  uint8_t command_opc;

  /* Disable busy byte */
  BusyState = 0U;

  /* Check if there is any activity on SPI */
  while (OPENBL_SPI_ReadByte() != SPI_SYNC_BYTE)
  {}

  /* Get the command opcode */
  command_opc = OPENBL_SPI_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_SPI_ReadByte()) != 0xFFU)
  {
    command_opc = ERROR_COMMAND;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from SPI pipe.
  *         Read operation is synchronized on SPI Rx buffer not empty interrupt.
  * @retval Returns the read byte.
  */
#if defined (__ICCARM__)
__ramfunc uint8_t OPENBL_SPI_ReadByte(void)
#else
__attribute__((section(".ramfunc"))) uint8_t OPENBL_SPI_ReadByte(void)
#endif /* (__ICCARM__) */
{
  uint8_t data;

  /* Wait until SPI Rx buffer not empty interrupt */
  while (SpiRxNotEmpty == 0U)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;
  }

  /* Reset the RX not empty token */
  SpiRxNotEmpty = 0U;

  /* Read the SPI data register */
  data = SPIx->DR;

  /* Enable the interrupt of Rx not empty buffer */
  SPIx->CR2 |= SPI_CR2_RXNEIE;

  return data;
}

/**
  * @brief  This function is used to send one busy byte each receive interrupt through SPI pipe.
  *         Read operation is synchronized on SPI Rx buffer not empty interrupt.
  * @retval Returns the read byte.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_SendBusyByte(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_SendBusyByte(void)
#endif /* (__ICCARM__) */
{
  /* Wait until SPI Rx buffer not empty interrupt */
  while (SpiRxNotEmpty == 0U)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;
  }

  /* Reset the RX not empty token */
  SpiRxNotEmpty = 0U;

  /* Transmit the busy byte */
  *((__IO uint8_t *)&SPIx->DR) = SPI_BUSY_BYTE;

  /* Read bytes from the host to avoid the overrun */
  OPENBL_SPI_ClearFlag_OVR();

  /* Enable the interrupt of Rx not empty buffer */
  SPIx->CR2 |= SPI_CR2_RXNEIE;
}

/**
  * @brief  This function is used to send one byte through SPI pipe.
  * @param  byte The byte to be sent.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_SendByte(uint8_t byte)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_SendByte(uint8_t byte)
#endif /* (__ICCARM__) */
{
  /* Wait until SPI transmit buffer is empty */
  while ((SPIx->SR & SPI_SR_TXE) == 0U)
  {}

  /* Transmit the data */
  *((__IO uint8_t *)&SPIx->DR) = byte;
}

/**
  * @brief  This function is used to send acknowledge byte through SPI pipe.
  * @param  byte The byte to be sent.
  * @retval None.
  */
void OPENBL_SPI_SendAcknowledgeByte(uint8_t byte)
{
  /* Check the AN4286 for the acknowledge procedure */
  OPENBL_SPI_SendByte(SPI_DUMMY_BYTE);

  OPENBL_SPI_SendByte(byte);

  /* Wait for the host to send ACK synchronization byte */
  while (OPENBL_SPI_ReadByte() != ACK_BYTE)
  {}

  OPENBL_SPI_SendByte(SPI_DUMMY_BYTE);
}

/**
  * @brief  Handle SPI interrupt request.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_IRQHandler(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_IRQHandler(void)
#endif /* (__ICCARM__) */
{
  /* Check that SPI Rx buffer not empty interrupt has been raised */
  if (((SPIx->SR & SPI_SR_OVR) == RESET)
      && ((SPIx->SR & SPI_SR_RXNE) != RESET)
      && ((SPIx->CR2 & SPI_CR2_RXNEIE) != RESET))
  {
    if (BusyState == 0U)
    {
      /* Set the RX not empty token */
      SpiRxNotEmpty = 1U;

      /* Disable the interrupt of Rx not empty buffer */
      SPIx->CR2 &= ~ SPI_CR2_RXNEIE;
    }
    else
    {
      /* Set the RX not empty token */
      SpiRxNotEmpty = 1U;

      /* Disable the interrupt of Rx not empty buffer */
      SPIx->CR2 &= ~ SPI_CR2_RXNEIE;

      /* Send busy byte */
      OPENBL_SPI_SendBusyByte();
    }
  }

  while (((SPIx->SR & SPI_SR_OVR) != RESET)
         && ((SPIx->SR & SPI_SR_RXNE) != RESET)
         && ((SPIx->CR2 & SPI_CR2_RXNEIE) != RESET))
  {
    /* Clear underrun flag */
    OPENBL_SPI_ClearFlag_OVR();
  }
}

/**
  * @brief  This function enables the send of busy state.
  * @retval None.
  */
void OPENBL_SPI_EnableBusyState(void)
{
  BusyState = 1U;
}

/**
  * @brief  This function disables the send of busy state.
  * @retval None.
  */
void OPENBL_SPI_DisableBusyState(void)
{
  BusyState = 0U;
}

/**
  * @brief  Clear overrun error flag
  * @note   Clearing this flag is done by a read access to the SPIx_DR
  *         register followed by a read access to the SPIx_SR register
  * @retval None
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_SPI_ClearFlag_OVR(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_SPI_ClearFlag_OVR(void)
#endif /* (__ICCARM__) */
{
  __IO uint32_t tmpreg;

  tmpreg = SPIx->DR;
  (void) tmpreg;
  tmpreg = SPIx->SR;
  (void) tmpreg;
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  special_cmd Pointer to the @arg OPENBL_SpecialCmdTypeDef structure.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
void OPENBL_SPI_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *special_cmd)
{
  switch (special_cmd->OpCode)
  {
    /* Unknown command opcode */
    default:
      if (special_cmd->CmdType == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_SPI_SendByte(0x00U);
        OPENBL_SPI_SendByte(0x00U);

        /* Send NULL status size */
        OPENBL_SPI_SendByte(0x00U);
        OPENBL_SPI_SendByte(0x00U);
      }
      else if (special_cmd->CmdType == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL status size */
        OPENBL_SPI_SendByte(0x00U);
        OPENBL_SPI_SendByte(0x00U);
      }
      break;
  }
}
