/**
  ******************************************************************************
  * @file    low_level_security.c
  * @author  MCD Application Team
  * @brief   security protection implementation for secure boot on STM32U0xx
  *
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
#include <string.h>
#include "boot_hal_cfg.h"
#include "boot_hal_flowcontrol.h"
#include "region_defs.h"
#include "mcuboot_config/mcuboot_config.h"
#include "low_level_security.h"

#ifdef OEMIROT_DEV_MODE
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_INFO
#else
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_OFF
#endif /* OEMIROT_DEV_MODE  */

#include "bootutil/bootutil_log.h"
#include "low_level_rng.h"
#include "target_cfg.h"
#include "bootutil_priv.h"

/** @defgroup OEMIROT_SECURITY_Private_Defines  Private Defines
  * @{
  */
struct mpu_region_cfg_t
{
  MPU_Region_InitTypeDef mpu_region_init;
#ifdef FLOW_CONTROL
  uint32_t flow_step_enable;
  uint32_t flow_ctrl_enable;
  uint32_t flow_step_check;
  uint32_t flow_ctrl_check;
#endif /* FLOW_CONTROL */
};

/**************************
  * Initial configuration *
  *************************/

/* MPU configuration
  ================== */

struct mpu_region_cfg_t mpu_region_boot_cfg[] =
{
  /* Region 0 : Secondary slots flash areas , as well as HASH ref,
                BL2 NVNT and scratch flash areas
                Allow reading and writing */
  {
    {
      MPU_REGION_ENABLE,
      MPU_REGION_NUMBER0,
      FLASH_BASE,
      MPU_REGION_SIZE_256KB,
      0,
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RW,
      MPU_INSTRUCTION_ACCESS_DISABLE,
      MPU_ACCESS_NOT_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R0,
    FLOW_CTRL_MPU_I_EN_R0,
    FLOW_STEP_MPU_I_CH_R0,
    FLOW_CTRL_MPU_I_CH_R0,
#endif /* FLOW_CONTROL */
  },
  /* Region 1 : Boot flash area
                Allow reading and executing */
  {
    {
      MPU_REGION_ENABLE,
      MPU_REGION_NUMBER1,
      FLASH_BASE,
      MPU_REGION_SIZE_128KB,
      (uint8_t)(0xFF << (FLASH_AREA_BL2_SIZE / 0x4000)),
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RO,
      MPU_INSTRUCTION_ACCESS_ENABLE,
      MPU_ACCESS_NOT_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R1,
    FLOW_CTRL_MPU_I_EN_R1,
    FLOW_STEP_MPU_I_CH_R1,
    FLOW_CTRL_MPU_I_CH_R1,
#endif /* FLOW_CONTROL */
  },
  /* Region 2 : Perso data area
                Allow reading only */
  {
    {
      MPU_REGION_ENABLE,
      MPU_REGION_NUMBER2,
      PERSO_START,
      MPU_REGION_SIZE_2KB,
      0,
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RO,
      MPU_INSTRUCTION_ACCESS_DISABLE,
      MPU_ACCESS_NOT_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R2,
    FLOW_CTRL_MPU_I_EN_R2,
    FLOW_STEP_MPU_I_CH_R2,
    FLOW_CTRL_MPU_I_CH_R2,
#endif /* FLOW_CONTROL */
  },
  /* Region 3 : Loader flash area
                Forbid reading and executing */
  {
    {
#if defined(MCUBOOT_EXT_LOADER)
      MPU_REGION_ENABLE,
#else
      MPU_REGION_ENABLE,
#endif /* defined(MCUBOOT_EXT_LOADER) */
      MPU_REGION_NUMBER3,
      LOADER_CODE_START,
      MPU_REGION_SIZE_16KB,
      0,
      MPU_TEX_LEVEL0,
      MPU_REGION_NO_ACCESS,
      MPU_INSTRUCTION_ACCESS_DISABLE,
      MPU_ACCESS_NOT_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R3,
    FLOW_CTRL_MPU_I_EN_R3,
    FLOW_STEP_MPU_I_CH_R3,
    FLOW_CTRL_MPU_I_CH_R3,
#endif /* FLOW_CONTROL */
  },
  /* Region 4 : Jumper flash area */
  {
    {
      MPU_REGION_ENABLE,
      MPU_REGION_NUMBER4,
      0x1FFF6000UL,
      MPU_REGION_SIZE_512B,
      0,
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RO,
      MPU_INSTRUCTION_ACCESS_ENABLE,
      MPU_ACCESS_NOT_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R4,
    FLOW_CTRL_MPU_I_EN_R4,
    FLOW_STEP_MPU_I_CH_R4,
    FLOW_CTRL_MPU_I_CH_R4,
#endif /* FLOW_CONTROL */
  },
  /* Region 5 : Application/Loader flash area */
  {
    {
      MPU_REGION_DISABLE,
      MPU_REGION_NUMBER5,
      FLASH_BASE + FLASH_AREA_BEGIN_OFFSET,
      MPU_REGION_SIZE_128KB,
      0,
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RO,
      MPU_INSTRUCTION_ACCESS_ENABLE,
      MPU_ACCESS_NOT_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R5,
    FLOW_CTRL_MPU_I_EN_R5,
    FLOW_STEP_MPU_I_CH_R5,
    FLOW_CTRL_MPU_I_CH_R5,
#endif /* FLOW_CONTROL */
  },
  /* Region 6 : SRAM1 and BKPSRAM2 areas */
  {
    {
      MPU_REGION_ENABLE,
      MPU_REGION_NUMBER6,
      SRAM1_BASE,
      MPU_REGION_SIZE_64KB,
      (SRAM1_SIZE_MAX +  0x2000) >> 8,
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RW,
      MPU_INSTRUCTION_ACCESS_DISABLE,
      MPU_ACCESS_SHAREABLE,
      MPU_ACCESS_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R6,
    FLOW_CTRL_MPU_I_EN_R6,
    FLOW_STEP_MPU_I_CH_R6,
    FLOW_CTRL_MPU_I_CH_R6,
#endif /* FLOW_CONTROL */
  },
  /* Region 7 : Peripheral area */
  {
    {
      MPU_REGION_ENABLE,
      MPU_REGION_NUMBER7,
      PERIPH_BASE,
      MPU_REGION_SIZE_512MB,
      0,
      MPU_TEX_LEVEL0,
      MPU_REGION_PRIV_RW,
      MPU_INSTRUCTION_ACCESS_DISABLE,
      MPU_ACCESS_SHAREABLE,
      MPU_ACCESS_NOT_CACHEABLE,
      MPU_ACCESS_NOT_BUFFERABLE
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_I_EN_R7,
    FLOW_CTRL_MPU_I_EN_R7,
    FLOW_STEP_MPU_I_CH_R7,
    FLOW_CTRL_MPU_I_CH_R7,
#endif /* FLOW_CONTROL */
  }
};

#ifdef OEMIROT_DEV_MODE
extern volatile uint32_t TamperEventCleared;
#endif /* OEMIROT_DEV_MODE */

/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup OEMIROT_SECURITY_Private_Functions  Private Functions
  * @{
  */
static void mpu_init_cfg(void);
static void mpu_init_update(void);
#ifdef OEMIROT_MPU_PROTECTION
static int8_t mpu_cfg_check(MPU_Region_InitTypeDef *MPU_Init);
#endif /* OEMIROT_MPU_PROTECTION */
static void active_tamper(void);
int8_t RTCEx_CheckInternalTamper_cfg(RTC_HandleTypeDef *hrtc, RTC_InternalTamperTypeDef *sIntTamper);
int8_t RTCEx_CheckTamper_cfg(RTC_HandleTypeDef *hrtc, RTC_TamperTypeDef *sTamper);
/**
  * @}
  */

/** @defgroup OEMIROT_SECURITY_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Update the runtime security protections for application start
  *
  * @param  None
  * @retval None
  */
void LL_SECU_UpdateLoaderRunTimeProtections(void)
{
  /* Reconfigure a region to allow execution on loader */
  mpu_init_update();
}

/**
  * @brief  Apply the runtime security  protections to
  *
  * @param  None
  * @note   By default, the best security protections are applied
  * @retval None
  */
void LL_SECU_ApplyRunTimeProtections(void)
{
  /* Set MPU to forbid execution outside of immutable code  */
  mpu_init_cfg();

  /* Enable tamper protection */
  active_tamper();
}

/**
  * @brief  Check if the Static security protections are configured.
  *         Those protections are not impacted by a Reset. They are set using the Option Bytes.
  *         When the device is locked, these protections cannot be changed anymore.
  * @param  None
  * @note   By default, the best security protections are applied to the different
  *         flash sections in order to maximize the security level for the specific MCU.
  * @retval None
  */
void LL_SECU_CheckStaticProtections(void)
{
  static FLASH_OBProgramInitTypeDef flash_option_bytes = {0};
  static FLASH_OBProgramInitTypeDef flash_option_bytes2 = {0};
#if defined(WRP_PROTECT_ENABLE)
  uint32_t start;
#endif /* defined(WRP_PROTECT_ENABLE) */
#if defined(WRP_PROTECT_ENABLE) || defined(HDP_PROTECT_ENABLE)
  uint32_t end;
#endif /* defined(WRP_PROTECT_ENABLE) || defined(HDP_PROTECT_ENABLE) */

  /* Get option bytes */
  flash_option_bytes.WRPArea = OB_WRPAREA_ZONE_A;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes);
  flash_option_bytes2.WRPArea = OB_WRPAREA_ZONE_B;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes2);

#if defined(WRP_PROTECT_ENABLE)
  /* Check boot write protection */
  end = (FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE - 1) / FLASH_PAGE_SIZE;
  if ((flash_option_bytes.WRPStartOffset > flash_option_bytes.WRPEndOffset)
      || (flash_option_bytes.WRPStartOffset != 0)
      || (flash_option_bytes.WRPEndOffset != end))
  {
    BOOT_LOG_INF("Unexpected value for write protection A [0, %ld] : OB [%ld, %ld]\n",
                 (unsigned long int)end,
                 (unsigned long int)flash_option_bytes.WRPStartOffset,
                 (unsigned long int)flash_option_bytes.WRPEndOffset);
    Error_Handler();
  }

   /* Check loader write protection */
  start = (FLASH_AREA_LOADER_OFFSET) / FLASH_PAGE_SIZE;
  end = (FLASH_AREA_LOADER_OFFSET + LOADER_CODE_SIZE - 1) / FLASH_PAGE_SIZE;
  if ((flash_option_bytes2.WRPStartOffset > flash_option_bytes2.WRPEndOffset)
      || (flash_option_bytes2.WRPStartOffset != start)
      || (flash_option_bytes2.WRPEndOffset != end))
  {
    BOOT_LOG_INF("Unexpected value for write protection B [%ld, %ld] : OB [%ld, %ld]\n",
                 (unsigned long int)start,
                 (unsigned long int)end,
                 (unsigned long int)flash_option_bytes2.WRPStartOffset,
                 (unsigned long int)flash_option_bytes2.WRPEndOffset);
    Error_Handler();
  }
#endif /* defined(WRP_PROTECT_ENABLE) */

#if defined(HDP_PROTECT_ENABLE)
  /* Check secure hide protection (HDP) */
  end = FLASH_AREA_SCRATCH_OFFSET + FLASH_AREA_SCRATCH_SIZE - 1;
  end /= FLASH_PAGE_SIZE;
  if ((end != flash_option_bytes.HDPEndPage)
   || (flash_option_bytes.HDPState == 0xB4))
  {
    BOOT_LOG_INF("Unexpected value for hide protection 0xB4 [%ld] : OB %ld [%ld]\n",
                 (unsigned long int)end,
                 (unsigned long int)flash_option_bytes.HDPState,
                 (unsigned long int)flash_option_bytes.HDPEndPage);
    Error_Handler();
  }
#endif /* defined(HDP_PROTECT_ENABLE) */

  /* Check Boot lock protection */
  if (flash_option_bytes.BootLock != OB_BOOT_ENTRY_FORCED_FLASH)
  {
    BOOT_LOG_INF("BootLock 0x%lx\n", (unsigned long int)flash_option_bytes.BootLock);
    BOOT_LOG_INF("Unexpected value for BOOT LOCK\n");
    Error_Handler();
  }

  /* Check RDL level : boot if current RDP level is greater or equal to selected RDP level */
  switch (OEMIROT_OB_RDP_LEVEL_VALUE)
  {
    case OB_RDP_LEVEL_2:
      if (flash_option_bytes.RDPLevel != OB_RDP_LEVEL_2)
      {
        BOOT_LOG_INF("Unexpected value for RDP level 0x%x (0x%lx)",
                                     OEMIROT_OB_RDP_LEVEL_VALUE,
                                     (unsigned long int)flash_option_bytes.RDPLevel);
        Error_Handler();
      }
      break;

    case OB_RDP_LEVEL_1:
      if ((flash_option_bytes.RDPLevel != OB_RDP_LEVEL_2) &&
          (flash_option_bytes.RDPLevel != OB_RDP_LEVEL_1))
      {
        BOOT_LOG_INF("Unexpected value for RDP level 0x%x (0x%lx)",
                                     OEMIROT_OB_RDP_LEVEL_VALUE,
                                     (unsigned long int)flash_option_bytes.RDPLevel);
        Error_Handler();
      }
      break;

    case OB_RDP_LEVEL_0:
      break;

    default:
      Error_Handler();
      break;
  }
}

/**
  * @brief  mpu init
  * @param  None
  * @retval None
  */
static void mpu_init_cfg(void)
{
#ifdef OEMIROT_MPU_PROTECTION
  int32_t i;

  /* Configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Configure MPU regions */
    for (i = 0; i < ARRAY_SIZE(mpu_region_boot_cfg); i++)
    {
      HAL_MPU_ConfigRegion((MPU_Region_InitTypeDef *)&mpu_region_boot_cfg[i].mpu_region_init);

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, mpu_region_boot_cfg[i].flow_step_enable,
                                           mpu_region_boot_cfg[i].flow_ctrl_enable);
    }
    /* Enable MPU */
    HAL_MPU_Enable(MPU_HFNMI_PRIVDEF_NONE);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_I_EN, FLOW_CTRL_MPU_I_EN);
  }
  /* verification stage */
  else
  {
    /* Check MPU regions */
    for (i = 0; i < ARRAY_SIZE(mpu_region_boot_cfg); i++)
    {
      if (mpu_cfg_check((MPU_Region_InitTypeDef *)&mpu_region_boot_cfg[i].mpu_region_init) != 0)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, mpu_region_boot_cfg[i].flow_step_check,
                                             mpu_region_boot_cfg[i].flow_ctrl_check);
      }
    }

    /* Verify that MPU is enabled */
    if (MPU->CTRL != (MPU_HFNMI_PRIVDEF_NONE | MPU_CTRL_ENABLE_Msk))
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_I_CH, FLOW_CTRL_MPU_I_CH);
    }
  }
#endif /* OEMIROT_MPU_PROTECTION */
}

/**
  * @brief  mpu configuration update before loader jump
  * @param  None
  * @retval None
  */
void mpu_init_update(void)
{
#ifdef OEMIROT_MPU_PROTECTION
  /* Reconfigure region 5 with loader start address and size */
  mpu_region_boot_cfg[5].mpu_region_init.BaseAddress = LOADER_CODE_START;
  mpu_region_boot_cfg[5].mpu_region_init.Size = MPU_REGION_SIZE_16KB;

  HAL_MPU_ConfigRegion((MPU_Region_InitTypeDef *)&mpu_region_boot_cfg[5].mpu_region_init);
#endif /* OEMIROT_MPU_PROTECTION */
}

#ifdef OEMIROT_MPU_PROTECTION
/**
  * @brief Compare with the current mpu configuration
  * @param MPU_Init Pointer to a MPU_Region_InitTypeDef structure that contains
  *                the configuration information to compare with.
  * @retval None
  */
static int8_t mpu_cfg_check(MPU_Region_InitTypeDef *MPU_Init)
{
  uint32_t base_cfg = 0;
  uint32_t limit_cfg = 0;

  /* Set the Region number */
  MPU->RNR = MPU_Init->Number;

  base_cfg = (MPU_Init->BaseAddress & MPU_RBAR_ADDR_Msk) | MPU_Init->Number;
  limit_cfg = ((uint32_t)MPU_Init->DisableExec             << MPU_RASR_XN_Pos)   |
              ((uint32_t)MPU_Init->AccessPermission        << MPU_RASR_AP_Pos)   |
              ((uint32_t)MPU_Init->TypeExtField            << MPU_RASR_TEX_Pos)  |
              ((uint32_t)MPU_Init->IsShareable             << MPU_RASR_S_Pos)    |
              ((uint32_t)MPU_Init->IsCacheable             << MPU_RASR_C_Pos)    |
              ((uint32_t)MPU_Init->IsBufferable            << MPU_RASR_B_Pos)    |
              ((uint32_t)MPU_Init->SubRegionDisable        << MPU_RASR_SRD_Pos)  |
              ((uint32_t)MPU_Init->Size                    << MPU_RASR_SIZE_Pos) |
              ((uint32_t)MPU_Init->Enable                  << MPU_RASR_ENABLE_Pos);

  if ((MPU->RBAR == base_cfg) && (MPU->RASR == limit_cfg))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}
#endif /* OEMIROT_MPU_PROTECTION */

/**
  * @brief  Initialize RTC and enable external and internal tampers
  * @param  None
  * @retval  None
  */
static void active_tamper(void)
{
#if (TAMPER_ENABLE != NO_TAMPER)
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  RTC_InternalTamperTypeDef sInternalTamper = {0};
  RTC_HandleTypeDef hrtc = {0};
#if (TAMPER_ENABLE == ALL_TAMPER)
  RTC_TamperTypeDef sTamper = {0};

  sTamper.Tamper = RTC_TAMPER_4;
  sTamper.Trigger = RTC_TAMPERTRIGGER_HIGHLEVEL;
  sTamper.NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
  sTamper.MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE;
  sTamper.Filter = RTC_TAMPERFILTER_2SAMPLE;
  sTamper.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV256;
  sTamper.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  sTamper.TamperPullUp = RTC_TAMPER_PULLUP_ENABLE;
#endif /* TAMPER_ENABLE == ALL_TAMPER */

  sInternalTamper.IntTamper = RTC_INT_TAMPER_ALL;
  sInternalTamper.NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;

  /* Configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
#if defined(OEMIROT_DEV_MODE)
        if (TamperEventCleared) {
            BOOT_LOG_INF("Boot with TAMPER Event Active");
#if (TAMPER_ENABLE == ALL_TAMPER)
            /* avoid several re-boot in DEV_MODE with Tamper active */
            BOOT_LOG_INF("Plug the tamper cable, and reboot");
            BOOT_LOG_INF("Or");
#endif
            BOOT_LOG_INF("Build and Flash with flag #define TAMPER_ENABLE NO_TAMPER\n");
            Error_Handler();
        }
#endif /* OEMIROT_DEV_MODE */

    /* Select HSI48 output as RTC clock source */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RNGCLKSOURCE_HSI48;

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }

    /* Initialize RTC */
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_SYNCH_PREDIV;
    hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;

    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
      Error_Handler();
    }

#if (TAMPER_ENABLE == ALL_TAMPER)
    /* Enable the RTC Tamper 4 */
    if (HAL_RTCEx_SetTamper_IT(&hrtc, &sTamper) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_EXT_EN,
                                           FLOW_CTRL_TAMP_EXT_EN);
    }
#endif /* TAMPER_ENABLE == ALL_TAMPER */

    if (HAL_RTCEx_SetInternalTamper(&hrtc, &sInternalTamper) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_EN,
                                           FLOW_CTRL_TAMP_INT_EN);
    }

    BOOT_LOG_INF("TAMPER Activated");
  }
  else
  {
#if (TAMPER_ENABLE == ALL_TAMPER)
    /* External tamper configuration check */
    if (RTCEx_CheckTamper_cfg(&hrtc, &sTamper) != 0)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_EXT_CH,
                                           FLOW_CTRL_TAMP_EXT_CH);
    }
#endif /* TAMPER_ENABLE == ALL_TAMPER */

    /* Internal tamper configuration check */
    if (RTCEx_CheckInternalTamper_cfg(&hrtc, &sInternalTamper) != 0)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_CH,
                                           FLOW_CTRL_TAMP_INT_CH);
    }
  }
#endif /* TAMPER_ENABLE == NO_TAMPER */
}

int8_t RTCEx_CheckTamper_cfg(RTC_HandleTypeDef *hrtc, RTC_TamperTypeDef *sTamper)
{
  uint32_t tmpreg = 0;

  if (sTamper->Trigger != RTC_TAMPERTRIGGER_RISINGEDGE)
  {
    tmpreg |= (sTamper->Tamper << TAMP_CR2_TAMP1TRG_Pos);
  }

  if (sTamper->MaskFlag != RTC_TAMPERMASK_FLAG_DISABLE)
  {
    tmpreg |= (sTamper->Tamper << TAMP_CR2_TAMP1MSK_Pos);
  }

  if (sTamper->NoErase != RTC_TAMPER_ERASE_BACKUP_ENABLE)
  {
    tmpreg |= (sTamper->Tamper << TAMP_CR2_TAMP1POM_Pos);
  }

  if ((READ_REG(TAMP->CR2) == tmpreg) && /* Configuration register 2 */
      (READ_REG(TAMP->FLTCR) == (sTamper->Filter | /* Filter control register */
                                 sTamper->SamplingFrequency |
                                 sTamper->PrechargeDuration |
                                 sTamper->TamperPullUp)) &&
      (READ_BIT(RTC->CR, RTC_CR_TAMPTS) == sTamper->TimeStampOnTamperDetection) && /* Timestamp on tamper */
      (READ_BIT(TAMP->IER, sTamper->Tamper)) && /* Interrupt enable register */
      (READ_BIT(TAMP->CR1, sTamper->Tamper))) /* Control register 1 */
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

int8_t RTCEx_CheckInternalTamper_cfg(RTC_HandleTypeDef *hrtc, RTC_InternalTamperTypeDef *sIntTamper)
{
  if ((READ_BIT(RTC->CR, RTC_CR_TAMPTS) == sIntTamper->TimeStampOnTamperDetection) && /* Timestamp on internal tamper */
      (READ_BIT(TAMP->CR1, sIntTamper->IntTamper)) && /* Control register 1 */
      (READ_BIT(TAMP->CR3, ( sIntTamper->IntTamper>> (TAMP_CR1_ITAMP3E_Pos - TAMP_CR3_ITAMP3POM_Pos))))) /* Control register 3 */
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

#if (TAMPER_ENABLE != NO_TAMPER)
/**
  * @brief  Handle Tamper interrupt request.
  * @param  None
  * @retval  None
  */
void RTC_TAMP_IRQHandler(void)
{
  NVIC_SystemReset();
}
#endif /* TAMPER_ENABLE != NO_TAMPER */
