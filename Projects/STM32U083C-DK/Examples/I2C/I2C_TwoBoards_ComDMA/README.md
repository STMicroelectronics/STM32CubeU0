## <b>I2C_TwoBoards_ComDMA Example Description</b>

How to handle I2C data buffer transmission/reception between two boards, 
via DMA.

Board: STM32U083C-DK (embeds a STM32U083MCTx device)
SCL Pin: PB8 (CN5, pin10)
SDA Pin: PB7 (CN5, pin9)

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_DMA() and the HAL_I2C_Master_Transmit_DMA() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using DMA.
- Slave Board
  The HAL_I2C_Slave_Receive_DMA() and the HAL_I2C_Slave_Transmit_DMA() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using DMA.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the Joystick Sel push-button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_DMA()
to I2C Slave which receives aRxBuffer through HAL_I2C_Slave_Receive_DMA(). 
The second step starts when the user press the Joystick Sel push-button on the Master Board,
the I2C Slave sends aTxBuffer through HAL_I2C_Slave_Transmit_DMA()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive_DMA().
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32U083C-DK's LEDs can be used to monitor the transfer status:
 - LED4 is ON when the transmission process is complete.
 - LED4 is OFF when the reception process is complete.
 - LED5 is ON when there is an error in transmission/reception process.  

#### <b>Notes</b>
 -  Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 -  The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>
Connectivity, Communication, I2C, DMA, Master, Slave, Transmission, Reception, Fast mode plus


### <b>Directory contents</b> 

  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32u083c_discovery_conf.h     BSP configuration file
  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32u0xx_it.h          DMA and I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_ComDMA/Src/stm32u0xx_it.c          DMA and I2C interrupt handlers
  - I2C/I2C_TwoBoards_ComDMA/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComDMA/Src/system_stm32u0xx.c      STM32U0xx system source file
  - I2C/I2C_TwoBoards_ComDMA/Src/stm32u0xx_hal_msp.c     HAL MSP file    

### <b>Hardware and Software environment</b>

  - This example runs on STM32U031xx devices.
    
  - This example has been tested with STM32U083C-DK board and can be
    easily tailored to any other supported device and development board.    

  - STM32U083C-DK Set-up

    - Connect I2C_SCL line of Master board PB8 (CN5, pin10) to I2C_SCL line of Slave Board PB8 (CN5, pin10).
    - Connect I2C_SDA line of Master board PB7 (CN5, pin9) to I2C_SDA line of Slave Board PB7 (CN5, pin9) .
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

