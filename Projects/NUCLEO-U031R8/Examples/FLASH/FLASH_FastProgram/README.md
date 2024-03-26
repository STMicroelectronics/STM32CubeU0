## <b>FLASH_FastProgram Example Description</b>

How to configure and use the FLASH HAL API to erase and fast program the
internal Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 48 MHz.

After Reset, the Flash memory Program/Erase Controller is locked. A dedicated function
is used to enable the FLASH control register access.
Before programming the desired addresses, an erase operation is performed using
the flash erase page feature. The erase procedure is done by filling the erase init
structure giving the bank which will be mass erased.

Once this operation is finished, page fast programming operation will be performed
in the Flash memory. The written data is then read back and checked.

The NUCLEO-U031R8 board LEDs can be used to monitor the transfer status:

 - LED4 is ON when there are no errors detected after programming
 - LED4 is blinking with a 1s period when there are errors detected after programming
 - LED4 is blinking (100ms on, 2s off) when there is an issue during erase or program procedure

#### <b>Notes</b>

1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

3. All the executable code is mapped in SRAM1 area.

4. The example only works in debug mode.

### <b>Keywords</b>

Memory, FLASH, Fast Program, Sector, Mass Erase

### <b>Directory contents</b>

  - FLASH/FLASH_FastProgram/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - FLASH/FLASH_FastProgram/Inc/stm32u0xx_hal_conf.h        HAL Configuration file
  - FLASH/FLASH_FastProgram/Inc/stm32u0xx_it.h              Header for stm32u0xx_it.c
  - FLASH/FLASH_FastProgram/Inc/main.h                      Header for main.c module
  - FLASH/FLASH_FastProgram/Src/stm32u0xx_it.c              Interrupt handlers
  - FLASH/FLASH_FastProgram/Src/main.c                      Main program
  - FLASH/FLASH_FastProgram/Src/stm32u0xx_hal_msp.c         MSP initialization and de-initialization
  - FLASH/FLASH_FastProgram/Src/system_stm32u0xx.c          STM32U0xx system clock configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U031R8Ix devices.

  - This example has been tested with NUCLEO-U031R8 board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
