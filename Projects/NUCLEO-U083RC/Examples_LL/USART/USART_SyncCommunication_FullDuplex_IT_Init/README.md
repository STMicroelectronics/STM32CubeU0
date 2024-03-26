## <b>USART_SyncCommunication_FullDuplex_IT_Init Example Description</b>

Configuration of GPIO, USART, DMA and SPI peripherals to transmit 
bytes between a USART and an SPI (in slave mode) in Interrupt mode. This example is based on the STM32U0xx USART LL API 
(the SPI uses the DMA to receive/transmit characters sent from/received by the USART). The peripheral 
initialization uses LL unitary service functions for optimization purposes (performance and size).

This example works with only one NUCLEO-U083RC.


USART1 Peripheral is acts as Master using IT.
SPI1 Peripheral is configured in Slave mode using DMA.
GPIO associated to User push-button is linked with EXTI.

Example execution:

LED4 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD start a Full-Duplex communication from USART using IT.
On USART1 side, Clock will be generated on SCK line, Transmission done on TX, reception on RX.
On SPI1 side, reception is done through the MOSI Line, transmission on MISO line.

At end of transmission, both received buffers are compared to expected ones.
In case of both transfers successfully completed, LED4 is turned on.
In case of errors, LED4 is blinking Slowly (1s).


### <b>Keywords</b>

Connectivity, UART/USART, Synchronous, Interrupt, SPI interface, Receiver, Transmitter, full-duplex

### <b>Directory contents</b>

  - USART/USART_SyncCommunication_FullDuplex_IT_Init/Inc/stm32u0xx_it.h        Interrupt handlers header file
  - USART/USART_SyncCommunication_FullDuplex_IT_Init/Inc/main.h                 Header for main.c module
  - USART/USART_SyncCommunication_FullDuplex_IT_Init/Inc/stm32_assert.h         Template file to include assert_failed function
  - USART/USART_SyncCommunication_FullDuplex_IT_Init/Src/stm32u0xx_it.c        Interrupt handlers
  - USART/USART_SyncCommunication_FullDuplex_IT_Init/Src/main.c                 Main program
  - USART/USART_SyncCommunication_FullDuplex_IT_Init/Src/system_stm32u0xx.c    STM32U0xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect USART1 SCK PB.05 to Slave SPI1 SCK PA.01
    - Connect USART1 TX PB.06 to Slave SPI1 MOSI PA.07
    - Connect USART1 RX PB.07 to Slave SPI1 MISO PA.06

Board connector:

 - PB.05 : connected to pin 5 of CN9 connector
 - PA.01 : connected to pin 2 of CN8 connector
 - PB.06 : connected to pin 3 of CN5 connector
 - PA.07 : connected to pin 15 of CN10 connector
 - PB.07 : connected to pin 21 of CN7 connector
 - PA.06 : connected to pin 13 of CN10 connector

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Launch the program. Press on User push-button on board to initiate data transfers.

