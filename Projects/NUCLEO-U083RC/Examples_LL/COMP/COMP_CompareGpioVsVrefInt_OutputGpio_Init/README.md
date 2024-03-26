## <b>COMP_CompareGpioVsVrefInt_OutputGpio_Init Example Description</b>

How to use a comparator peripheral to compare a voltage level applied on
a GPIO pin to the the internal voltage reference (VrefInt)
with comparator output connected to a GPIO pin.

This example is based on the STM32U0xx COMP LL API.

The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:

Comparator instance COMP1 is configured to compare:

- input plus set to a GPIO pin (cf pin below) in analog mode.

- input minus set to internal voltage reference VrefInt (voltage level 1.2V,
  refer to device datasheet for min/typ/max values)

A GPIO pin a configured in alternate function to be connected directly
to comparator output (cf pin below).

Example execution:

From the main program execution, comparator is enabled.

Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt voltage threshold (comparator input minus),
the comparator output set the logical level on its affected GPIO.
The program reads this GPIO state and reflects it on LED state.

LED4 is used to monitor program execution status:

- Normal operation:

  - LED4 turned on if comparator voltage level on input plus is higher than input minus

  - LED4 turned off if comparator voltage level on input plus is lower than input minus

- Error: LED4 remaining turned on

#### <b>Notes</b>

Use an external power supply to generate a voltage in range [0V; 3.3V]
and connect it to analog input pin (cf pin below).

Note: If no power supply available, this pin can be let floating (in this case
      comparator output state will be undetermined).

Other peripherals used:
  1 GPIO for LED4

  1 GPIO for analog input: pin PA.01 (Arduino connector CN8 pin A1)

  1 GPIO for digital output of comparator: PA.00 (Arduino connector CN8 pin A0)

### <b>Keywords</b>

Comparator, COMP, analog, voltage, analog input, threshold, VrefInt


### <b>Directory contents</b>

  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be

    easily tailored to any other supported device and development board.



### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
