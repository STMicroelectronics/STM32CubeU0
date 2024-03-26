#!/bin/bash
# Getting the CubeProgammer_cli path
if [ $# -ge 1 ]; then mode=$1; else mode=MANUAL; fi

source ../env.sh NULL

# Environment variable for AppliCfg
SCRIPT=$(readlink -f $0)
projectdir=`dirname $SCRIPT`

# Initial configuration
ob_flash_programming="ob_flash_programming.sh"
appli_dir="../../"$oemirot_boot_path_project
app_bin=../$appli_dir"/Binary/rot_app.bin"
app_enc_sign_bin=../$appli_dir"/Binary/rot_app_enc_sign.bin"
code_xml=$projectdir"/Images/OEMiROT_Code_Image.xml"
boot_cfg_h="${cube_fw_path}/Projects/NUCLEO-U083RC/Applications/ROT/OEMiROT_Boot/Inc/boot_hal_cfg.h"

connect_no_reset="-c port=SWD speed=fast mode=Hotplug"
connect_reset="-c port=SWD mode=UR"

rdp_str="OB_RDP_LEVEL_0"
oem2_key="0x11223344 0x55667788 0x99AABBCC 0xDDEEFF00"

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
    return
  fi

  echo "       WRONG rdp level selected"
  echo
  rdp_level_choice
}
# =================================================== Option Bytes and flash programming ===================================================
set_oem2_key()
{
  action="Provisioning of OEM2 key : "$oem2_key
  echo "   * $action"
  "$stm32programmercli" $connect_reset -lockRDP2 $oem2_key > "provisioning.log"
  echo
  if [ $? -ne 0 ]; then step_error; fi
}

change_rdp_level_in_boot_cfg()
{
  $python$applicfg modifyfilevalue --variable OEMIROT_OB_RDP_LEVEL_VALUE --value "${rdp_str}" "${boot_cfg_h}" --str
}

set_rdp_level()
{
  action="Setting the rdp level $rdp_level"
  echo "   * $action"
  "$stm32programmercli" $connect_no_reset -ob RDP=$rdp_value >> "provisioning.log"
  echo
  if [ $? -ne 0 ]; then step_error; fi
}

ob_programming()
{
  action="Programming the option bytes and flashing the images ..."
  command="source "$ob_flash_programming
  echo "   * $action"
  $command > "provisioning.log"
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
  echo "====="

  if [ "$mode" != "AUTO" ]; then $SHELL; fi
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
echo "===== Application selected through env.sh: $oemirot_boot_path_project"
echo "====="
echo

# Path validation
action="Validating OEMiROT boot path project"

if [ ! -d $appli_dir ]; then
  echo "====="
  echo "===== Wrong Boot path: $oemirot_boot_path_project"
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

python $applicfg xmlval -v $app_bin --string -n "Firmware binary input file" $code_xml
if [ $? != "0" ]; then step_error; fi
python $applicfg xmlval -v $app_enc_sign_bin --string -n "Image output file" $code_xml
if [ $? != "0" ]; then step_error; fi

echo "   * Application firmware image generation"
echo "       Open the OEMiROT_Appli project with preferred toolchain and rebuild all files"
echo "       Press any key to continue..."
echo
if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi

echo "   * Loader firmware image generation"
echo "       Open the OEMiROT_Loader project with preferred toolchain and rebuild all files"
echo "       Press any key to continue..."
echo
if [ "$mode" != "AUTO" ]; then read -p "" -n1 -s; fi

# ========================================================= OEM2 key provisioning ==========================================================
set_oem2_key

# =================================================== Option Bytes and flash programming ===================================================
ob_programming

# =========================================================== RDP level setting ============================================================
set_rdp_level

# ================================================================== End ===================================================================
final_step