## <b>FLASH_ChangeOptionBytes Example Description</b>
How to configure and modify the Option Bytes of the Flash memory controller.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 48 MHz.

After Reset, the Flash memory controller is locked. Dedicated functions are used to enable the
FLASH control register access and the FLASH option bytes register access.

The Option Bytes modification procedure is done by filling the Option Bytes init structure giving
the option byte type and the option parameters. At this stage, the new option bytes will be programmed
through the HAL FLASH OB programming API. Then, an OBL_Launch command is executed to start option bytes loading.

Once this operation is launched, the STM32U0 device will reset and debugger connection will be lost.

#### <b>Notes</b>

In this example, the user can switch between the RDP Levels using the USER_BUTTON.
The NUCLEO-U083RC board LEDs can be used to monitor the RDP status:

 - LED4 is blinking with a frequency of 200ms when RDP Level is equal to 0.
 - LED4 is blinking with a frequency 1s when RDP Level is equal to 1.
 - Due to STLink's limitation, make sure to disable the STLink Mass Storage interface by using STLinkUpgrade and 
   changing the type from Debug+VCP+Mass Storage to Debug+VCP.
 - After flashing the example on the board, make sure to remove the ST-Link jumper from the board and put it in CHG.
 - User need to keep pushing the PB for several seconds when switching from RDP level 1 to level 0.
 - STLink Cable unplug/plug to perform hard reset is required after RDP level 1 to level 0 regression.

### <b>Keywords</b>

FLASH, Memory

### <b>Directory contents</b>

  - FLASH_ChangeOptionBytes/Src/main.c                 Main program
  - FLASH_ChangeOptionBytes/Src/system_stm32u0xx.c     STM32U0xx system clock configuration file
  - FLASH_ChangeOptionBytes/Src/stm32u0xx_it.c         Interrupt handlers
  - FLASH_ChangeOptionBytes/Src/stm32u0xx_hal_msp.c    HAL MSP module
  - FLASH_ChangeOptionBytes/Inc/main.h                 Main program header file
  - FLASH_ChangeOptionBytes/Inc/stm32u0xx_hal_conf.h   HAL Configuration file
  - FLASH_ChangeOptionBytes/Inc/stm32u0xx_it.h         Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083xx devices.

  - This example has been tested with NUCLEO-U083RC board and can be easily tailored to any other
    supported device and development board.

   - NUCLEO-U083RC Set-up
     - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
     - A virtual COM port will then appear in the HyperTerminal.

  - Hyperterminal configuration:
    - Data Length : 8 Bits
    - Stop Bit : 1 Bit
    - Parity: None
    - BaudRate = 115200 baud
    - Flow control: None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example