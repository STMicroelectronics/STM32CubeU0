## <b>PWR_LPMODE_RTC Example Description</b>

How to enter the system to different available low power modes and wake up from
these modes by using an interrupt from RTC wakeup timer.

This project is targeted to run on STM32U083RCTx devices on NUCLEO-U083RC board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 48 MHz.

The system automatically enters low power mode 5 sec after start-up.
The RTC wake-up is configured to generate an interrupt on rising edge about 10 sec. afterwards.

After wake-up from low power mode, program performs a system reset when it was on Sleep and Stop modes
and the system restarts automatically when it was on Standby mode.

This projects performs multi-configuration :

 - Uncomment CPU_SLEEP_MODE defines in the main.h file to allow the CPU to enter Sleep mode.
 - Uncomment SYSTEM_STOP_MODE defines in the main.h file to allow the SYSTEM to enter Stop mode.
 - Uncomment SYSTEM_STANDBY_MODE defines in the main.h file to allow the SYSTEM to enter in Standby mode.

This sequence is repeated in loop.

NUCLEO-U083RC board's LED can be used to monitor the project operation status:

 - LED4 toggle when no error detected.
 - LED4 is ON when any project error was occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

RAMCFG, ECC, SRAM, Single error, Double error

### <b>Directory contents</b>

  - PWR/PWR_LPMODE_RTC/Src/main.c                  Main program
  - PWR/PWR_LPMODE_RTC/Src/system_stm32u0xx.c      STM32U0xx system clock configuration file
  - PWR/PWR_LPMODE_RTC/Src/stm32u0xx_it.c          Interrupt handlers
  - PWR/PWR_LPMODE_RTC/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - PWR/PWR_LPMODE_RTC/Inc/main.h                  Main program header file
  - PWR/PWR_LPMODE_RTC/Inc/stm32u0xx_nucleo_conf.h BSP Configuration file
  - PWR/PWR_LPMODE_RTC/Inc/stm32u0xx_hal_conf.h    HAL Configuration file
  - PWR/PWR_LPMODE_RTC/Inc/stm32u0xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices .

  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - UnPlug then Plug STLINK connection to perform a power-on-reset