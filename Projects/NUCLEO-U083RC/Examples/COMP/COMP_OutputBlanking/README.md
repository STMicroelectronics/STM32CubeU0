## <b>COMP_OutputBlanking Example Description</b>

How to use the comparator-peripheral output blanking feature. The purpose of the
output blanking feature in motor control is to prevent tripping of the current
regulation upon short current spikes at the beginning of the PWM period.

In this example COMP2 and TIM2 are used:

COMP2 is configured as following:

 - Non Inverting input is connected to PA3 pin 1 in CN9 connector
 - Inverting input is connected to VREFINT (1.22V)
 - Output is available on PA2 pin 2 in CN9 connector
 - Blanking source is connected internally to TIM2 OC3

TIM2 is configured as following:

 - TIM2 period is APB2 clock / period = 48000000 / 50000 = 9600 Hz.
 - TIM2 OC3 (internal channel: not available on GPIO) configured in PWM mode
   with a frequency equal to 9600 Hz and high level equal to 
   pulse / period = 2000 / 48000000  = 41.66 micro second.
 - Timer channel intended to be connected to an external device (motor, ...) in user application:
   TIM2 CH3 (PB10 pin 7 in CN9 connector) configured in PWM mode with a frequency equal to 9600 Hz
   and duty cycle pulse/period = 100 * (37500 / 50000) =  75%.
   
Connection needed:

 - either connect COMP2 input (PA3 pin 1 in CN9) to TIM2 CH3 (PB10 pin 7 in CN9 connector).
 - either connect COMP2 output (PA2 pin 2 in CN9 ) to a signal generator, at the frequency of TIM2 period (to have comparator input signal frequency matching blanking signal frequency. To do this, possibility to use TIM2 CH3 (PB10 pin 7 in CN 9 connector) as trigger of signal generator).

Comparator output signal expected:

 - Between t=0us and t=41.66us: Comparator positive input at level high, blanking source at level high.
   Blanking source is high, therefore comparator output is forced to level low.
   (otherwise, as comparator positive input is higher than negative input, comparator output would have been high.)
 - Between t=41.66us and t=duty cycle pulse: Comparator positive input at level high, blanking source at level low.
   Comparator positive input is higher than negative input, blanking source is low, therefore comparator output is high.
 - Between t=duty cycle pulse and t=period: Comparator positive input at level low, blanking source at level low.
   Comparator positive input is higher than negative input, therefore comparator output is low.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides 
    accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
    This implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower) than the 
    peripheral interrupt. Otherwise the caller ISR process will be blocked. 
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 2. The application need to ensure that the SysTick time base is always set 
    to 1 millisecond to have correct HAL operation.
	  

### <b>Keywords</b>

COMP, Timer

### <b>Directory contents</b> 

  - COMP/COMP_OutputBlanking/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - COMP/COMP_OutputBlanking/Inc/stm32u0xx_it.h          COMP interrupt handlers header file
  - COMP/COMP_OutputBlanking/Inc/main.h                  Header for main.c module
  - COMP/COMP_OutputBlanking/Inc/stm32u0xx_nucleo_conf.h HAL configuration file  
  - COMP/COMP_OutputBlanking/Src/stm32u0xx_it.c          COMP interrupt handlers
  - COMP/COMP_OutputBlanking/Src/main.c                  Main program
  - COMP/COMP_OutputBlanking/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - COMP/COMP_OutputBlanking/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083xx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
  
    - Connect TIM2 CH3 (PB10 pin 7 in CN9 connector) to a scope and to the input trigger of a wave generator
    - Connect the wave generator output to COMP2 Non Inverting input (PA3 pin 1 in CN9 connector)
    - Configure the wave generator to output a pulse at rising edge event with 
      high level greater than VREFINT (1.22V) and pulse duration lower 41.66 micro second
      (as an example you can use 10 micro second). 
      You can change the pulse delay and note that the break event is generated
      only when the pulse is triggered after 41.66 micro second


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

