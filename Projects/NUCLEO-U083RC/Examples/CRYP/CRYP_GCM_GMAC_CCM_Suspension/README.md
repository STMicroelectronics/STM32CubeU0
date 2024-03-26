## <b>CRYP_GCM_GMAC_CCM_Suspension Example Description</b>

How to use the CRYP AES peripheral to suspend then resume the AES GCM and GMAC 
CCM processing of a message in order to carry out the encryption, decryption 
or authentication tag computation of a higher-priority message(CCM).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U083RCTx Devices :
The CPU at 16 MHz.

For GCM algorithm:

 - AES 128 GCM header phase of a low priority message (32-bit data type) is interrupted by the
 - AES 256 CCM processing of a high priority message (32-bit data type).  
 - When the latter is done (encryption and tag generation), the low priority GCM header
   phase is resumed.  

For GMAC algorithm:

 - AES 256 GMAC header phase of a low priority message (32-bit data type) is interrupted by the
 - AES 256 CCM processing of a high priority message (32-bit data type).  
 - When the latter is done (encryption and tag generation), the low priority GMAC header
   phase is resumed followed by the GMAC final p

The example unrolls as follows.

 - An AES GCM low-priority message processing is suspended due to the interruption by a high priority messages CCM processing.   
    Low priority message GCM header then GCM payload phases are in turn suspended then resumed.  
 - Next, an AES GMAC low-priority message processing is suspended due to the interruption by a high priority messages CCM processing.   
    Low-priority message GMAC header phase is suspended then resumed.        

Low and high-priority blocks AES outputs are compared with expected outputs. 

When all ciphering and deciphering operations are successful, (LED4) is turned on.
In case of ciphering or deciphering issue, LED4 is blinking slowly.  

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.

 2. To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

The application needs to ensure that the SysTick time base is always set to 1 millisecond
to have correct HAL operation.

### <b>Keywords</b>

Security, CRYP, AES, TrustZone disabled

### <b>Directory contents</b>

  - CRYP/CRYP_SAES_WrapKey/Inc/stm32u0xx_nucleo_conf.h            BSP Configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Src/stm32u0xx_it.c          Interrupt handlers
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Src/main.c                  Main program
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_GCM_GMAC_CCM_Suspension/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This template has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
