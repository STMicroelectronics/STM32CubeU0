## <b>PWR_SHUTDOWN Example Description</b>

how to enter the system in SHUTDOWN mode and wake-up from this
mode using external RESET or WKUP pin.

In the associated software, the system clock is set to 2 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LED4 is toggled in order to indicate whether the MCU is in SHUTDOWN or RUN mode.

When the User push-button is pressed a falling edge is detected on the EXTI line and
an interrupt is generated. In the EXTI handler routine, the wake-up pin PWR_WAKEUP_PIN2
is enabled and the corresponding wake-up flag cleared.
Then, the system enters SHUTDOWN mode causing LED4 to stop toggling.

Next, the user can wake-up the system in pressing the User push-button which is connected
to the wake-up pin PWR_WAKEUP_PIN2.
A falling edge on the wake-up pin wakes-up the system from SHUTDOWN.
Alternatively, an external RESET of the board leads to a system wake-up as well.

After wake-up from SHUTDOWN mode, program execution restarts from the beginning.
Exit from SHUTDOWN is detected through a flag set in an RTC back-up register; in
such a case, LED4 is kept on for about 4 sec. before toggling again.

User can re-enter SHUTDOWN in pressing again the User push-button.

LED4 is used to monitor the system state as follows:

 - LED4 toggling: system in RUN mode
 - LED4 is on for about 4 sec.: system is restarting and out-of-shutdown has been detected
 - LED4 off : system in SHUTDOWN mode

#### <b>Notes</b>

These steps are repeated in an infinite loop.

 1. To measure the current consumption in SHUTDOWN mode, remove IDD_JP jumper
    and connect an amperemeter to IDD_JP to measure IDD current.

 2. This example can not be used in DEBUG mode, this is due to the fact
    that the Cortex-M0+ core is no longer clocked during low power mode
    so debugging features are disabled

 3. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, EXTI, Shutdown mode, Interrupt, Wakeup, External reset, WKUP pin

### <b>Directory contents</b>

  - PWR/PWR_SHUTDOWN/Inc/stm32u0xx_nucleo_conf.h  BSP configuration file
  - PWR/PWR_SHUTDOWN/Inc/stm32u0xx_conf.h         HAL Configuration file
  - PWR/PWR_SHUTDOWN/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - PWR/PWR_SHUTDOWN/Inc/main.h                   Header file for main.c
  - PWR/PWR_SHUTDOWN/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - PWR/PWR_SHUTDOWN/Src/stm32u0xx_it.c           Interrupt handlers
  - PWR/PWR_SHUTDOWN/Src/main.c                   Main program

### <b>Hardware and Software environment</b>

  - NUCLEO-U031R8 Set-up
    - LED4 connected to PA.05 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)
    - WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13

  - This example runs on STM32U0xx devices

  - This example has been tested with STMicroelectronics NUCLEO-U031R8
    evaluation board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - UnPlug then Plug STLINK connection to perform a power-on-reset