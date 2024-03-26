## <b>TIM_ExtTriggerSynchro Example Description</b>

 This example shows how to synchronize TIM peripherals in cascade mode with an external trigger.
 In this example three timers are used:

  1/TIM1 is configured as Master Timer:

      - Toggle Mode is used
      - The TIM1 Enable event is used as Trigger Output 

  2/TIM1 is configured as Slave Timer for an external Trigger connected to TIM1 
    TI2 pin (TIM1 CH2 configured as input pin):

      - The TIM1 TI2FP2 is used as Trigger Input
      - Rising edge is used to start and stop the TIM1: Gated Mode.

  3/TIM2 is slave for TIM1 and Master for TIM3,

      - Toggle Mode is used
      - The ITR0 (TIM1) is used as input trigger 
      - Gated mode is used, so start and stop of slave counter
        are controlled by the Master trigger output signal(TIM1 enable event).
      - The TIM2 enable event is used as Trigger Output. 

  4/TIM3 is slave for TIM2,

      - Toggle Mode is used
      - The ITR1 (TIM2) is used as input trigger
      - Gated mode is used, so start and stop of slave counter
        are controlled by the Master trigger output signal(TIM2 enable event).

   The TIM1CLK is set to SystemCoreClock, 
   TIM2CLK is set to SystemCoreClock and 
   TIM3CLK frequency is set to SystemCoreClock,
   To get TIMx counter clock at 8 MHz the Prescaler is computed as following: 

   - Prescaler = (TIMx CLK / TIMx counter clock) - 1

   SystemCoreClock is set to 48 MHz.

   TIMx frequency = TIMx  counter clock/ 2*(TIMx_Period + 1) = 100 KHz.
   
  The starts and stops of the TIM1 counters are controlled by the external trigger.

  The TIM2 starts and stops are controlled by the TIM1, and the TIM3 starts and 
  stops are controlled by the TIM2.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Timer, PWM, External Trigger, Synchronization, Cascade mode, Master, Slave, Duty Cycle, Waveform,
Oscilloscope, Output, Signal

### <b>Directory contents</b>

  - TIM/TIM_ExtTriggerSynchro/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - TIM/TIM_ExtTriggerSynchro/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - TIM/TIM_ExtTriggerSynchro/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - TIM/TIM_ExtTriggerSynchro/Inc/main.h                  Header for main.c module  
  - TIM/TIM_ExtTriggerSynchro/Src/stm32u0xx_it.c          Interrupt handlers
  - TIM/TIM_ExtTriggerSynchro/Src/main.c                  Main program
  - TIM/TIM_ExtTriggerSynchro/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - TIM/TIM_ExtTriggerSynchro/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U031R8Ix devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-U031R8 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U031R8 Set-up
    - Connect an external trigger to the TIM1 CH2 PA.09 (pin 21 in CN10 connector).
    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 PA.08 (pin 23 in CN10 connector)
      - TIM2 CH1 PA.15 (pin 17 in CN7 connector)
      - TIM3 CH1 PA.06 (pin 13 in CN10 connector)
	  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

