## <b>ADC_TemperatureSensor Example Description</b>

How to use an ADC peripheral to perform a single ADC conversion on the 
internal temperature sensor and calculate the temperature in degrees Celsius. 
This example uses the polling programming model (for interrupt or DMA 
programming models, refer to other examples).
This example is based on the STM32U0xx ADC HAL API.

Example configuration:

ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.

Sequencer of ADC group regular (default group available on ADC of all STM32 devices)
is configured to convert channel: internal channel temperature sensor.

Example execution:

Program configures ADC and GPIO.

Then, program performs in infinite loop:

- start ADC conversion
- poll for conversion completion
- read conversion data
- compute temperature from raw data to physical values using LL ADC driver helper macro (unit: degree Celsius)

LED4 is used to monitor program execution status:

- Normal operation: Turn on LED during 1s after each temperature computation,
  then turn off during 0.2s before restarting an infinite loop cycle.
- Error: LED remaining turned on

Debug: variables to monitor with debugger:

- "uhADCxConvertedData": ADC group regular conversion data
- "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celsius)

Connection needed:

None.

Other peripherals used:

  1 GPIO for LED4

### <b>Keywords</b>

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, trigger, temperature sensor

### <b>Directory contents</b>

  - ADC/ADC_TemperatureSensor/Inc/stm32u083c_discovery_conf.h     BSP configuration file
  - ADC/ADC_TemperatureSensor/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - ADC/ADC_TemperatureSensor/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - ADC/ADC_TemperatureSensor/Inc/main.h                  Header for main.c module
  - ADC/ADC_TemperatureSensor/Src/stm32u0xx_it.c          Interrupt handlers
  - ADC/ADC_TemperatureSensor/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - ADC/ADC_TemperatureSensor/Src/main.c                  Main program
  - ADC/ADC_TemperatureSensor/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with STM32U083C-DK board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example