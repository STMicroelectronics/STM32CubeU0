@ECHO OFF

:: ==============================================================================
::                               General
:: ==============================================================================
:: Configure tools installation path
set stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
set stm32tpccli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32TrustedPackageCreator_CLI.exe"
:: Configure OEM2 Key for RDP Regression
set oem2_key=0x11223344 0x55667788 0x99AABBCC 0xDDEEFF00

:: ==============================================================================
::                            OEMiSB boot path
:: ==============================================================================
:: Select application project below
set oemisb_boot_path_project=Applications/ROT/OEMiSB_Appli
:: ==============================================================================

:: ==============================================================================
::               !!!! DOT NOT EDIT --- UPDATED AUTOMATICALLY !!!!
:: ==============================================================================
set PROJECT_GENERATED_BY_CUBEMX=false
set cube_fw_path=%~dp0..\..\..\
set rot_provisioning_path=%~dp0
