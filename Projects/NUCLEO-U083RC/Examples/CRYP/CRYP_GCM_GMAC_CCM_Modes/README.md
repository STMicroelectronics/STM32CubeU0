## <b>CRYP_GCM_GMAC_CCM_Modes Example Description</b>

This example describes how to use the CRYP AES peripheral to encrypt and decrypt data as well as to compute an authentication tag using 
AES-based GCM algorithm with a 128-bit long key,compute an authentication tag using AES-based GMAC algorithm with a 256-bit long key, 
encrypt data as well as tocompute an authentication tag using AES-based CCM algorithm with a 256-bit long key.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U083RCTx Devices :
The CPU at 48 MHz.

For GCM algorithm :

 - Ciphering/Tag computation with a 128-bit long key is used in interrupt mode with data type set to 32-bit (no swapping).
 - Deciphering/Tag computation is used in polling mode.
 - The ciphering output is used as deciphering input.

For GMAC algorithm :

 - Tag computation with a 256-bit long key is used in DMA mode with data type set to 1-bit (bit swapping).

For CCM algorithm :

 - CCM encryption and tag computation with a 256-bit long key is used in polling mode with data type set to 32-bit (no swapping).  

The example unrolls as follows:

- GCM encryption and tag computation (Header + Plain Data --> Encrypted Data + Authentication tag)
- GCM decryption and tag computation (Header + Encrypted Data --> Decrypted Data + Authentication tag)
- GMAC tag computation (Header --> Authentication tag)
- CCM encryption and tag computation (Header + Plain Data --> Encrypted Data + Authentication tag)

When resorting to IAR Embedded Workbench IDE, AES algorithms processing status
are displayed on debugger Terminal I/O as follows: View --> Terminal I/O.

When resorting to MDK-ARM KEIL IDE, AES algorithms processing status 
are displayed on debugger as follows:  View --> Serial Viewer --> Debug (printf) Viewer.

When resorting to STM32CubeIDE:
Command Code is displayed on debugger as follows: Window--> Show View--> Console.

In Debug configuration :

- Window\Debugger, select the Debug probe : ST-LINK(OpenOCD)
- Window\Startup, add the command "monitor arm semihosting enable"

Results can be displayed on PC (through HyperTerminal or TeraTerm) by Virtual Com Port
by enabling USE_VCP_CONNECTION compilation switch with adding (#define USE_VCP_CONNECTION  1) in
stm32u083c_discovery_conf.h and with the proper configuration
(115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).

The green LED (LED4) is turned ON, in case the encryption/decryption and tag are
computed correctly, in case there is a mismatch the LED4 is blinking slowly.
	
### <b>Keywords</b>

Security, CRYP, AES, TrustZone disabled

### <b>Directory contents</b>

  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32u0xx_nucleo_conf.h BSP configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/stm32u0xx_it.c          Interrupt handlers
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/main.c                  Main program
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_GCM_GMAC_CCM_Modes/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RC devices : make sure that the AES is enabled in the device.

  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
