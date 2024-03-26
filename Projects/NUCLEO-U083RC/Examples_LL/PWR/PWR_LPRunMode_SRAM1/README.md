## <b>PWR_LPRunMode_SRAM1 Example Description</b>

How to execute code in Low-power run mode from SRAM1.

In the associated software, the system clock is set to 48 MHz, an EXTI line
is connected to the User push-button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.

 - After start-up LED4 is toggling FAST(100-ms blinking period), indicates that device is running at 48 MHz.

The User push-button can be pressed to set system in LowPowerRun mode (Freq: 100KHz).

 - In this case LED4 toggling SLOW(400-ms blinking period).

LED4 toggling speed is controlled by variable "uhLedBlinkSpeed".


To measure the current consumption in Standby mode, remove JP5 jumper 
      and connect an ampere meter to JP5 to measure IDD current.
      NB: LED4 have an impact on power consumption. 
          Remove LED4 blinking to have a stable power consumption, 
          comment line  "#define USE_LED" in main.c file  

#### <b>Notes</b>

  1. This example is executed from SRAM1 to decrease power consumption.

  2. Flash is deactivated during LowPowerRun mode activation.

  3. This example cannot be executed in standalone mode, so using debugger is mandatory to run this example.

### <b>Keywords</b>

Power, PWR, Low Power

### <b>Directory contents</b>

  - PWR/PWR_LPRunMode_SRAM1/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - PWR/PWR_LPRunMode_SRAM1/Inc/main.h                  Header for main.c module
  - PWR/PWR_LPRunMode_SRAM1/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_LPRunMode_SRAM1/Src/stm32u0xx_it.c          Interrupt handlers
  - PWR/PWR_LPRunMode_SRAM1/Src/main.c                  Main program
  - PWR/PWR_LPRunMode_SRAM1/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U083RC Set-up
    - LED4 connected to PA.05 pin
    - User push-button connected to pin PC.13 (EXTI_Line13)

### <b>How to use it ?</b>  

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files
 - In using ST-Link Utility tool, load your image into target memory SRAM1 area 
 at start address 0x20000000, having checked beforehand the "Reset after programming" 
 box. Make sure option bytes are correctly set to select SRAM1 as boot area (refer to 
 Reference Manual if needed)
 - Run the example
