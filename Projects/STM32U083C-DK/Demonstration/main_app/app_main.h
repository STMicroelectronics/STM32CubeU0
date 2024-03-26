/**
  ******************************************************************************
  * @file    app_main.h
  * @author  MCD Application Team
  * @brief   Main application (root menu) interface header file
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

#undef GLOBAL
#ifdef __APP_MAIN_C
#define GLOBAL
#else
#define GLOBAL extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
/* External variables --------------------------------------------------------*/
extern TSC_HandleTypeDef htsc;
/* Exported constants --------------------------------------------------------*/
#define DEMO_NAME_CHAR_NB         20
#define MODE_NAME_CHAR_NB         15
///* Exported types ------------------------------------------------------------*/
//I2C_HandleTypeDef hi2c1;
//extern I2C_HandleTypeDef hbus_i2c1;
/* USER CODE BEGIN ET */
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[DEMO_NAME_CHAR_NB];
  uint32_t DemoIndex;
}Demo_Typedef;

typedef struct
{
  void   (*ModeFunc)(void);
  uint8_t ModeName[MODE_NAME_CHAR_NB];
  uint32_t ModeIndex;
  uint8_t Message[6];
}StopModes_Typedef;

typedef enum {
  MODULE_OK,
  /* Module exec */
  MODULE_ERROR_PRE,
  MODULE_ERROR_EXEC,
  MODULE_ERROR_POST,
  MODULE_RESMISSIG
} MODULE_RETURN;

typedef struct AirQuakity_ValStruct AirQuakity_ValTypeDef;
struct AirQuakity_ValStruct
{
  uint16_t CO2PPM;
  uint16_t TVOCPPB;
};

typedef enum
{
  AirQuality_OK       = 0x00U,
  AirQuality_ERROR    = 0x01U,
  AirQuality_NOTAVAILABLE     = 0x02U,
} AirQuality_StatusTypeDef;

typedef enum
{
  TMODE_CELSIUS     = 0x00U,
  TMODE_FAHRENHEIT  = 0x01U,
} Temp_ModeTypedef;/* USER CODE END ET */
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define CCS811_Address 0x5A<<1
//Register Address
#define STATUS 0x00
#define MEAS_MODE 0x01
#define ALG_RESULT_DATA 0x02
#define ERROR_ID 0xE0
#define HW_ID 0x20
#define APP_START 0xF4
#define ENV_DATA 0x05

#define nReset_Pin GPIO_PIN_10
#define nReset_GPIO_Port GPIOB
#define nINT_Pin GPIO_PIN_4
#define nINT_GPIO_Port GPIOB
#if defined (USE_STM32U083C_DISCO_REVB) || defined (USE_STM32U083C_DISCO_REVC)
#define NWAKE_Pin GPIO_PIN_15
#define NWAKE_GPIO_Port GPIOA
#elif defined (USE_STM32U083C_DISCO_REVA)
#define NWAKE_Pin GPIO_PIN_6
#define NWAKE_GPIO_Port GPIOB
#endif /* USE_STM32U083C_DISCO_REVB || USE_STM32U083C_DISCO_REVC */
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)       (sizeof(x)/sizeof(Demo_Typedef))
#define COUNT_OF_MODES(x)         (sizeof(x)/sizeof(StopModes_Typedef))
#define ABS(x)                    (x < 0) ? (-x) : x
/* Exported functions prototypes ---------------------------------------------*/
void AirQuality_demo(void);
void LPModes_demo(void);
void TouchWakeup_demo(void);
void TempSensor_demo(void);
void Demo_Start(void);
void Menu_Init(void);
void Display_DemoDescription(void);
void SystemHardwareInit(void);
void SystemHardwareDeInit(void);

void Sleep_Mode(void);
void Stop1_Mode(void);
void Stop2_Mode(void);
void Standby_Mode(void);
void LpSleep_Mode(void);



void CCS811_Init(void);
AirQuality_StatusTypeDef CCS811_Status(void);
void CCS811_Start(void);
void CCS811_Measure(void);
AirQuakity_ValTypeDef CCS811_GetValue(void);

void Convert_IntegerIntoChar(float number, uint16_t *p_tab);
void SystemPower_Config(void);