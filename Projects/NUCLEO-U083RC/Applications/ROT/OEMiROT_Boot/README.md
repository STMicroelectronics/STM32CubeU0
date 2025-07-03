## <b>OEMiROT_Boot application Description</b>

This project provides an OEMiROT example. OEMiROT boot path performs authenticity and the integrity checks of the project firmware and data
images.


The core function of this application relies on the mcuboot middleware and the mbed-crypto middleware.


This project shows how to implement an OEMiROT.


To ease the development process, a prebuild command and postbuild command are integrated in the project.
The prebuild command is in charge of preparing the OEMiROT\_Boot scatter file according to flash layout description.
The postbuild command is in charge of preparing the provisioning scripts and the application image
(firmware application managed by OEMiROT) configuration files in ROT\_Provisioning/OEMiROT folder,
according to the flash layout and OEMiROT configuration.


The maximum system clock frequency at 48Mhz is configured.


For more details, refer to Wiki article available here : [Link to Security Wiki](https://wiki.st.com/stm32mcu/wiki/Category:Security)


### <b>Keywords</b>

OEMiROT, boot path, Root Of Trust, Security, mcuboot


### **Directory contents**


* OEMiROT\_Boot/Src/bl2\_nv\_services.c Non Volatile services for OEMiROT\_Boot
* OEMiROT\_Boot/Src/boot\_hal.c Platform initialization
* OEMiROT\_Boot/Src/image\_macros\_to\_preprocess\_bl2.c Images definitions for bl2 preprocess
* OEMiROT\_Boot/Src/keys.c keys arrays
* OEMiROT\_Boot/Src/keys\_map.c keys provisionned in PERSO area
* OEMiROT\_Boot/Src/low\_level\_com.c UART low level interface
* OEMiROT\_Boot/Src/low\_level\_device.c Flash low level device configuration
* OEMiROT\_Boot/Src/low\_level\_flash.c Flash low level interface
* OEMiROT\_Boot/Src/low\_level\_rng.c Random generator interface
* OEMiROT\_Boot/Src/low\_level\_security.c Security low level services
* OEMiROT\_Boot/Src/stm32u0xx\_hal\_msp.c HAL MSP module
* OEMiROT\_Boot/Src/system\_stm32u0xx.c System Init file
* OEMiROT\_Boot/Src/tick.c HAL Tick implementation
* OEMiROT\_Boot/Src/startup\_stm32u0xx.c Startup file
* OEMiROT\_Boot/Inc/mcuboot\_config/mcuboot\_config.h Mcuboot configuration file
* OEMiROT\_Boot/Inc/boot\_hal\_cfg.h Platform configuration file for OEMiROT\_Boot
* OEMiROT\_Boot/Inc/boot\_hal\_flowcontrol.h Header file for flow control code in boot\_hal.c
* OEMiROT\_Boot/Inc/boot\_hal\_hash\_ref.h Header file for hash reference code in boot\_hal.c
* OEMiROT\_Boot/Inc/boot\_hal\_imagevalid.h Header file for image validation code in boot\_hal.c
* OEMiROT\_Boot/Inc/cmsis.h Header file for CMSIS
* OEMiROT\_Boot/Inc/mbedtls_config.h Mbed-crypto configuration file
* OEMiROT\_Boot/Inc/flash\_layout.h Flash layout header file
* OEMiROT\_Boot/Inc/low\_level\_flash.h Header file for low\_level\_flash.c
* OEMiROT\_Boot/Inc/low\_level\_rng.h Header file for low\_level\_rng.c
* OEMiROT\_Boot/Inc/low\_level\_security.h Header file for low\_level\_security.c
* OEMiROT\_Boot/Inc/region\_defs.h RAM and FLASH regions definitions
* OEMiROT\_Boot/Inc/stm32u0xx\_hal\_conf.h HAL driver configuration file
* OEMiROT\_Boot/Inc/target\_cfg.h Header file for target start up


### **Hardware and Software environment**


* This example runs on STM32U083xx devices.
* This example has been tested with STMicroelectronics NUCLEO-U083RC (MB1932)
board and can be easily tailored to any other supported device and development board.
* To get debug print in your UART console you have to configure it using these parameters:
Speed: 115200, Data: 8bits, Parity: None, stop bits: 1, Flow control: none.


### **How to use it?**


In order to build the OEMiROT\_Boot project, you must do the following:


* Open your preferred toolchain
* Rebuild the project


Then refer to OEMiROT\_Appli readme for example of application booted through OEMiROT boot path.
