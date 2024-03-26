## <b>OPAMP_PGA_Init Example Description</b>

How to use the OPAMP peripheral in PGA mode (OPAMP programmable gain) with DAC and COMP.
This example is based on the STM32U0xx OPAMP LL API. The peripheral
initialization uses LL unitary service functions for optimization purposes
(performance and size).

Example configuration:

The OPAMP is configured with PGA Gain 2, non inverting input is connected internally
to DAC_OUT1 that generate a constant voltage of Vdda/4 = 825mV. OPAMP output is connected
to COMP2 non inverting input internally (PA7 pin 4 on connector CN5).

Example execution:

The DAC is configured and activated: constant signal is generated on DAC output indefinitely.
With an OPAMP PGA gain of 2 (825mV*2 = 1.65V), the output of OPAMP is higher than the internal VRef value (1.2V).
This output is then connected to the non-inverting input of COMP2.
If the output of COMP2 is high, the test is PASS and LED4 is turned ON. Otherwise, the test is FAIL and LED4 is turned OFF.

### <b>Keywords</b>

Analog, OPAMP, Comparator, Voltage, Signal


### <b>Directory contents</b>

  - OPAMP/OPAMP_Follower_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - OPAMP/OPAMP_Follower_Init/Inc/main.h                  Header for main.c module
  - OPAMP/OPAMP_Follower_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - OPAMP/OPAMP_Follower_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_Follower_Init/Src/main.c                  Main program
  - OPAMP/OPAMP_Follower_Init/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices.

  - This example has been tested with STM32U083RCTx board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

