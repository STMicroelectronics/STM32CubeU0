#!/bin/bash
# Getting the CubeProgammer_cli path
if [ $# -ge 1 ]; then mode=$1; else mode=MANUAL; fi

source ../env.sh NULL

# Environment variable for AppliCfg
SCRIPT=$(readlink -f "$0")
projectdir=$(dirname "$SCRIPT")

#Updated with boot postbuild
data_image_number=0x1
ext_loader=0x1

# Initial configuration
ob_flash_programming="ob_flash_programming.sh"
appli_dir="../../"$oemirot_appli_path_project
code_xml="\"$projectdir/Images/OEMiROT_Code_Image.xml\""
data_xml="\"$projectdir/Images/OEMiRoT_Data_Image.xml\""
init_data_xml="\"$projectdir/Images/OEMiRoT_Data_Init_Image.xml\""
boot_cfg_h="${cube_fw_path}/Projects/NUCLEO-U083RC/Applications/ROT/OEMiROT_Boot/Inc/boot_hal_cfg.h"
provisioning_log_file="provisioning.log"
connect_no_reset="-c port=SWD speed=fast mode=Hotplug"
connect_reset="-c port=SWD mode=UR"

rdp_str="OB_RDP_LEVEL_0"

applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/AppliCfg.py"
python="python "

# ======================================================== Script functions ========================================================
# ====================================================== Rdp level selection =======================================================
rdp_level_choice()
{
  action="Define RDP level"
  echo "   * $action"
  if [ "$mode" != "AUTO" ]; then
    read -p "     [ 2 | 1 | 0 ]: " rdp_level
    rdp_level=$(echo "$rdp_level" | tr -d '\n\r')
  else
    rdp_level=$2
  fi

  if [ "$rdp_level" == "0" ]; then
    echo
    rdp_value=0xAA
    rdp_str="OB_RDP_LEVEL_0"
    return
  fi

  if [ "$rdp_level" == "1" ]; then
    echo
    rdp_value=0xBB
    rdp_str="OB_RDP_LEVEL_1"
    return
  fi

  if [ "$rdp_level" == "2" ]; then
    echo
    rdp_value=0xCC
    rdp_str="OB_RDP_LEVEL_2"
    set_oem2_key  # OEM2 key Setting
    return
  fi

  echo "       WRONG rdp level selected"
  echo
  rdp_level_choice
}
# =================================================== Option Bytes and flash programming ===================================================
set_oem2_key()
{
  # Step to configure OEM2 key
  echo "   * OEM2 key setup"
  echo "       Default oem2_password is kept in env.sh"
  echo "       Warning: Default OEM2 keys must NOT be used in a product. Make sure to regenerate your own OEM2 keys!"
  echo "       If you do not want to use Default OEM2 keys, then replace new OEM2 keys in env.sh, close this script and start again"
  echo "       Press any key to continue with default oem2 keys..."
  echo
  if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi
}

change_rdp_level_in_boot_cfg()
{
  eval $python"\"$applicfg\"" modifyfilevalue --variable OEMIROT_OB_RDP_LEVEL_VALUE --value "\"${rdp_str}\"" "\"${boot_cfg_h}\"" --str
}

oem2_key_provisioning()
{
  "$stm32programmercli" $connect_reset -hardRst -lockRDP2 $oem2_key > "provisioning.log"
  if [ $? -ne 0 ]; then step_error; fi
  echo "   * Provisioning of OEM2 key Done"
  echo
}

set_rdp_level()
{
  action="Setting the rdp level $rdp_level"
  echo "   * $action"
  "$stm32programmercli" $connect_reset -ob RDP=$rdp_value >> "provisioning.log"
  echo
  if [ $? -ne 0 ]; then step_error; fi
  if [ "$rdp_value" != "0xAA" ]; then
    echo "   *Please Unplug USB Cable and Plug it again to recover SWD connection"
    echo "        Press any key to continue..."
    if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi
  fi
}

ob_programming()
{
  action="Programming the option bytes and flashing the images ..."
  command="source "$ob_flash_programming
  echo "   * $action"
  $command >> "provisioning.log"
  ob_flash_error=$?
  if [ $ob_flash_error -ne 0 ]; then step_error; fi
  echo "       Successful option bytes programming and images flashing"
  echo "       (see provisioning.log for details)"
  echo
}

# ============================================================= End functions ==============================================================
# All the steps to set the STM32U0 product were executed correctly
final_step()
{
  echo "====="
  echo "===== The board is correctly configured."
  echo "===== Connect UART console (115200 baudrate) to get application menu."
  no_menu
}

no_menu()
{
  echo "====="
  exit 0
}

# Error when external script is executed
step_error()
{
  echo
  echo "====="
  echo "===== Error while executing "$action"."
  echo "===== See provisioning.log for details. Then try again."
  echo "====="
  if [ "$mode" != "AUTO" ]; then $SHELL; fi
  exit 1
}

# ============================================================== Script start ==============================================================

# ========================================================== Initial instructions ==========================================================
echo "====="
echo "===== Provisioning of OEMiRoT boot path"
echo "===== Application selected through env.sh: $oemirot_appli_path_project"
echo "====="
echo

# Path validation
action="Validating OEMiROT boot path project"

if [ ! -d $appli_dir ]; then
  echo "====="
  echo "===== Wrong Boot path: $oemirot_appli_path_project"
  echo "===== please modify the env.sh to the right path"
  step_error
fi

# ============================================================= RDP selection ==============================================================
rdp_level_choice

change_rdp_level_in_boot_cfg

# ======================================================== Images generation steps =========================================================
echo "   * Boot firmware image generation"
echo "       Open the OEMiROT_Boot project with preferred toolchain and rebuild all files."
echo "       Press any key to continue..."
echo
if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi

echo "   * Application firmware image generation"
echo "       Open the OEMiROT_Appli project with preferred toolchain and rebuild all files"
echo "       Press any key to continue..."
echo
if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi

if [ "$data_image_number" == "0x1" ]; then
	echo "   * Data generation (if Data image is enabled)"
	echo "       Select OEMiROT_Data_Image.xml(Default path is ROT_Provisioning/OEMiROT/Images/OEMiROT_Data_Image.xml)"
	echo "       Generate the data_enc_sign.hex image"
	echo "       Press any key to continue..."
	echo
	if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi
	eval "\"$stm32tpccli\"" -pb $data_xml >> $provisioning_log_file
	if [ $? != "0" ]; then step_error; fi
	eval "\"$stm32tpccli\"" -pb $init_data_xml >> $provisioning_log_file
	if [ $? != "0" ]; then step_error; fi
fi
if [ "$ext_loader" == "0x1" ]; then
	echo "   * Loader firmware image generation"
	echo "       Open the OEMiROT_Loader project with preferred toolchain and rebuild all files"
	echo "       Press any key to continue..."
	echo
	if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi
fi
# ============================================================ OEM2 key provisioning ======================================================
oem2_key_provisioning

# =================================================== Option Bytes and flash programming ===================================================
ob_programming

# =========================================================== RDP level setting ============================================================
set_rdp_level

# ================================================================== End ===================================================================
final_step
