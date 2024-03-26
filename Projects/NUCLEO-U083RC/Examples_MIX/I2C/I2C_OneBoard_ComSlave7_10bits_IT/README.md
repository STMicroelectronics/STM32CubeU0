## <b>I2C_OneBoard_ComSlave7_10bits_IT Example Description</b>

How to perform I2C data buffer transmission/reception between 
one master and two slaves with different address sizes (7-bit or 10-bit). This example 
uses the STM32U0xx I2C HAL and LL API (LL API usage for performance improvement)
and an interrupt.

       Board: NUCLEO-U083RC (embeds a STM32U083RCTx device)
       - SCL MASTER Pin:PB8 (Arduino SCL/D15 CN5 pin 10, Morpho CN10 pin3)
       - SDA MASTER Pin:PB9 (Arduino SDA/D14 CN5 pin 9, Morpho CN10 pin5)

       - SCL SLAVE Pin: PC0 (Arduino SCL/A5 CN8 pin 6, Morpho CN7 pin38)
       - SDA SLAVE Pin: PC1 (Arduino SDA/A4 CN8 pin 5, Morpho CN7 pin36)

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick. 
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 48 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function. 
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The user can disable internal pull-up by opening ioc file.

For that, user can follow the procedure :

1- Double click on the I2C_OneBoard_ComSlave7_10bits_IT.ioc file
2- When CUBEMX tool is opened, select System Core category
3- Then in the configuration of GPIO/I2C1, change Pull-up to No pull-up and no pull-down for the both pins
4- Same action in the configuration of GPIO/I2C3, change Pull-up to No pull-up and no pull-down for the both pins
5- Last step, generate new code thanks to button "GENERATE CODE"

The example is updated with no pull on each pin used for I2C communication


For this example two buffers are used :

- aTxBuffer buffer contains the data to be transmitted 
- aRxBuffer buffer is used to save the received data

Note that both buffers have same size
                       
Example execution:

First step, press the User push-button, this action initiates a reception process (aRxBuffer) for 
I2C Slave1 or Slave2 address through HAL_I2C_Slave_Receive_IT().

Then I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_IT() 
for Slave1 (10-Bit address). 

The end of this step is monitored through the HAL_I2C_GetState() function
result.

Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.

Toggle LED4 when data is received correctly, otherwise LED4 is slowly blinking (1 sec. period)

Second step, press the User push-button, this action initiates a transmission process (aTxBuffer) for 
I2C Slave1 or Slave2 address through HAL_I2C_Slave_Transmit_IT().

Then I2C Master starts the communication by receiving aRxBuffer through HAL_I2C_Master_Receive_IT() 
from Slave2 (7-Bit address).

The end of this two steps are monitored through the HAL_I2C_GetState() function
result.

Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.

Toggle LED4 when data is received correctly, otherwise LED4 is slowly blinking (1 sec. period)

#### <b>Notes</b>

 1. In Master side, only Acknowledge failure error is handled. When this error
    occurs Master restart the current operation until Slave acknowledges it's
    address.
        
 2. I2C1 and I2C3 instance used and associated resources can be updated in "main.h"
    file depending hardware configuration used.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I2C, SDA, SCL, Slave, 7 bits, 10 Bits, Interrupt, Transmission, Reception, Acknowledge

### <b>Directory contents</b> 

  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/stm32u0xx_hal_conf.h    HAL configuration file
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/stm32u0xx_it.h          I2C interrupt handlers header file
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/main.h                  Header for main.c module  
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/stm32u0xx_it.c          I2C interrupt handlers
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/main.c                  Main program
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/system_stm32u0xx.c      STM32U0xx system source file
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/stm32u0xx_hal_msp.c     HAL MSP file    

### <b>Hardware and Software environment</b>

  - This example runs on STM32U0xx devices.
    
  - This example has been tested with NUCLEO-U083RC board and can be
    easily tailored to any other supported device and development board.    

  -NUCLEO-U083RC Set-up
    - Connect GPIOs connected to I2C3 SCL/SDA (PC0 and PC1)
    to respectively SCL and SDA pins of I2C1 (PB8 and PB9).
      - I2C3_SCL  PC0(Arduino SCL/A5 CN8 pin 6, Morpho CN7 pin38) : connected to I2C1_SCL PB8(Arduino SCL/D15 CN5 pin 10, Morpho CN10 pin3)
      - I2C3_SDA  PC1(Arduino SDA/A4 CN8 pin 5, Morpho CN7 pin36) : connected to I2C1_SDA PB9(Arduino SDA/D14 CN5 pin 9, Morpho CN10 pin5)

  - Launch the program.
  - Press User push-button to initiate a write request by Master
    then Slave1 (10-Bit address) receive a Buffer. LED4 Toggle when data is received correctly.
  - Press again User push-button to initiate a read request by Master
    then Slave2 (7-Bit address) Transmit a Buffer. LED4 Toggle when data is received correctly.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

