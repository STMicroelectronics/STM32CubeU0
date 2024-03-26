## <b>DAC_GenerateWaveform_TriggerHW_Init Example Description</b>

How to use the DAC peripheral to generate a voltage waveform from a digital data
stream transferred by DMA. This example is based on the STM32U0xx 
DAC LL API. The peripheral initialization uses LL initialization
functions to demonstrate LL init usage.

Example configuration:

One DAC channel (DAC1 channel1) is configured to connect DAC channel output on GPIO pin
to get the samples from DMA transfer and to get conversion trigger from timer.

Other peripherals configured in this example:

one DMA channel and one timer are configured to provide samples to be generated
by DAC peripheral:

sinus waveform, frequency 1kHz, amplitude Vdda.
(these settings are configurable by changing literals values in file header).

Example execution:

From the main program execution, LED4 is toggling quickly while waiting for
user button press.

DMA and timer are configured and activated to transfer data and trig DAC conversion.

Then, the DAC is configured and activated: waveform is generated on DAC output
indefinitely.

DAC channel output value is provided by DMA transfer, a new value is loaded
at each trigger from timer.

Finally, LED4 is turned-on.

Connection needed:

None.
 
Oscilloscope for monitoring DAC channel output (cf pin below).

Other peripheral used:

  1 GPIO for push button
  
  1 GPIO for DAC channel output PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  For waveform voltage generation: 1 DMA channel, 1 timer.

### <b>Keywords</b>

Analog, DAC, Digital to Analog, Continuous conversion,  DMA, Sine-wave generation, External Trigger


### <b>Directory contents</b>

  - DAC/DAC_GenerateWaveform_TriggerHW_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - DAC/DAC_GenerateWaveform_TriggerHW_Init/Inc/main.h                  Header for main.c module
  - DAC/DAC_GenerateWaveform_TriggerHW_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - DAC/DAC_GenerateWaveform_TriggerHW_Init/Src/main.c                  Main program
  - DAC/DAC_GenerateWaveform_TriggerHW_Init/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.



### <b>How to use it ?</b>  

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

