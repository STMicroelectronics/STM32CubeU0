### <b>SPI_FullDuplex_ComPolling_Master Example Description</b>

Data buffer transmission/reception between two boards via SPI using Polling mode (Master).

Board: NUCLEO-U083RC (embeds a STM32U083RCTx device)

 - CLK Pin: PB3 (CN10, pin 31)
 - MISO Pin: PB4 (CN10, pin 27)
-  MOSI Pin: PB5 (CN10, pin 29)

HAL architecture allows user to easily change code to move to IT or DMA mode. 
To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComDMA_Master and SPI\SPI_FullDuplex_ComDMA_Slave
SPI\SPI_FullDuplex_ComIT_Master and SPI\SPI_FullDuplex_ComIT_Slave

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode).
If the Master board is used, the project SPI_FullDuplex_ComPolling_Master must be used.
If the Slave board is used, the project SPI_FullDuplex_ComPolling_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive(), at the same time SPI Slave transmits aTxBuffer 
and receives aRxBuffer through HAL_SPI_TransmitReceive(). 
The end of this step is monitored through the HAL_SPI_GetState() function
result.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

#### <b>Notes</b>

 1. Timeout is set to 5 seconds which means that if no communication occurs during 5 seconds,
    a timeout error is generated.

 2. STM32 board LED can be used to monitor the transfer status:
 - LED4 toggles quickly on master board waiting User push-button to be pressed.
 - LED4 turns ON if transmission/reception is complete and OK.
 - LED4 toggles slowly when there is a timeout or an error in transmission/reception process.   

 3. You need to perform a reset on Slave board, then perform it on Master board
    to have the correct behaviour of this example.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Polling, Transmission, Reception, Master, Slave, MISO, MOSI,

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_ComPolling_Slave/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - SPI/SPI_FullDuplex_ComPolling_Slave/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComPolling_Slave/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComPolling_Slave/Inc/main.h                  Header for main.c module  
  - SPI/SPI_FullDuplex_ComPolling_Slave/Src/stm32u0xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComPolling_Slave/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_ComPolling_Slave/Src/system_stm32u0xx.c      stm32u0xx system source file
  - SPI/SPI_FullDuplex_ComPolling_Slave/Src/stm32u0xx_hal_msp.c     HAL MSP file

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U083RCTx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect Master board PB3 (CN10, pin 31) to Slave Board PB3 (CN10, pin 31)
    - Connect Master board PB4 (CN10, pin 27) to Slave Board PB4 (CN10, pin 27)
    - Connect Master board PB5 (CN10, pin 29) to Slave Board PB5 (CN10, pin 29)
    - Connect Master board GND  to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain 

 - Rebuild all files (master project) and load your image into target memory o Load the project in Master Board

 - Rebuild all files (slave project) and load your image into target memory o Load the project in Slave Board

 - Run the example

