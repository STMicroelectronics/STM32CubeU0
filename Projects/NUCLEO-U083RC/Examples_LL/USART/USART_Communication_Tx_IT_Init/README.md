## <b>USART_Communication_Tx_IT_Init Example Description</b>

This example shows how to configure GPIO and USART peripheral to send characters
asynchronously to HyperTerminal (PC) in Interrupt mode. This example is based on
STM32U0xx USART LL API. Peripheral initialization is done using LL unitary services
functions for optimization purpose (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button is linked with EXTI.
Virtual Com port feature of STLINK could be used for UART communication between board and PC.

Example execution:

On press on push button , USART TX Empty interrupt is enabled.

First character of buffer to be transmitted is written into USART Transmit Data Register (TDR) in order to initialise transfer procedure.

When character is sent from TDR, a TXE interrupt occurs.

USART IRQ Handler routine is sending next character on USART Tx line.

IT will be raised until last byte is to be transmitted : Then, Transmit Complete (TC) interrupt is enabled
instead of TX Empty (TXE).

On last byte transmission complete, LED4 is turned on.

In case of errors, LED4 is blinking (1sec period).

Program is written so that, any new press on User push-button will lead to new transmission of complete buffer.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, Transmitter

### <b>Directory contents</b>

  - USART/USART_Communication_Tx_IT_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Tx_IT_Init/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Tx_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Tx_IT_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - USART/USART_Communication_Tx_IT_Init/Src/main.c                  Main program
  - USART/USART_Communication_Tx_IT_Init/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    Connect USART2 TX/RX to respectively RX and TX pins of PC UART (could be done through a USB to UART adapter) :
    - Connect STM32 MCU board USART2 TX pin (GPIO PA.02 connected to pin 35 in CN10)
      to PC COM port RX signal
    - Connect STM32 MCU board USART2 RX pin (GPIO PA.03 connected to pin 37 in CN10)
      to PC COM port TX signal
    - Connect STM32 MCU board GND to PC COM port GND signal

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).

  - Launch the program. Press on User push button on board to initiate data transfer.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
