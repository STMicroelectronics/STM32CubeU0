## <b>OPAMP_Calibration Example Description</b>

This example shows how to calibrate the OPAMP.

On OPAMP1, this example shows how to :

- Get factory calibration (trimming) settings.
- Enable/disable OPAMP
- Calibrate OPAMP peripheral before and after use of OPAMP.

Hence changes can be monitored (versus factory settings or in case of temperature impact for instance).

- Connect an oscilloscope to DAC_CHANNEL_2 output (PA4 : pin 3 on connector CN8) to display the sine wave signal
  generated by the DAC. 

- Connect an oscilloscope to OPAMP1 output (PA3 : pin 1 on connector CN9) 

  -> to display the OPAMP buffered sine wave signal.

  -> Monitor enabling/disabling of OPAMP.

- Connect DAC_CHANNEL_1 output (PA4 : pin 3 on connector CN8) to OPAMP1 non-inverting input (PA3 : pin 37 on connector CN10).

- Push the Key Push button (connected to PC13 pin) to switch from one step to the 
next one.

- Key push button steps are:

  -> 1st Push: 
     + Factory trimming retrieved 
     + User trimming set according to self calibration (1st run)
     + Factory trimming and user trimming are compared
       - If The LED4 blinks rapidly: Calibrated trimming are different from Factory Trimming
       - If The LED4 blinksslowly : Calibrated trimming same as Factory Trimming 

  -> 2nd Push: 

     + OPAMP start
     + Use of previously calibrated user Trimming values

  -> 3rd Push: 

     + change OPAMP setting on the fly (dummy change, without application purpose in this example: change non-inverting input source)

  -> 4th Push: 

     + OPAMP stopped    

  -> 5th Push: 

     + Self calibration (2nd run)
     + results compare to 1st run.
       - If The LED4 blinks rapidly: New calibrated trimming are different from ones calibrated before run
       - If The LED4 blinks slowly : New calibrated trimming are same as ones calibrated before run 

  -> 6th Push: end of test

Note: In case of error, LED4 is toggling twice at a frequency of 1Hz.

For debug: variables to monitor with debugger watch window:

 - "UserButtonPressState": represents the number of button press


### <b>Keywords</b>

Analog, OPAMP, DAC, Voltage, Signal

### <b>Directory contents</b> 

  - OPAMP/OPAMP_Calibration/Inc/stm32u0xx_nucleo_conf.h BSP configuration file
  - OPAMP/OPAMP_Calibration/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - OPAMP/OPAMP_Calibration/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - OPAMP/OPAMP_Calibration/Inc/main.h                  Header for main.c module  
  - OPAMP/OPAMP_Calibration/Src/stm32u0xx_it.c          Interrupt handlers
  - OPAMP/OPAMP_Calibration/Src/main.c                  Main program
  - OPAMP/OPAMP_Calibration/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - OPAMP/OPAMP_Calibration/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect an oscilloscope to pin 3 on connector CN8 to display DAC_CHANNEL_2 output waveform.
    - Connect an oscilloscope to pin 1 on connector CN9 to display OPAMP1 output waveform.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example