## <b>TIM_TimeBase Example Description</b>

This example shows how to configure the TIM peripheral to generate a time base of 
one second with the corresponding Interrupt request.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U031R8Ix Devices :
The CPU at 48 MHz 

    In this example TIM2 input clock (TIM2CLK)  is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM2CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK = SystemCoreClock
    To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM2CLK / TIM2 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 48 MHz for STM32U0xx Devices.

The TIM2 ARR register value is equal to 10000 - 1

Update rate = TIM2 counter clock / (Period + 1) = 1 Hz

So the TIM2 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM update 
interrupt is generated and, in the handler routine, pin PA.05 (connected to LED4 on board NUCLEO-U031R8)
is toggled. So led blinks at the following frequency: 0.5Hz.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Timer, TIM, time base , Interrupt, Clock source

### <b>Directory contents</b>

  - TIM/TIM_TimeBase/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - TIM/TIM_TimeBase/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_TimeBase/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - TIM/TIM_TimeBase/Inc/main.h                  Header for main.c module  
  - TIM/TIM_TimeBase/Src/stm32u0xx_it.c          Interrupt handlers
  - TIM/TIM_TimeBase/Src/main.c                  Main program
  - TIM/TIM_TimeBase/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_TimeBase/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U031R8Ix devices.
  - In this example, the clock is set to 48 MHz.
    
  - This example has been tested with STMicroelectronics NUCLEO-U031R8
    board and can be easily tailored to any other supported device
    and development board.      

  - NUCLEO-U031R8 Set-up
    - Use LED4 connected to PA.05 pin (pin 11 in CN10 connector) and connect it on an oscilloscope 
      to show the time base signal.  


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


