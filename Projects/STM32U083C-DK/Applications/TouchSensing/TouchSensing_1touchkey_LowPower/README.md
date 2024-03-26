## <b>TouchSensing_1touchkey_LowPower Application Description</b>

Use of the TSC to perform continuous acquisitions of one channel in Interrupt mode (low power mode).

This project is targeted to run on STM32U083xx devices on STM32U083C-DK board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The TSC peripheral configuration is ensured by the HAL_TSC_Init() function.
This later is calling the HAL_TSC_MspInit()function which core is implementing
the configuration of the needed TSC resources according to the used hardware (CLOCK,
GPIO and NVIC). You may update this function to change TSC configuration.

The Touchsensing module allows the detection of a touch on the Touchsensor button TS1 after a low power phase using
the embedded Comparator peripheral to decrease power consumption.

In this specific STM32U0xx series, some Touch IO pins are interconnected with the Comparator module
which gives us the the possibility to change the detection voltage level.
Changing this voltage level will help in detecting a physical Touch earlier, depending on the value
of the comparator input.

This means that the lower the level the less time you will need to reach it and thus an acquisition cycle is smaller.
In other words you will detect a physical touch quicker.

The inputP of the comparator is connected to the button TS1 IO group, and the inputM is connected
to the available VREF level (1/4Vref, 1/2Vref, 3/4Vref, Vref).

In this example, the inputP is connected to TSC_G6_IO1 (COMP_INPUT_PLUS_IO4) and the inputM to VREFINT.

In association of the inputM at VREF level, the threshold of the Touchsensing is tuned for the discovery
through the function tsl_user_SetThresholds().
The function tsl_user_SetThresholds() set the threshold depending of the comparator inputM value.

Some limitation can appear if the inputM level is set too low. If it is too low Touchsensing middleware
will have less range and therefore the measurement might approach the noise level.
Care should be taken at this point.

The touchsensing module software is composed of different steps:

   At first, the main module initialize the peripheral Touch, Comparator, RTC and middleware Touchsensing
   through respectively MX_TSC_Init(), MX_COMP2_Init(), MX_RTC_Init() and MX_TOUCHSENSING_Init().

   Then, the Touchsensing/TouchWakeup module displays in scroll mode the message "RUN MODE" twice ,
   and then initiate the calibration of the TSC, this phase is around 5 seconds.

   Finally, after starting, the RTC wakeup the MCU each 250 ms, in a while loop the Touchsensing/TouchWakeup
   module manage a detection and no detection in this way:

    If no touch is detected: The module display the message "ENTER STOP2 MODE", then it goes to low power stop 2 mode.
                             It stay in low power until RTC wakeup to catch if a touch is detected or not.
                             If no touch detected, the module go back to low power stop 2 mode.

    If a touch is detected: The module display the message "WAKEUP TOUCH DETECTED" during 5 seconds
                            then go back to low power stop 2 mode until RTC wakeup

STM32U083C-DK's LEDs can be used to monitor the Touch detection status:

- LED4 is ON when a touch is detected.
- LED4 is OFF when STM32U083C-DK enter in low power stop 2 mode.
- LED5 toggles when an error occurs.

The LCD is used as follow:

  - USE_LCD         : To use or not the glass LCD.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 3. This example can not be used in DEBUG mode, this is due to the fact
    that the Cortex-M0+ core is no longer clocked during low power mode
    so debugging features are disabled.

### <b>Keywords</b>

System, Touch sensing, TSC, Input, Output, Alternate function, Toggle, Glass LCD, Low Power

### <b>Directory contents</b>

  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/stm32u0xx_hal_conf.h          HAL configuration file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/stm32u0xx_it.h                Interrupt handlers header file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/main.h                        Header for main.c module
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/stm32u083c_discovery_conf.h   STM32U083C-DK board configuration file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/stmCriticalSection.h          Header for stmCriticalSection.c file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/touchsensing.h                Header for touchsensing.c file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/tsl_conf.h                    STMTouch driver configuration file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Inc/tsl_user.h                    Header for tsl_user.c file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/stm32u0xx_it.c                Interrupt handlers
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/main.c                        Main program
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/stm32u0xx_hal_msp.c           HAL MSP file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/system_stm32u0xx.c            STM32U0xx system source file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/stmCriticalSection.c          STMStudio lock/unlock mechanism file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/touchsensing.c                Configuration of the touchsensing instances file
  - Applications/TouchSensing/TouchSensing_1touchkey_LowPower/Src/tsl_user.c                    Touchsensing channels/banks description file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083xx devices.

  - This example has been tested with STM32U083C-DK (MB1933) board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

