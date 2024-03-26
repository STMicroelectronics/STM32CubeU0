## <b>PWR_LPRUN Example Description</b>
 
How to enter and exit the Low-power run mode.

In the associated software, the system clock is set to 48 MHz.
The SysTick is programmed to generate an interrupt each 1 ms.

The User push-button can be pressed at any time to exit from Low Power Run. 
The software then comes back in Run mode for 5 sec. before automatically 
entering LP Run mode again. 

LED4 is used to monitor the system state as follows:

 - LED4 toggling : system in Run mode
 - LED4 off : system in LP Run mode
 - LED4 on : system in error

These steps are repeated in an infinite loop.

#### <b>Notes</b>

  1. To measure the current consumption in LP SLEEP mode, remove JP5 jumper 
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

Power, PWR, Low Power, Run mode, Interrupt, EXTI, Wakeup, External reset

### <b>Directory contents</b>

  - PWR/PWR_LPRUN/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_LPRUN/Inc/stm32u0xx_conf.h         HAL Configuration file
  - PWR/PWR_LPRUN/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - PWR/PWR_LPRUN/Inc/main.h                         Header file for main.c
  - PWR/PWR_LPRUN/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - PWR/PWR_LPRUN/Src/stm32u0xx_it.c           Interrupt handlers
  - PWR/PWR_LPRUN/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - PWR/PWR_LPRUN/Src/main.c                   Main program

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices
    
  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U083RC set-up:
    - LED4 connected to PA.05 pin
    - Use the User push-button connected to pin PC.13.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - Unplug then Plug STLINK connection to perform a power-on-reset

