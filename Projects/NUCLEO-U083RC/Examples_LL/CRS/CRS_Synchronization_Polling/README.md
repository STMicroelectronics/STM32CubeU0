## <b>CRS_Synchronization_Polling example Description</b>
How to configure the clock recovery system in polling mode through the
STM32U0xx CRS LL API. The peripheral initialization uses LL unitary
service functions for optimization purposes (performance and size).

### <b>Notes</b>

At the beginning of the main program the SystemClock_Config() function is used to 
configure the system clock (SYSCLK) to run at 48 MHz.

Then, HSI48 and LSE oscillators are enabled.
In this example CRS will use LSE 
oscillator to calibrate HSI48 frequency.
When HSI48 and LSE are started, automatic calibration starts and program call the
polling function to wait for synchronization status.

In this test, there are results which could occur:

 - Wait for User push-button press to start the test (Fast toggle LED4)
 - few loops occurs up to SYNC OK event (if SYNC WARN, HSI48 trimming value will be
 automatically modified)
 - in case of SYNC MISS or SYNC ERROR, there is an issue with synchronization input
 parameters. In this case, user need to apply new parameters after checking synchronization
 information and restart the synchronization.
 - LED4 is slowly blinking (1 sec. period) in case of timeout during polling 
 procedure. It may due to an issue with LSE synchronization frequency.
 - LED4 will finally power on if SYNC OK is returned by CRS.


### <b>Keywords</b>

CRS, automatic calibration, polling, HSI48, LSE

### <b>Directory contents</b>

      - CRS/CRS_Synchronization_Polling/Inc/stm32u0xx_it.h          Interrupt handlers header file
      - CRS/CRS_Synchronization_Polling/Inc/main.h                  Header for main.c module
      - CRS/CRS_Synchronization_Polling/Inc/stm32_assert.h          Template file to include assert_failed function
      - CRS/CRS_Synchronization_Polling/Src/stm32u0xx_it.c          Interrupt handlers
      - CRS/CRS_Synchronization_Polling/Src/main.c                  Main program
      - CRS/CRS_Synchronization_Polling/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect the MCO pin to an oscilloscope to monitor HSI48 frequency:
      - PA.08: connected to pin 8 of CN9 connector for Nucleo-64

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
