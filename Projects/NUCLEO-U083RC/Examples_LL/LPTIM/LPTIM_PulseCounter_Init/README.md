## <b>LPTIM_PulseCounter_Init Example Description</b>

To reduce power consumption, MCU enters stop mode after starting counting. Each
time the counter reaches the maximum value (Period/Autoreload), an interruption
is generated, the MCU is woken up from stop mode and LED4 toggles the last state.

In this example Period value is set to 1000, so each time the counter counts
(1000 + 1) rising edges on LPTIM1_IN1 pin, an interrupt is generated and LED4
toggles. If the external function generator is set to provide a square waveform at 1Khz,
the led will toggle each second.

In this example the internal clock provided to the LPTIM1 is LSI (32 kHz),
so the external input is sampled with LSI clock. In order not to miss any event,
the frequency of the changes on the external Input1 signal should never exceed the
frequency of the internal clock provided to the LPTIM1 (LSI for the
present example).

Press reset button before connecting the signal.

### <b>Keywords</b>

Timer, Low Power, Pulse Counter, Stop mode, Interrupt

### <b>Directory contents</b>

  - LPTIM/LPTIM_PulseCounter_Init/Inc/stm32u0xx_it.h         Interrupt handlers header file
  - LPTIM/LPTIM_PulseCounter_Init/Inc/main.h                  Header for main.c module
  - LPTIM/LPTIM_PulseCounter_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - LPTIM/LPTIM_PulseCounter_Init/Src/stm32u0xx_it.c         Interrupt handlers
  - LPTIM/LPTIM_PulseCounter_Init/Src/main.c                  Main program
  - LPTIM/LPTIM_PulseCounter_Init/Src/system_stm32u0xx.c     STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.
    
  - Connect a square waveform generator to PC0 (pin 38 in CN7 connector).
    If the frequency of the signal is 1 kHz, LED4 toggles every second.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

