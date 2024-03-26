## <b>DMA_CopyFromFlashToMemory_Init Example Description</b>

How to use a DMA channel to transfer a word data buffer
from Flash memory to embedded SRAM. The peripheral initialization uses LL
initialization functions to demonstrate LL init usage.

At the beginning of the main program the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

Then the LED_Init() function is used to initialize the LED4.

Then the Configure_DMA() function is used to configure the DMA1_Channel1 to transfer the contents of a 32-word data
buffer stored in Flash memory to the reception buffer declared in RAM.

The start of transfer is triggered by software(LL_DMA_EnableChannel()). DMA1_Channel1 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.

The transfer is started by setting the channel enable bit for DMA1_Channel1.

At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.

Finally, aSRC_Const_Buffer and aDST_Buffer are compared through Buffercmp() in order to 
check buffers correctness.  

NUCLEO-U083RC's LED4 can be used to monitor the transfer status:

  - LED4 is turned ON if the DMA data transfer is successfully completed.
  - LED4 is blinking every 1 sec in case of error.

### <b>Keywords</b>

System, DMA, Data Transfer, Memory to memory, Flash, SRAM


### <b>Directory contents</b>

  - DMA/DMA_CopyFromFlashToMemory_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - DMA/DMA_CopyFromFlashToMemory_Init/Inc/main.h                  Header for main.c module  
  - DMA/DMA_CopyFromFlashToMemory_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - DMA/DMA_CopyFromFlashToMemory_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - DMA/DMA_CopyFromFlashToMemory_Init/Src/main.c                  Main program
  - DMA/DMA_CopyFromFlashToMemory_Init/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
