#!/bin/bash
# Absolute path to this script
if [ $# -ge 1 ] && [ -d $1 ]; then
    projectdir=$1
else
    projectdir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
fi

# ==============================================================================
#                                   General
# ==============================================================================
# Configure tools installation path
if [ "$OS" == "Windows_NT" ]; then
    stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
    stm32tpccli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32TrustedPackageCreator_CLI.exe"
    imgtool="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\Utilities\Windows\imgtool.exe"
else
    stm32programmercli_path=~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/
    PATH=$stm32programmercli_path:$PATH
    PATH=$stm32programmercli_path/Utilities/Windows/:$PATH
    stm32programmercli="STM32_Programmer_CLI"
    stm32tpccli="STM32TrustedPackageCreator_CLI"
    imgtool="imgtool"
fi

# ==============================================================================
#                               OEMiROT boot path
# ==============================================================================
# Select application project below
#oemirot_boot_path_project="Templates_ROT"
oemirot_boot_path_project="Applications/ROT/OEMiROT_Appli"
# ==============================================================================

cube_fw_path=$projectdir/../../../
oemirot_appli=rot_app_enc_sign.bin
rot_provisioning_path=$projectdir