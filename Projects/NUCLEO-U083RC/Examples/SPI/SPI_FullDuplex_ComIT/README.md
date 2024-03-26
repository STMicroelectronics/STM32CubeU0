## <b>SPI_FullDuplex_ComIT Example Description</b>

Data buffer transmission/reception between two boards via SPI using Interrupt mode.

 - Board: NUCLEO-U083RC (embeds a STM32U083RC device)
 - CLK Pin: PA1 (CN8, pin 2)
 - MISO Pin: PA11 (CN10, pin 14)
 - MOSI Pin: PA12 (CN10, pin 12)

HAL architecture allows user to easily change code to move to DMA or Polling 
mode. To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComDMA
SPI\SPI_FullDuplex_ComPolling

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_IT() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode) 
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.

 - If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
 - If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive_IT(), at the same time SPI Slave transmits aTxBuffer 
and receives aRxBuffer through HAL_SPI_TransmitReceive_IT(). 
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update 
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 board's LEDs can be used to monitor the transfer status:

 - LED4 toggles quickly on master board waiting User push-button to be pressed.
 - LED4 turns ON if transmission/reception is complete and OK.
 - LED4 toggles slowly when there is a timeout or an error in transmission/reception process.   

#### <b>Notes</b>

 1. SPIx instance used and associated resources can be updated in "main.h"
    file depending hardware configuration used.

 2. You need to perform a reset on Slave board, then perform it on Master board
    to have the correct behaviour of this example.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

SPI, Full-duplex, Transmission, Reception,

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_ComIT/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComIT/Inc/stm32u0xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComIT/Inc/main.h                  Header for main.c module  
  - SPI/SPI_FullDuplex_ComIT/Src/stm32u0xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComIT/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_ComIT/Src/system_stm32u0xx.c      stm32u0xx system source file
  - SPI/SPI_FullDuplex_ComIT/Src/stm32u0xx_hal_msp.c     HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083xx devices.

  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U083RC Set-up
    - Connect Master board SCK pin (PA1) to Slave Board SCK pin (PA1) (connector CN8, pin 2)
    - Connect Master board MISO pin (PA11) to Slave Board MISO pin (PA11) (connector CN10, pin 14)
    - Connect Master board MOSI pin (PA12) to Slave Board MOSI pin (PA12) (connector CN10, pin 12)
    - Connect Master board GND  to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Uncomment "#define MASTER_BOARD" and load the project in Master Board
 - Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

