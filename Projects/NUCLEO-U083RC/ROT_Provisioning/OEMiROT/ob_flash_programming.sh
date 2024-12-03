#!/bin/bash
source ../env.sh

wrp1a_end=0x20
wrp1b_start=0x30
wrp1b_end=0x37
hdp_end=0x2F
bootaddress=0x8000000
appliaddress=0x8030000
dataaddress=0x801E000
loaderaddress=0x8018000
data_image_number=0x1
ext_loader=0x1
code_image=$oemirot_appli
boot_image=$oemirot_boot

connect_no_reset="-c port=SWD speed=fast mode=Hotplug"
connect_reset="-c port=SWD speed=fast mode=UR"

error()
{
  echo "        Error when trying to "$action > "ob_flash_programming.log"
  echo "        Programming aborted" >> "ob_flash_programming.log"
}

# ============================================= Remove protections and initialize Option Bytes =============================================
remove_protect_init="-ob WRP1A_STRT=0x7f WRP1A_END=0 WRP1B_STRT=0x7f WRP1B_END=0 HDP1_PEND=0 HDP1EN=0xB4 BOOT_LOCK=0"
# ========================================================== Erase the user flash ==========================================================
erase_all="-e all"
# =============================================================== Hardening ================================================================
hide_protect="HDP1_PEND=$hdp_end HDP1EN=1"
boot_write_protect="WRP1A_STRT=0x0 WRP1A_END=$wrp1a_end"
loader_write_protect="WRP1B_STRT=$wrp1b_start WRP1B_END=$wrp1b_end"
boot_lock="BOOT_LOCK=1"
# ========================================================= Configure Option Bytes =========================================================
action="Remove protections and mass erase"
echo "$action"
echo "$stm32programmercli" $connect_reset $remove_protect_init $erase_all
"$stm32programmercli" $connect_reset $remove_protect_init $erase_all
if [ $? -ne 0 ]; then error; return 1; fi
# ============================================================ Download images =============================================================
echo "Images programming"

action="Write OEMiROT_Boot"
echo "$action"
"$stm32programmercli" $connect_reset -d "../../"$oemirot_boot_path_project"/Binary/"$boot_image $bootaddress -v
if [ $? -ne 0 ]; then error; return 1; fi
echo "OEMiROT_Boot Written"

action="Write OEMiROT_Appli"
echo "$action"
"$stm32programmercli" $connect_reset -d "../../"$oemirot_appli_path_project"/Binary/"$code_image $appliaddress -v
if [ $? -ne 0 ]; then error; return 1; fi
echo "OEMiROT_Appli Written"

if [ "$data_image_number" == "0x1" ]; then
	action="Write OEMiROT_Data"
	echo "$action"
	"$stm32programmercli" $connect_reset -d "$rot_provisioning_path/OEMiROT/Binary/data_init_sign.bin" $dataaddress -v
	if [ $? -ne 0 ]; then error; return 1; fi
fi

if [ "$ext_loader" == "0x1" ]; then
	action="Write OEMiROT_Loader"
	echo "$action"
	"$stm32programmercli" $connect_reset -d "$cube_fw_path/Projects/NUCLEO-U083RC/Applications/ROT/OEMiROT_Loader/Binary/OEMiROT_Loader.bin" $loaderaddress -v
	if [ $? -ne 0 ]; then error; return 1; fi
	echo "OEMiROT_Loader Written"
fi
# ========================================================= Extra board protections ========================================================
action="Configure Option Bytes: Write Protection, Hide Protection and Boot Lock"
echo "$action"
"$stm32programmercli" $connect_no_reset -ob $boot_write_protect $loader_write_protect $hide_protect $boot_lock
if [ $? -ne 0 ]; then error; return 1; fi

echo "Programming success"

return 0;
