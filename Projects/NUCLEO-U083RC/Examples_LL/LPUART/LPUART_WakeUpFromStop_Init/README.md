## <b>LPUART_WakeUpFromStop_Init Example Description</b>
  
Configuration of GPIO and LPUART peripherals to allow characters 
received on LPUART_RX pin to wake up the MCU from low-power mode (Stop). This example is based 
on the LPUART LL API. The peripheral initialization uses LL 
initialization function to demonstrate LL init usage.

LPUART Peripheral is configured in asynchronous mode (9600 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
LPUART Clock is based on HSI.

Example execution:

After startup from reset and system configuration, LED4 is blinking quickly during 3 sec,
then MCU  enters "Stop 0" mode (LED4 off).
On first  character reception by the LPUART from PC Com port (ex: using HyperTerminal)
after "Stop 0" Mode period, MCU wakes up from "Stop 0" Mode.

Received character value is checked :

- On a specific value ('S' or 's'), LED4 is turned On and program ends.
- If different from 'S' or 's', program performs a quick LED4 blinks during 3 sec and 
  enters again "Stop 0" mode, waiting for next character to wake up.

In case of errors, LED4 is slowly blinking (1 sec period).

### <b>Keywords</b>

Connectivity, LPUART, baud rate, RS-232, HyperTerminal, full-duplex,
Transmitter, Receiver, Asynchronous, Low Power

### <b>Directory contents</b>

  - LPUART/LPUART_WakeUpFromStop_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - LPUART/LPUART_WakeUpFromStop_Init/Inc/main.h                  Header for main.c module
  - LPUART/LPUART_WakeUpFromStop_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - LPUART/LPUART_WakeUpFromStop_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - LPUART/LPUART_WakeUpFromStop_Init/Src/main.c                  Main program
  - LPUART/LPUART_WakeUpFromStop_Init/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    Example is delivered for using Virtual Com port feature of STLINK for connection between NUCLEO-U083RC and PC,
    GPIOs connected to LPUART1 TX/RX (PA2 and PA3) are automatically mapped
    on RX and TX pins of PC UART Com port selected on PC side (please ensure VCP com port is selected).

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (9600 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control). 

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

