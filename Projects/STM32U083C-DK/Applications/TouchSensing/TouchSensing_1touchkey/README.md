## <b>TouchSensing_1touchkey Application Description</b>

Use of the TSC to perform continuous acquisitions of one channel in Interrupt mode.

This project is targeted to run on STM32U083xx devices on STM32U083C-DK board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The TSC peripheral configuration is ensured by the HAL_TSC_Init() function.
This later is calling the HAL_TSC_MspInit()function which core is implementing
the configuration of the needed TSC resources according to the used hardware (CLOCK,
GPIO and NVIC). You may update this function to change TSC configuration.

First we initialize the hardware (leds). This step is assured by SystemHardwareInit().

Push the USER button to start the example.

The TSC acquisition is then initiated by TscInit().
When the end of acquisition occurs, the acquired data are affected to the TSCAcquisitionValue variable.
Depending of value of TSCAcquisitionValue under the max threshold values, the corresponding LEDs are set ON.
These max threshold values can be modified in top of main.c, due to disparity link to board layout quality

The LEDs are used as follow:

- LED3 toggles when ECS is ON (when no touch is detected (all sensors are in the RELEASE state)).
- LED4 is ON when a touch is detected.
- LED4 is OFF when the touch is released.
- LED5 toggles when an error occurs.

The LCD is used as follow:

  - USE_LCD         : To use or not the glass LCD.


#### <b>Notes</b>

 1. TSCx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

 2. TSCx_TS1_MAXTHRESHOLD can be modified if expected result is not satisfied.
This threshold value is linked to the board layout quality.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, TSC, Input, Output, Alternate function, Toggle, Glass LCD

### <b>Directory contents</b>

  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32u0xx_hal_conf.h          HAL configuration file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32u0xx_it.h                Interrupt handlers header file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/main.h                        Header for main.c module
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32u083c_discovery_conf.h   STM32U083C-DK board configuration file
  - TSC/TSC_BasicAcquisition_Interrupt/Src/stm32u0xx_it.c                Interrupt handlers
  - TSC/TSC_BasicAcquisition_Interrupt/Src/main.c                        Main program
  - TSC/TSC_BasicAcquisition_Interrupt/Src/stm32u0xx_hal_msp.c           HAL MSP file
  - TSC/TSC_BasicAcquisition_Interrupt/Src/system_stm32u0xx.c            STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083xx devices.

  - This example has been tested with STM32U083C-DK (MB1933) board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

