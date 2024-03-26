## <b>SPI_HalfDuplex_ComPolling_Slave Example Description</b>

This example shows how to ensure SPI data buffer half-duplex transmission/reception 
in Polling mode between two boards (Slave).


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The communication is handled with direct register access. 
On slave side, a state machine manages the different communication steps, to handle
two kinds of communication:

 - register read : master use this operation to get a value from the slave.
 - register write: master use this operation to update a value on slave side.

On master side basic operations are done by calling the functions : Command_write and Command_read.

If the Master board is used, the project SPI_HalfDuplex_ComPolling_Master must be used.
If the Slave board is used, the project SPI_HalfDuplex_ComPolling_Slave must be used.

This example desmonstrates how to implement a basic protocol between master and slave. 
The master is writing and reading a register and checking that write and read values are 
identical. The slave sw is an infinite loop which executes the operations requested by the master.

In a first step, after the user presses the User push-button, SPI Master starts slave communication, 
a loop to request READ_COUNTER_COMMAND and WRITE_COUNTER_COMMAND. The master controls the 
communication by a comparison between the read and write values. 

STM32 board's LEDs can be used to monitor the master transfer status:

 - LED4 toggles quickly on master board waiting User push-button to be pressed.
 - LED4 toggles very quickly on master when the test is running.
 - LED4 toggles slowly on master when register comparison failed.

#### <b>Notes</b>

1. You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
3. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, SPI, Half-duplex, Polling, Transmission, Reception, Master, Slave, MISO, MOSI,

### <b>Directory contents</b>

  - SPI/SPI_HalfDuplex_ComPolling_Slave/Inc/stm32u0xx_nucleo_conf.h     BSP configuration file
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Inc/stm32u0xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Inc/main.h                  Header for main.c module  
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Src/stm32u0xx_it.c          SPI interrupt handlers
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Src/main.c                  Main program
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Src/system_stm32u0xx.c      STM32U0xx system source file
  - SPI/SPI_HalfDuplex_ComPolling_Slave/Src/stm32u0xx_hal_msp.c     HAL MSP file
  

### <b>Hardware and Software environment</b>

  - This example runs on STM32U083RCTx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.

  -NUCLEO-U083RC Set-up

    - Connect Master board CLK to Slave Board CLK (PB.03 - CN10, pin 31)
    - Connect Master board MOSI (PB.05 - CN10, pin 29) to Slave Board MISO (PB.04 - CN10, pin 27)
    - Connect Master board GND to Slave Board GND
    - Slave board powered by master board
          - JP4 fitted on E5V on Slave board
          - Connect Master board +5V (pin 18 on CN7 connector) to Slave board E5V (pin 6 on CN7 connector)

### <b>How to use it ?</b>  

In order to make the program work, you must do the following:

 - Open your preferred toolchain 
 - Rebuild all files (master project) and load your image into target memory

    - Load the project in Master Board
 - Rebuild all files (slave project) and load your image into target memory
 
    - Load the project in Slave Board
    
 - Run the example