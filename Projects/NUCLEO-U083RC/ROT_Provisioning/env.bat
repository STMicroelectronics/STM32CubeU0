@ECHO OFF

:: ==============================================================================
::                                   General
:: ==============================================================================
:: Configure tools installation path
set stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
set stm32tpccli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32TrustedPackageCreator_CLI.exe"
set imgtool="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\Utilities\Windows\imgtool.exe"
::Configure OEM2 Key for RDP Regression
set oem2_key=0x11223344 0x55667788 0x99AABBCC 0xDDEEFF00

:: ==============================================================================
::                              OEMiROT boot path
:: ==============================================================================
:: Select application project below
:: Give relative path of application and boot from NUCLEO-U083RC folder
::set oemirot_appli_path_project=Templates_ROT
set oemirot_appli_path_project=Applications/ROT/OEMiROT_Appli
set oemirot_boot_path_project=Applications/ROT/OEMiROT_Boot
:: ==============================================================================

set cube_fw_path=%~dp0..\..\..\
set oemirot_appli=rot_app_init_sign.bin
set oemirot_boot=OEMiROT_Boot.bin
set rot_provisioning_path=%~dp0
