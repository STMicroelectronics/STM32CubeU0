## <b>LPUART_TwoBoards_ComIT Example Description</b>

LPUART transmission (transmit/receive) in Interrupt mode 
between two boards.

 - Board: NUCLEO-U031R8 (embeds a STM32U031R8 device)
 - Tx Pin: PC.1 (pin 5 on CN8)
 - Rx Pin: PC.0 (pin 6 on CN8)

Two identical boards are connected :

 - Board 1: transmitting then receiving board
 - Board 2: receiving then transmitting board

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The user presses the User push-button on board 1.
Then data are transmitted through LPUART1 in IT mode. 2nd board has configured LPUART1
to receive data in IT mode. Once data received is completed, 2nd board also 
transmit same characters on LPUART1 which will be received by 1st board.
Both boards compare transmitted data to received ones. If they are same,
LED4 is turned on, else LED4 is turned off.

Warning: As both boards do not behave same way, "TRANSMITTER_BOARD" switch 
compilation exists to determine either software is for 1st transmitter board or 
2nd receiver (then transmitter) board. In other words, 1st board has to be 
flashed with software compiled with switch enable, 2nd board has to be flashed 
with software compiled with switch disable.

LED4 can be used to monitor the transfer status:

 - LED4 is ON on master board waiting User push-button to be pressed.
 - LED4 turns OFF on master board waiting the transmission process complete.
 - LED4 turns ON when the transmission/reception process is correct.
 - If there is an initialization or transfer error, LED4 is slowly blinking (1 sec. period).

The LPUART is configured as follow:

    - BaudRate = 115200 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - None parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

#### <b>Notes</b>

 1. LPUART instance used and associated resources can be updated in "main.h"
    file depending hardware configuration used.

 2. When the parity is enabled, the computed parity is inserted at the MSB
    position of the transmitted data.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, LPUART, baud rate, RS-232, full-duplex, Interrupt
Transmitter, Receiver, Asynchronous, Low Power

### <b>Directory contents</b>

  - UART/LPUART_TwoBoards_ComIT/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - UART/LPUART_TwoBoards_ComIT/Inc/stm32u0xx_it.h          IT interrupt handlers header file
  - UART/LPUART_TwoBoards_ComIT/Inc/main.h                  Header for main.c module  
  - UART/LPUART_TwoBoards_ComIT/Src/stm32u0xx_it.c          IT interrupt handlers
  - UART/LPUART_TwoBoards_ComIT/Src/main.c                  Main program
  - UART/LPUART_TwoBoards_ComIT/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - UART/LPUART_TwoBoards_ComIT/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U031xx devices.

  - This example has been tested with NUCLEO-U031R8 and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U031R8 set-up
    - Connect a wire between 1st board PC.1 (pin 5 on CN8) pin (LPUART Tx) and 2nd board PC.0 (pin 6 on CN8) pin (LPUART Rx).
    - Connect a wire between 1st board PC.0 (pin 6 on CN8) pin (LPUART Rx) and 2nd board PC.1 (pin 5 on CN8) pin (LPUART Tx).
    - Connect 1st board GND to 2nd Board GND.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

