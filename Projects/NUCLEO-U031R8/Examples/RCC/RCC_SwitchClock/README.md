## <b>RCC_SwitchClock Example Description</b>

Switch of the system clock (SYSCLK) from Low frequency clock to high frequency clock, using the RCC HAL API.

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the User push-button (connected to EXTI_Line4_15) will be
used to change the system clock source:

- from LSI to PLL
- from PLL to LSI

Each time the User push-button is pressed EXTI_Line4_15 interrupt is generated and in the ISR
the System Clock source is checked using __HAL_RCC_GET_SYSCLK_SOURCE() macro:

- If PLL is selected as System clock source, the following steps will be followed to switch
   SYSCLK from PLL to LSI:
     a- Configure the SysTick to have interrupt in 100ms time basis
     b- Select the LSI as system clock source

- If the LSI oscillator is selected as System clock source, the following steps will be followed to switch
   SYSCLK from LSI to PLL:
     a- Select the PLL as system clock source
     b- Configure the SysTick to have interrupt in 1ms time basis

In this example the SYSCLK is outputted on the MCO1 pin(PA.08).

LED1 is toggling with a timing defined by HAL_IncTick() API depending on the system clock source.

#### <b>Notes</b>

Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
### <b>Keywords</b>

System, RCC, PLL, PLLCLK, SYSCLK, LSI, Switch Clock, Oscillator,

### <b>Directory contents</b>

  - RCC/RCC_SwitchClock/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - RCC/RCC_SwitchClock/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - RCC/RCC_SwitchClock/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - RCC/RCC_SwitchClock/Inc/main.h                  Header for main.c module  
  - RCC/RCC_SwitchClock/Src/stm32u0xx_it.c          Interrupt handlers
  - RCC/RCC_SwitchClock/Src/main.c                  Main program
  - RCC/RCC_SwitchClock/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - RCC/RCC_SwitchClock/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U031R8Tx devices.

  - This example has been tested with NUCLEO-U031R8
    board and can be easily tailored to any other supported device
    and development board.

  - Connect MCO1 pin (pin 8 in CN9 connector) to an oscilloscope to monitor the system clock.


### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

