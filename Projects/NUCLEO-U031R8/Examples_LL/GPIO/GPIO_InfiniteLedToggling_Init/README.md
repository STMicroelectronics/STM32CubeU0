## <b>GPIO_InfiniteLedToggling_Init Example Description</b>

How to configure and use GPIOs to toggle the on-board user LEDs
every 250 ms. This example is based on the STM32U0xx LL API. The peripheral
is initialized with LL initialization function to demonstrate LL init usage.

PA.05 IO (configured in output pushpull mode) toggles in a forever loop.
On NUCLEO-U031R8 board this IO is connected to LED4.

In this example, HCLK is configured at 48 MHz.

### <b>Keywords</b>

System, GPIO, Output, No pull, Speed, Push-pull, Toggle

### <b>Directory contents</b>

  - GPIO/GPIO_InfiniteLedToggling_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_InfiniteLedToggling_Init/Inc/main.h                  Header for main.c module
  - GPIO/GPIO_InfiniteLedToggling_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - GPIO/GPIO_InfiniteLedToggling_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - GPIO/GPIO_InfiniteLedToggling_Init/Src/main.c                  Main program
  - GPIO/GPIO_InfiniteLedToggling_Init/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U031R8Tx devices.

  - This example has been tested with NUCLEO-U031R8 board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


