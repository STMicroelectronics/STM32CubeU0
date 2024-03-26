/**
  ******************************************************************************
  * @file    Templates/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "stm32u0xx_hal.h"

/** @addtogroup Templates
  * @{
  */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef hcom_uart[COMn];
FLASH_OBProgramInitTypeDef OptionsBytesInit={0};
/* Private functions ---------------------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

COM_InitTypeDef COM_Init;
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Init LED4 */
  BSP_LED_Init(LED4);

  /* Init BUTTON USER */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /*Init COM parameters */
  COM_Init.BaudRate   = 115200;
  COM_Init.WordLength = COM_WORDLENGTH_8B;
  COM_Init.StopBits   = COM_STOPBITS_1;
  COM_Init.Parity     = COM_PARITY_NONE;
  COM_Init.HwFlowCtl  = COM_HWCONTROL_NONE;
  BSP_COM_Init(COM1, &COM_Init);

  /* Output a message on Hyperterminal using printf function */
  printf("\n\r********* Application Started *********\n\r");


  /* Unlock Flash Control register and Option Bytes */
  HAL_FLASH_Unlock();
  HAL_FLASH_OB_Unlock();

  HAL_FLASHEx_OBGetConfig(&OptionsBytesInit);

  /* Choose option byte type.*/
  OptionsBytesInit.OptionType = OPTIONBYTE_RDP;

  while (1)
  {

    /* Wait for BUTTON_USER is released */
    if (BSP_PB_GetState(BUTTON_USER) == RESET)
    {
      while (BSP_PB_GetState(BUTTON_USER) == RESET);

      if (OptionsBytesInit.RDPLevel == OB_RDP_LEVEL_0)
      {
        /* Configure option byte parameters values.*/
        OptionsBytesInit.RDPLevel = OB_RDP_LEVEL_1;

        /* Program Option Bytes */
        HAL_FLASHEx_OBProgram(&OptionsBytesInit);

        /* Launch Option Bytes Loading */
        HAL_FLASH_OB_Launch();
      }
      else if (OptionsBytesInit.RDPLevel == OB_RDP_LEVEL_1)
      {
        /* Configure option byte parameters values.*/
        OptionsBytesInit.RDPLevel = OB_RDP_LEVEL_0;

        /* Program Option Bytes */
        HAL_FLASHEx_OBProgram(&OptionsBytesInit);

        /* Launch Option Bytes Loading */
        HAL_FLASH_OB_Launch();
      }

    }
    else
    {
      if (OptionsBytesInit.RDPLevel == OB_RDP_LEVEL_0)
      {
        printf("\n\r------------ Running on RDP0 ------------\n\r");
        BSP_LED_Toggle(LED4);
        HAL_Delay(200);
      }
      else
      {
        printf("\n\r------------ Running on RDP1 ------------\n\r");
        BSP_LED_Toggle(LED4);
        HAL_Delay(1000);
      }
    }
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            Flash Latency(WS)              = 1
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
   if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_11; /* MSI = 48MHz */
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
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 );
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    /* HAL RCC configuration error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Infinite loop */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;


  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();


  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
}
/**
  * @}
  */

