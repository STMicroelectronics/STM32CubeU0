#!/bin/bash

absolutpath() {
echo "$(cd "$(dirname "$1")" && pwd)"
}

# Getting the CubeProgammer_cli path
source ../env.sh

SCRIPT=$(absolutpath $0)
project_dir=`dirname $SCRIPT`
if [ $# -ge 1 ]; then mode=$1; else mode=MANUAL; fi

state_change_log="${project_dir}/OEMiSB/provisioning.log"

data_hex_val="0x10000"
subregion_val="0x0"
rdp_value="0xAA"
rdp_str="OB_RDP_LEVEL_0"
oem2_key="0x11223344 0x55667788 0x99AABBCC 0xDDEEFF00"
action=""

final_execution() {
    echo "======"
    echo "====== The board is correctly configured."
    echo "======"
    if [ "$mode" != "AUTO" ]; then $SHELL; fi
    exit 0
}

step_error() {
    echo ""
    echo "======"
    echo "====== Error while executing \"$action\"."
    echo "====== See $state_change_log for details. Then try again."
    echo "======"
    if [ "$mode" != "AUTO" ]; then $SHELL; fi
    exit 1
}

define_rdp_level() {
    action="Define final RDP value"
    echo "* $action"
    read -p "      ${USERREG} [ 0 | 1 | 2 ]: " RDP_level
    RDP_level=$(echo "$RDP_level" | tr -d '\n\r')
    case "$RDP_level" in
        "0")
            echo ""
            rdp_value="0xAA"
            rdp_str="OB_RDP_LEVEL_0"
            ;;
        "1")
            echo ""
            rdp_value="0xBB"
            rdp_str="OB_RDP_LEVEL_1"
            ;;
        "2")
            echo ""
            rdp_value="0xCC"
            rdp_str="OB_RDP_LEVEL_2"
            ;;
        *)
            echo "WRONG RDP level selected"
            echo ""
            define_rdp_level
            ;;
    esac
}

define_data_size() {
    action="Define data area size in Kbytes"
    echo "* $action"
    read -p "      ${USERREG} [ 0 | 2 | 4 | 6 | 8 | 10 | 12 | 14 | 16 ]: " data_size
    data_size=$(echo "$data_size" | tr -d '\n\r')
    case "$data_size" in
        "0")
            echo ""
            data_hex_val="0x0"
            subregion_val="0xFF"
            ;;
        "2")
            echo ""
            data_hex_val="0x800"
            subregion_val="0x7F"
            ;;
        "4")
            echo ""
            data_hex_val="0x1000"
            subregion_val="0x3F"
            ;;
        "6")
            echo ""
            data_hex_val="0x1800"
            subregion_val="0x1F"
            ;;
        "8")
            echo ""
            data_hex_val="0x2000"
            subregion_val="0xF"
            ;;
        "10")
            echo ""
            data_hex_val="0x2800"
            subregion_val="0x7"
            ;;
        "12")
            echo ""
            data_hex_val="0x3000"
            subregion_val="0x3"
            ;;
        "14")
            echo ""
            data_hex_val="0x3800"
            subregion_val="0x1"
            ;;
        "16")
            echo ""
            data_hex_val="0x4000"
            subregion_val="0x0"
            ;;
        *)
            echo "Invalid input. Please enter a valid option."
            echo ""
            define_data_size
            ;;
    esac
}

# Application binary file
appli_binary="${project_dir}/../Applications/ROT/OEMiSB_Appli/Binary/OEMiSB_Appli.bin"
# Sha.bin
sha256="${project_dir}/OEMiSB/Binary/sha256.bin"

# Data updated with the postbuild of OEMiSB-Boot
hdp_end=0x3
bootaddress="0x8000000"
appliaddress="0x8002000"
shaaddress="0x8001FC0"
# CubeProgammer connection
connect_no_reset="-c port=SWD mode=Hotplug"
connect_reset="-c port=SWD mode=UR"

# =============================================== Remove protections and initialize Option Bytes  ==========================================
remove_protect_init="-ob WRP1A_STRT=0x1f WRP1A_END=0 WRP1B_STRT=0x1f WRP1B_END=0 HDP1_PEND=0 HDP1EN=0xB4 BOOT_LOCK=0"
# =============================================== Erase the user flash =====================================================================
erase_all="-e all"

# ================================================ hardening ===============================================================================
hide_protect="HDP1_PEND=${hdp_end} HDP1EN=1"

boot_lock="BOOT_LOCK=1"

isGeneratedByCubeMX="${PROJECT_GENERATED_BY_CUBEMX}"

appli_main_h="${cube_fw_path}/Projects/NUCLEO-U031R8/Applications/ROT/OEMiSB_Appli/Inc/main.h"
boot_main_h="${cube_fw_path}/Projects/NUCLEO-U031R8/Applications/ROT/OEMiSB_Boot/Inc/main.h"
boot_cfg_h="${cube_fw_path}/Projects/NUCLEO-U031R8/Applications/ROT/OEMiSB_Boot/Inc/boot_cfg.h"
ld_appli="${cube_fw_path}/Projects/NUCLEO-U031R8/Applications/ROT/OEMiSB_Appli/STM32CubeIDE/STM32U031R8TX_FLASH.ld"

#line for python
echo AppliCfg with python script
applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/AppliCfg.py"
#determine/check python version command
python="python "


echo "======"
echo "====== Provisioning of OEMiSB boot path"
echo "====== Application selected through env.bat:"
echo "====== ${oemisb_boot_path_project}"
echo "======"

# ============================================================= RDP selection ==============================================================
define_rdp_level
# ========================================================= Data size selection ==============================================================
define_data_size

tmp_file="${project_dir}/tmp.c"
echo "DATA_SIZE=${data_hex_val}" > "${tmp_file}"
echo "DATA_MPU_SUB_REG=${subregion_val}" >> "${tmp_file}"
$python$applicfg definevalue -l "${tmp_file}" -m DATA_SIZE -n DATA_SIZE "${boot_main_h}"
$python$applicfg definevalue -l "${tmp_file}" -m DATA_SIZE -n DATA_SIZE "${appli_main_h}"
$python$applicfg definevalue -l "${tmp_file}" -m DATA_MPU_SUB_REG -n DATA_MPU_SUB_REG "${boot_main_h}"
$python$applicfg linker -l "${tmp_file}" -m DATA_SIZE -n FLASH_DATA_AREA_SIZE "${ld_appli}"
$python$applicfg modifyfilevalue --variable OEMISB_OB_RDP_LEVEL_VALUE --value "${rdp_str}" "${boot_cfg_h}" --str

wrpend=$((0x1F - data_hex_val/0x800))
write_protect="WRP1A_STRT=0 WRP1A_END=${wrpend}"

echo "Step 2 : Projects generation"
echo "    * Boot project generation"
echo "        Open the OEMiSB_Boot project with preferred toolchain and rebuild all files."
echo "        Press any key to continue..."
if [ "$mode" != "AUTO" ]; then read -p "" -s; fi
echo ""


echo "    * Application project generation"
echo "        Open the OEMiSB_Appli project with preferred toolchain and rebuild all files."
echo "        Press any key to continue..."
if [ "$mode" != "AUTO" ]; then read -p "" -s; fi
echo ""

echo "Step 3 : Product programming"
action="Programming the option bytes and flashing binaries ..."

action="Provisioning of OEM2 key : "$oem2_key
echo "    * $action"
"$stm32programmercli" $connect_reset -lockRDP2 $oem2_key > "$state_change_log"
if [ $? -ne 0 ]; then
    echo "Error: Failed to set OEM2 key"
    step_error
fi

echo ""
action="Remove protection and flash erase"
echo "    * $action"
"$stm32programmercli" $connect_reset $remove_protect_init $erase_all >> "$state_change_log"
if [ $? -ne 0 ]; then
    echo "Error: Failed to remove protection and erase flash."
    step_error
fi

echo ""
echo "    * Project flash programming"
echo "        OEMiSB application programming"
action="OEMiSB_Appli Written"
"$stm32programmercli" "$connect_reset" -d "$cube_fw_path/Projects/NUCLEO-U031R8/Applications/ROT/OEMiSB_Appli/Binary/OEMiSB_Appli.bin" "$appliaddress" -v >> "$state_change_log"
if [ $? -ne 0 ]; then
    echo "Error: Failed to program OEMiSB_Appli."
    step_error
fi
echo "        - $action"
echo "$action" >> ob_flash_programming.log

action="OEMiSB boot programming"
echo "        - $action"
"$stm32programmercli" "$connect_reset" -d "$cube_fw_path/Projects/NUCLEO-U031R8/Applications/ROT/OEMiSB_Boot/Binary/OEMiSB_Boot.bin" "$bootaddress" -v >> "$state_change_log"
if [ $? -ne 0 ]; then
    echo "Error: Failed to program OEMiSB boot."
    step_error
fi

action="OEMiSB_Boot Written"
echo "            - $action"
echo "$action" >> "$state_change_log"

action="OEMiSB application SHA256 programming"
echo "        - $action"
$python$applicfg hashcontent tmp.bin -i "$appli_binary" -t "10000" -d "$sha256"
"$stm32programmercli" "$connect_reset" -d "$cube_fw_path/Projects/NUCLEO-U031R8/ROT_Provisioning/OEMiSB/Binary/sha256.bin" "$shaaddress" -v --skipErase >> "$state_change_log"
if [ $? -ne 0 ]; then
    echo "Error: Failed to program OEMiSB application SHA256."
    step_error
fi
action="SHA Appli Written"
echo "            - $action"
echo "$action" >> ob_flash_programming.log

echo ""
echo "    * Configure Option Bytes:"
echo "        - Write Protection"
echo "        - Hide Protection"
echo "        - Boot Lock"
echo "        Press any key to continue..."
if [ "$mode" != "AUTO" ]; then read -p "" -s; fi

action="Configure Option Bytes"
echo "        - $action"
"$stm32programmercli" "$connect_reset" -ob "$write_protect" "$hide_protect" "$boot_lock" >> "$state_change_log"
if [ $? -ne 0 ]; then
    echo "Error: Failed to configure option bytes."
    step_error
fi
echo ""

action="Configure Option Bytes Done"
echo "        - $action"
echo "$action" >> "$state_change_log"

action="Setting the final RDP Level $RDP_level"
echo "    * $action"

"$stm32programmercli" "$connect_no_reset" -ob RDP="$rdp_value" >> "$state_change_log"
echo ""

final_execution