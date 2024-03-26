## <b>USART_HardwareFlowControl_Init Example Description</b>

Configuration of GPIO and  peripheral 
to receive characters asynchronously from an HyperTerminal (PC) in Interrupt mode 
with the Hardware Flow Control feature enabled. This example is based on STM32U0xx 
USART LL API. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

USART2 Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
HW flow control is used.
GPIO associated to User push-button is linked with EXTI.

Example execution:

After startup from reset and system configuration, LED4 is blinking quickly.

On press on User push-button, USART TX Empty interrupt is enabled.

First character of buffer to be transmitted is written into USART Transmit Data Register (TDR)
in order to initialise transfer procedure.

When character is sent from TDR, a TXE interrupt occurs.

USART2 IRQ Handler routine is sending next character on USART Tx line.

IT will be raised until last byte is to be transmitted : Then, Transmit Complete (TC) interrupt is enabled 
instead of TX Empty (TXE).

When character is received on USART Rx line, a RXNE interrupt occurs.

USART2 IRQ Handler routine is then checking received character value.

On a specific value ('S' or 's'), LED4 is turned On.

Received character is echoed on Tx line.

In case of errors, LED4 is blinking.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, 
hardware flow control, CTS/RTS, Transmitter, Receiver

### <b>Directory contents</b>

  - USART/USART_HardwareFlowControl_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - USART/USART_HardwareFlowControl_Init/Inc/main.h                  Header for main.c module
  - USART/USART_HardwareFlowControl_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_HardwareFlowControl_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - USART/USART_HardwareFlowControl_Init/Src/main.c                  Main program
  - USART/USART_HardwareFlowControl_Init/Src/system_stm32u0xx.c      STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32C031xx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    GPIOs connected to USART2 TX/RX/CTS/RTS signals should be wired
    to corresponding pins on PC UART (could be done through a USB to UART adapter).
    - Connect STM32 MCU board USART2 TX pin (PA.02 connected to pin 21 in CN10)
      to PC COM port RX signal
    - Connect STM32 MCU board USART2 RX pin (PA.03 connected to pin 33 in CN10)
      to PC COM port TX signal
    - Connect STM32 MCU board USART2 CTS pin (PA.00 connected to pin 36 in CN7)
      to PC COM port RTS signal
    - Connect STM32 MCU board USART2 RTS pin (PA.01 connected to pin 38 in CN7)
      to PC COM port CTS signal
    - Connect STM32 MCU board GND to PC COM port GND signal

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, HW flow control enabled).

  - Launch the program. Enter characters on PC communication SW side.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

