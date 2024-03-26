## <b>USART_WakeUpFromStop1_Init Example Description</b>
  
Configuration of GPIO and USART2 peripherals to allow the characters received on USART_RX pin to wake up the MCU from low-power mode (Stop1). 
This example is based on the STM32U0xx USART2 LL API. The peripheral 
initialization uses LL unitary service functions for optimization purposes (performance and size).

USART2 Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
USART2 Clock is based on HSI.

Example execution:
After startup from reset and system configuration, LED4 is blinking quickly during 3 sec,
then MCU  enters "Stop 1" mode (LED4 off).
On first  character reception by the USART2 from PC Com port (ex: using HyperTerminal)
after "Stop 1" Mode period, MCU wakes up from "Stop 1" Mode.
Received character value is checked :

- On a specific value ('S' or 's'), LED4 is turned On and program ends.
- If different from 'S' or 's', program performs a quick LED4 blinks during 3 sec and
  enters again "Stop 1" mode, waiting for next character to wake up.

In case of errors, LED4 is slowly blinking (1 sec period).

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal,
Receiver, Asynchronous, Low Power, Wake Up

### <b>Directory contents</b> 

  - USART/USART_WakeUpFromStop1_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - USART/USART_WakeUpFromStop1_Init/Inc/main.h                  Header for main.c module
  - USART/USART_WakeUpFromStop1_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_WakeUpFromStop1_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - USART/USART_WakeUpFromStop1_Init/Src/main.c                  Main program
  - USART/USART_WakeUpFromStop1_Init/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect STM32 MCU board USART2 TX pin (GPIO PA.02 connected to pin 2 in CN9)
      to PC COM port RX signal
    - Connect STM32 MCU board USART2 RX pin (GPIO PA.03 connected to pin 3 in CN9)
      to PC COM port TX signal
    - Connect STM32 MCU board GND to PC COM port GND signal

    - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
      (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control). 

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
