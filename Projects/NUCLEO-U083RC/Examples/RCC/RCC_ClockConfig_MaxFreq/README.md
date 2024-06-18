## <b>RCC_ClockConfig_MaxFreq Example Description</b>

Configuration of the system clock (SYSCLK) to the maximum frequency (56MHz), using the RCC HAL API.

In this example, after startup SYSCLK is configured to the max frequency (56MHz) using the PLL with
HSI as clock source.
SYSCLK clock waveform is available on the MCO1 pin PA.08 and can be captured on an oscilloscope.

#### <b>Notes</b>

### <b>Keywords</b>

RCC, System, Clock Configuration, HSI, System clock, Oscillator, PLL

### <b>Directory contents</b>

  - RCC/RCC_ClockConfig_MaxFreq/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig_MaxFreq/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig_MaxFreq/Inc/main.h                  Header for main.c module
  - RCC/RCC_ClockConfig_MaxFreq/Src/stm32u0xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig_MaxFreq/Src/main.c                  Main program
  - RCC/RCC_ClockConfig_MaxFreq/Src/system_stm32u0xx.c      STM32U0xx system source file
  - RCC/RCC_ClockConfig_MaxFreq/Src/stm32u0xx_hal_msp.c     HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083Rx devices.

  - This example has been tested with NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
