/**
  ******************************************************************************
  * @file    BSP/Src/joystick.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the joystick feature in the 
  *         STM32U083C-DK driver
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

/** @addtogroup STM32U0xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* extern variables ----------------------------------------------------------*/
extern FlagStatus KeyPressed;
extern FlagStatus AppLaunched;
extern JOYPin_TypeDef JoyState;
  
/**
* @brief  Joystick demo
* @param  None
* @retval None
*/
void Joystick_demo (void)
{
  JOYPin_TypeDef previousstate = JOY_NONE;
  uint32_t tickstart;
  
  
  while(1)
  {
    JoyState =  (JOYPin_TypeDef) BSP_JOY_GetState(JOY1);
    switch(JoyState)
    {
    case JOY_LEFT:
      BSP_LED_Toggle(LED3);
      BSP_LED_Toggle(LED4);
      BSP_LCD_GLASS_DisplayString((uint8_t *)"  LEFT");
      break;
      
    case JOY_RIGHT:
      BSP_LED_Toggle(LED4);
      BSP_LED_Toggle(LED5);
      BSP_LCD_GLASS_DisplayString((uint8_t *)" RIGHT");
      break;
      
    case JOY_UP:
      BSP_LED_Toggle(LED3);
      BSP_LCD_GLASS_DisplayString((uint8_t *)"    UP");
      break;
      
    case JOY_DOWN:
      BSP_LED_Toggle(LED5);
      BSP_LCD_GLASS_DisplayString((uint8_t *)"  DOWN");
      break;
      
    case JOY_SEL:
      BSP_LED_On(LED3);
      BSP_LED_On(LED4);
      BSP_LED_On(LED5);
      BSP_LCD_GLASS_DisplayString((uint8_t *)"   SEL");
      break;
      
    default:
      break;
    }
    
    if(previousstate != JoyState)
    {
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
      BSP_LED_Off(LED5);
      if(JoyState == JOY_UP)
      {
        BSP_LED_On(LED5);
      }
    }
    tickstart = HAL_GetTick();
    while(JoyState == JOY_NONE) 
    {
      JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      if ((HAL_GetTick() - tickstart) > 5000)
      {
        return;
      }
    }
    
    previousstate = JoyState;
    KeyPressed = RESET; 
    
  } 
}

/**
* @}
*/ 

/**
* @}
*/ 

  