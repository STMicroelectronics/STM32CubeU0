## <b>OEMiROT_Appli application Description</b>

This project provides a OEMiROT boot path application example. Boot is performed through OEMiROT boot path after authenticity and the integrity checks of the project firmware and project data
image.


The application will display a menu on the console allowing to download a new version
of the application firmware and the application data images.
At next reset, the OEMiROT will decrypt and install these new versions after successful check of the authenticity and the integrity of each image.


The maximum system clock frequency at 48Mhz is configured.


For more details, refer to Wiki article available here : [Link to Security Wiki](https://wiki.st.com/stm32mcu/wiki/Category:Security)


### <b>Keywords</b>

OEMiROT, boot path, Root Of Trust, Security, mcuboot


### **Directory contents**


* OEMiROT\_Appli/Src/com.c UART low level interface
* OEMiROT\_Appli/Src/common.c UART common functionalities
* OEMiROT\_Appli/Src/data.c Data image example
* OEMiROT\_Appli/Src/fw\_update\_app.c Firmware update interface
* OEMiROT\_Appli/Src/low\_level\_device.c Flash low level device setting
* OEMiROT\_Appli/Src/low\_level\_flash.c Flash low level interface
* OEMiROT\_Appli/Src/main.c Main program
* OEMiROT\_Appli/Src/stm32u0xx\_it.c Interrupt handlers
* OEMiROT\_Appli/Src/system\_stm32u0xx.c System init file
* OEMiROT\_Appli/Src/ymodem.c Ymodem support
* OEMiROT\_Appli/Src/startup\_stm32u083xx.s Startup file
* OEMiROT\_Appli/Src/appli\_flash\_layout.h Flash layout header file
* OEMiROT\_Appli/Src/com.h Header file for com.c
* OEMiROT\_Appli/Src/common.h Header file for common.c
* OEMiROT\_Appli/Src/data.h Header file for data.c
* OEMiROT\_Appli/Src/fw\_update\_app.h Header file for fw\_update\_app.c
* OEMiROT\_Appli/Src/low\_level\_flash.h Header file for low\_level\_flash.c
* OEMiROT\_Appli/Inc/main.h Main program header file
* OEMiROT\_Appli/Inc/stm32u0xx\_hal\_conf.h HAL driver Configuration file
* OEMiROT\_Appli/Inc/stm32u0xx\_it.h Interrupt handlers header file
* OEMiROT\_Appli/Inc/ymodem.h Header file for Ymodem.c
* OEMiROT\_Appli/Binary Application firmware image (application binary + MCUBoot header and metadata)


### **Hardware and Software environment**


* This example runs on STM32U083xx devices.
* This example has been tested with STMicroelectronics NUCLEO-U083RC (MB1932)
board and can be easily tailored to any other supported device
and development board.
* To print the application menu in your UART console you have to configure it using these parameters:
Speed: 115200, Data: 8bits, Parity: None, stop bits: 1, Flow control: none.


### **How to use it ?**


This project is targeted to boot through **OEMiROT boot path**.


Before compiling the project, you should first start the provisioning process. During the provisioning process, the linker files
of project as well as the postbuild commands will be automatically updated.


The **provisioning process** (ROT\_Provisioning/OEMiROT/provisioning.bat) is divided in 4 majors steps :


* Step 1 : Selecting the RDP level

* Step 2 : Boot and local loader building


	+ OEMiROT boot binary: build OEMiROT\_Boot project
	+ OEMiROT Loader binary: build OEMiROT\_Loader project* Step 3 : Images generation


	+ Code firmware image generation: automatically generated at the end of compilation (postbuild command)
	+ Data image generation with TrustedPackageCreator, if data image enabled
* Step 4 : Provisioning


	+ Programming the option bytes
	+ Flashing the images
	+ Setting the RDP level


After application startup, check in your "UART console" the menu is well displayed:



```
======================== Main Menu ========================
 Toggle LED -------------------------------------------- 1
 New Firmware Image ------------------------------------ 2
 Display Data ------------------------------------------ 3
 Selection :

```

To update the application firmware and/or the application data image version, you must:


* Select the function "New Firmware Image"
* Download the new image(s)
* Reset the board
* After authenticity and integrity checks the new images are decrypted and installed.


#### **Notes:**



2. Two versions of ROT\_AppliConfig are available: windows executable and python version. By default, the windows executable is selected. It
is possible to switch to python version by:


	* installing python (Python 3.10 or newer) with the required modules listed in requirements.txt.
	```
	pip install -r requirements.txt
	
	```
	* having python in execution path variable
	* deleting main.exe in Utilities\PC\_Software\ROT\_AppliConfig\dist