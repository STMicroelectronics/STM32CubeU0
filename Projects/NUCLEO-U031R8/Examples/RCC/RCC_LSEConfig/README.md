## <b>RCC_LSEConfig Example Description</b>

Enabling/disabling of the low-speed external(LSE) RC oscillator (about 32 KHz) at run time, using the RCC HAL API.

   - Board: NUCLEO-U031R8 (embeds a STM32U031R8Ix device)
   - Microcontroller Clock Output MCO1 Pin: PA.08( Pin 8 on connector CN9).

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
MSI as clock source, the User push-button (connected to EXTI_Line4_15)
is used to enable/disable LSE:
Each time the User push-button is pressed, EXTI_Line4_15 interrupt is generated and the ISR
enables the LSE when it is off, disables it when it is on.

LED4 is toggling as follows according to LSE activation:

- when LSE is on, LED4 is emitting a double flash every half-second
- when LSE is off, LED4 is fast-toggling (every 100 ms)

LSE clock waveform is available on the MCO1 pin PA.08 ( Pin 8 on connector CN9) and can be captured
on an oscilloscope.

In case of configuration error, LED4 is slowly blinking (1 sec. Period).


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, RCC, PLL, PLLCLK, SYSCLK, LSE, Clock, Oscillator,

### <b>Directory contents</b>

  - RCC/RCC_LSEConfig/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - RCC/RCC_LSEConfig/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_LSEConfig/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - RCC/RCC_LSEConfig/Inc/main.h                        Header for main.c module
  - RCC/RCC_LSEConfig/Src/stm32u0xx_it.c          Interrupt handlers
  - RCC/RCC_LSEConfig/Src/main.c                        Main program
  - RCC/RCC_LSEConfig/Src/system_stm32u0xx.c      STM32U0xx system source file
  - RCC/RCC_LSEConfig/Src/stm32u0xx_hal_msp.c     HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices.

  - This example has been tested with NUCLEO-U031R8
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

