/**
  ******************************************************************************
  * @file    BSP/Src/tempsensor.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led display feature in the 
  *          STM32U083C-DK driver
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
/* Private functions ---------------------------------------------------------*/
float Value_Temp;
uint8_t Id_Env;
uint16_t tab[6];
/* extern variables ----------------------------------------------------------*/
extern JOYPin_TypeDef JoyState;


/**
  * @brief  Temperature Sensor demo
  * @param  None
  * @retval None
  */
void TempSensor_demo(void)
{ 
  uint32_t tickstart;

  /* -1- TEMP SENSOR Enable =================================================*/ 
   BSP_ENV_SENSOR_Enable(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
   BSP_ENV_SENSOR_ReadID(ENV_SENSOR_STTS22H_0, &Id_Env);
   
   while(1)
   {
     BSP_ENV_SENSOR_GetValue(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE, &Value_Temp);
     Convert_IntegerIntoChar(Value_Temp*10000, (uint16_t *)tab);
     tab[5] = 'C';
     tab[4] = '°';
     /* Test the significant digit for displays 3 or 4 digits*/
     if (tab[0] != '0')
     {
       tab[1] |= DOT; /* To add decimal point */
     }
     else
     {
       /* To shift for suppress '0' before decimal */
       tab[0] = tab[1] ;
       tab[0] |= DOT ;
       tab[1] = tab[2] ;
       tab[2] = tab[3] ;
       tab[3] = ' ';
     }
     BSP_LCD_GLASS_DisplayStrDeci(tab);
     HAL_Delay(1000);
     
     tickstart = HAL_GetTick();
     JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
     while((JoyState != JOY_NONE)) 
     {
       JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
       if ((HAL_GetTick() - tickstart) > 100)
       {
         return;
       }
     }
   }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
