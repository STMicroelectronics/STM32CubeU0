## <b>LPUART_WakeUpFromStop2_Init Example Description</b>

Configuration of GPIO and LPUART peripherals to allow characters
received on LPUART_RX pin to wake up the MCU from low-power mode (Stop2). This example is based
on the LPUART LL API. The peripheral initialization uses LL
initialization function to demonstrate LL init usage.

LPUART Peripheral is configured in asynchronous mode (9600 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
LPUART Clock is based on HSI.

Example execution:
After startup from reset and system configuration, LED4 is blinking quickly during 3 sec,
then MCU  enters "Stop 2" mode (LED4 off).
On first  character reception by the LPUART from PC Com port (ex: using HyperTerminal)
after "Stop 2" Mode period, MCU wakes up from "Stop 2" Mode.
Received character value is checked :
- On a specific value ('S' or 's'), LED4 is turned On and program ends.
- If different from 'S' or 's', program performs a quick LED4 blinks during 3 sec and
  enters again "Stop 2" mode, waiting for next character to wake up.

In case of errors, LED4 is slowly blinking (1 sec period).

### <b>Keywords</b>

Connectivity, LPUART, baud rate, RS-232, HyperTerminal, full-duplex,
Transmitter, Receiver, Asynchronous, Low Power

### <b>Directory contents</b>

  - LPUART/LPUART_WakeUpFromStop2_Init/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - LPUART/LPUART_WakeUpFromStop2_Init/Inc/main.h                  Header for main.c module
  - LPUART/LPUART_WakeUpFromStop2_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - LPUART/LPUART_WakeUpFromStop2_Init/Src/stm32u0xx_it.c          Interrupt handlers
  - LPUART/LPUART_WakeUpFromStop2_Init/Src/main.c                  Main program
  - LPUART/LPUART_WakeUpFromStop2_Init/Src/system_stm32u0xx.c      STM32U0xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect STM32 MCU board LPUART1 TX pin (GPIO PA2 connected to pin 35 of CN10 connector)
      to PC COM port RX signal
    - Connect STM32 MCU board LPUART1 RX pin (GPIO PA3 connected to pin 37 of CN10 connector)
      to PC COM port TX signal
    - Connect STM32 MCU board GND to PC COM port GND signal

    - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
      (9600 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
