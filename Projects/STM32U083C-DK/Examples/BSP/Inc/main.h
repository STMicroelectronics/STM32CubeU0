/**
  ******************************************************************************
  * @file    BSP/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"
#include "stm32u083c_discovery.h"
#include "stm32u083c_discovery_env_sensors.h"
#include "stm32u083c_discovery_glass_lcd.h"

/* Exported types ------------------------------------------------------------*/
#define DEMO_NAME_CHAR_NB         20
/* Exported constants --------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[DEMO_NAME_CHAR_NB];
  uint32_t DemoIndex;
}BSP_DemoTypedef;
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)       (sizeof(x)/sizeof(BSP_DemoTypedef))
#define ABS(x)                    (x < 0) ? (-x) : x
/* Exported functions ------------------------------------------------------- */
void Gyro_demo(void);
void Joystick_demo (void);
void LCDGlass_demo(void);
void Led_demo(void);
void TempSensor_demo(void);
uint8_t CheckForUserInput(void);
void      SystemClock_Config(void);
void      SystemLowClock_Config(void);
void      SystemHardwareInit(void);
void      SystemHardwareDeInit(void);

void Error_Handler(void);
void Convert_IntegerIntoChar(float number, uint16_t *p_tab);

#endif /* __MAIN_H */

