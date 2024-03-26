## <b>CORTEX_MPU example Description</b>

Presentation of the MPU features. This example configures MPU attributes of different
MPU regions then configures a memory area as privileged read-only, and attempts to
perform read and write operations in different modes.

If the access is permitted LED4 is toggling. If the access is not permitted,
a memory management fault is generated and LED4 is ON.

To generate an MPU memory fault exception due to an access right error, press
user button.

### <b>Notes</b>

  - Using STM32CubeIDE debugger, when hard fault is generated user shall press the debugger "Run" button to continue the execution

### <b>Keywords</b>

MPU, interrupt, MPU memory fault exception,

### <b>Directory contents</b>

  - CORTEX/CORTEX_MPU/Inc/stm32u0xx_it.h         Interrupt handlers header file
  - CORTEX/CORTEX_MPU/Inc/main.h                  Header for main.c module
  - CORTEX/CORTEX_MPU/Inc/stm32_assert.h          Template file to include assert_failed function
  - CORTEX/CORTEX_MPU/Src/stm32u0xx_it.c         Interrupt handlers
  - CORTEX/CORTEX_MPU/Src/main.c                  Main program
  - CORTEX/CORTEX_MPU/Src/system_stm32u0xx.c     STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with a STM32U083RCTx embedded on an
    NUCLEO-U083RC board and can be easily tailored to any other supported
    device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

