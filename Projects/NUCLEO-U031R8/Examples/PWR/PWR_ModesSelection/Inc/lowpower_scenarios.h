/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/lowpower_scenarios.h
  * @author  MCD Application Team
  * @brief   Header for lowpower_scenarios.c module
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
#ifndef LOWPOWER_SCENARIOS_H
#define LOWPOWER_SCENARIOS_H

/* Includes ------------------------------------------------------------------*/
#include "system_config.h"
#include "console.h"

/* Exported types ------------------------------------------------------------*/

/** @defgroup  Scenario Status Enumeration Definition.
  * @{
  */
typedef enum
{
  SCENARIO_OK    = 0x00, /*!< Scenario state OK            */
  SCENARIO_ERROR = 0x01  /*!< Scenario state ERROR         */
} Scenario_StatusTypeDef;

/**
  * @brief  System Configuration Structure Definition.
  */
typedef struct
{
  System_LowPowerModeScenarioTypeDef   lowpower_mode;     /*!< Specifies system low power mode configuration.  */
  char* lowpower_mode_str;
  char* rtc_state_str;
  char* regulator_str;
  char* clock_str;
  uint32_t                     ram_retention;            /*!< Specifies ram retention configuration.          */
  char* ram_retention_str;
} LowPower_ScenarioTypeDef;

/* Exported constants --------------------------------------------------------*/
#define SCENARIO_CMDBUF_SIZE (0x02U)
#define DUMMY_PATTERN        (0xFFU)

/* Header buffer */
static const char header_limiter_msg[] = " ******************************************************************************************************************** \r\n";
static const char header_start_msg[]   = " *                                 Low power mode measure current consumption example                               * \r\n";

/* Configuration selection buffer */
static const char power_mode_select_msg[]    = " Select system power mode index:                    \r\n";

/* Low power buffer */
static const char *const lowpower_mode_msg[] =
{
  " Power mode : SHUTDOWN                              \r\n",
  " Power mode : STANDBY                               \r\n",
  " Power mode : STANDBY + RTC                         \r\n",
  " Power mode : STANDBY + RTC + SRAM2                 \r\n",
  " Power mode : STOP2                                 \r\n",
  " Power mode : STOP2 + RTC                           \r\n",
  " Power mode : STOP1 + MR OFF                        \r\n",
  " Power mode : STOP1 + MR OFF + RTC                  \r\n",
  " Power mode : LP-Sleep 2MHz - FLASH OFF             \r\n",
  " Power mode : LP-Run 2MHz - FLASH ART ON            \r\n",
  " Power mode : SLEEP Range 2 - 16MHz - FLASH ART ON  \r\n",
  " Power mode : SLEEP Range 1 - 48MHz - FLASH ART ON  \r\n",
  " Power mode : RUN Range 2 - 16MHz - FLASH ART ON    \r\n",
  " Power mode : RUN Range 1 - 48MHz - FLASH ART ON    \r\n",
};

/* Footer buffer */
static const char system_lowpower_config_msg[] = " Your system low power configuration is as follow : \r\n";
static const char start_measure_msg[]          = " ***************************           You can start measuring power consumption           ************************** \r\n";

/* Index  buffer */
static const char index0_msg[]  = "   00    -->";
static const char index1_msg[]  = "   01    -->";
static const char index2_msg[]  = "   02    -->";
static const char index3_msg[]  = "   03    -->";
static const char index4_msg[]  = "   04    -->";
static const char index5_msg[]  = "   05    -->";
static const char index6_msg[]  = "   06    -->";
static const char index7_msg[]  = "   07    -->";
static const char index8_msg[]  = "   08    -->";
static const char index9_msg[]  = "   09    -->";
static const char index10_msg[] = "   10    -->";
static const char index11_msg[] = "   11    -->";
static const char index12_msg[] = "   12    -->";
static const char index13_msg[] = "   13    -->";

/* Empty line_space buffer */
static const char empty_line_msg[]  = "\r\n";
static const char empty_space_msg[] = "  ";

/* Cursor buffer */
static const char user_cursor_msg[] = "=> ";

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
Scenario_StatusTypeDef lowpower_header_scenario(LowPower_ScenarioTypeDef *scenario);
Scenario_StatusTypeDef lowpower_footer_scenario(LowPower_ScenarioTypeDef *scenario);
Scenario_StatusTypeDef lowpower_get_scenario(LowPower_ScenarioTypeDef *scenario);
Scenario_StatusTypeDef lowpower_config_scenario(LowPower_ScenarioTypeDef *scenario);

#endif /* LOWPOWER_SCENARIOS_H */
