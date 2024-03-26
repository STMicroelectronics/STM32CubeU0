## <b>Templates ROT Description</b>

This project provides a OEMiROT boot path application template. Boot is performed through OEMiROT boot path after authenticity and the integrity checks of the project firmware and project data
image.


This project template is based on the STM32Cube HAL API.


At the beginning of the main program, the HAL\_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.


The SystemClock\_Config() function is used to configure the system clock (SYSCLK)


The maximum system clock frequency at 48Mhz is configured.


For more details, refer to Wiki article available here : [Link to Security Wiki](https://wiki.st.com/stm32mcu/wiki/Category:Security)


### <b>Keywords</b>

OEMiROT, boot path, Root Of Trust, Security, mcuboot


### <b>Directory contents</b>


* Templates/Src/main.c Main program
* Templates/Src/system\_stm32u0xx.c STM32U0xx system clock configuration file
* Templates/Src/stm32u0xx\_it.c Interrupt handlers
* Templates/Src/stm32u0xx\_hal\_msp.c HAL MSP module
* Templates/Inc/main.h Main program header file
* Templates/Inc/stm32u0xx\_hal\_conf.h HAL Configuration file
* Templates/Inc/stm32u0xx\_it.h Interrupt handlers header file
* OEMiROT\_Appli/Binary Application firmware image (application binary + MCUBoot header and metadata)


### <b>Hardware and Software environment</b>


* This example runs on STM32U083xx devices.
* This example has been tested with STMicroelectronics NUCLEO-U083RC (MB1932)
board and can be easily tailored to any other supported device
and development board.
* To print the application menu in your UART console you have to configure it using these parameters:
Speed: 115200, Data: 8bits, Parity: None, stop bits: 1, Flow control: none.


### <b>How to use it ?</b>


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


#### <b>Notes</b>

2. Two versions of ROT\_AppliConfig are available: windows executable and python version. By default, the windows executable is selected. It
is possible to switch to python version by:


	* installing python (Python 3.10 or newer) with the required modules listed in requirements.txt.
	```
	pip install -r requirements.txt
	
	```
	* having python in execution path variable
	* deleting main.exe in Utilities\PC\_Software\ROT\_AppliConfig\dist
