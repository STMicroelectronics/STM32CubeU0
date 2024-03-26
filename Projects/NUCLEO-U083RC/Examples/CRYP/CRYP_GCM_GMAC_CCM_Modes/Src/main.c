/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file     CRYP/CRYP_GCM-GMAC_CCM_Modes/Src/main.c
  * @author   MCD Application Team
  * @brief    This example provides a short description of how to use the CRYP
  *            AES peripheral to encrypt and decrypt data as well as to compute
  *           an authentication tag using AES-based GCM algorithm with a 128-bit
  *           long key, compute an authentication tag using AES-based GMAC
  *            algorithm with a 256-bit long key, encrypt data as well as
  *           to compute an authentication tag using AES-based CCM algorithm
  *           with a 256-bit long key.
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRYP_HandleTypeDef hcryp;
uint32_t pKeyAES[4] = {0xC939CC13,0x397C1D37,0xDE6AE0E1,0xCB7C423C};
uint32_t pInitVectAES[4] = {0xB3D8CC01,0x7CBB89B3,0x9E0F67E2,0x00000002};
__ALIGN_BEGIN static const uint32_t HeaderAES[4] __ALIGN_END = {
                            0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f};
DMA_HandleTypeDef hdma_aes_in;
DMA_HandleTypeDef hdma_aes_out;

/* USER CODE BEGIN PV */
#if (USE_VCP_CONNECTION == 1)
/**
  * @brief Defines related to Timeout to uart transmission
  */
#define UART_TIMEOUT_VALUE  1000 /* 1 Second */

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/**
  * @brief  Retargets the C library printf function to the USARTx.
  * @param  ch: character to send
  * @param  f: pointer to file (not used)
  * @retval The character transmitted
  */


{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE);

  return ch;
}

#endif/* USE_VCP_CONNECTION */

/******************************************************************************/
/*                                                                            */
/*                    GCM input / expected output data                        */
/*                                                                            */
/******************************************************************************/
  /* Initialization Vector used for GCM */
uint32_t InitVector[4] = {0xb3d8cc01, 0x7cbb89b3 , 0x9e0f67e2 , 0x00000002};

/* Header message, will be authenticated but not encrypted */
uint32_t HeaderMessage[4] = {0x24825602,0xbd12a984,0xe0092d3e,0x448eda5f};

  /* Payload message, will be authenticated and encrypted */
uint32_t Plaintext[4] = {0xc3b3c41f,0x113a31b7,0x3d9a5cd4,0x32103069};

  /* Expected Cypher TAG */
uint32_t Ciphertext[4] = {0x93FE7D9E,0x9BFD1034,0x8A5606E5,0xCAFA7354};

  /* Expected authentication text */
uint32_t ExpectedTAG[4]={0x0032A1DC,0x85F1C978,0x6925A2E7,0x1D8272DD};


/******************************************************************************/
/*                                                                            */
/*                    GMAC input / expected output data                       */
/*                                                                            */
/******************************************************************************/
  /* AESkey used for GMAC */
uint32_t AES256Key_GMAC[8] = {0x691D3EE9,0x09D7F541,0x67FD1CA0,0xB5D76908,
                              0x1F2BDE1A,0xEE655FDB,0xAB80BD52,0x95AE6BE7};

  /* Initialization Vector used for GMAC */
uint32_t InitVector_GMAC[4] = {0xF0761E8D, 0xCD3D0001, 0x76D457ED, 0x00000002};

 /* Header message, will be authenticated but not encrypted */
uint32_t HeaderMessage_GMAC[17] = {0xEB608047,0x6E0FB0B3,0xBCB3B178,0x0002A711,
                                0xB7EA2B6E,0x08F00010,0x28C84888,0x18E868A8,
                                0x38D85898,0x04F878B8,0x24C44484,0x14E464A4,
                                0x34D45494,0x0CF474B4,0x2CCC4C8C,0x1CEC6CAC,
                                0xC0005C9C};

  /* Expected authentication TAG */
uint32_t ExpectedTAG_GMAC[4]={0xEE3E84AC, 0x6D8C3DD2,0x9F3D668C,0xE0B7D52B};


/******************************************************************************/
/*                                                                            */
/*                    CCM input / expected output data                        */
/*                                                                            */
/******************************************************************************/
  /* AESkey used for CCM */
uint32_t AES256Key_CCM[8] = {0xD346D11A,0x7117CE04,0x08089570,0x7778287C,
                             0x40F5F473,0xA9A8F2B1,0x570F6137,0x4669751A};

 /* BlockB0 used for CCM*/
uint32_t BlockB0[4] = {0x7A05C8CC,0x7732B3B4,0x7F08AF1D,0xAF000011};

 /* BlockB1 used for CCM*/
uint32_t BlockB1[4] = {0x00073421,0x5F032567,0x0B000000,0x00000000};

/* Payload message, will be authenticated and encrypted */
uint32_t aPlaintext_CCM[8] = {0xBBD88334,0x000075F6,0xF4E89F9D,0xDA50F5EA,
                              0xB1000000,0x00000000,0x00000000,0x00000000};

/* Expected Cypher text */
uint32_t Ciphertext_CCM[8] = {0xA7B7653C,0x5D600AF3,0x9CA0DB48,0x0F4F5CCE,
                              0x99000000,0x00000000,0x00000000};

/* Expected authentication TAG */
uint32_t ExpectedTAG_CCM[4] = {0x352C36D3,0x935B8894 ,0x0426A004,0x3BBAB7EE};

/******************************************************************************/
/*                                                                            */
/*                           Output buffers                                   */
/*                                                                            */
/******************************************************************************/

/* Used for storing the computed MAC (TAG) & encryption/decryption */
uint32_t TAG[4] = {0};
uint32_t Decryptedtext[8] = {0}; /*size buffer should be 8 not 5 other wise WA
                                will break the TAG generated after decryption */
uint32_t Encryptedtext[8] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_DMA_Init(void);
static void MX_AES_Init(void);
/* USER CODE BEGIN PFP */
void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint8_t Size);
/* Private functions ---------------------------------------------------------*/
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
extern void initialise_monitor_handles(void);
#endif
AES_TypeDef * AES_DBG;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
  initialise_monitor_handles();
  printf("Semihosting Test...\n\r");
#endif

  __HAL_RCC_AES_CLK_ENABLE();
  AES_DBG = (AES_TypeDef *) AES_BASE;
#if (USE_VCP_CONNECTION == 1)
  COM_InitTypeDef COM_Init;
#endif /* USE_VCP_CONNECTION */

  /* STM32U0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_AES_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LED4 */
  BSP_LED_Init(LED4);
#if (USE_VCP_CONNECTION == 1)
  /* Configure COM port */
  COM_Init.BaudRate   = 115200;
  COM_Init.WordLength = COM_WORDLENGTH_8B;
  COM_Init.StopBits   = COM_STOPBITS_1;
  COM_Init.Parity     = COM_PARITY_NONE;
  COM_Init.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &COM_Init) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  if (BSP_COM_SelectLogPort(COM1) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
#endif /* USE_VCP_CONNECTION */

      /******************************************************************************/
    /*                                                                            */
    /*   AES 128 GCM mode authentication tag computation and message ciphering    */
    /*                         (interrupt mode)                                   */
    /*                                                                            */
    /******************************************************************************/

      /* Encryption */
    if (HAL_CRYP_Encrypt_IT(&hcryp, Plaintext, 4, Encryptedtext) != HAL_OK)
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/
    data_cmp(Encryptedtext, Ciphertext, 16);

   /* Compute the authentication TAG */
   if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE) != HAL_OK)
   {
      /* Processing Error */
      Error_Handler();
   }
    /* Check the Authentication tag with the expected one *************************/

    data_cmp(TAG, ExpectedTAG, 16);



    /******************************************************************************/
    /*                                                                            */
    /*  AES 128 GCM mode authentication tag computation and message deciphering   */
    /*                          (polling mode)                                    */
    /*                                                                            */
    /******************************************************************************/



    if (HAL_CRYP_Init(&hcryp) != HAL_OK)
    {
      Error_Handler();
    }

         /* Decryption */
    if (HAL_CRYP_Decrypt(&hcryp, Encryptedtext, 4, Decryptedtext,TIMEOUT_VALUE) != HAL_OK)
    {
      /* Processing Error */
      Error_Handler();
    }

    /* Check the encrypted text with the expected one *************************/
    data_cmp(Decryptedtext, Plaintext, 4);

   /* Compute the authentication TAG */
   if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE) != HAL_OK)
   {
      /* Processing Error */
      Error_Handler();
   }

   /* Check the Authentication tag with the expected one *************************/
   data_cmp(TAG, ExpectedTAG, 16);

  printf("===================================================================\n ");
  printf("\n\r GCM ciphering/deciphering/authentication tag computation done.\n ");
  printf("No issue detected.\n\n ");


    /******************************************************************************/
    /*                                                                            */
    /*             AES 256 GMAC mode authentication tag computation               */
    /*                            (DMA mode)                                      */
    /*                                                                            */
    /******************************************************************************/

       if (HAL_CRYP_DeInit(&hcryp) != HAL_OK)
    {
        Error_Handler();
    }



    hcryp.Init.DataType      = CRYP_DATATYPE_1B;
    hcryp.Init.KeySize       = CRYP_KEYSIZE_256B;
    hcryp.Init.pKey          = AES256Key_GMAC;
    hcryp.Init.Algorithm     = CRYP_AES_GCM_GMAC;
    hcryp.Init.pInitVect     = InitVector_GMAC;
    hcryp.Init.Header        = HeaderMessage_GMAC;
    hcryp.Init.HeaderSize    = 17;


    /****************************************************************************/
    /*                        AES 256 GMAC mode                                 */
    /*                            (DMA mode)                                    */
    /****************************************************************************/
       /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp) != HAL_OK)
    {
        Error_Handler();
    }

      /* Encryption */
    if (HAL_CRYP_Encrypt_DMA(&hcryp, NULL, 0, NULL)!= HAL_OK)
    {
      /* Processing Error */
      Error_Handler();
    }
while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY){};

   /* Compute the authentication TAG */
   if (HAL_CRYPEx_AESGCM_GenerateAuthTAG(&hcryp, TAG, TIMEOUT_VALUE) != HAL_OK)
   {
      /* Processing Error */
      Error_Handler();
   }
    /* Check the Authentication tag with the expected one *************************/

    data_cmp(TAG, ExpectedTAG_GMAC, 16);

  printf("==============================================\n ");
  printf("\n\r GMAC authentication tag computation done.\n ");
  printf("No issue detected.\n ");

    /******************************************************************************/
    /*                                                                            */
    /*                         AES 256 CCM mode                                   */
    /*                          (polling mode)                                    */
    /*                                                                            */
    /******************************************************************************/

    if (HAL_CRYP_DeInit(&hcryp) != HAL_OK)
    {
        Error_Handler();
    }

    hcryp.Init.DataType      = CRYP_DATATYPE_32B ;
    hcryp.Init.KeySize       = CRYP_KEYSIZE_256B ;
    hcryp.Init.pKey          = AES256Key_CCM;
    hcryp.Init.Algorithm     = CRYP_AES_CCM;
    hcryp.Init.B0            = BlockB0;
    hcryp.Init.Header        = BlockB1;
    hcryp.Init.HeaderSize    = 4;
    hcryp.Init.DataWidthUnit     = CRYP_DATAWIDTHUNIT_BYTE;


    /****************************************************************************/
    /*                                  AES mode CCM                            */
    /****************************************************************************/
      /* Set the CRYP parameters */
    if (HAL_CRYP_Init(&hcryp) != HAL_OK)
    {
        Error_Handler();
    }

      /* Encryption */
    if (HAL_CRYP_Encrypt(&hcryp, aPlaintext_CCM, 17, Encryptedtext,TIMEOUT_VALUE) != HAL_OK)
    {
      /* Processing Error */
      Error_Handler();
    }
    /* Check the encrypted text with the expected one *************************/

    Encryptedtext[4]=Encryptedtext[4]&0xFF000000; // to compare only first byte
    data_cmp(Encryptedtext, Ciphertext_CCM, 17);

   /* Compute the authentication TAG */
   if (HAL_CRYPEx_AESCCM_GenerateAuthTAG(&hcryp,TAG, TIMEOUT_VALUE) != HAL_OK)
   {
      /* Processing Error */
      Error_Handler();
   }

    /* Compare the GMAC authentication tag with the expected one *******************/
    data_cmp(TAG, ExpectedTAG_CCM, 16);

  printf("\n==============================================\n ");
  printf("\n\r CMAC authentication tag computation done.\n ");
  printf("No issue detected.\n ");

  /* Turn LED1 on */
  BSP_LED_On(LED4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV8;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief AES Initialization Function
  * @param None
  * @retval None
  */
static void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectAES;
  hcryp.Init.Algorithm = CRYP_AES_GCM_GMAC;
  hcryp.Init.Header = (uint32_t *)HeaderAES;
  hcryp.Init.HeaderSize = 4;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/* USER CODE BEGIN 4 */
/**
  * @brief  buffer data comparison
  * @param
  * @retval None
  */
void data_cmp(uint32_t *EncryptedText, uint32_t *RefText, uint8_t Size)
{
  /*  Before starting a new process, you need to check the current state of the peripheral;
      if it is busy you need to wait for the end of current transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      process, but application may perform other tasks while transfer operation
      is ongoing. */
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY)
  {
  }

  /*##-3- Check the encrypted text with the expected one #####################*/
  if(memcmp(EncryptedText, RefText, Size) != 0)
  {
    Error_Handler();
  }
  else
  {
    /* Correct encryption */
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  while (1)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(1000);
  }
  /* USER CODE END Error_Handler_Debug */
}

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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */