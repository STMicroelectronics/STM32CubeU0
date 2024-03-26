## <b>LCD_Blink_Frequency Example Description</b> 

How to use the embedded LCD glass controller and how to set the LCD 
blink mode and blinking frequency.

After Reset the Blink frequency is equal to LCDCLK/16 and LCD Glass display "BK0016" string.
By pressing on the Joystick Sel push-button (PC.02) the Blink frequency is incremented 
and the message displayed on the Pacific Display LCD glass is proportional 
to the blink frequency.

In this example, 7 different Blink frequency are used:

   - The Blink frequency = fLcd/16

   - The Blink frequency = fLcd/32

   - The Blink frequency = fLcd/64

   - The Blink frequency = fLcd/128

   - The Blink frequency = fLcd/256

   - The Blink frequency = fLcd/512

   - The Blink frequency = fLcd/1024

### <b>Note</b> 
   Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
### <b>Note</b> 
The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

LCD

### <b>Directory contents</b>

  - LCD/LCD_Blink_Frequency/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - LCD/LCD_Blink_Frequency/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - LCD/LCD_Blink_Frequency/Inc/main.h                  Header for main.c module  
  - LCD/LCD_Blink_Frequency/Src/stm32u0xx_it.c          Interrupt handlers
  - LCD/LCD_Blink_Frequency/Src/main.c                  Main program
  - LCD/LCD_Blink_Frequency/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - LCD/LCD_Blink_Frequency/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices.

  - This example has been tested with STMicroelectronics STM32U083C-DK
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
