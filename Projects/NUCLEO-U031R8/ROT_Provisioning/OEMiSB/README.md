## <b>ROT_Provisioning Description</b>

This section provides available configuration scripts for OEMiSB example.


To ease the configuration process, provisioning script is used.

### <b>Keywords</b>

OEMiSB, boot path, Root Of Trust, Security, mcuboot


### **Directory contents**

* OEMiSB/provisioning.bat/.sh Target provisioning.

### **Hardware and Software environment**


* This example runs on STM32U031xx devices.
* This example has been tested with STMicroelectronics NUCLEO-U031R8 board and can be easily tailored to any other supported device and development board.

### **How to use it?**


Before compiling the OEMiSB project, you should first start the provisioning process. During the provisioning process, the linker files
of project will be automatically updated.

All scripts are relying on env.bat/env.sh for tool path and application path.

The **provisioning process** (ROT\_Provisioning/OEMiSB/provisioning.bat/.sh) is divided in 3 majors steps :


* Step 1 : Product configuration


	+ Select RDP Level
	+ Select data area size

* Step 2 : Image generation


	+ Build OEMiSB\_Boot project
	+ Build OEMiSB\_Appli project


* Step 3 : Provisioning


	+ Programming the option bytes
	+ Flashing the images
	+ Setting the RDP level


