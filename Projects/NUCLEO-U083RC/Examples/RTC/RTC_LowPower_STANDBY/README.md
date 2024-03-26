## <b>RTC_LowPower_STANDBY Example Description</b>

How to enter STANDBY mode and wake up from this mode using the RTC alarm event.

This event is connected to EXTI_Line20.

In the associated software, the system clock is set to 48 MHz, the SysTick is
programmed to generate an interrupt each 1 ms.

The LSI clock is used as RTC clock source by default.

    The user can use also LSE as RTC clock source.
    - The user uncomment the adequate line on the main.c file.
      @code
        #define RTC_CLOCK_SOURCE_LSI
        /* #define RTC_CLOCK_SOURCE_LSE */
      @endcode
    - Open the ioc file with STM32CubeMX and select :
      - LSE as "Crystal/Ceramic Resonator" in RCC configuration.
      - LSE as RTC clock source in Clock configuration.
    - Generate code
    LSI oscillator clock is delivered by a 32 kHz RC.
    LSE (when available on board) is delivered by a 32.768 kHz crystal.

In the associated software:

  - the system clock is set to 48 MHz.
  - the EXTI_Line20 connected internally to the RTC Alarm event is configured
    to generate an interrupt on rising edge after 15s.

The system enters STANDBY mode after configurating time, alarm and date:

- time to 23h59:55
- date to 31th of October 2014
- alarm to 00h00:10.

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET. A Check is performed on date and time to make sure system has been
woken up by alarm event.

One of the above scenario can occur.

 - LED4 is turned ON when RTC Alarm occurs.
 - LED4 toggling: HAL configuration failed (system will go to an infinite loop)

#### <b>Notes</b>

 1. This example can not be used in DEBUG mode, this is due to the fact
    that the Cortex-M0+ core is no longer clocked during low power mode
    so debugging features are disabled

 2. Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
    the RTC clock source; in this case the Backup domain will be reset in
    order to modify the RTC Clock source, as consequence RTC registers (including
    the backup registers) and RCC_CSR register are set to their reset values.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.

 4. To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function   .
    The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

System, RTC, EXTI, Internal time stamp, Low power, Standby mode, Wake up,

### <b>Directory contents</b>

  - RTC/RTC_LowPower_STANDBY/Inc/stm32u0xx_nucleo_conf.h  BSP configuration file
  - RTC/RTC_LowPower_STANDBY/Inc/stm32u0xx_hal_conf.h     HAL configuration file
  - RTC/RTC_LowPower_STANDBY/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - RTC/RTC_LowPower_STANDBY/Inc/main.h                   Header file for main.c
  - RTC/RTC_LowPower_STANDBY/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
  - RTC/RTC_LowPower_STANDBY/Src/stm32u0xx_it.c           Interrupt handlers
  - RTC/RTC_LowPower_STANDBY/Src/main.c                   Main program
  - RTC/RTC_LowPower_STANDBY/Src/stm32u0xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices
    
  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    evaluation board and can be easily tailored to any other supported device 
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
