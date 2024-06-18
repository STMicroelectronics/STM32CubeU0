## <b>CRYP_AESModes Example Description</b>

How to use the CRYP peripheral to encrypt and decrypt data using AES in chaining
modes (ECB, CBC, CTR).

In this example, the following key sizes are used: 128, 192 or 256.

For this example, a firmware is used to interface with the Crypto peripheral
in polling mode.

This example behaves as follows:

1. ECB mode:

   - AES128 ECB encryption
   - AES192 ECB encryption
   - AES256 ECB encryption
   - AES128 ECB decryption
   - AES192 ECB decryption
   - AES256 ECB decryption

2. CBC mode:

   - AES128 CBC encryption
   - AES192 CBC encryption
   - AES256 CBC encryption
   - AES128 CBC decryption
   - AES192 CBC decryption
   - AES256 CBC decryption

3. CTR mode:

   - AES128 CTR encryption
   - AES192 CTR encryption
   - AES256 CTR encryption
   - AES128 CTR decryption
   - AES192 CTR decryption
   - AES256 CTR decryption

When resorting to IAR Embedded Workbench IDE, AES algorithms processing status are displayed on :
- Debugger Terminal I/O as follows: View --> Terminal I/O.
- Hyperterminal using VCP connection as follows: General Options --> Library Options 1 --> Uncheck Buffered terminal output.


When resorting to MDK-ARM KEIL IDE:
Command Code could not be displayed on debugger but user can use the Virtual Com port of the NUCLEO-U083RC.

When resorting to STM32CubeIDE:

Command Code is displayed on debugger as follows: Window--> Show View--> Console.

In Debug configuration using Semihosting:

- Window\Debugger, select the Debug probe : ST-LINK(OpenOCD)
- Window\Startup, add the command "monitor arm semihosting enable"

In Debug and Release configurations using VCP connection :
   Command Code is displayed on PC (as HyperTerminal or TeraTerm) with proper configuration :
   - uncheck "exclude resource from build" for "syscall.c" file
   - remove "-specs=rdimon.specs -lc -lrdimon" from from cubeIDE Propreties --> C/C++ Build --> Settings
   --> Tool Settings --> MCU GCC linker --> Miscellaneous

Results can be displayed on PC (through HyperTerminal or TeraTerm) by Virtual Com Port
by enabling USE_VCP_CONNECTION compilation switch and with the proper configuration
(115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).
When all ciphering and deciphering operations are successful, LED4 is turned on.
In case of ciphering or deciphering issue, LED4 is blinking slowly.

### <b>Keywords</b>

Security, Cryptography, CRYPT, AES, ECB, CBC, CTR, MAC, USART

### <b>Directory contents</b>

  - CRYP/CRYP_AESModes/Inc/stm32u0xx_nucleo_conf.h  BSP configuration file
  - CRYP/CRYP_AESModes/Inc/stm32u0xx_hal_conf.h     HAL configuration file
  - CRYP/CRYP_AESModes/Inc/stm32u0xx_it.h           Interrupt handlers header file
  - CRYP/CRYP_AESModes/Inc/main.h                   Header for main.c module  
  - CRYP/CRYP_AESModes/Src/stm32u0xx_it.c           Interrupt handlers
  - CRYP/CRYP_AESModes/Src/main.c                   Main program
  - CRYP/CRYP_AESModes/Src/stm32u0xx_hal_msp.c      HAL MSP module
  - CRYP/CRYP_AESModes/Src/system_stm32u0xx.c       STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RC devices : make sure that the AES is enabled in the device.

  - This example has been tested with a STM32U083RCTx embedded on an
    NUCLEO-U083RC board and can be easily tailored to any other supported
    device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
