#!/bin/bash

# Absolute path to this script
if [ $# -ge 1 ] && [ -d $1 ]; then
    projectdir=$1
else
    projectdir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
fi

# ==============================================================================
#                               General
# ==============================================================================
# Configure tools installation path
#Configure tools installation path
if [ "$OS" == "Windows_NT" ]; then
    stm32programmercli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
    stm32tpccli="C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32TrustedPackageCreator_CLI.exe"
else
    stm32programmercli_path=~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/
    PATH=$stm32programmercli_path:$PATH
    stm32programmercli="STM32_Programmer_CLI"
    stm32tpccli="STM32TrustedPackageCreator_CLI"
fi
#Configure OEM2 Key for RDP Regression
oem2_key="0x11223344 0x55667788 0x99AABBCC 0xDDEEFF00"
# ==============================================================================
#                            OEMiSB boot path
# ==============================================================================
# Select application project below
oemisb_boot_path_project="Applications/ROT/OEMiSB_Appli"
# ==============================================================================

# ==============================================================================
#               !!!! DOT NOT EDIT --- UPDATED AUTOMATICALLY !!!!
# ==============================================================================
PROJECT_GENERATED_BY_CUBEMX=false
cube_fw_path=$projectdir/../../../
rot_provisioning_path=$projectdir