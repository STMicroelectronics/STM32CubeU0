/*
 * Copyright (c) 2018 Arm Limited. All rights reserved.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* Flash layout configuration : begin */

#define MCUBOOT_EXT_LOADER             /* Defined: Add external local loader application.
                                                   To enter it, press user button at reset.
                                          Undefined: No external local loader application. */

#define MCUBOOT_DATA_IMAGE_NUMBER 1    /* 1: Data image for application.
                                          0: No data image. */

/*#define MCUBOOT_PRIMARY_ONLY*/           /* Defined: No secondary (download) slot(s),
                                                   only primary slot(s) for each image.
                                          Undefined: Primary and secondary slot(s) for each image. */

/*#define MCUBOOT_OVERWRITE_ONLY*/         /* Defined: the FW installation uses overwrite method.
                                          UnDefined: The FW installation uses swap mode. */

/* Flash layout configuration : end */

/* Total number of images */
#define MCUBOOT_APP_IMAGE_NUMBER 1
#define MCUBOOT_IMAGE_NUMBER (MCUBOOT_APP_IMAGE_NUMBER + MCUBOOT_DATA_IMAGE_NUMBER)
#define MCUBOOT_NS_DATA_IMAGE_NUMBER MCUBOOT_DATA_IMAGE_NUMBER

/* Use image hash reference to reduce boot time (signature check bypass) */
#define MCUBOOT_USE_HASH_REF

/* control configuration */
#if defined(MCUBOOT_PRIMARY_ONLY) && !defined(MCUBOOT_OVERWRITE_ONLY)
#error "Config not supported : When MCUBOOT_PRIMARY_ONLY is enabled, MCUBOOT_OVERWRITE_ONLY is required."
#endif

/* Flash layout info for BL2 bootloader */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x800)        /* 2 KB */
#define FLASH_TOTAL_SIZE                (0x40000)      /* 256 KBytes */
#define FLASH_BASE_ADDRESS              (0x08000000)

/* Flash area IDs */
#define FLASH_AREA_0_ID                 (1)
#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLASH_AREA_2_ID                 (3)
#endif /* MCUBOOT_PRIMARY_ONLY */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_5_ID                 (6)
#endif /* MCUBOOT_NS_DATA_IMAGE_NUMBER == 1 */
#if !defined(MCUBOOT_PRIMARY_ONLY)
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_7_ID                 (8)
#endif /* MCUBOOT_NS_DATA_IMAGE_NUMBER == 1 */
#define FLASH_AREA_SCRATCH_ID           (9)
#endif /* MCUBOOT_PRIMARY_ONLY */

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_2, SCRATCH
 * is used as a temporary storage during image swapping.
 */

/* area for BL2 code */
#define FLASH_AREA_BL2_OFFSET           (0x0)
#define FLASH_AREA_BL2_SIZE             (0x10000)

/* personal area */
#define FLASH_AREA_PERSO_OFFSET         (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_PERSO_SIZE           (0x800)

/* control area under WRP protection */
#if ((FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE) % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "Perso area not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* ((FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE) % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

#if (FLASH_AREA_LOADER_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "Loader offset not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_LOADER_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

#if (FLASH_AREA_LOADER_OFFSET + LOADER_CODE_SIZE % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "Loader end not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_LOADER_OFFSET + LOADER_CODE_SIZE % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* area for image HASH references */
#define FLASH_HASH_REF_AREA_OFFSET      (FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE)
#if defined(MCUBOOT_USE_HASH_REF)
#define FLASH_HASH_REF_AREA_SIZE        (0x800)
#else
#define FLASH_HASH_REF_AREA_SIZE        (0x0000)
#endif /* MCUBOOT_USE_HASH_REF */

/* area for HUK and anti roll back counter */
#define FLASH_BL2_NVCNT_AREA_OFFSET     (FLASH_HASH_REF_AREA_OFFSET + FLASH_HASH_REF_AREA_SIZE)
#define FLASH_BL2_NVCNT_AREA_SIZE       (0x3000)  /* 12 KB */

/* scratch area */
#define FLASH_AREA_SCRATCH_OFFSET       (FLASH_BL2_NVCNT_AREA_OFFSET + FLASH_BL2_NVCNT_AREA_SIZE)
#if defined(FLASH_AREA_SCRATCH_ID)
#define FLASH_AREA_SCRATCH_DEVICE_ID    (FLASH_DEVICE_ID - FLASH_DEVICE_ID)
#if !defined(MCUBOOT_OVERWRITE_ONLY)
#define FLASH_AREA_SCRATCH_SIZE         (0x4000)  /* 16 KB */
#else
#define FLASH_AREA_SCRATCH_SIZE         (0x0000) /* Not used in MCUBOOT_OVERWRITE_ONLY mode */
#endif /* defined(MCUBOOT_OVERWRITE_ONLY) */
/* control scratch area */
#if (FLASH_AREA_SCRATCH_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_SCRATCH_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_SCRATCH_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* defined(FLASH_AREA_SCRATCH_ID) */
#define FLASH_AREA_SCRATCH_SIZE         (0x0)
#endif /* defined(FLASH_AREA_SCRATCH_ID) */

/* Loader area */
#define LOADER_CODE_SIZE                  (0x4000) /* 16 Kbytes  */
#define FLASH_AREA_LOADER_OFFSET          (FLASH_AREA_SCRATCH_OFFSET + FLASH_AREA_SCRATCH_SIZE)
/* Control Loader Image */
#if (FLASH_AREA_LOADER_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_LOADER_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_LOADER_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* BL2 partitions size */
#if defined(MCUBOOT_PRIMARY_ONLY)
#define FLASH_PARTITION_SIZE            (0x20000) /* 128 KB */
#else
#define FLASH_PARTITION_SIZE            (0x10000) /* 64 KB */
#endif

#define FLASH_MAX_APP_PARTITION_SIZE     FLASH_PARTITION_SIZE
#if (MCUBOOT_DATA_IMAGE_NUMBER == 1)
#define FLASH_DATA_PARTITION_SIZE       (0x2000)
#else
#define FLASH_DATA_PARTITION_SIZE       (0x0)
#endif /* (MCUBOOT_DATA_IMAGE_NUMBER == 1) */

#define FLASH_MAX_DATA_PARTITION_SIZE   (FLASH_DATA_PARTITION_SIZE)
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_MAX_APP_PARTITION_SIZE >   \
                                         FLASH_MAX_DATA_PARTITION_SIZE) ? \
                                         FLASH_MAX_APP_PARTITION_SIZE : \
                                         FLASH_MAX_DATA_PARTITION_SIZE)

/* BL2 flash areas */
#define FLASH_AREA_BEGIN_OFFSET         (0x20000)
#define FLASH_AREAS_DEVICE_ID           (FLASH_DEVICE_ID - FLASH_DEVICE_ID)

/* App image primary slot */
#if defined(FLASH_AREA_0_ID)
#define FLASH_AREA_0_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_BEGIN_OFFSET)
#define FLASH_AREA_0_SIZE               (FLASH_PARTITION_SIZE)
/* Control app image primary slot */
#if (FLASH_AREA_0_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_0_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_0_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_0_ID */
#define FLASH_AREA_0_SIZE               (0x0)
#endif /* FLASH_AREA_0_ID */

/* App image secondary slot */
#if defined(FLASH_AREA_2_ID)
#define FLASH_AREA_2_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE               (FLASH_PARTITION_SIZE)
/* Control Secure app image secondary slot */
#if (FLASH_AREA_2_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_2_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_2_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_2_ID */
#define FLASH_AREA_2_SIZE               (0x0)
#endif /* FLASH_AREA_2_ID */

/* Data image primary slot */
#if defined(FLASH_AREA_5_ID)
#define FLASH_AREA_5_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_5_OFFSET             (FLASH_AREA_LOADER_OFFSET + LOADER_CODE_SIZE)
#define FLASH_AREA_5_SIZE               (FLASH_DATA_PARTITION_SIZE)
/* Control data image primary slot */
#if (FLASH_AREA_5_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_5_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_5_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0  */
#else /* FLASH_AREA_5_ID */
#define FLASH_AREA_5_SIZE               (0x0)
#endif /* FLASH_AREA_5_ID */

/* Data image secondary slot */
#if defined(FLASH_AREA_7_ID)
#define FLASH_AREA_7_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_7_OFFSET             (FLASH_AREA_5_OFFSET + FLASH_AREA_5_SIZE)
#define FLASH_AREA_7_SIZE               (FLASH_DATA_PARTITION_SIZE)
/* Control data image secondary slot */
#if (FLASH_AREA_7_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_7_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_7_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_7_ID */
#define FLASH_AREA_7_SIZE               (0x0)
#endif /* FLASH_AREA_7_ID */

/* flash areas end offset */
#define FLASH_AREA_END_OFFSET           (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_0_SIZE + \
                                         FLASH_AREA_2_SIZE)

#if (FLASH_AREA_END_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_END_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_END_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/*
 * The maximum number of status entries supported by the bootloader.
 */
#if defined (MCUBOOT_OVERWRITE_ONLY)
#define MCUBOOT_STATUS_MAX_ENTRIES      (0)
#else
#define MCUBOOT_STATUS_MAX_ENTRIES      (((FLASH_MAX_PARTITION_SIZE - 1) / \
                                          FLASH_AREA_SCRATCH_SIZE)+1)
#endif
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS         ((FLASH_MAX_PARTITION_SIZE) / \
                                          FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Flash device name used by BL2 and NV Counter
 * Name is defined in flash driver file: low_level_flash.c */
#define FLASH_DEV_NAME                   Driver_FLASH0

/* BL2 NV Counters definitions  */
#define BL2_NV_COUNTERS_AREA_ADDR        FLASH_BL2_NVCNT_AREA_OFFSET
#define BL2_NV_COUNTERS_AREA_SIZE        FLASH_BL2_NVCNT_AREA_SIZE

#endif /* __FLASH_LAYOUT_H__ */
