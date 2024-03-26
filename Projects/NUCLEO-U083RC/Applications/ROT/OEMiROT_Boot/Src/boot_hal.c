/**
  ******************************************************************************
  * @file    boot_hal.c
  * @author  MCD Application Team
  * @brief   This file contains  mcuboot stm32u0xx hardware specific implementation
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
#include "boot_hal.h"
#include "boot_hal_hash_ref.h"
#include "boot_hal_imagevalid.h"
#include "boot_hal_flowcontrol.h"
#include "mcuboot_config/mcuboot_config.h"
#include "uart_stdout.h"
#include "low_level_security.h"
#include "bootutil/boot_record.h"
#include "target_cfg.h"
#include "Driver_Flash.h"
#include "region_defs.h"
#include "low_level_rng.h"
#include "bootutil_priv.h"

/* exported variables --------------------------------------------------------*/
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define JUMPER_ADDRESS 0x1FFF6000UL
#define MAGIC_NUMBER 0x08192A3C

#ifdef MCUBOOT_EXT_LOADER
#define BUTTON_PORT                       GPIOC
#define BUTTON_CLK_ENABLE                 __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_PIN                        GPIO_PIN_13
#endif /* MCUBOOT_EXT_LOADER */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
/* Global variables to memorize images validation status */
#if (MCUBOOT_IMAGE_NUMBER == 1)
uint8_t ImageValidStatus[MCUBOOT_IMAGE_NUMBER] = {IMAGE_INVALID};
#elif (MCUBOOT_IMAGE_NUMBER == 2)
uint8_t ImageValidStatus[MCUBOOT_IMAGE_NUMBER] = {IMAGE_INVALID, IMAGE_INVALID};
#else
#error "MCUBOOT_IMAGE_NUMBER not supported"
#endif /* (MCUBOOT_IMAGE_NUMBER == 1)  */
uint8_t ImageValidIndex = 0;
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF) || defined(MCUBOOT_USE_HASH_REF)
uint8_t ImageValidEnable = 0;
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF || MCUBOOT_USE_HASH_REF */

#if defined(MCUBOOT_USE_HASH_REF)
#define BL2_HASH_REF_ADDR     (FLASH_HASH_REF_AREA_OFFSET)
uint8_t ImageValidHashUpdate = 0;
uint8_t ImageValidHashRef[MCUBOOT_IMAGE_NUMBER * SHA256_LEN] = {0};
#endif /* MCUBOOT_USE_HASH_REF */

#if defined(FLOW_CONTROL)
/* Global variable for Flow Control state */
volatile uint32_t uFlowProtectValue = FLOW_CTRL_INIT_VALUE;
#endif /* FLOW_CONTROL */
volatile uint32_t uFlowStage = FLOW_STAGE_CFG;

/* Private function prototypes -----------------------------------------------*/
#if defined(MCUBOOT_EXT_LOADER)
static void execute_loader(void) __NO_RETURN;
#endif /* MCUBOOT_EXT_LOADER */
static void SystemClock_Config(void);
void boot_clean_ram_area(void);
static void jump_to_application(uint32_t bootloader_address, uint32_t magic, uint32_t application_vector_address, uint8_t mpu_region_number);

#if defined(MCUBOOT_EXT_LOADER)
/**
  * @brief This function manage the jump to Local loader application.
  * @note
  * @retval void
  */
void boot_platform_noimage(void)
{
    /* Check Flow control */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_2);

    /* Jumper to loader */
    execute_loader();
}

/**
  * @brief This function activates the protection before executing local loader.
  * @note
  * @retval void
  */
void execute_loader(void)
{
    /* Check Flow control */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_3);

    /* Update run time protections for loader execution */
    LL_SECU_UpdateLoaderRunTimeProtections();

    /* Check Flow control */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_4);

    /* Second function call to resist to basic hardware attacks */
    LL_SECU_UpdateLoaderRunTimeProtections();

    /* Check Flow control */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_5);

    jump_to_application(JUMPER_ADDRESS, MAGIC_NUMBER, LOADER_CODE_START, MPU_REGION_NUMBER5);

    /* Avoid compiler to pop registers after having changed MSP */
#if !defined(__ICCARM__)
    __builtin_unreachable();
#else
    while (1);
#endif /* defined(__ICCARM__) */
}
#endif /* MCUBOOT_EXT_LOADER */

/**
  * @brief This function manage the jump to secure application.
  * @note
  * @retval void
  */
void boot_platform_quit(struct boot_arm_vector_table *vector)
{
#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
    uint32_t image_index;

    (void)fih_delay();
    /* Check again if images have been validated, to resist to basic hw attacks */
    for (image_index = 0; image_index < MCUBOOT_IMAGE_NUMBER; image_index++)
    {
        if (ImageValidStatus[image_index] != IMAGE_VALID)
        {
            BOOT_LOG_ERR("Error while double controlling images validation");
            Error_Handler();
        }
    }
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

#if defined(MCUBOOT_USE_HASH_REF)
    /* Store new hash references in flash for next boot */
    if (ImageValidHashUpdate)
    {
        if (boot_hash_ref_store())
        {
            BOOT_LOG_ERR("Error while storing hash references");
            Error_Handler();
        }
    }
#endif /* MCUBOOT_USE_HASH_REF */

    /* Check Flow control state */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_2);

    RNG_DeInit();

    jump_to_application(JUMPER_ADDRESS, MAGIC_NUMBER, (uint32_t)vector, MPU_REGION_NUMBER5);

    /* Avoid compiler to pop registers after having changed MSP */
#if !defined(__ICCARM__)
    __builtin_unreachable();
#else
    while (1);
#endif /* defined(__ICCARM__) */
}

/**
  * @brief This function is called to clear all RAM area before jumping in
  * in application.
  * @note
  * @retval void
  */
void boot_clean_ram_area(void)
{
    __IO uint32_t *pt = (uint32_t *)SRAM1_BASE;
    uint32_t index;

    /* clean all SRAM1 and BKPSRAM2 */
    for (index = 0; index < ((_SRAM1_SIZE_MAX + _SRAM2_SIZE_MAX) / 4); index++)
    {
        pt[index] = 0;
    }
}

#if defined(MCUBOOT_USE_HASH_REF)
/**
  * @brief This function store all hash references in flash
  * @return 0 on success; nonzero on failure.
  */
int boot_hash_ref_store(void)
{
  /* Erase hash references flash sector */
  if (FLASH_DEV_NAME.EraseSector(BL2_HASH_REF_ADDR) != ARM_DRIVER_OK)
  {
    return BOOT_EFLASH;
  }

  /* Store new hash references in flash */
  if (FLASH_DEV_NAME.ProgramData(BL2_HASH_REF_ADDR, ImageValidHashRef,
      (SHA256_LEN * MCUBOOT_IMAGE_NUMBER)) != ARM_DRIVER_OK)
  {
    return BOOT_EFLASH;
  }

  return 0;
}

/**
  * @brief This function load all hash references from flash
  * @return 0 on success; nonzero on failure.
  */
int boot_hash_ref_load(void)
{
  /* Read hash references */
  if (FLASH_DEV_NAME.ReadData(BL2_HASH_REF_ADDR, ImageValidHashRef,
      (SHA256_LEN * MCUBOOT_IMAGE_NUMBER)) != ARM_DRIVER_OK)
  {
    return BOOT_EFLASH;
  }

  return 0;
}

/**
  * @brief This function set one hash reference in ram
  * @param hash_ref hash reference to update
  * @param size size of the hash references
  * @param image_index index of image corresponding to hash reference
  * @return 0 on success; nonzero on failure.
  */
int boot_hash_ref_set(uint8_t *hash_ref, uint8_t size, uint8_t image_index)
{
  /* Check size */
  if (size != SHA256_LEN)
  {
    return BOOT_EFLASH;
  }

  /* Check image index */
  if (image_index >= MCUBOOT_IMAGE_NUMBER)
  {
    return BOOT_EFLASH;
  }

  /* Set hash reference */
  memcpy(ImageValidHashRef + (image_index * SHA256_LEN), hash_ref, SHA256_LEN);

  /* Memorize that hash references will have to be updated in flash (later) */
  ImageValidHashUpdate++;

  return 0;
}

/**
  * @brief This function get one hash reference from ram
  * @param hash_ref hash reference to get
  * @param size size of the hash reference
  * @param image_index index of image corresponding to hash reference
  * @return 0 on success; nonzero on failure.
  */
int boot_hash_ref_get(uint8_t *hash_ref, uint8_t size, uint8_t image_index)
{
  /* Check size */
  if (size != SHA256_LEN)
  {
    return BOOT_EFLASH;
  }

  /* Check image index */
  if (image_index >= MCUBOOT_IMAGE_NUMBER)
  {
    return BOOT_EFLASH;
  }

  /* Get hash reference */
  memcpy(hash_ref, ImageValidHashRef + (image_index * SHA256_LEN), SHA256_LEN);

  return 0;
}
#endif /* MCUBOOT_USE_HASH_REF */

/**
  * @brief  Platform init
  * @param  None
  * @retval status
  */
int32_t boot_platform_init(void)
{
#if defined(MCUBOOT_EXT_LOADER)
    GPIO_InitTypeDef GPIO_Init;
#endif /* MCUBOOT_EXT_LOADER */
    /* STM32U0xx HAL library initialization:
         - Systick timer is configured by default as source of time base, but user
               can eventually implement his proper time base source (a general purpose
               timer for example or other time source), keeping in mind that Time base
               duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
               handled in milliseconds basis.
         - Set NVIC Group Priority to 3
         - Low Level Initialization
       */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

#ifdef OEMIROT_DEV_MODE
    /* Init for log */
    stdio_init();
#endif /*  OEMIROT_DEV_MODE */

    /* Start HW randomization */
    RNG_Init();

    (void)fih_delay_init();

    /* Apply Run time Protection */
    LL_SECU_ApplyRunTimeProtections();
    /* Check static protections */
    LL_SECU_CheckStaticProtections();

    /* Check Flow control state */
    FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_1);
    uFlowStage = FLOW_STAGE_CHK;
    /* Double protections apply / check to resist to basic fault injections */
    /* Apply Run time Protection */
   (void)fih_delay();
    LL_SECU_ApplyRunTimeProtections();
    /* Check static protections */
    LL_SECU_CheckStaticProtections();

    if (FLASH_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
    {
        BOOT_LOG_ERR("Error while initializing Flash Interface");
        Error_Handler();
    }

#if defined(MCUBOOT_USE_HASH_REF)
    /* Load all images hash references (for mcuboot) */
    if (boot_hash_ref_load())
    {
        BOOT_LOG_ERR("Error while loading Hash references from FLash");
        Error_Handler();
    }
#endif

#if defined(MCUBOOT_EXT_LOADER)
    /* configure Button pin */
    BUTTON_CLK_ENABLE;
    GPIO_Init.Pin       = BUTTON_PIN;
    GPIO_Init.Mode      = 0;
    GPIO_Init.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Pull      = GPIO_PULLUP;
    GPIO_Init.Alternate = 0;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_Init);
    /* read pin value */
    if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET)
    {
        boot_platform_noimage();
    }
#endif /* MCUBOOT_EXT_LOADER */
    return 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.MSICalibrationValue = 4/*RCC_MSICALIBRATION_DEFAULT*/;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11; /* MSI = 48MHz*/
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2; /* 24MHz */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1
     clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    /* HAL RCC configuration error */
    Error_Handler();
  }
}

__attribute__((naked)) void jump_to_application(uint32_t bootloader_address, uint32_t magic, uint32_t application_vector_address, uint8_t mpu_region_number)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                    \n"
#endif /* !defined(__ICCARM__) */
        "MOV     R4, R0                     \n" /* Save arguments */
        "MOV     R5, R1                     \n"
        "MOV     R6, R2                     \n"
        "MOV     R7, R3                     \n"
        "BL      boot_clean_ram_area        \n" /* Clear RAM before jump */
        "MOVS    R0, #0                     \n" /* Clear registers: R0-R12, LR */
        "MOV     R1, R0                     \n" /* except R4 to R7 */
        "MOV     R2, R0                     \n"
        "MOV     R3, R0                     \n"
        "MOV     R8, R0                     \n"
        "MOV     R9, R0                     \n"
        "MOV     R10, R0                    \n"
        "MOV     R11, R0                    \n"
        "MOV     R12, R0                    \n"
        "MOV     LR, R0                     \n"

        "MOV     R1, R5                     \n"
        "MOV     R2, R6                     \n"
        "MOV     R3, R7                     \n"
        "LDR     R0, [R4]                   \n"
        "MOV     SP, R0                     \n"
        "LDR     R0, [R4,#4]                \n"
        "BX      R0                         \n" /* Jump to Reset_handler */
    );
}

/**
  * @brief  This function is executed in case of error occurrence.
  *         This function does not return.
  * @param  None
  * @retval None
  */
#ifdef OEMIROT_ERROR_HANDLER_STOP_EXEC
void Error_Handler(void)
{
    while(1);
}
#else /* OEMIROT_ERROR_HANDLER_STOP_EXEC */
#if defined(__ICCARM__)
__NO_RETURN void Error_Handler(void)
#else
void Error_Handler(void)
#endif
{
    /* It is customizeable */
    NVIC_SystemReset();
#if !defined(__ICCARM__)
    /* Avoid bx lr instruction (for fault injection) */
    __builtin_unreachable();
#endif /* defined(__ICCARM__) */
}
#endif /* OEMIROT_ERROR_HANDLER_STOP_EXEC */

#if defined(__ARMCC_VERSION)
/* reimplement the function to reach Error Handler */
void __aeabi_assert(const char *expr, const char *file, int line)
{
#ifdef OEMIROT_DEV_MODE
    printf("assertion \" %s \" failed: file %s %d\n", expr, file, line);
#endif /*  OEMIROT_DEV_MODE  */
    Error_Handler();
}
#endif  /*  __ARMCC_VERSION */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    Error_Handler();
}
#endif /* USE_FULL_ASSERT */
