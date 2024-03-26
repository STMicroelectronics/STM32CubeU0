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

code_xml="$provisioningdir/OEMiROT/Images/OEMiRoT_Code_Image.xml"
data_xml="$provisioningdir/OEMiROT/Images/OEMiRoT_Data_Image.xml"
code_bin="$project_dir/$config/NUCLEO-U083RC_OEMiROT_Appli.bin"
code_dest_bin="$project_dir/../Binary/rot_app.bin"

echo "Postbuild image" >> $current_log_file 2>&1

echo "Copy binary to folder" >> $current_log_file 2>&1
cp -f $code_bin $code_dest_bin >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

echo "Creating application image"  >> $current_log_file 2>&1
"$stm32tpccli" -pb $code_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

echo "Creating data image"  >> $current_log_file 2>&1
"$stm32tpccli" -pb $data_xml >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

exit 0