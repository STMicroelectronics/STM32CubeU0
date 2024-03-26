## <b>PWR_STOP2 Example Description</b> 

This example shows how to enter the system in STOP 2 mode and wake-up from this
mode using external RESET or wake-up interrupt.

In the associated software, the system clock is set to 48 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED4 is toggled in order to indicate whether the MCU is in STOP 2 mode 
or RUN mode.

5 seconds after start-up, the system automatically enters STOP 2 mode and 
LED4 stops toggling.

The User push-button can be pressed at any time to wake-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering STOP 2 mode again. 

LED4 is used to monitor the system state as follows:

 - LED4 toggling: system in RUN mode
 
 - LED4 off : system in STOP 2 mode

These steps are repeated in an infinite loop.

#### <b>Notes</b>

  1. To measure the current consumption in STOP 2 mode, remove JP5 jumper 
      and connect an amperemeter to JP5 to measure IDD current.     

  2. This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M0+ core is no longer clocked during low power mode 
      so debugging features are disabled.

  3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  4. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power,

### <b>Directory contents</b>

  - PWR/PWR_STOP2/Inc/stm32u0xx_nucleo_conf.h  BSP configuration file
  - PWR/PWR_STOP2/Inc/stm32u0xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP2/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - PWR/PWR_STOP2/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP2/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - PWR/PWR_STOP2/Src/stm32u0xx_it.c           Interrupt handlers
  - PWR/PWR_STOP2/Src/main.c                   Main program
  - PWR/PWR_STOP2/Src/stm32u0xx_hal_msp.c      HAL MSP module

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

