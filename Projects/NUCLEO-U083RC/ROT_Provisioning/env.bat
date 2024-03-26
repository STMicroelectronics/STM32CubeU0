@ECHO OFF

:: ==============================================================================
::                                   General
:: ==============================================================================
:: Configure tools installation path
set stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
set stm32tpccli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32TrustedPackageCreator_CLI.exe"
set imgtool="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\Utilities\Windows\imgtool.exe"

:: ==============================================================================
::                              OEMiROT boot path
:: ==============================================================================
:: Select application project below
::set oemirot_boot_path_project=Templates_ROT
set oemirot_boot_path_project=Applications/ROT/OEMiROT_Appli
:: ==============================================================================

set cube_fw_path=%~dp0..\..\..\
set oemirot_appli=rot_app_enc_sign.bin
set rot_provisioning_path=%~dp0
