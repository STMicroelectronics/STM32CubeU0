## <b>OEMiROT_Loader Application Description</b>

This application is a sample code of a standalone local loader using the Ymodem protocol. This application allows the download of a new version of the firmware and data images.


In order to ease the development process, prebuild and postbuild commands are integrated in each toolchain project. The prebuild command is in charge of preparing the scatter file according to common flash layout description in linker folder. The postbuild command is in charge of preparing the loader image.


For more details, refer to Wiki article available here : [Link to Security Wiki](https://wiki.st.com/stm32mcu/wiki/Category:Security)


### <b>Keywords</b>

OEMiROT, boot path, Root Of Trust, Security, mcuboot



### **Directory contents**


* OEMiROT\_Loader/Src/com.c UART low level interface
* OEMiROT\_Loader/Src/common.c UART common functionalities
* OEMiROT\_Loader/Src/fw\_update\_app.c Firmware update interface
* OEMiROT\_Loader/Src/low\_level\_device.c Flash low level device setting
* OEMiROT\_Loader/Src/low\_level\_flash.c Flash low level interface
* OEMiROT\_Loader/Src/main.c Main program
* OEMiROT\_Loader/Src/system\_stm32u5xx.c System init file
* OEMiROT\_Loader/Src/tick.c HAL Tick implementation
* OEMiROT\_Loader/Src/ymodem.c Ymodem support
* OEMiROT\_Loader/Src/startup\_stm32u5xx.c Startup file
* OEMiROT\_Loader/Inc/loader\_flash\_layout.h Flash layout header file
* OEMiROT\_Loader/Inc/com.h Header file for com.c
* OEMiROT\_Loader/Inc/common.h Header file for common.c
* OEMiROT\_Loader/Inc/fw\_update\_app.h Header file for fw\_update\_app.c
* OEMiROT\_Loader/Inc/low\_level\_flash.h Header file for low\_level\_flash.c
* OEMiROT\_Loader/Inc/main.h Main program header file
* OEMiROT\_Loader/Inc/stm32u5xx\_hal\_conf.h HAL driver configuration file
* OEMiROT\_Loader/Inc/ymodem.h Header file for Ymodem.c


### **How to use it ?**


In order to build the OEMiROT\_Loader project, you must do the following:


* Open your preferred toolchain
* Rebuild the project


Then refer to OEMiROT\_Appli readme for example of application booted through OEMiROT boot path.