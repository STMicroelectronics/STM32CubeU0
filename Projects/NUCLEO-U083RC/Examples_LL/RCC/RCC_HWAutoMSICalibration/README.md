## <b>RCC_HWAutoMSICalibration Example Description</b>
Use of the MSI clock source hardware autocalibration and LSE clock (PLL mode) to obtain a precise MSI clock. 

In the associated software, after start-up, SYSCLK is configured to the max 
frequency using the PLL with MSI as clock source.

When a 32.768 kHz external oscillator is present in the application,it is possible to configure 
the MSI in a PLL-mode by setting the MSIPLLEN bit. 

When configured in PLL-mode, the MSI automatically calibrates itself thanks to the LSE. 
This mode is available for all MSI frequency ranges. At 48 MHz, the MSI in PLL-mode can 
be used for the USB OTG FS device, saving the need of an external high-speed crystal.

The program waits until User push-button is pressed to begin the auto calibration 
procedure (fast blink of the LED4):

 - Enable and wait for LSE is ready.
 - Enable PLL-Mode feature
 - LED4 is turn on to indicate end of the HW calibration procedure.

### <b>Notes</b>

1- User can connect on oscilloscope to MCO pin (PA.08) to check MSI frequency before and after
calibration procedure.

2-note User can enable USE_TIMEOUT switch to detect a problem with LSE activation (LED4 is blinking 
every 1 sec in case of issue)


### <b>Keywords</b>

RCC, System, MSI clock , PLL-mode, LSE clock, PLL


### <b>Directory contents</b>

  - RCC/RCC_HWAutoMSICalibration/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - RCC/RCC_HWAutoMSICalibration/Inc/main.h                  Header for main.c module
  - RCC/RCC_HWAutoMSICalibration/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_HWAutoMSICalibration/Src/stm32u0xx_it.c          Interrupt handlers
  - RCC/RCC_HWAutoMSICalibration/Src/main.c                  Main program
  - RCC/RCC_HWAutoMSICalibration/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - External LSE Oscillator is mandatory to be able to run this example.
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PA.08: connector CN10 pin 23


### <b>par How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example