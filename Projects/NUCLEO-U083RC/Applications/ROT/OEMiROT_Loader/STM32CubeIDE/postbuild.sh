#!/bin/bash
# Environment variable for log file
SCRIPT=$(readlink -f $0)
project_dir=`dirname $SCRIPT`
current_log_file="$project_dir/postbuild.log"

# arg1 is the config type (Debug, Release)
config=$1

code_bin="$project_dir/$config/NUCLEO-U083RC_OEMiROT_Loader.bin"
code_dest_bin="$project_dir/../Binary/OEMiROT_Loader.bin"

error()
{
    echo ""
    echo "====="
    echo "===== Error occurred."
    echo "===== See $current_log_file for details. Then try again."
    echo "====="
    exit 1
}

echo "Postbuild image" > $current_log_file 2>&1
echo "Copy binary to folder" >> $current_log_file 2>&1
cp -vf $code_bin $code_dest_bin >> $current_log_file 2>&1
if [ $? != 0 ]; then error; fi

exit 0