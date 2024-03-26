## <b>PWR_STOP1 Example Description</b>

How to enter the STOP 1 mode and wake up from this mode by using external 
reset or wakeup interrupt (all the RCC function calls use RCC LL API 
for minimizing footprint and maximizing performance).

In the associated software, the system clock is set to 48 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED4 is toggled in order to indicate whether the MCU is in STOP 1 mode 
or RUN mode.

5 seconds after start-up, the system automatically enters STOP 1 mode and 
LED4 stops toggling.

The User push-button can be pressed at any time to wake-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering STOP 1 mode again. 

LED4 is used to monitor the system state as follows:

 - LED4 toggling: system in RUN mode
 - LED4 off : system in STOP 1 mode
 - LED4 ON: configuration failed (system will go to an infinite loop)

These steps are repeated in an infinite loop.

Connect an amperemeter to jumper JP5 to measure the IDD current.

### <b>Notes</b>

 1. This example can not be used in DEBUG mode due to the fact 
    that the Cortex-M0+ core is no longer clocked during low power mode 
    so debugging features are disabled.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, stop mode, wake-up, external reset, Interrupt, low power mode    

### <b>Directory contents</b>

  - Examples_MIX/PWR/PWR_STOP1/Inc/stm32u0xx_conf.h         HAL Configuration file
  - Examples_MIX/PWR/PWR_STOP1/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - Examples_MIX/PWR/PWR_STOP1/Inc/main.h                   Header file for main.c
  - Examples_MIX/PWR/PWR_STOP1/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - Examples_MIX/PWR/PWR_STOP1/Src/stm32u0xx_it.c           Interrupt handlers
  - Examples_MIX/PWR/PWR_STOP1/Src/main.c                   Main program
  - Examples_MIX/PWR/PWR_STOP1/Src/stm32u0xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices
    

  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U083RC set-up:
    - LED4 connected to PA.05 pin
    - User push-button connected to pin PC.13 (EXTI_Line13)
    

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - Unplug then Plug STLINK connection to perform a power-on-reset