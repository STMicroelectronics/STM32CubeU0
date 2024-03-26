## <b>LPTIM_Encoder Example Description</b>

This example shows how to configure the LPTIM peripheral in Encoder mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U083RCTx Devices :
The CPU at 48 MHz

#### <b>Notes</b>

 1. In this example, the LPTIM encoder is configured to generate up or down counting according to the phase between the two inputs of LPTIM1.

In this example :

        - At the first there is no change of state , LED4 is Off .
        - LED4 on  , the LPTIM encoder mode is in up counting.
        - LED4 off , the LPTIM encoder is mode in down counting .
        - In case of error , LED4 is toggling each 1s.

To change the direction of counting switch the inputs 1 & 2 of LPTIM1.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
  3. The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation
  4. This example can not be used in DEBUG mode, this is due to the fact that the Cortex-M0+ core is no longer clocked during low power mode so debugging features are disabled.

### <b>Keywords</b>

Timer, Low Power, PWM, Stop mode, Interrupt, LSE, Output, Duty Cycle

### <b>Directory contents</b>

  - LPTIM/LPTIM_Encoder/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - LPTIM/LPTIM_Encoder/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - LPTIM/LPTIM_Encoder/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_Encoder/Inc/main.h                  Header for main.c module
  - LPTIM/LPTIM_Encoder/Src/stm32u0xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_Encoder/Src/main.c                  Main program
  - LPTIM/LPTIM_Encoder/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_Encoder/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U083RC Set-up

 Use GPIOx to generate 2 square signals with different phases
   Connect the following pins of GPIOx outputs :

        - GPIOB : pin (PB.3)pin 31 in CN10 connector
        - GPIOB : pin (PB.4)pin 27 in CN10 connector
   To the following inputs of LPTIM 1 using wires :

        - Input 1 : pin (PB5)pin 29 in CN10 connector
        - Input 2 : pin (PB7)pin 21 in CN7 connector
   

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

