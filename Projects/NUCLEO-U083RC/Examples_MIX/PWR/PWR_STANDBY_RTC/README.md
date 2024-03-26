## <b>PWR_STANDBY_RTC Example Description</b>

How to enter the Standby mode and wake up from this mode by using an external 
reset or the RTC wakeup timer through the STM32U0xx RTC and RCC HAL, 
and LL API (LL API use for maximizing performance).

It allows to measure the current consumption in STANDBY mode with RTC enabled.

In the associated software, the system clock is set to 48 MHz and the SysTick is 
programmed to generate an interrupt each 1 ms.

The Low Speed Internal (LSI) clock is used as RTC clock source by default.
EXTI_Line20 is internally connected to the RTC Wakeup event.

The system automatically enters STANDBY mode 5 sec. after start-up. The RTC wake-up 
is configured to generate an interrupt on rising edge about 33 sec. afterwards.

Current consumption in STANDBY mode with RTC feature enabled can be measured during that time.

More than half a minute is chosen to ensure current convergence to its lowest operating point.

Note: Due to LSI frequency variations, wake-up time is not guarantee. Adjustments need to be
done after getting the real measurement of LSI oscillator (or if available, LSE oscillator can
be used as well)

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET.

LED4 is used to monitor the system state as follows:

 - LED4 toggling: system in RUN mode
 - LED4 off : system in STANDBY mode

#### <b>Notes</b>

  1.  These steps are repeated in an infinite loop.
 
  2.  To measure the current consumption in STANDBY mode, remove JP5 jumper 
      and connect an amperemeter to JP5 to measure IDD current.     


  3.  This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M0+ core is no longer clocked during low power mode 
       so debugging features are disabled.

  4.  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

  5.  Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
  6.  The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      

### <b>Keywords</b>

Power, PWR, stop mode, wake-up, RTC, Interrupt, low power mode

### <b>Directory contents</b>

  - PWR/PWR_STANDBY_RTC/Inc/stm32u0xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY_RTC/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - PWR/PWR_STANDBY_RTC/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY_RTC/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - PWR/PWR_STANDBY_RTC/Src/stm32u0xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY_RTC/Src/main.c                   Main program
  - PWR/PWR_STANDBY_RTC/Src/stm32u0xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices

  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U083RC Set-up :
  
    - LED4 connected to PA.05 pin

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Once the image is loaded, power off the NUCLEO board in unplugging
   the power cable then power on the board again 
 - Run the example
