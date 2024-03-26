## <b>RCC_ClockConfig Example Description</b>

Configuration of the system clock (SYSCLK) and modification of the clock settings in Run mode, using the RCC HAL API.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
MSI as clock source, the Joystick Sel push-button (connected to EXTI line 2) will be
used to change the PLL source:

- from MSI to HSI
- from HSI to MSI
Each time the Joystick Sel push-button is pressed EXTI line 2 interrupt is generated and in the ISR
the PLL oscillator source is checked using __HAL_RCC_GET_PLL_OSCSOURCE() macro:


- If the MSI oscillator is selected as PLL source, the following steps will be followed to switch
   the PLL source to HSI oscillator:

     a. Switch the system clock source to MSI to allow modification of the PLL configuration
     b. Enable HSI Oscillator, select it as PLL source and finally activate the PLL
     c. Select the PLL as system clock source and configure the HCLK, PCLK1 clocks dividers
     d. Disable the MSI oscillator (optional, if the MSI is no more needed by the application)

- If the HSI oscillator is selected as PLL source, the following steps will be followed to switch
   the PLL source to MSI oscillator:

     a. Switch the system clock source to HSI to allow modification of the PLL configuration
     b. Enable MSI Oscillator, select it as PLL source and finally activate the PLL
     c. Select the PLL as system clock source and configure the HCLK and PCLK1 clocks dividers
     d. Disable the HSI oscillator (optional, if the HSI is no more needed by the application)

LED3 is toggled with a timing defined by the HAL_Delay() API.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

RCC, System, Clock Configuration, HSE bypass mode, HSI, System clock, Oscillator, PLL

### <b>Directory contents</b>

  - RCC/RCC_ClockConfig/Inc/stm32u083c_discovery_conf.h     BSP configuration file
  - RCC/RCC_ClockConfig/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_ClockConfig/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - RCC/RCC_ClockConfig/Inc/main.h                  Header for main.c module
  - RCC/RCC_ClockConfig/Src/stm32u0xx_it.c          Interrupt handlers
  - RCC/RCC_ClockConfig/Src/main.c                  Main program
  - RCC/RCC_ClockConfig/Src/system_stm32u0xx.c      STM32U0xx system source file
  - RCC/RCC_ClockConfig/Src/stm32u0xx_hal_msp.c     HAL MSP module

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083xx devices.

  - This example has been tested with STM32U083C-DK
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
