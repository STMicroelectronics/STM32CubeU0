## <b>TIM_PWMOutput Example Description</b>

This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) 
mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U031R8Tx devices :
The CPU at 48 MHz 

SystemCoreClock is set to 48 MHz for STM32U0xx devices.

    In this example TIM2 input clock (TIM2CLK) is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM2CLK = PCLK1
      PCLK1 = HCLK
      => TIM2CLK = HCLK = SystemCoreClock

    To get TIM2 counter clock at SystemCoreClock, the prescaler is set to 0

    To get TIM2 output clock at 24 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM2 counter clock / TIM2 output clock) - 1
           = 2295

    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR + 1)* 100 = 50%
    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR + 1)* 100 = 37.5%
    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR + 1)* 100 = 25%
    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR + 1)* 100 = 12.5%

LED4 is ON when there are an error.

The PWM waveforms can be displayed using an oscilloscope.

#### <b>Notes</b>

 1. The duty cycles values mentioned above are theoretical (obtained when the system clock frequency is exactly 48 MHz).
    They might be slightly different depending on system clock frequency precision.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 2. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Timer, TIM, Output, signal, PWM, Oscilloscope, Frequency, Duty cycle, Waveform

### <b>Directory contents</b>

  - TIM/TIM_PWMOutput/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - TIM/TIM_PWMOutput/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMOutput/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMOutput/Src/stm32u0xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput/Src/main.c                  Main program
  - TIM/TIM_PWMOutput/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMOutput/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U031R8Tx devices.
  - In this example, the clock is set to 48 MHz.
    
  - This example has been tested with STMicroelectronics NUCLEO-U031R8 
    board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-U031R8 Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:

        - TIM2_CH1 : PA.15 (pin 17 in CN7 connector)

        - TIM2_CH2 : PB.3 (pin 4 in CN9 connector)

        - TIM2_CH3 : PB.10 (pin 7 in CN9 connector)

        - TIM2_CH4 : PB.11 (pin 18 in CN10 connector)


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

