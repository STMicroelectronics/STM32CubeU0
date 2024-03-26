## <b>COMP_CompareGpioVsVrefInt_Window_IT_Init Example Description</b>

How to use a pair of comparator peripherals to compare a voltage level applied
on a GPIO pin to two thresholds: the internal voltage reference (VrefInt)
and a fraction of the internal voltage reference (VrefInt/2), in interrupt mode.
This example is based on the STM32U0xx COMP LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:

Comparator instances COMP1 and COMP2 are configured to work together
in window mode to compare 2 thresholds:

- COMP1 input plus set to a GPIO pin (cf pin below) in analog mode.
  COMP2 input plus is not used (connected internally to the 
  same input plus as COMP1).
- COMP1 input minus set to internal voltage reference VrefInt 
  (voltage level 1.2V, refer to device datasheet for min/typ/max values)
- COMP2 input minus set to 1/2 of internal voltage reference VrefInt 
  (voltage level 0.6V)

Comparator interruption is enabled through EXTI lines 17 and 18
with trigger edge set to both edges rising and falling.

Example execution:

From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing one of the two thresholds related to VrefInt (comparator input minus),
the comparator with corresponding threshold generates an interruption.

LED4 is used to monitor program execution status:

- Normal operation:
  - LED turned on if comparator voltage level is inside window thresholds
    (between VrefInt and 1/2 VrefInt: between 1.2V and 0.6V)
  - LED turned off is out of window thresholds
- Error: LED remaining turned on

Connection needed:

Use an external power supply to generate a voltage in range [0V; 3.3V]
and connect it to analog input pin (cf pin below).

Note: If no power supply available, this pin can be let floating (in this case
      comparator output state will be undetermined).

Other peripherals used:

  1 GPIO for LED4
  1 GPIO for analog input: pin PA.01 (Arduino connector CN8 pin A1)

### <b>Keywords</b>

Comparator, COMP, analog, voltage, analog input, threshold, VrefInt, window

### <b>Directory contents</b>

  - COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_Window_IT_Init/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example