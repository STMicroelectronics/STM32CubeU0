#!/bin/bash
# Getting the Trusted Package Creator CLI path
SCRIPT=$(readlink -f $0)
project_dir=`dirname $SCRIPT`
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
  #determine/check python version command
  python="python "
fi

error()
{
    echo ""
    echo "====="
    echo "===== Error occurred."
    echo "===== See $current_log_file for details. Then try again."
    echo "====="
    exit 1
}

# Environment variable for log file
current_log_file="$project_dir/postbuild.log"
echo "" > $current_log_file

# arg1 is the config type (Debug, Release)
config=$1

appli_dir="../../../Applications/ROT/OEMiROT_Appli"
app_bin=$appli_dir"/Binary/rot_app.bin"
app_enc_sign_bin=$appli_dir"/Binary/rot_app_enc_sign.bin"

code_xml="$provisioningdir/OEMiROT/Images/OEMiRoT_Code_Image.xml"
init_code_xml="$provisioningdir/OEMiROT/Images/OEMiRoT_Init_Code_Image.xml"
data_xml="$provisioningdir/OEMiROT/Images/OEMiRoT_Data_Image.xml"
init_data_xml="$provisioningdir/OEMiROT/Images/OEMiRoT_Init_Data_Image.xml"
code_bin="$project_dir/$config/NUCLEO-U083RC_OEMiROT_Appli.bin"
code_dest_bin="$project_dir/../Binary/rot_app.bin"

#Field updated with OEMiROT Boot postbuild
primary_only=0x1

echo "Postbuild image" >> $current_log_file 2>&1

$python $applicfg xmlval -v $app_bin --string -n "Firmware binary input file" $code_xml
if [ $? != "0" ]; then error; fi
$python $applicfg xmlval -v $app_enc_sign_bin --string -n "Image output file" $code_xml
if [ $? != "0" ]; then error; fi

# Update Init xml file , encrypt option is for init image in swap mode
if [ "$primary_only" == "0x1" ]; then
    $python$applicfg xmlparam --option rm -n "Encryption key" $init_code_xml
    if [ $? != 0 ]; then error; fi

    $python$applicfg xmlparam --option rm -n "Encryption key" $init_data_xml
    if [ $? != 0 ]; then error; fi

    $python$applicfg xmlparam --option rm -n "Clear Image" $init_code_xml
    if [ $? != 0 ]; then error; fi

    $python$applicfg xmlparam --option rm -n "Clear Image" $init_data_xml
    if [ $? != 0 ]; then error; fi
fi

echo "Copy binary to folder" >> $current_log_file 2>&1
cp -f $code_bin $code_dest_bin >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

echo "Creating application image"  >> $current_log_file 2>&1
"$stm32tpccli" -pb $code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

echo "Creating init application image"  >> $current_log_file 2>&1
"$stm32tpccli" -pb $init_code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

exit 0