## <b>PWR_STANDBY Example Description</b>

How to enter the Standby mode and wake up from this mode by using an external reset or the WKUP pin.

In the associated software, the system clock is set to 2 MHz, an EXTI line
is connected to the pin PC.05 and configured to generate an
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LED3 is toggled in order to indicate whether the MCU is in Standby or Run mode.

When a falling edge is detected on the EXTI line, an interrupt is generated and the system wakes up
the program then checks and clears the standby flag.
After clearing the standby flag, the software enables wake-up pin PWR_WAKEUP_PIN5 connected to PC.05, then
the corresponding flag indicating that a wakeup event was received from the PWR_WAKEUP_PIN5 is cleared.
Finally, the system enters again Standby mode causing LED3 to stop toggling.

A falling edge on WKUP pin will wake-up the system from Standby.
Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from Standby mode, program execution restarts in the same way as after
a RESET and LED3 restarts toggling.

Two leds LED3 and LED5 are used to monitor the system state as follows:

 - LED5 ON: configuration failed (system will go to an infinite loop)
 - LED3 toggling: system in Run mode
 - LED3 off : system in Standby mode

#### <b>Notes</b>
   1. To measure the current consumption in Standby mode, remove JP7 jumper
      and connect an amperemeter to JP7 to measure IDD current.

   2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

   3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset

### <b>Directory contents</b>

  - PWR/PWR_STANDBY/Inc/stm32u0xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                         Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32u0xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                         Main program
  - PWR/PWR_STANDBY/Src/stm32u0xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083RCTx devices

  - This example has been tested with STMicroelectronics STM32U083C-DK
    board and can be easily tailored to any other supported device
    and development board.

  - STM32U083C-DK Set-up
    - Use LED3 and LED5 connected respectively to PC.13 and PB.02 pins
    - WakeUp Pin PWR_WAKEUP_PIN5 connected to PC.05
	- Apply an external variable voltage on PC.05 (pin 8 on connector CN10) make it vary from 0 to 3.3V
    - If no power supply available : connect it to GND then plug it to 3.3V on the STM32 board

### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Unplug then Plug STLINK connection to perform a power-on-reset
 - Run the example
