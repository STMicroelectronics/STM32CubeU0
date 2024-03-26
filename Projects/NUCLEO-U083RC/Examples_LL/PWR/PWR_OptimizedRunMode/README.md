## <b>PWR_OptimizedRunMode Example Description</b>

How to increase/decrease frequency and VCORE and how to enter/exit the
Low-power run mode.

In the associated software, the system clock is set to 48MHz, an EXTI line
is connected to the User push-button through PC.13 and configured to generate an
interrupt on falling edge upon key press.

After start-up LED4 is toggling FAST (100ms blinking period), indicates that device
is running at 48MHz.

LED4 toggling speed is controlled by variable "uhLedBlinkSpeed".

The User push-button can be pressed at any time to change Frequency, VCore(VOS)
and Low Power Run mode.

Initial STATE:
--> Freq: 48MHz, VCore 1.2V, Core in Run Mode
--> LED4 toggling FAST (100ms)   - wait User push-button action

STATE 2:
User push-button pressed:
--> Freq: 16MHz, VCore 1.0V, Core in Run Mode
--> LED4 toggling MEDIUM (200ms) - wait User push-button action

STATE 3:
User push-button pressed:
--> Freq:  100KHz, VCore 1.0V, Core in Low Power Run Mode
--> LED4 toggling SLOW (400ms)   - wait User push-button action

STATE 4:
User push-button pressed:
--> Freq: 16MHz, VCore 1.0V, Core in Run Mode
--> LED4 toggling MEDIUM (200ms) - wait User push-button action

Final STATE:
User push-button pressed:
--> Freq: 48MHz, VCore 1.2V, Core in Run Mode
--> LED4 toggling FAST (100ms) in infinite loop

      NB: LED4 has an impact on power consumption.
          Remove LED4 blinking to have a constant power consumption,
          comment line  "#define USE_LED" in main.c file

#### <b>Notes</b>

  1. This example may not be used in debug mode depending on IDE and debugger
    configuration selected, due to system low frequency and low power mode
    constraints.

### <b>Keywords</b>

Power, PWR, Low-power run mode, Interrupt, VCORE, Low Power

### <b>Directory contents</b>

  - PWR/PWR_OptimizedRunMode/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - PWR/PWR_OptimizedRunMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_OptimizedRunMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_OptimizedRunMode/Src/stm32u0xx_it.c          Interrupt handlers
  - PWR/PWR_OptimizedRunMode/Src/main.c                  Main program
  - PWR/PWR_OptimizedRunMode/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U083RC Set-up
    - LED4 connected to pin PA.05
    - User push-button connected to pin PC.13

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Unplug then Plug STLINK connection to perform a power-on-reset
 - Run the example