## <b>LPTIM_PWM_LSE Example Description</b>

How to configure and use, through the HAL LPTIM API, the LPTIM peripheral using LSE 
as counter clock, to generate a PWM signal, in a low-power mode.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U083RCTx Devices :
The CPU at 48 MHz 

In this example, the LPTIM instance used is LPTIM1 and the low power mode is Stop 2 mode.


The counter clock is LSE (32.768 KHz), Autoreload equal to 99 so the output
frequency (FrequencyOutput) will be equal to 327.680.

  FrequencyOutput = Counter Clock Frequency / (Autoreload + 1)
                  = 32768 / 100
                  = 327.680 Hz

Pulse value equal to 49 and the duty cycle (DutyCycle) is computed as follow:

  DutyCycle = 1 - ((PulseValue + 1)/ (Autoreload + 1))
  DutyCycle = 50%

To minimize the power consumption, after starting generating the PWM signal,
the MCU enters in Stop 2 mode. Note that GPIOs are configured in Low Speed to
enhance the consumption. 

User push-button pin (PC.13)is configured as input with external interrupt (EXTI_Line13),
falling edge. When User push-button is pressed, wakeup event is generated, PWM signal generation is stopped and LED4 is ON.

If LED4 toggles with 1s delay, an error is occurred.

#### <b>Notes</b>

 1. This example can not be used in DEBUG mode, this is due to the fact
    that the Cortex-M0+ core is no longer clocked during low power mode
    so debugging features are disabled.

 2. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

Timer, Low Power, PWM, Stop mode, Interrupt, LSE, Output, Duty Cycle

### <b>Directory contents</b>

  - LPTIM/LPTIM_PWM_LSE/Inc/stm32u0xx_nucleo_conf.h BSP configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - LPTIM/LPTIM_PWM_LSE/Inc/main.h                  Header for main.c module  
  - LPTIM/LPTIM_PWM_LSE/Src/stm32u0xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_PWM_LSE/Src/main.c                  Main program
  - LPTIM/LPTIM_PWM_LSE/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_PWM_LSE/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-U083RC
    board and can be easily tailored to any other supported device
    and development board.      

  - Connect PB3 (pin 31 in CN10connector) to an oscilloscope to monitor the LPTIM1 output waveform.
  
  - LSE oscillator must be mounted on the board, else this example does not work.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
