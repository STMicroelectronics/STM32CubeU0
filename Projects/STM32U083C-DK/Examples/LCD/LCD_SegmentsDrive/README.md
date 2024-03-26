## <b>LCD_SegmentsDrive Example Description</b> 

How to use the embedded LCD glass controller to drive the on-board 
LCD glass by Pacific Display Devices.

The goal of this example is to display a "STM32U0xx" string on LCD glass in scrolling
mode. The scrolling number is fixed to 100 repetition and the speed is fixed to 
SCROLL_SPEED_LOW ms (defined in stm32u083c_discovery_glass_lcd.h file).

### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

LCD

### <b>Directory contents</b>

  - LCD/LCD_SegmentsDrive/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - LCD/LCD_SegmentsDrive/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - LCD/LCD_SegmentsDrive/Inc/main.h                  Header for main.c module  
  - LCD/LCD_SegmentsDrive/Src/stm32u0xx_it.c          Interrupt handlers
  - LCD/LCD_SegmentsDrive/Src/main.c                  Main program
  - LCD/LCD_SegmentsDrive/Src/stm32u0xx_hal_msp.c     HAL MSP file
  - LCD/LCD_SegmentsDrive/Src/system_stm32u0xx.c      STM32U0xx system source file


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
