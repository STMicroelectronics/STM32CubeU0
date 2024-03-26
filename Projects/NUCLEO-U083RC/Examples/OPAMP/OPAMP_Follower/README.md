## <b>OPAMP_Follower Example Description</b>

How to configure the OPAMP peripheral in follower mode interconnected
with DAC and COMP.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U083RCTx Devices :
The CPU at 54 MHz.

OPAMP1 is configured as following:

   - Follower mode.
   
   - OPAMP non inverting input is connected internally to DAC_OUT1 that generate a constant voltage of Vdda/2 = 1.65V
   
   - OPAMP output is connected to COMP2 non inverting input internally (PA3 pin 8 on connector CN9).

Test Description:

   - Push the user Push button to enter normal mode(LED4 On).
   - Push the user alternatively to switch from normal power mode to low power mode
     (LED4 Off)and vice versa.
   - DAC voltage is higher than internal VRef value (1.2V) and connected 
     to COMP2 non inverting input through OPAMP output.
   - Check COMP2 output (PA7 pin 4 on connector CN5) .
   - If COMP2 output is high test PASS otherwise test FAIL.

LED4 toggles slowly when an error occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set
    to 1 millisecond to have correct HAL operation.

### <b>Keywords</b>

Analog, OPAMP, Comparator, Voltage, Signal

### <b>Directory contents</b>

 -  OPAMP/OPAMP_InternalFollower/Src/main.c                   Main program
 -  OPAMP/OPAMP_InternalFollower/Src/system_stm32u0xx.c       STM32U0xx system clock configuration file
 -  OPAMP/OPAMP_InternalFollower/Src/stm32u0xx_it.c           Interrupt handlers
 -  OPAMP/OPAMP_InternalFollower/Src/stm32u0xx_hal_msp.c      HAL MSP module
 -  OPAMP/OPAMP_InternalFollower/Inc/main.h                   Main program header file
 -  OPAMP/OPAMP_InternalFollower/Inc/stm32u0xx_nucleo_conf.h  BSP Configuration file
 -  OPAMP/OPAMP_InternalFollower/Inc/stm32u0xx_hal_conf.h     HAL Configuration file
 -  OPAMP/OPAMP_InternalFollower/Inc/stm32u0xx_it.h           Interrupt handlers header file

### <b>Hardware and Software environment</b>

 - This example runs on STM32U083RCTx devices.

 - This example has been tested with STMicroelectronics NUCLEO-U083RC
   board and can be easily tailored to any other supported device
   and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

