/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   This file provides a BSP example tailored for STM32L476G-Discovery
  *          board, in order to manage the LEDs, the Joystick, the audio mems, 
  *          the gyroscope, quadSPI flash and LCD glass mounted on STM32L476G-Discovery
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
typedef enum
{
  DEMO_STATE_LED            = 0x00,   /*!< Led application launched */
  DEMO_STATE_JOYSTICK,                /*!< Joystick application launched */
  DEMO_STATE_LCD ,                    /*!< LCD application launched */
  DEMO_STATE_TEMP ,                   /*!< TEMP Sensor application launched */
  DEMO_STATE_FIRST_STARTUP,           /*!< Demo first Startup */
  DEMO_STATE_IDLE,                    /*!< Demo in idle */
} Demo_StateTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Menu / Application management */
uint32_t AppStatus = 0x00;
FlagStatus AppLaunched = RESET;
uint8_t DemoIndex = 0;

/* Counter for Sel Joystick pressed*/
__IO uint32_t PressCount = 0;

/* initialization variable */
__IO FlagStatus KeyPressed = RESET;
FlagStatus JoyInitialized  = RESET;
FlagStatus LcdInitialized  = RESET;
FlagStatus LedInitialized  = RESET;
FlagStatus TempInitialized = RESET;

JOYPin_TypeDef JoyState = JOY_NONE;
int32_t Joy_State;

__IO uint8_t NbLoop = 1;
__IO uint8_t JoystickValue = 0;


/* Private function prototypes -----------------------------------------------*/
static void Display_DemoDescription(void);

BSP_DemoTypedef  BSP_examples[]=
{
  {Led_demo, " LED ", DEMO_STATE_LED},
  {Joystick_demo, " JOYSTICK ", DEMO_STATE_JOYSTICK},
  {LCDGlass_demo, " LCD GLASS ", DEMO_STATE_LCD},
  {TempSensor_demo, " TEMP SENSOR ", DEMO_STATE_TEMP}
};
/* Private functions ---------------------------------------------------------*/

/* Extern variable -----------------------------------------------------------*/
extern __IO uint8_t bLCDGlass_KeyPressed;

/**
  * @brief  Main program
  *         The program follows the following steps 
  *         -1- LEDs initialization.
  *         -2- User-based LEDs management: all the LEDs are turned on and the user
  *         can next turn on/off/toggle the LEDs in resorting to the Wakeup/Tamper
  *         button or to the Joystick.
  *         Pressing the SEL joystick button turns on all the LEDs
  * @retval None
  */
 int main(void)
{
  uint8_t i = 0;
  uint8_t menudisplay[DEMO_NAME_CHAR_NB + 5] = {0};
  JOYPin_TypeDef tmp = JOY_NONE;
  uint32_t joystickstatus = JOY_NONE;
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /*##-1- Configure the system clock #########################################*/
  SystemClock_Config(); 
  
  /*##-2-Configure minimum hardware resources at boot ########################*/
  SystemHardwareInit();
  
  /*##-3- Check if we resume from low power mode #############################*/
  
  AppStatus = DEMO_STATE_FIRST_STARTUP;
  Display_DemoDescription();
  
  
  /* Menu display first characters initialization */
  for(i = 0; i < 5; i++)
  {
    menudisplay[i] = ' ';
  }
  
  /* Wait for JOY_DOWN or JOY_UP is pressed */
  tmp = JOY_RIGHT;
  while ((tmp != JOY_DOWN) && (tmp != JOY_UP))
  {
    tmp = (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
     KeyPressed = SET;
     AppLaunched = SET;
  }
  
  if(tmp == JOY_UP)
  {
    
    /* Display message */
    BSP_LCD_GLASS_ScrollSentence((uint8_t*)"   RIGHT: Next demo    ", 1, SCROLL_SPEED_HIGH); 
    BSP_LCD_GLASS_ScrollSentence((uint8_t*)"   LEFT : Previous     ", 1, SCROLL_SPEED_HIGH);
    BSP_LCD_GLASS_ScrollSentence((uint8_t*)"   SEL  : Switch to    ", 1, SCROLL_SPEED_HIGH);  
    HAL_Delay(200);
    JoystickValue = 2;
  }
  if(tmp == JOY_DOWN)
  {
    /* Display message */
    BSP_LCD_GLASS_ScrollSentence((uint8_t*)"   SEL : To restart   ", 1, SCROLL_SPEED_HIGH);  
    JoystickValue = 1;
    HAL_Delay(200);
  }

  while(1)
  {
    /* Ensure a small tempo between images display unless a user action occurs */
    while (joystickstatus  == JOY_NONE)
    {
      /* Get JoyStick status */    
      joystickstatus  = BSP_JOY_GetState(JOY1);
      HAL_Delay(200);
    }
    
    if(joystickstatus  == JOY_RIGHT)
    {
      DemoIndex++;
      for(i = 0; i < DEMO_NAME_CHAR_NB; i++)
      {
        menudisplay[i+5] = BSP_examples[DemoIndex].DemoName[i];
      }

      BSP_LCD_GLASS_ScrollSentence(menudisplay, 1, SCROLL_SPEED_HIGH);

      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        DemoIndex = 0U;
      }
      HAL_Delay(200);
      BSP_LCD_GLASS_Clear();
    }
    
    if(joystickstatus  == JOY_LEFT)
    {
      DemoIndex--;
      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        DemoIndex = (COUNT_OF_EXAMPLE(BSP_examples) - 1);
      }
      for(i = 0; i < DEMO_NAME_CHAR_NB; i++)
      {
        menudisplay[i+5] = BSP_examples[DemoIndex].DemoName[i];
      }
      
      BSP_LCD_GLASS_ScrollSentence(menudisplay, 1, SCROLL_SPEED_HIGH);
      HAL_Delay(200);
      BSP_LCD_GLASS_Clear();
    }
    
    if(joystickstatus  == JOY_SEL)
    {      
      JoystickValue++;
      if (JoystickValue > 2)
      {
        JoystickValue = 1;
      }
      /* Set Application status in case of specific joystick usage */
        AppStatus = BSP_examples[DemoIndex].DemoIndex;
      if (DemoIndex == 1)
      {
        joystickstatus  = JOY_NONE;
      }

       /* launched application */
        BSP_examples[DemoIndex].DemoFunc();
        
        /* Set Application status to Idle */
        AppStatus = DEMO_STATE_IDLE;
        
        /* Notify end of test and clear screen */
        BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      END OF DEMO", 1, SCROLL_SPEED_HIGH);
        BSP_LCD_GLASS_Clear();
        joystickstatus = JOY_NONE;
    }
    joystickstatus  = JOY_NONE;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4800000
  *            PLL_M                          = 8
  *            PLL_N                          = 8
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            Flash Latency(WS)              = 1
  * @retval None
  */
void SystemClock_Config(void)
{
  /* oscillator and clocks configs */
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Disable Power Control clock */
  __HAL_RCC_PWR_CLK_DISABLE();

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState            = RCC_HSE_ON;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT ;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11; /* MSI = 48MHz */
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN            = 8; 
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2; /* 24MHz */
  __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_DIVP);
  __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_DIVQ);
  __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL_DIVR);

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 );
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1))
  {
    /* HAL RCC configuration error */
    Error_Handler();
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 2000000
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @retval None
  */
void SystemLowClock_Config(void)
{
  /* oscillator and clocks configs */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t flatency = 0;

  /* Retrieve clock parameters */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &flatency );
  
  /* switch SYSCLK to MSI in order to modify PLL divider */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, flatency) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Retrieve oscillator parameters */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
  
  /* turn off PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  System Power Configuration at Boot
  * @retval None
  */
void SystemHardwareInit(void)
{
  /* Init LED4 and LED5  */
  if(LedInitialized != SET)
  {
    BSP_LED_Init(LED_BLUE);
    BSP_LED_Init(LED_RED);
    BSP_LED_Init(LED_GREEN);
    LedInitialized = SET;
  }
  
  /* Init STM32U083C-DK joystick in interrupt mode */
  if(JoyInitialized != SET)
  {
    BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);
    JoyInitialized = SET;
  }
  
  /* Initialize the LCD */
  if(LcdInitialized != SET)
  {
    BSP_LCD_GLASS_Init();
    LcdInitialized = SET;
  }

   /* Initialize the LCD */
  if(TempInitialized != SET)
  {
    BSP_ENV_SENSOR_Init(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);;
    TempInitialized = SET;
  }

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief  System Low Power Configuration
  * @retval None
  */
void SystemHardwareDeInit(void)
{
  RCC_OscInitTypeDef oscinitstruct = {0};

  if(LedInitialized != RESET)
  {
    BSP_LED_DeInit(LED_RED);
    BSP_LED_DeInit(LED_GREEN);
    LedInitialized = RESET;
  }

  if(JoyInitialized != RESET)
  {
    BSP_JOY_DeInit(JOY1, JOY_ALL);
    JoyInitialized = RESET;
  }

  if(LcdInitialized != RESET)
  {
    BSP_LCD_GLASS_DeInit();
    LcdInitialized = RESET;
  }

 
  /* LSE off in all modes */
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
  {
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.PLL.PLLState   = RCC_PLL_NONE;
    oscinitstruct.LSEState       = RCC_LSE_OFF;
    if(HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
    {
      while(1);
    }
  }
  
  HAL_PWR_DisableBkUpAccess();

  /* Disable remaining clocks */
  __HAL_RCC_PWR_CLK_DISABLE();
  HAL_PWREx_DisablePullUpPullDownConfig();

 
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_I2C2_CLK_DISABLE();
  
  RCC->AHBSMENR = 0x0;
  RCC->APBSMENR1 = 0x0;
  RCC->APBSMENR2 = 0x0;
}

/**
  * @brief  Display main demo messages
  * @retval None
  */
static void Display_DemoDescription(void)
{

  /* Clear the LCD */ 
  BSP_LCD_GLASS_Clear();

  /* Display LCD messages */
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"  STM32U083C-DK BSP DRIVER EXAMPLE", 1, SCROLL_SPEED_HIGH);

  /* Introduce short delay before new message displayed */
  HAL_Delay(50);

  /* Clear the LCD & reset Key pressed variable */
  BSP_LCD_GLASS_Clear();

  AppStatus = DEMO_STATE_IDLE;
  
  /* Display message */  
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)" 1: LEDs DEMO       ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)" 2: JOYSTICK DEMO   ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)" 3: LCD DEMO        ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)" 4: TEMP SENS DEMO  ", 1, SCROLL_SPEED_HIGH);
  
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)"Press Joystick   ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)"LEFT/RIGHT for DEMO selection        ", 1, SCROLL_SPEED_HIGH);
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)"UP/DOWN for DEMO Start    ", 1, SCROLL_SPEED_HIGH);
  

}

/**
  * @brief  Check for user input
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  /* Init STM32U083C-DK joystick in interrupt mode */
  if(JoyInitialized != SET)
  {
    BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);
    JoyInitialized = SET;
  }
  if(BSP_JOY_GetState(JOY1) == JOY_SEL)
  {
    while (BSP_JOY_GetState(JOY1) == JOY_SEL);
    return 1;
  }
  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED RED on */
  while(1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(40);
  }
}

/**
  * @brief converts a 32bit unsined int into ASCII 
  * @caller several callers for display values
  * @param number digit to displays
  * @param p_tab values in array in ASCII   
  * @retval None
  */ 
void Convert_IntegerIntoChar(float number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, tenthousand=0, hundredthousand=0;
  
  uint32_t number1 = (uint32_t) number;
  
  units = ((((number1%100000)%10000)%1000)%100)%10;
  tens = (((((number1-units)/10)%10000)%1000)%100)%10;
  hundreds = ((((number1-tens-units)/100)%1000)%100)%10;
  thousands = (((number1-hundreds-tens-units)/1000)%100)%10;
  tenthousand = ((number1-thousands-hundreds-tens-units)/10000)%10;
  hundredthousand = ((number1-tenthousand-thousands-hundreds-tens-units)/100000);
  
  *(p_tab+5) = units + 0x30;
  *(p_tab+4) = tens + 0x30;
  *(p_tab+3) = hundreds + 0x30;
  *(p_tab+2) = thousands + 0x30;
  *(p_tab+1) = tenthousand + 0x30;
  *(p_tab+0) = hundredthousand + 0x30;
}

/**
  * @}
  */ 

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

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

