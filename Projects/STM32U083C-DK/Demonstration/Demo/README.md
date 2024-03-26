## <b>Demo Description</b>

The STM32Cube demonstration platform comes on top of the STM32Cube as a
firmware package that offers a full set of software components based on a modular
architecture. All modules can be reused separately in standalone applications.

The STM32Cube demonstration platform is based on the STM32Cube HAL BSP and Touch Sensing middleware
components and running on STM32U083C-DK board.

### <b>Demonstration Overview</b>

At startup once the "STM32U083C-DK Demo" message display is completed, the supported demos liste will be displayed.

Use the joystick keys (UP & DOWN) to navigate through the supported demos liste. Use the SEL & RIGHT key to select the demonstration modules.

### Below you find an overview of the different modules present in the demonstration:

MENU NAVIGATION
========================================
Done with joystick keys:

- RIGHT & SEL :   launch application or enter in sub-menu 
- LEFT        :   exit application or back to upper menu level
- DOWN        :   next menu item or switch to Fahrenheit in temperature application
- UP          :   previous menu item or switch to Celsius in temperature application 

LEDs
========================================
- LED3/4/5 are turned ON after demo start.
- LED3/4/5 are OFF when STM32U083C-DK enter in low power stop modes.
- LED4 is ON when a touch is detected (for the touch wakeup module).
- LED4 is OFF when STM32U083C-DK enter in low power stop 2 mode (for the touch wakeup module).
- LED4 is toggling during the ULP modes selection and after wakeup from ULP modes.

### 

========================================
APPLICATIONS LIST
========================================

+ Air Quality Demo
========================================

The MIKROE-2953 Sensor module allows the measurements of the air quality.

This is done through the use of an I2C based MICROE (CCS811) sensor that can be easily connected to the board via CN13 and CN14.

Users can view the CO2 and TVOC measurements on the Glass LCD in a loop.

The application displays messages such as NORMAL/POLLUTION/HIGH POLLUTION to indicate the pollution levels based on threshold values.

To switch to another demo module, user could press the LEFT joystick key for 5 seconds.

If, the air quality sensor is not connected the Air Quality application/demo will not be displayed.

+ Temperature Sensor Demo
========================================

The Temperature Sensor module allows the measurements of temperature.

This is done via the use of an I2C based STTS22H sensor build with the STM32U083C-DK board.

The application will continuously display the Temperature measurements in the Glass LCD.

Users can switch between Celsius and Fahrenheit format using the UP/DOWN joystick keys.

To switch to another demo module, the user could press the LEFT joystick key for 5 seconds.

+ TouchWakeup/Touchsensing Demo
========================================
The Touchsensing module allows the detection of a touch on the Touchsensor button TSC1 after a low power phase using
the embedded Comparator peripheral to decrease power consumption.

In this specific STM32U0xx series, some Touch IO pins are interconnected with the Comparator module
which gives the possibility to change the detection voltage level.
Changing this voltage level will help in detecting a physical Touch earlier, depending on the value
of the comparator input.

This means that the lower the level the less time you will need to reach it and thus an acquisition cycle is smaller.
In other words, you will detect a physical touch quicker.

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

   At first, the main module initializes the peripheral Touch, Comparator, RTC and middleware Touchsensing
   through respectively MX_TSC_Init(), MX_COMP2_Init(), MX_RTC_Init() and MX_TOUCHSENSING_Init().
   
   Then, the Touchsensing/TouchWakeup module displays in scroll mode the message "RUN MODE" twice,
   and then initiates the calibration of the TSC, this phase is around 5 seconds.
   
   Finally, after starting, the RTC wakeup the MCU each 250 ms, in a while loop the Touchsensing/TouchWakeup
   module manage a detection and no detection in this way:

    If no touch is detected: The module displays the message "ENTER STOP2 MODE", then it goes to low power stop 2 mode.
                             It stays in low power until RTC wakes up to catch if a touch is detected or not.
                             If no touch is detected, the module goes back to low power stop 2 mode.
    
    If a touch is detected: The module displays the message "WAKEUP TOUCH DETECTED" for 5 seconds
                            then go back to low power stop 2 mode until RTC wakes up.

TM32U083C-DK's LEDs can be used to monitor the Touch detection status:

- LED4 is ON when a touch is detected.
- LED4 is OFF when STM32U083C-DK enter in low power stop 2 mode.

To switch to another demo module, the user could press the LEFT joystick key for 5 seconds.

+ Ultra Low Power Demo
========================================

Users can switch between ULP modes using the UP/DOWN joystick keys. RIGHT or SEL joystick key are used to select the ULP mode.
Once ULP mode is selected, the system will enter ULP mode for ~33 seconds when goes out from ULP mode.
If a user would like to exit from stop mode before the ~33 seconds, he could use the "SEL" joystick key (during ULP the "SEL" joystick key is switched to Push Button mode). 
During ULP mode, the Glass LCD will display the typical power consumption (no embedded measurement).

After Wakeup from stop mode, user could switch to another demo module by pressing the LEFT joystick key for 5 seconds.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

LCD, BSP, RTC, low power modes

### <b>Directory contents</b>

  - Templates/Src/main.c                 Main program
  - Templates/Src/system_stm32u0xx.c     STM32U0xx system clock configuration file
  - Templates/Src/stm32u0xx_it.c         Interrupt handlers
  - Templates/Src/stm32u0xx_hal_msp.c    HAL MSP module
  - Templates/Inc/main.h                 Main program header file
  - Templates/Inc/stm32u0xx_hal_conf.h   HAL Configuration file
  - Templates/Inc/stm32u0xx_it.h         Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This demonstration runs on STM32U083xx devices.

  - This demonstration has been compiled on EWARM 9.20.4

  - This demonstration has been tested with STMicroelectronics STM32U083C-DK (80 pins)
    board embedding a multispeed clock MSI and can be easily tailored to any other
    supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the demonstration