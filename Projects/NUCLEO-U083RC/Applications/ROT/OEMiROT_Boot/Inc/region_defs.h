/*
 * Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__
#include "flash_layout.h"

#define BL2_HEAP_SIZE        0x00005F0
#define BL2_MSP_STACK_SIZE   0x0001100

#define _SRAM1_SIZE_MAX         (0x8000)  /*!< SRAM1=32 KB */
#define _SRAM2_SIZE_MAX         (0x2000)  /*!< SRAM2=8 KB */

/* Flash and internal SRAMs base addresses - Non secure aliased */
#define _FLASH_BASE          (0x08000000) /*!< FLASH(256 KB) base address */
#define _SRAM1_BASE          (0x20000000) /*!< SRAM1(32 KB) base address */
#define _SRAM2_BASE          (0x20004000) /*!< SRAM2(8 KB) base address */

#define TOTAL_ROM_SIZE          FLASH_TOTAL_SIZE

/*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
#define BOOT_SHARED_DATA_SIZE        0
#define BOOT_SHARED_DATA_BASE        0

/*
 * Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0xz_zzzz Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_PARTITION_SIZE because we reserve space
 * for the image header and trailer introduced by the bootloader.
 */

#define BL2_HEADER_SIZE                     (0x400) /*!< Appli image header size */
#define BL2_DATA_HEADER_SIZE                (0x20)  /*!< Data image header size */
#define BL2_TRAILER_SIZE                    (0x2000)
#ifdef BL2
#define IMAGE_PRIMARY_PARTITION_OFFSET        (FLASH_AREA_0_OFFSET)
#define IMAGE_SECONDARY_PARTITION_OFFSET      (FLASH_AREA_2_OFFSET)
#if (MCUBOOT_DATA_IMAGE_NUMBER == 1)
#define DATA_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_5_OFFSET)
#define DATA_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_7_OFFSET)
#endif /* MCUBOOT_DATA_IMAGE_NUMBER == 1 */
#else
#error "Config without BL2 not supported"
#endif /* BL2 */

#define IMAGE_CODE_SIZE \
    (FLASH_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

#define ROM_ALIAS_BASE                   (_FLASH_BASE)

#define RAM_ALIAS_BASE                   (_SRAM1_BASE)

/* Alias definitions */
#define ROM_ALIAS(x)                     (ROM_ALIAS_BASE + (x))
#define LOADER_ROM_ALIAS(x)              (_FLASH_BASE + (x))

#define RAM_ALIAS(x)                     (RAM_ALIAS_BASE + (x))

/* Regions */
#define IMAGE_PRIMARY_AREA_OFFSET        (IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define CODE_START                       (ROM_ALIAS(IMAGE_PRIMARY_AREA_OFFSET))
#define CODE_SIZE                        (IMAGE_CODE_SIZE)
#define CODE_LIMIT                       (CODE_START + CODE_SIZE - 1)
#define DATA_START                       (_SRAM1_BASE)
#define NO_INIT_DATA_SIZE                (0x100)
#define DATA_SIZE                        (_SRAM1_SIZE_MAX + _SRAM2_SIZE_MAX)
#define DATA_LIMIT                       (DATA_START + DATA_SIZE - 1)

/* Secondary partition for new images in case of firmware upgrade */
#define SECONDARY_PARTITION_START           (ROM_ALIAS(IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE            (FLASH_AREA_2_SIZE)

#ifdef BL2
/* Personalized region */
#define PERSO_START                         (ROM_ALIAS(FLASH_AREA_PERSO_OFFSET))
#define PERSO_SIZE                          (FLASH_AREA_PERSO_SIZE)
#define PERSO_LIMIT                         (PERSO_START + PERSO_SIZE - 1)

/* Bootloader region protected by hdp */
#define BL2_CODE_START                      (ROM_ALIAS(FLASH_AREA_BL2_OFFSET))
#define BL2_CODE_SIZE                       (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT                      (BL2_CODE_START + BL2_CODE_SIZE - 1)

/* Bootloader boot address */
#define BL2_BOOT_VTOR_ADDR                  (BL2_CODE_START)

/*  keep 256 bytes unused to place while(1) for non secure to enable */
/*  regression from local tool with non secure attachment
 *  This avoid blocking board in case of hardening error */
#define BL2_DATA_START                      (_SRAM1_BASE)
#define BL2_DATA_SIZE                       (_SRAM1_SIZE_MAX + _SRAM2_SIZE_MAX)
#define BL2_DATA_LIMIT                      (BL2_DATA_START + BL2_DATA_SIZE - 1)

/* Define BL2 MPU SRAM protection to remove execution capability */
/* Area is covering the complete SRAM memory space non secure alias and secure alias */
#define BL2_SRAM_AREA_BASE                  (_SRAM1_BASE)
#define BL2_SRAM_AREA_END                   (_SRAM2_BASE + _SRAM2_SIZE_MAX - 1)
#endif /* BL2 */

#define LOADER_CODE_START                 (LOADER_ROM_ALIAS(FLASH_AREA_LOADER_OFFSET))
#define LOADER_CODE_LIMIT                 (LOADER_CODE_START + LOADER_CODE_SIZE - 1)
#define LOADER_DATA_START                 (RAM_ALIAS(0x0))
#define LOADER_DATA_SIZE                  (_SRAM1_SIZE_MAX)
#define LOADER_DATA_LIMIT                 (LOADER_DATA_START + LOADER_DATA_SIZE - 1)

/* Additional Check to detect flash overflow */
#define FLASH_AREA_END_OFFSET_MAX (FLASH_TOTAL_SIZE)
#if FLASH_AREA_END_OFFSET > FLASH_AREA_END_OFFSET_MAX
#error "Flash memory overflow"
#endif /* FLASH_AREA_END_OFFSET > FLASH_AREA_END_OFFSET_MAX */

#if (MCUBOOT_DATA_IMAGE_NUMBER == 1)
/* DATA image layout */
#define DATA_IMAGE_DATA_OFFSET          (BL2_DATA_HEADER_SIZE)
#define DATA_IMAGE_DATA_SIZE            (0x20)
#endif /* (MCUBOOT_DATA_IMAGE_NUMBER == 1) */

#endif /* __REGION_DEFS_H__ */
