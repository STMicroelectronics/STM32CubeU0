## <b>BSP Example Description</b>

How to use the different BSP drivers of the board.

This project is targeted to run on STM32U083xx devices on STM32U083C-DK board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 48 MHz.

This example shows how to use the different functionalities of components available on the board by switching between all tests using Joystick UP/DOWN buttons and RIGHT/LEFT button to launch the tests.

This example could be tested by selecting the required module from the menu dislyaed/scrolling on the LCD glass.

#### <b>LED</b>

- LED3, LED4 and LED5 are ON for a second, then OFF for a second, then toggle for 5 seconds and finally LED3, LED4 and LED5 are set back to OFF.

#### <b>JOYSTICK</b>

- Use any joystick buttons to display the corresponding direction on the LCD glass (UP/DOWN/SEL/RIGHT/LEFT). No buttons selection for 5 seconds allow to exit the Joystick test.


#### <b>LCD GLASS</b>
- This example shows how to use the different LCD glass features.

#### <b>TEMP SENSOR</b>
- This example shows how to use read the Temparture Sensor data and display on the LCD lass.

#### Notes
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

BSP, Led, Button, Joystick, LCD, Temp sensor 

### <b>Directory contents</b>

  - BSP/Src/main.c                      Main program
  - BSP/Src/system_stm32u0xx.c          STM32U0xx system clock configuration file
  - BSP/Src/stm32u0xx_it.c              Interrupt handlers
  - BSP/Src/stm32u0xx_hal_msp.c         HAL MSP module
  - BSP/Src/led.c                       Test Led features
  - BSP/Src/joystick.c                  joystick features
  - BSP/Src/lcdglass.c                  LCD Glass features
  - BSP/Src/led.c                       LEDs features
  - BSP/Src/tempsensor.c                Temperature sensor features
  - BSP/Inc/main.h                      Main program header file
  - BSP/Inc/stm32u083c_discovery_conf.h BSP Configuration file
  - BSP/Inc/stm32u0xx_hal_conf.h        HAL Configuration file
  - BSP/Inc/stm32u0xx_it.h              Interrupt handlers header file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083xx devices.
  - This example has been tested with STMicroelectronics STM32U083C-DK (MB1933)
    board and can be easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

