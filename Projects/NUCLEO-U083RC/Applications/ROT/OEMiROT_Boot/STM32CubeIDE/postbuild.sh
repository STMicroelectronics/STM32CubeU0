#!/bin/bash
# Getting the Trusted Package Creator CLI path
SCRIPT=$(readlink -f $0)
project_dir=`dirname $SCRIPT`
echo $project_dir
cd "$project_dir/../../../../ROT_Provisioning"
provisioningdir=$(pwd)
cd $project_dir
source $provisioningdir/env.sh "$provisioningdir"

applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/dist/AppliCfg.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [ -e "$applicfg" ]; then
  #line for window executable
  echo "AppliCfg with windows executable"
  python=""
else
  #line for python
  echo "AppliCfg with python script"
  applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/AppliCfg.py"
  python="python "
fi

error()
{
    echo ""
    echo "====="
    echo "===== Error occurred."
    echo "===== See the $current_log_file for details. Then try again."
    echo "====="
    exit 1
}

# Environment variable for log file
current_log_file="$project_dir/postbuild.log"
echo "" > $current_log_file

# arg1 is the config type (Debug, Release)
config=$1

preprocess_bl2_file="$project_dir/image_macros_preprocessed_bl2.c"
appli_dir="../../../../$oemirot_boot_path_project"
loader_dir="../../../../Applications/ROT/OEMiROT_Loader"
option_bytes="$project_dir/../../../../ROT_Provisioning/OEMiROT/ob_flash_programming.sh"

appli_system_file="$appli_dir/Src/system_stm32u0xx.c"
loader_system_file="$loader_dir/Src/system_stm32u0xx.c"

appli_flash_layout="$appli_dir/Inc/appli_flash_layout.h"
loader_flash_layout="$loader_dir/Inc/loader_flash_layout.h"

appli_ld_file="$appli_dir/STM32CubeIDE/STM32U083RCIX_FLASH.ld"
loader_ld_file="$loader_dir/STM32CubeIDE/STM32U083RCIX_FLASH.ld"

code_xml="$project_dir/../../../../ROT_Provisioning/OEMiROT/Images/OEMiRoT_Code_Image.xml"
data_xml="$project_dir/../../../../ROT_Provisioning/OEMiROT/Images/OEMiRoT_Data_Image.xml"

if [ -f $appli_ld_file ]; then
    $python$applicfg linker --layout $preprocess_bl2_file -m RE_AREA_0_OFFSET -n CODE_OFFSET --vb $appli_ld_file >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg linker --layout $preprocess_bl2_file -m RE_CODE_IMAGE_SIZE -n CODE_SIZE --vb $appli_ld_file >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi
fi

$python$applicfg linker --layout $preprocess_bl2_file -m RE_LOADER_CODE_OFFSET -n LOADER_CODE_START --vb $loader_ld_file >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg linker --layout $preprocess_bl2_file -m RE_LOADER_CODE_SIZE -n LOADER_CODE_SIZE --vb $loader_ld_file >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

if [ -f $appli_system_file ]; then
    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_IMAGE_PRIMARY_AREA_OFFSET -n VECT_TAB_OFFSET --vb $appli_system_file >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi
fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_LOADER_CODE_OFFSET -n VECT_TAB_OFFSET --vb $loader_system_file >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

if [ -f $appli_flash_layout ]; then
    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_PRIMARY_ONLY -n PRIMARY_ONLY --vb $appli_flash_layout  >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_OVER_WRITE -n OVERWRITE_ONLY --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_IMAGE_NUMBER -n DATA_IMAGE_NUMBER --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_AREA_0_OFFSET -n IMAGE_PRIMARY_PARTITION_OFFSET --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_AREA_2_OFFSET -n IMAGE_SECONDARY_PARTITION_OFFSET --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_AREA_5_OFFSET -n DATA_IMAGE_PRIMARY_PARTITION_OFFSET --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_AREA_7_OFFSET -n DATA_IMAGE_SECONDARY_PARTITION_OFFSET --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_CODE_IMAGE_SIZE -n IMAGE_PARTITION_SIZE --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_IMAGE_SIZE -n DATA_PARTITION_SIZE --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_OFFSET -n DATA_HEADER_OFFSET --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi

    $python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_SIZE -n DATA_SIZE --vb $appli_flash_layout >> $current_log_file 2>&1
    if [ $? != 0 ]; then error; fi
fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_PRIMARY_ONLY -n PRIMARY_ONLY --vb $loader_flash_layout >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_IMAGE_NUMBER -n DATA_IMAGE_NUMBER --vb $loader_flash_layout >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_IMAGE_PARTITION_OFFSET -n IMAGE_PARTITION_OFFSET --vb $loader_flash_layout >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_IMAGE_PARTITION_OFFSET -n DATA_IMAGE_PARTITION_OFFSET --vb $loader_flash_layout >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_CODE_IMAGE_SIZE -n IMAGE_PARTITION_SIZE --vb $loader_flash_layout >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg definevalue --layout $preprocess_bl2_file -m RE_DATA_IMAGE_SIZE -n DATA_PARTITION_SIZE --vb $loader_flash_layout >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b wrp1a_end -m RE_BL2_WRP_END -d 0x800 --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b wrp1b_start -m RE_LOADER_CODE_OFFSET -d 0x800 --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b wrp1b_end -m RE_LOADER_WRP_END -d 0x800 --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b hdp_end -m RE_BL2_HDP_END -d 0x800 --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b appliaddress -m RE_IMAGE_PARTITION_ADDRESS --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b dataaddress -m RE_DATA_IMAGE_PARTITION_ADDRESS --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b loaderaddress -m RE_LOADER_CODE_START --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg flash --layout $preprocess_bl2_file -b data_image_number -m RE_DATA_IMAGE_NUMBER --vb $option_bytes >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlval --layout $preprocess_bl2_file -m RE_CODE_IMAGE_SIZE -c S --vb $code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlval --layout $preprocess_bl2_file -m RE_DATA_IMAGE_SIZE -c S --vb $data_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlparam --layout $preprocess_bl2_file -m RE_PRIMARY_ONLY -n "Slot Option" -t Data -c --primary-only -h 1 -d "" --vb $code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlparam --layout $preprocess_bl2_file -m RE_PRIMARY_ONLY -n "Slot Option" -t Data -c --primary-only -h 1 -d "" --vb $data_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlparam --layout $preprocess_bl2_file -m RE_OVER_WRITE -n "Write Option" -t Data -c --overwrite-only -h 1 -d "" --vb $code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlparam --layout $preprocess_bl2_file -m RE_OVER_WRITE -n "Write Option" -t Data -c --overwrite-only -h 1 -d "" --vb $data_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlparam --layout $preprocess_bl2_file -m RE_ENCRYPTION -n "Encryption key" -t File -c -E -h 1 -d "../Keys/OEMiRoT_Encryption.pem" --vb $code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

$python$applicfg xmlparam --layout $preprocess_bl2_file -m RE_ENCRYPTION -n "Encryption key" -t File -c -E -h 1 -d "../Keys/OEMiRoT_Encryption.pem" --vb $data_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

cp $project_dir/$config/NUCLEO-U083RC_OEMiROT_Boot.bin $project_dir/../Binary/OEMiROT_Boot.bin >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

exit 0