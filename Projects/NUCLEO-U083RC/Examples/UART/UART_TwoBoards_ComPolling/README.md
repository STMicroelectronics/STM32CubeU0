## <b>UART_TwoBoards_ComPolling Example Description</b>

UART transmission (transmit/receive) in Polling mode 
between two boards.

- Board: NUCLEO-U083RC (embeds a STM32U083RCTx device)
- Tx Pin: PA.09 (Pin 21 in CN10)
- Rx Pin: PA.10 (Pin 33 in CN10)

Two identical boards are connected :

- Board 1: transmitting then receiving board
- Board 2: receiving then transmitting board

The user presses the User push-button on board 1.
Then, board 1 sends in polling mode a message to board 2 that sends it back to 
board 1 in polling mode as well.
Finally, board 1 and 2 compare the received message to that sent.
If the messages are the same, the test passes.

WARNING: as both boards do not behave the same way, "TRANSMITTER_BOARD" compilation
switch is defined in /Src/main.c and must be enabled
at compilation time before loading the executable in the board that first transmits
then receives.
The receiving then transmitting board needs to be loaded with an executable
software obtained with TRANSMITTER_BOARD disabled. 

NUCLEO-U083RC board LED is used to monitor the transfer status:

- While board 1 is waiting for the user to press the User push-button, its LED4 is
  blinking rapidly (100 ms period).
- While board 2 is waiting for the message from board 1, its LED4 is emitting
  a couple of flashes every half-second.
- When the test passes, LED4 on both boards is turned on, otherwise the test has failed. 
- If there is an initialization or transfer error, LED4 is slowly blinking (1 sec. period).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The UART is configured as follows:

    - BaudRate = 9600 baud  
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

#### <b>Notes</b>

 1. When the parity is enabled, the computed parity is inserted at the MSB position of the transmitted data.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, baud rate, RS-232, full-duplex, Polling, parity, stop bit,
Transmitter, Receiver, Asynchronous

### <b>Directory contents</b>

  - UART/UART_TwoBoards_ComPolling/Inc/stm32u0xx_nucleo_conf.h BSP configuration file
  - UART/UART_TwoBoards_ComPolling/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - UART/UART_TwoBoards_ComPolling/Inc/stm32u0xx_it.h          interrupt handlers header file
  - UART/UART_TwoBoards_ComPolling/Inc/main.h                  Header for main.c module  
  - UART/UART_TwoBoards_ComPolling/Src/stm32u0xx_it.c          interrupt handlers
  - UART/UART_TwoBoards_ComPolling/Src/main.c                  Main program
  - UART/UART_TwoBoards_ComPolling/Src/stm32u0xx_hal_msp.c     HAL MSP module
  - UART/UART_TwoBoards_ComPolling/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083RCTx devices.    
  - This example has been tested with two NUCLEO-U083RC boards embedding
    a STM32U083RCTx device and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U083RC set-up
    - Connect a wire between 1st board PA9 (Pin 21 in CN10) and 2nd board PA10 (Pin 33 in CN10)
    - Connect a wire between 1st board PA10 (Pin 33 in CN10) and 2nd board PA9 (Pin 21 in CN10)
    - Connect 1st board GND to 2nd Board GND    

### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
