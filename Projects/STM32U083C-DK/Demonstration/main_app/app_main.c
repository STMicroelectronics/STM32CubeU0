/**
  ******************************************************************************
  * @file    app_main.c
  * @author  MCD Application Team
  * @brief   Main application (root menu) implementation
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
#include "app_main.h"
#include "touchsensing.h"
#include "tsl.h"

/* Exported types ------------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hbus_i2c1;
extern RTC_HandleTypeDef hrtc;

/* Private constants ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_2MHz(void);
void PB_Config(void);
void JOY_Config(Button_TypeDef Button);
void SystemPeripheralClockDisable(void);

/* Private Variable ----------------------------------------------------------*/
/* Menu / Application management */
uint32_t AppStatus = 0x00;
FlagStatus AppLaunched = RESET;
uint8_t DemoIndex = 0;
uint8_t ModeIndex = 0;

JOYPin_TypeDef JoyState = JOY_NONE;
int32_t Joy_State;

uint32_t joystickstatus = JOY_NONE;

__IO uint8_t JoystickValue = 0;

/* initialization variable */
__IO FlagStatus KeyPressed = RESET;
FlagStatus AirQualityInitialized  = RESET;
FlagStatus LPModesInitialized     = RESET;
FlagStatus TouchWakeupInitialized = RESET;
FlagStatus TempInitialized        = RESET;
FlagStatus JoyInitialized         = RESET;
FlagStatus LedInitialized         = RESET;
FlagStatus LcdInitialized         = RESET;
FlagStatus AirQualitynotAvailable = RESET;

uint8_t msg[32];
uint8_t buf_rx[8];
uint8_t buf_tx[8];

float Value_Temp;
float Fahrenheit_Temp;
uint8_t Id_Env;
uint16_t tab[6];

/* CCS811 variable and define */
uint8_t CCS811_DevStatus;

#define CCS811_CO2_LOWTH  800
#define CCS811_CO2_HIGHTH 1000

#define CCS811_TVOC_LOWTH  250
#define CCS811_TVOC_HIGHTH 2000

static __IO uint32_t TimingDelay;

uint8_t ModeStatus = 0;

/* TSC variable and define */
extern uint32_t idx_bank_it;
extern uint32_t config_done_it;
extern uint32_t acq_done_it;

#define TKEY_DET(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_DETECT)
#define TKEY_PRX(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_PROX)
#define TKEY_REL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_RELEASE)
#define TKEY_CAL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_CALIB)
extern __IO TSL_tTick_ms_T ECSLastTick;

extern TSL_TouchKeyParam_T MyTKeys_Param[];
extern COMP_HandleTypeDef hcomp2;
uint32_t uwTSC_WakeupDetected = 1;

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  DEMO_STATE_AIR_QUALITY      = 0x00,   /*!< Air Quality Sensor application launched */
  DEMO_STATE_TEMP,                      /*!< TEMP Sensor application launched */
  DEMO_STATE_TOUCH_WAKEUP,              /*!< Touch Wakeup application launched */
  DEMO_STATE_LOW_POWER_MODES,           /*!< Low Power Modes application launched */
  DEMO_STATE_FIRST_STARTUP,             /*!< Demo first Startup */
  DEMO_STATE_IDLE,                      /*!< Demo in idle */
} Demo_StateTypeDef;

Demo_Typedef  MainMenuItems[]=
{
  {AirQuality_demo, "  AIR ", DEMO_STATE_AIR_QUALITY},
  {TempSensor_demo, " TEMP ", DEMO_STATE_TEMP},
  {TouchWakeup_demo, " TOUCH", DEMO_STATE_TOUCH_WAKEUP},
  {LPModes_demo, "  ULP ", DEMO_STATE_LOW_POWER_MODES}
};

StopModes_Typedef  StopModesItems[]=
{
  {Standby_Mode, " STDBY", 0,"160 nA" },
  {Sleep_Mode, " SLEEP", 1,"101 uA" },
  {LpSleep_Mode, "LP SLP", 2,"62 uA" },
  {Stop1_Mode, " STOP1", 3,"3.3 uA" },
  {Stop2_Mode, " STOP2", 4,"825 nA" }
};

/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100
/**
 * @defgroup meas_mode Measurement mode
 * @{
 */
#define CCS811_DRIVE_MODE_0  ( 0x00 << 4 )
#define CCS811_DRIVE_MODE_1  ( 0x01 << 4 )
#define CCS811_DRIVE_MODE_2  ( 0x02 << 4 )
#define CCS811_DRIVE_MODE_3  ( 0x03 << 4 )
#define CCS811_DRIVE_MODE_4  ( 0x04 << 4 )
#define CCS811_INT_DATARDY_0 ( 0x00 << 3 )
#define CCS811_INT_DATARDY_1 ( 0x01 << 3 )
#define CCS811_INT_THRESH_0  ( 0x00 << 2 )
#define CCS811_INT_THRESH_1  ( 0x01 << 2 )
/**
  * @}
  */
/* External variables --------------------------------------------------------*/


/* Private function ----------------------------------------------------------*/

void Demo_Start(void)
{
  uint8_t i = 0;
  uint8_t menudisplay[DEMO_NAME_CHAR_NB + 5] = {0};

  AppStatus = DEMO_STATE_FIRST_STARTUP;
  
  /* Menu display first characters initialization */
  for(i = 0; i < 5; i++)
  {
    menudisplay[i] = ' ';
  }

 
  if (AirQualitynotAvailable == SET)
  {
    DemoIndex = 1U;
  }else
  {
    DemoIndex = 0U;
  }

  while(1)
  {
    if(TAMP->BKP0R == 1U)
    {
      DemoIndex = TAMP->BKP2R;
      ModeIndex = TAMP->BKP1R;
      MainMenuItems[DemoIndex].DemoFunc();
    }
    for(i = 0; i < DEMO_NAME_CHAR_NB; i++)
      {
        menudisplay[i] = MainMenuItems[DemoIndex].DemoName[i];
      }

      BSP_LCD_GLASS_DisplayString(menudisplay);
      HAL_Delay(200);
      
    while (joystickstatus  == JOY_NONE)
    {
      /* Get JoyStick status */
      joystickstatus  = BSP_JOY_GetState(JOY1);
      HAL_Delay(200);
    }
    
    if (joystickstatus  == JOY_UP)
    {
      DemoIndex++;
      if (DemoIndex >= COUNT_OF_EXAMPLE(MainMenuItems))
      {
         DemoIndex = 0U;
      }
      /* Remove the Air Quality Demo in case of sensor not available */
      if (AirQualitynotAvailable == SET)
      {
        if (DemoIndex == 0U)
        {
          DemoIndex = 1U;
        }
      }
    }

    if (joystickstatus  == JOY_DOWN)
    {
      DemoIndex--;
      if (DemoIndex >= COUNT_OF_EXAMPLE(MainMenuItems))
      {
        DemoIndex = (COUNT_OF_EXAMPLE(MainMenuItems) - 1);
      }
      /* Remove the Air Quality Demo in case of sensor not available */
      if (AirQualitynotAvailable == SET)
      {
        if (DemoIndex == 0U)
        {
          DemoIndex = 3U;
        }
      }
    }

    if ((joystickstatus  == JOY_SEL) || (joystickstatus  == JOY_RIGHT))
    {
      JoystickValue++;
      if (JoystickValue > 2)
      {
        JoystickValue = 1;
      }
      /* Set Application status in case of specific joystick usage */
        AppStatus = MainMenuItems[DemoIndex].DemoIndex;

       /* launched application */
        MainMenuItems[DemoIndex].DemoFunc();

        /* Set Application status to Idle */
        AppStatus = DEMO_STATE_IDLE;

        joystickstatus = JOY_NONE;
    }
    joystickstatus  = JOY_NONE;
  }

}

void AirQuality_demo(void)
{
  AirQuakity_ValTypeDef AirQuakity_Val;
  char * Message;
  uint32_t tickstart;

  if (AirQualitynotAvailable != SET)
  {
    CCS811_Start();
    CCS811_Measure();
    AirQuakity_Val = CCS811_GetValue();
    BSP_LCD_GLASS_DisplayString((uint8_t *)"CALIBR");    
    HAL_Delay(3000);
    BSP_LCD_GLASS_ScrollSentence((uint8_t *) "", 1, SCROLL_SPEED_HIGH);
    while(1)
    {
      AirQuakity_Val = CCS811_GetValue();
      if ((AirQuakity_Val.CO2PPM < CCS811_CO2_LOWTH) && (AirQuakity_Val.TVOCPPB < CCS811_CO2_LOWTH))
      {
        Message="NORMAL"; 
      }
      else if ((AirQuakity_Val.CO2PPM >= CCS811_CO2_LOWTH) || (AirQuakity_Val.TVOCPPB >= CCS811_CO2_LOWTH)) 
      {
        Message="POLLUTION"; 
        BSP_LCD_GLASS_ScrollSentence((uint8_t *)Message, 1, SCROLL_SPEED_MEDIUM);
      }
      else if ((AirQuakity_Val.CO2PPM >= CCS811_CO2_LOWTH) || (AirQuakity_Val.TVOCPPB >= CCS811_CO2_LOWTH)) 
      {
        Message="HIGH POLLUTION"; 
        BSP_LCD_GLASS_ScrollSentence((uint8_t *)Message, 1, SCROLL_SPEED_MEDIUM);
      }
      BSP_LCD_GLASS_DisplayString((uint8_t *)" CO2  ");
      HAL_Delay(1000);
      sprintf((char *) msg, "%dppm", AirQuakity_Val.CO2PPM);
      BSP_LCD_GLASS_DisplayString(msg);
      HAL_Delay(2000);
      BSP_LCD_GLASS_DisplayString((uint8_t *)" TVOC ");
      HAL_Delay(1000);
       sprintf((char *) msg, "%d ppb ", AirQuakity_Val.TVOCPPB);
      BSP_LCD_GLASS_DisplayString(msg);
      HAL_Delay(2000);
      
      tickstart = HAL_GetTick();
      JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      while(JoyState == JOY_LEFT)
      {
        JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
        if ((HAL_GetTick() - tickstart) > 250)
        {
          return;
        }
      }
    }

  }
}

void LPModes_demo(void)
{
  uint8_t modedisplay[MODE_NAME_CHAR_NB + 5] = {0};
  uint8_t i = 0;
  uint32_t tickstart;

  
   /* Menu display first characters initialization */
  for(i = 0; i < 5; i++)
  {
    modedisplay[i] = ' ';
  }
  ModeStatus = 0;
  joystickstatus  = JOY_NONE;
  
  /* Ensure that MSI is wake-up system clock */ 
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
  
  /* Configure the system Power */
  SystemPower_Config();

  /* Turn off LEDs */
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_BLUE);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_DeInit(LED_RED);
  BSP_LED_DeInit(LED_GREEN);

  ModeIndex = 0;
  while(1)
  {
    if(TAMP->BKP0R == 1U)
    {
      DemoIndex = TAMP->BKP2R;
      ModeIndex = TAMP->BKP1R;
      TAMP->BKP0R = 0U;
    }
    for(i = 0; i < MODE_NAME_CHAR_NB; i++)
      {
        modedisplay[i] = StopModesItems[ModeIndex].ModeName[i];
      }
      ModeStatus = 2;
      BSP_LCD_GLASS_DisplayString(modedisplay);
      HAL_Delay(500);
    
    /* Ensure a small tempo between images display unless a user action occurs */
    while (joystickstatus  == JOY_NONE)
    {
      /* Get JoyStick status */    
      joystickstatus  = BSP_JOY_GetState(JOY1);
      HAL_Delay(50);
      
    }
    if(joystickstatus  == JOY_UP)
    {
      ModeIndex++;
      if (ModeIndex >= COUNT_OF_MODES(StopModesItems))
      {
        ModeIndex = 0U;
      }
      ModeStatus = 1; 
    }
    
    if (joystickstatus  == JOY_DOWN)
    {
      ModeIndex--;
      if (ModeIndex >= COUNT_OF_MODES(StopModesItems))
      {
        ModeIndex = (COUNT_OF_MODES(StopModesItems) - 1);
      }
      ModeStatus = 2;
    }
    
    if ((joystickstatus  == JOY_SEL) || (joystickstatus  == JOY_RIGHT))
    {      
      BSP_LED_Off(LED_BLUE); 
      /* Configure the User Push Button */
      PB_Config();
      
      /* Disable all used wakeup source */
      HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
      
      /* Re-enable wakeup source */
      /* ## Setting the Wake up time ############################################*/
      /* RTC Wakeup Interrupt Generation: 
      the wake-up counter is set to its maximum value to yield the longest
      stop time to let the current reach its lowest operating point.
      The maximum value is 0xFFFF, corresponding to about 32 sec. when 
      RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16  */
      
      /* Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE))
      Wakeup Time = Wakeup Time Base * WakeUpCounter 
      = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE)) * WakeUpCounter
      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base   */
      
      /* To configure the wake up timer to 33s the WakeUpCounter is set to 0xFFFF:
      Wakeup Time Base = 16 /(~32.76 kHz RC) = ~0.488 ms
      Wakeup Time = 0.488 ms  * WakeUpCounter
      Therefore, with wake-up counter =  0xFFFF  = 65,535 
      Wakeup Time =  0.488 ms *  65,535 = ~ 32 sec. */
      HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0xFFFF, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0);
      
      /* Enter STOP 2 mode */
      HAL_SuspendTick();

      StopModesItems[ModeIndex].ModeFunc();
      
      /* ... STOP2 mode ... */
      HAL_ResumeTick();
      
      /* Re-configure and turn on LED4  */
      BSP_LED_Init(LED4);
      
      /* Configure the Joystick */
      JOY_Config(BUTTON_USER);
      TAMP->BKP0R = 0U;
    }

    JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
    tickstart = HAL_GetTick();
    while (JoyState == JOY_LEFT) 
    {
      JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      if ((HAL_GetTick() - tickstart) > 250)
      {
        TAMP->BKP0R = 0U;
        return;
      }
    }

    joystickstatus  = JOY_NONE;
  }
  
}

void LpSleep_Mode(void)
{
 
   tab[0] = ' ';
   tab[1] = '6';
   tab[2] = '2';
   tab[3] = ' ';
   tab[4] = u'µ';
   tab[5] = 'A';
   BSP_LCD_GLASS_DisplayStrDeci(tab);

  SystemClock_2MHz();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_RTCAPB_CLK_SLEEP_ENABLE();
  HAL_NVIC_ClearPendingIRQ(RTC_TAMP_IRQn);
  SystemPeripheralClockDisable();
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void Sleep_Mode(void)
{
  tab[0] = '1';
  tab[1] = '0';
  tab[2] = '1';
  tab[3] = ' ';
  tab[4] = u'µ';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);


  SystemClock_2MHz();
  __HAL_RCC_PWR_CLK_ENABLE();
    
  SystemPeripheralClockDisable();
  
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

}

void Stop1_Mode(void)
{
  tab[0] = ' ';
  tab[1] = '3';
  tab[1] |= DOT;
  tab[2] = '3';
  tab[3] = ' ';
  tab[4] = u'µ';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);
  /* Clear Stop1 Flag if returned from Stop1 mode */
  if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOP1))
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP1);
  }
  HAL_PWREx_DisableLowPowerRunMode();
  HAL_PWREx_EnableUltraLowPowerMode();
   
  SystemPeripheralClockDisable();
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
}

void Stop2_Mode(void)
{
  tab[0] = '8';
  tab[1] = '2';
  tab[2] = '5';
  tab[3] = ' ';
  tab[4] = u'n';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);
  /* Clear Stop2 Flag if returned from Stop2 mode */
  if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOP2))
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP2);
  }

  HAL_PWREx_DisableLowPowerRunMode();
  HAL_PWREx_EnableUltraLowPowerMode();
  SystemPeripheralClockDisable();
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

void Standby_Mode(void)
{
  BSP_LCD_GLASS_DisplayString(StopModesItems[ModeIndex].Message);
  /* Clear StandBy Flag if returned from StandBy mode */
  if ( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) )
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
  }
  SystemClock_2MHz();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  
  HAL_PWR_EnableBkUpAccess();
  SystemPeripheralClockDisable();

  TAMP->BKP0R = 1U;
  TAMP->BKP1R = ModeIndex;
  TAMP->BKP2R = DemoIndex;
  HAL_SuspendTick();
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  System Power Configuration
  *         The system Power is configured as follow:
  *            + VREFINT OFF, with fast wakeup enabled
  *            + No IWDG
  *            + Automatic Wakeup using RTC clocked by LSI (after ~5s)
  * @param  None
  * @retval None
  */
void SystemPower_Config(void)
{
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPowerMode();

  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableInternalWakeUpLine();
}


/**
  * @brief  Push Button Configuration
  *         The Push Button is configured as follow:
  *            + BUTTON_USER: User Push Button
  *            + BUTTON_MODE_EXTI:Button will be connected to EXTI line
  *                               with interrupt generation capability 
  * @param  None
  * @retval None
  */
void PB_Config(void)
{
  /* De-Initialize the joystick pins*/
  BSP_JOY_DeInit(JOY1,  JOY_ALL);
  
  /* Init the User Button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  
   /* Reset Joystick init flag */
  JoyInitialized = RESET;
}

/**
  * @brief  Joystick Configuration
  * @param  JoyMode Button mode.
  *          This parameter can be one of the following values:
  *            @arg  JOY_MODE_GPIO: Joystick pins will be used as simple IOs
  * @param  None
  * @retval None
  */
void JOY_Config(Button_TypeDef Button)
{
  /* This function should be implemented by the user application.
  It is called into this driver when an event on Button is triggered. */
  
  /* De-Initialize the user push button */
  BSP_PB_DeInit(Button);

  /* Initialize the joystick pins*/
  BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);
  /* Set Joystick init flag */
  JoyInitialized = SET;
   __HAL_RCC_LCD_CLK_ENABLE();
   __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  /* This function should be implemented by the user application.
  It is called into this driver when an event on Button is triggered. */
  
 JOY_Config(Button);
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_ResumeTick();
  if (MainMenuItems[DemoIndex].DemoIndex == DEMO_STATE_LOW_POWER_MODES)
  {
    if (ModeStatus != 0)
    {
      if (TimingDelay != 0)
      {
        TimingDelay--;
      }
      else
      {
        /* Toggle LED4 */
        BSP_LED_Toggle(LED4);
        TimingDelay = LED_TOGGLE_DELAY;
      }
    }
  }
}

/**
  * @brief  TouchWakeup/Touchsensing Demonstration through Comparator
  * @note  Goal of the demonstration is to detect a touch on Touchsensor button TS1
  *        after a low power phase using Comparator peripheral to decrease power consumption
  * @param  None
  * @retval None
  */
void TouchWakeup_demo(void)
{
  uint32_t tickstart;
  uint32_t count_nb_acq = 5000;

  /* Start Comparator for TSC detection */
  HAL_COMP_Start(&hcomp2);

  /* /!\ Missing HAL interface to manage those field ? /!\ */
  MODIFY_REG(SYSCFG->TSCCR, 0x3f, (SYSCFG_TSCCR_G6IO1|SYSCFG_TSCCR_TSCIOCTRL));

  /* ShutDown the debugger to reduce current consumption */
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();
  __HAL_RCC_DBGMCU_CLK_DISABLE();

  /* Display "RUN MODE" at startup */
  BSP_LCD_GLASS_Clear();
  BSP_LED_On(LED4);
  HAL_Delay(500);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" RUN MODE", 2, SCROLL_SPEED_MEDIUM);

  /* Let TSC and TSL run for 5s to set up ECS */
  while(count_nb_acq)
  {
    if (tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY)
    {
      /* Restart TSLPRM_TOTAL_BANKS banks acquisition */
      idx_bank_it = 0;
      acq_done_it = 0;
      TSL_acq_BankConfig(idx_bank_it);
      TSL_acq_BankStartAcq_IT();
      count_nb_acq--;
    }
    else
    {
      HAL_Delay(1);
    }
  }

  /* Reduce the dischage time of the sampling capacitor */
  TSL_Globals.DelayDischarge = 0;

  /* Start RTC Timer for 250ms interval */
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x1ff, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0x0);

  while(1)
  {
    /* Run TSC acquisition in blocking mode after wake up */
    if (tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY)
    {
      int id;
      for (id=0; id < TSLPRM_TOTAL_CHANNELS; id++)
      {
        if (!TKEY_CAL(id))
        {
          if (TKEY_DET(id))
          {
            BSP_LCD_GLASS_Clear();
            BSP_LED_On(LED4);
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)"WAKEUP TOUCH DETECTED", 1, SCROLL_SPEED_MEDIUM);
            HAL_Delay(5000);
            uwTSC_WakeupDetected = 1;
          }
          else if (TKEY_REL(id) && (uwTSC_WakeupDetected == 1))
          {
            BSP_LCD_GLASS_Clear();
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)" STOP2 MODE ENTERING", 1, SCROLL_SPEED_MEDIUM);
            BSP_LED_Off(LED4);
            uwTSC_WakeupDetected = 0;
          }
        }
      }

      /* Go into stop 2 mode */
      HAL_SuspendTick();
      HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
      HAL_ResumeTick();

      /* Clear Wakeup flag */
      SET_BIT(RTC->SCR,RTC_SCR_CWUTF);

      /* Wakeup and Restart TSLPRM_TOTAL_BANKS banks acquisition */
      idx_bank_it = 0;
      acq_done_it = 0;
      TSL_acq_BankConfig(idx_bank_it);
      TSL_acq_BankStartAcq_IT();
    }
    else
    {
      /* SuspendTick to reduce current consumption BUT ECS WILL NOT WORK */
      HAL_SuspendTick();
      /* Enter the CPU to SLEEP mode */
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      HAL_ResumeTick();
    }

    JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
    tickstart = HAL_GetTick();
    while(JoyState == JOY_LEFT)
    {
      JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      if ((HAL_GetTick() - tickstart) > 250)
      {
        return;
      }
    }
  }
}

/**
  * @brief  Temperature sensor Demonstration
  * @note  Goal of the demonstration is to measure and display temperature
  *        in degree Celsius and Fahrenheit.
  * @param  None
  * @retval None
  */
void TempSensor_demo(void)
{
  uint32_t tickstart;
  Temp_ModeTypedef Temp_mode;

  /* -1- TEMP SENSOR Enable =================================================*/
   BSP_ENV_SENSOR_Init(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
   BSP_ENV_SENSOR_Enable(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
   BSP_LCD_GLASS_ScrollSentence((uint8_t *) " ", 1, SCROLL_SPEED_HIGH);
   HAL_Delay(2000);
   Temp_mode = TMODE_CELSIUS;
   while(1)
   {
     joystickstatus  = BSP_JOY_GetState(JOY1);
     
     if (joystickstatus  == JOY_DOWN)
     {
        Temp_mode = TMODE_FAHRENHEIT;
     }
     if (joystickstatus  == JOY_UP)
     {
        Temp_mode = TMODE_CELSIUS; 
     }
          
     BSP_ENV_SENSOR_GetValue(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE, &Value_Temp);
     if (Temp_mode == TMODE_FAHRENHEIT)
     {
       /* Formula for conversion from Celsius to Fahrenheit */
       Fahrenheit_Temp = (Value_Temp * 9 / 5) + 32;
       Convert_IntegerIntoChar(Fahrenheit_Temp*10000, (uint16_t *)tab);
       tab[5] = 'F';
       tab[4] = u'°';
     }else
     {
       Convert_IntegerIntoChar(Value_Temp*10000, (uint16_t *)tab);
       tab[5] = 'C';
       tab[4] = u'°';
     }
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
     HAL_Delay(100);
     
     joystickstatus =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
     tickstart = HAL_GetTick();
     while(joystickstatus == JOY_LEFT)
     {
       joystickstatus =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
       if ((HAL_GetTick() - tickstart) > 250)
       {
         return;
       }
     }
   }
}

/**
  * @brief  Display main demo messages
  * @retval None
  */
 void Display_DemoDescription(void)
{

  /* Clear the LCD */
  BSP_LCD_GLASS_Clear();

  /* Display LCD messages */
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      STM32U083C-DK DEMO", 1, SCROLL_SPEED_MEDIUM);

  /* Introduce short delay before new message displayed */
  HAL_Delay(50);
  
  /* Display message */
  if (AirQualitynotAvailable != SET)
  {
    BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[0].DemoName);
    HAL_Delay(1000);
  }
  BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[1].DemoName);
  HAL_Delay(1000);
  BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[2].DemoName);
  HAL_Delay(1000);
  BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[3].DemoName);
  HAL_Delay(1000);
}

/**
  * @brief  System Power Configuration at Boot
  * @retval None
  */
void SystemHardwareInit(void)
{
  /* Init LED4 and LED5  */
  if (LedInitialized != SET)
  {
    BSP_LED_Init(LED_BLUE);
    BSP_LED_Init(LED_RED);
    BSP_LED_Init(LED_GREEN);
    BSP_LED_On(LED_BLUE);
    BSP_LED_On(LED_RED);
    BSP_LED_On(LED_GREEN);
    LedInitialized = SET;
  }

  /* Init STM32U083C-DK joystick in interrupt mode */
  if (JoyInitialized != SET)
  {
    BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);
    JoyInitialized = SET;
  }

  /* Initialize the LCD */
  if (LcdInitialized != SET)
  {
    BSP_LCD_GLASS_Init();
    LcdInitialized = SET;
  }

/* Initialize the AirQuality */
  if (AirQualityInitialized != SET)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    CCS811_Init();
    AirQualityInitialized = SET;
    if (CCS811_Status() != AirQuality_OK)
    {
      AirQualitynotAvailable = SET;
    }
  }

  /* Initialize the TempSensor */
  if ((TempInitialized != SET) && (AirQualitynotAvailable != RESET))
  {
    BSP_ENV_SENSOR_Init(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
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

  if (LedInitialized != RESET)
  {
    BSP_LED_DeInit(LED_RED);
    BSP_LED_DeInit(LED_GREEN);
    BSP_LED_DeInit(LED_BLUE);
    LedInitialized = RESET;
  }

  if (JoyInitialized != RESET)
  {
    BSP_JOY_DeInit(JOY1, JOY_ALL);
    JoyInitialized = RESET;
  }

  if (LcdInitialized != RESET)
  {
    BSP_LCD_GLASS_DeInit();
    LcdInitialized = RESET;
  }


  /* LSE off in all modes */
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
  {
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.PLL.PLLState   = RCC_PLL_NONE;
    oscinitstruct.LSEState       = RCC_LSE_OFF;
    if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
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
  * @brief  Air Quality Sensor Init peripherals
  * @retval None
  */
void CCS811_Init(void)
{
  BSP_I2C1_Init();

  hi2c1=hbus_i2c1;
  /* Reset peripherals */

  HAL_GPIO_WritePin(nReset_GPIO_Port, nReset_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(nReset_GPIO_Port, nReset_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(NWAKE_GPIO_Port, NWAKE_Pin, GPIO_PIN_RESET);
    /* Init sensor */
  HAL_Delay(100);
}

/**
  * @brief  Air Quality Sensor Status Check
  * @retval None
  */
AirQuality_StatusTypeDef CCS811_Status(void)
{
  AirQuality_StatusTypeDef tmp_AirQuality_Status = AirQuality_OK;
  uint8_t CCS811_hwid;


  /* CCS811 Connection Check */
  buf_tx[0] = HW_ID;
  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 1, 1000);
  HAL_I2C_Master_Receive(&hi2c1, CCS811_Address, buf_rx, 1, 1000);
  HAL_Delay(100);
  CCS811_hwid = buf_rx[0];

  if (CCS811_hwid != 0x81){
    tmp_AirQuality_Status = AirQuality_ERROR;
  }

  /* Status Check */
  buf_tx[0] = STATUS;
  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 1, 1000);
  HAL_I2C_Master_Receive(&hi2c1, CCS811_Address, buf_rx, 1, 1000);
  HAL_Delay(100);
  CCS811_DevStatus = buf_rx[0];

  /* CCS811 Application Validity Check */
  if ((CCS811_DevStatus & 0b00010000)>>4 != 0x01){
    tmp_AirQuality_Status = AirQuality_NOTAVAILABLE;
  }
  return tmp_AirQuality_Status;
}

/**
  * @brief  Air Quality Sensor Start Application
  * @retval None
  */
void CCS811_Start(void)
{
  /* Start Application */
  buf_tx[0] = APP_START;
  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 1, 1000);

  /* Status Check */
  buf_tx[0] = STATUS;
  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 1, 1000);
  HAL_I2C_Master_Receive(&hi2c1, CCS811_Address, buf_rx, 1, 1000);
  HAL_Delay(100);
  CCS811_DevStatus = buf_rx[0];

  /* CCS811 Application Running Check */
  /* We check if Firmware is in application mode */
  if ((CCS811_DevStatus & 0b10000000)>>7 != 0x01){
	  Error_Handler();
  }

}

/**
  * @brief  Air Quality Sensor Measuring
  * @retval None
  */
void CCS811_Measure(void)
{
  /* Measurement Mode setting */
  buf_tx[0] = MEAS_MODE;
  buf_tx[1] = buf_rx[0];

  buf_tx[1] = (CCS811_DRIVE_MODE_1 | CCS811_INT_DATARDY_0); /* Mode 1 */

  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 2, 1000);
  HAL_I2C_Master_Receive(&hi2c1, CCS811_Address, buf_rx, 1, 1000);

  /* End init sensor */

  HAL_Delay(500); /* Wait 500ms */

  /* Check status */
  buf_tx[0] = STATUS;
  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 1, 1000);
  HAL_I2C_Master_Receive(&hi2c1, CCS811_Address, buf_rx, 1, 1000);
  CCS811_DevStatus = buf_rx[0];

  /* CCS811 Application Running Check */
  /* We check if there's error */

  if ((CCS811_DevStatus & 0b00000001) != 0x00){
    Error_Handler();
  }
  /* End check status */

  HAL_Delay(500); /* Wait 500ms */
}

/**
  * @brief  Air Quality Sensor get Value
  * @retval None
  */
AirQuakity_ValTypeDef CCS811_GetValue(void)
{
  AirQuakity_ValTypeDef  tmp_AirQuakity_Val={0};

  buf_tx[0] = STATUS;
  HAL_I2C_Master_Transmit(&hi2c1, CCS811_Address, buf_tx, 1, 1000);
  HAL_I2C_Master_Receive(&hi2c1, CCS811_Address, buf_rx, 1, 1000);
   CCS811_DevStatus = buf_rx[0];
   if (( CCS811_DevStatus & 0x08 ) != 0)
   {
     /* Read CO2 and TVOC DATA */
     HAL_I2C_Mem_Read(&hi2c1, CCS811_Address, ALG_RESULT_DATA, 1, buf_rx, 4, 100);

     tmp_AirQuakity_Val.CO2PPM = (buf_rx[0]<<8)|(buf_rx[1]);
     tmp_AirQuakity_Val.TVOCPPB = (buf_rx[2]<<8)|(buf_rx[3]);
   }
   return tmp_AirQuakity_Val;
}

/**
  * @brief  Convert Integer values to Char
  * @retval None
  */
void Convert_IntegerIntoChar(float number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, tenthousand=0, hundredthousand=0;

  units = (((((uint32_t)number%100000)%10000)%1000)%100)%10;
  tens = ((((((uint32_t)number-units)/10)%10000)%1000)%100)%10;
  hundreds = ((((uint32_t)(number-tens-units)/100)%1000)%100)%10;
  thousands = ((((uint32_t)number-hundreds-tens-units)/1000)%100)%10;
  tenthousand = (((uint32_t)number-thousands-hundreds-tens-units)/10000)%10;
  hundredthousand = (((uint32_t)number-tenthousand-thousands-hundreds-tens-units)/100000);

  *(p_tab+5) = units + 0x30;
  *(p_tab+4) = tens + 0x30;
  *(p_tab+3) = hundreds + 0x30;
  *(p_tab+2) = thousands + 0x30;
  *(p_tab+1) = tenthousand + 0x30;
  *(p_tab+0) = hundredthousand + 0x30;
}


/**
  * @brief  Configure system clock frequency to 2MHz (MSI range 5).
  * @param  None.
  * @retval System Status.
  */
static void SystemClock_2MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_OFF;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_5; /* MSI = 2MHz */
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_NONE;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2; /* 24MHz */
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2; /* 24MHz */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
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

void SystemPeripheralClockDisable(void)
{
  
  /* Enforcing true STOP and STANDBY modes, by disabling their emulation. */
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();
  __HAL_RCC_DBGMCU_CLK_DISABLE();
  
  /* Disable remaining clocks */
  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  
  __HAL_RCC_I2C1_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  
  HAL_PWREx_DisablePullUpPullDownConfig();

  RCC->AHBSMENR  = 0x0;
  RCC->APBSMENR1 = 0x10000600;
  RCC->APBSMENR2 = 0x0;
  CLEAR_REG(RCC->IOPSMENR);
}