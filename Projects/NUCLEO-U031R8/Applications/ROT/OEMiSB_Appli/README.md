## <b>OEMiSB_Appli application Description</b>


This project provides a OEMiSB boot path application example. Boot is performed through OEMiSB boot path after integrity checks of the project firmware image.


The application will display a menu on the console. If data is available, three menus are provided that allows reading, writing and erasing data.


The maximum system clock frequency at 48Mhz is configured.


For more details, refer to Wiki article available here : [Link to Security Wiki](https://wiki.st.com/stm32mcu/wiki/Category:Security)


### <b>Keywords</b>

OEMiSB, boot path, Root Of Trust, Security, mcuboot


### **Directory contents**


* OEMiSB\_Appli/Src/main.c Main program
* OEMiSB\_Appli/Src/stm32u0xx\_it.c Interrupt handlers
* OEMiSB\_Appli/Src/system\_stm32u0xx.c System init file
* OEMiSB\_Appli/Src/main.h Main program header file
* OEMiSB\_Appli/Src/stm32u0xx\_hal\_conf.h HAL driver Configuration file
* OEMiSB\_Appli/Src/stm32u0xx\_it.h Interrupt handlers header file


### **Hardware and Software environment**


* This example runs on STM32U031xx devices.
* This example has been tested with STMicroelectronics NUCLEO-U031R8 (MB1932)
board and can be easily tailored to any other supported device
and development board.
* To print the application menu in your UART console you have to configure it using these parameters:
Speed: 115200, Data: 8bits, Parity: None, stop bits: 1, Flow control: none.


### **How to use it ?**


This project is targeted to boot through **OEMiSB boot path**.


Before compiling the project, you should first start the provisioning process. During the provisioning process, the linker files
of the project will be automatically updated.


The **provisioning process** (ROT\_Provisioning/OEMiSB/provisioning.bat) is divided in 3 majors steps :


* Step 1 : Images generation


	+ Select RDP Level
	+ Select data area size
	+ Build OEMiSB\_Boot project
	+ Build OEMiSB\_Appli project
* Step 2 : Provisioning


	+ Programming the option bytes
	+ Flashing the images
	+ Setting the RDP level


After application startup, check in your "UART console" the menu is well displayed:



```
======================================================================
=              (C) COPYRIGHT 2023 STMicroelectronics                 =
=                                                                    =
=                          OEMiSB User App                           =
======================================================================

============================= Main Menu ==============================
  Read flash data ------------------------------------------ 1
  Write flash data ----------------------------------------- 2
  Erase flash data ----------------------------------------- 3
 Selection :

```

It is possible to read, write or erase data


#### **Notes:**



2. Two versions of ROT\_AppliConfig are available: windows executable and python version. By default, the windows executable is selected. It
is possible to switch to python version by:


	* installing python (Python 3.10 or newer) with the required modules listed in requirements.txt.
	```
	pip install -r requirements.txt
	
	```
	* having python in execution path variable
	* deleting main.exe in Utilities\PC\_Software\ROT\_AppliConfig\dist