## <b>OEMiSB_Boot application Description</b>

This project provides an OEMiSB example. OEMiSB boot path performs authenticity checks of the project firmware image.


This project shows how to implement an OEMiSB.


The maximum system clock frequency at 48Mhz is configured.


For more details, refer to Wiki article available here : [Link to Security Wiki](https://wiki.st.com/stm32mcu/wiki/Category:Security)


### <b>Keywords</b>

OEMiSB, boot path, Root Of Trust, Security


### **Directory contents**


* OEMiSB\_Boot/Src/boot.c Platform initialization
* OEMiSB\_Boot/Src/low\_level\_rng.c Random generator interface
* OEMiSB\_Boot/Src/low\_level\_security.c Security low level services
* OEMiSB\_Boot/Src/main.c Main program
* OEMiSB\_Boot/Src/stm32u0xx\_it.c Interrupt handlers
* OEMiSB\_Boot/Src/system\_stm32u0xx.c System init file
* OEMiSB\_Boot/Src/startup\_stm32u083xx.s Startup file
* OEMiSB\_Boot/Inc/mcuboot\_config/mcuboot\_config.h Mcuboot configuration file
* OEMiSB\_Boot/Inc/boot.h Header file for boot.c
* OEMiSB\_Boot/Inc/boot\_cfg.h Configuration file
* OEMiSB\_Boot/Inc/boot\_hal\_flowcontrol.h Header file for flow control code in boot\_hal.c
* OEMiSB\_Boot/Inc/low\_level\_rng.h Header file for low\_level\_rng.c
* OEMiSB\_Boot/Inc/low\_level\_security.h Header file for low\_level\_security.c
* OEMiSB\_Boot/Inc/main.h Main program header file
* OEMiSB\_Boot/Inc/mbed-crypto\_config.h Mbed-crypto configuration file
* OEMiSB\_Boot/Inc/stm32u0xx\_hal\_conf.h HAL driver configuration file
* OEMiSB\_Boot/Inc/stm32u0xx\_it.h Interrupt handlers header file


### **Hardware and Software environment**


* This example runs on STM32U031xx devices.
* This example has been tested with STMicroelectronics NUCLEO-U031R8 (MB1932)
board and can be easily tailored to any other supported device and development board.
* To get debug print in your UART console you have to configure it using these parameters:
Speed: 115200, Data: 8bits, Parity: None, stop bits: 1, Flow control: none.


### **How to use it?**


In order to build the OEMiSB\_Boot project, you must do the following:


* Open your preferred toolchain
* Rebuild the project


Then refer to OEMiSB\_Appli readme for example of application booted through OEMiSB boot path.