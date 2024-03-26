## <b>PWR_ModesSelection Example Description</b>

How to configure the system to measure the current consumption in different low-power modes.
The objective is to provide a mean to measure the power consumption using an amperemeter on IDD connector.

The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick interrupt handler.

#### <b>Notes</b>

 1. To measure the current consumption remove JP5 jumper and
    connect an amperemeter to JP5 to measure IDD current.

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

Power, PWR, Run, Stop1, Stop2, Stop3, Standby, Shutdown, Retention, Regulator, SMPS, LDO, Clock, RTC, External reset

### <b>Directory contents</b>

  - PWR/PWR_ModesSelection/Inc/stm32u0xx_hal_conf.h     HAL Configuration file
  - PWR/PWR_ModesSelection/Inc/stm32u0xx_nucleo_conf    STM32U0xx_Nuleo board configuration header file
  - PWR/PWR_ModesSelection/Inc/stm32u0xx_it.h           Header for stm32u0xx_it.c
  - PWR/PWR_ModesSelection/Inc/main.h                   Header file for main.c
  - PWR/PWR_ModesSelection/Inc/console.h                Header for console.c module
  - PWR/PWR_ModesSelection/Inc/lowpower_scenarios.h     Header for lowpower_scenarios.c module
  - PWR/PWR_ModesSelection/Inc/system_config.h          Header for system_config.c module
  - PWR/PWR_ModesSelection/Src/main.c                   Main program body
  - PWR/PWR_ModesSelection/Src/lowpower_scenarios.c     Low-power scenario module
  - PWR/PWR_ModesSelection/Src/console.c                Console output input module
  - PWR/PWR_ModesSelection/Src/stm32u0xx_hal_msp.c      HAL MSP module
  - PWR/PWR_ModesSelection/Src/stm32u0xx_it.c           Interrupt handlers
  - PWR/PWR_ModesSelection/Src/system_config.c          Systeme configuration

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices

  - This example has been tested with STMicroelectronics NUCLEO-U083RC (64 pins)
    board embedding a low-speed clock LSE and can be easily tailored to any other
    supported device and development board.

  - NUCLEO-U083RC (64 pins) set-up:
	  - USART2 interface connected to ST-LINK

  - HyperTerminal setup of COM port associated to STLink Virtual COM Port:
    - Baud rate = 115200
    - Data = 8 bit
    - Parity = none
    - Stop = 1 bit
    - No hardware flow control

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
