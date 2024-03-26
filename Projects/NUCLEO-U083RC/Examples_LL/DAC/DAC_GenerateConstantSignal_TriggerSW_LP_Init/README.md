## <b>DAC_GenerateConstantSignal_TriggerSW_LP_Init Example Description</b>

How to use the DAC peripheral to generate a constant voltage signal with the DAC
low-power feature sample-and-hold. To be effective, a capacitor must be
connected to the DAC channel output and the sample-and-hold timings must be
tuned depending on the capacitor value. This example is based on the
STM32U0xx DAC LL API. The peripheral initialization uses
LL unitary service functions for optimization purposes (performance and size).

Example configuration:

One DAC channel (DAC1 channel1) is configured to connect DAC channel output on GPIO pin
to get the samples from SW (no DMA transfer) and to get conversion trigger from SW.
DAC low power feature sample-and-hold is enabled. Mode sample-and-hold timings
are set to an arbitrary value, in a real application they should be tuned
in function of capacitor value connected to DAC output (refer to reference manual).
Low speed clock (LSI: Low Speed Internal oscillator) is required
to drive DAC in low power mode sample-and-hold.

Example execution:

From the main program execution, LED4 is toggling quickly while waiting for
user button press.

Then, the DAC is configured and activated: constant signal is generated on DAC output
indefinitely.

DAC channel output value is provided by SW, a new value is loaded
at each press on push button: start from 0, increased by a quarter of Vdda and
finishing at Vdda voltage.
Finally, LED4 is turned-on.

Caution: If a capacitor is effectively connected to DAC channel output,
         the voltage appears as pseudo continuous (with refresh and hold variations).
         But if no capacitor is connected on DAC channel output,
         the voltage appears as discontinuous (phases with voltage grounded).

Connection needed:

External capacitor:

  Mode sample-and-hold requires an external capacitor
  to be connected between DAC channel output and ground.
  Capacitor value depends on load on DAC channel output and 
  sample-and-hold timings configured.
  GENERATOR error "Specify capacitor typ value and naming in datasheet for the selected STM32 series"
Oscilloscope for monitoring DAC channel output (cf pin below).
Other peripheral used:

  1 GPIO for push button
  1 GPIO for DAC channel output PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  
### <b>Keywords</b>

Analog, DAC, Digital to Analog, Single conversion,  Software Trigger, Sample and Hold

### <b>Directory contents</b>

  - DAC/DAC_GenerateConstantSignal_TriggerSW_LP_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - DAC/DAC_GenerateConstantSignal_TriggerSW_LP_Init/Inc/main.h                  Header for main.c module
  - DAC/DAC_GenerateConstantSignal_TriggerSW_LP_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - DAC/DAC_GenerateConstantSignal_TriggerSW_LP_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - DAC/DAC_GenerateConstantSignal_TriggerSW_LP_Init/Src/main.c                  Main program
  - DAC/DAC_GenerateConstantSignal_TriggerSW_LP_Init/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>  

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

