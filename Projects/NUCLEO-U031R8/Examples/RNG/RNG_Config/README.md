### <b>RNG_Config Example Description</b>

Configuration of the RNG using the HAL API. This example uses the RNG to generate 32-bit long random numbers.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The RNG peripheral initialization is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change RNG initialization.

Next, the peripheral entropy configuration parameters are updated by HAL_RNGEx_SetConfig().
The configuration is locked by HAL_RNGEx_LockConfig() then read back by HAL_RNGEx_GetConfig()
to check that the configuration is that expected.

Finally, user is asked to press User push-button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers 
at each User push-button press.

The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED4 is toggling at a frequency of 1Hz.

### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

### <b>Directory contents</b>

  - RNG/RNG_Config/Inc/stm32u0xx_hal_conf.h         HAL configuration file
  - RNG/RNG_Config/Inc/stm32u0xx_it.h               Interrupt handlers header file
  - RNG/RNG_Config/Inc/main.h                       Header for main.c module
  - RNG/RNG_Config/Inc/stm32u0xx_nucleo_conf.h      STM32U0xx_Nucleo board configuration file
  - RNG/RNG_Config/Src/stm32u0xx_it.c               Interrupt handlers
  - RNG/RNG_Config/Src/main.c                       Main program
  - RNG/RNG_Config/Src/stm32u0xx_hal_msp.c          HAL MSP module 
  - RNG/RNG_Config/Src/system_stm32u0xx.c           STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U031R8Ix devices.

  - This example has been tested with NUCLEO-U031R8 board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 