#!/bin/bash

source ../env.sh NULL

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script
projectdir=`dirname $SCRIPT`

current_directory=`pwd`
cd $projectdir"/Keys"
keys_dir=`pwd`
cd $projectdir"/../../Applications/ROT/OEMiROT_Boot/Src"
source_dir=`pwd`
keys_c_path=$source_dir"/keys.c"

#ecc 256 auth key
key_ecc="$keys_dir/OEMiRoT_Authentication.pem"

command_key="keygen -k $key_ecc -t ecdsa-p256"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" > $projectdir"/keygen.txt"
exit 1
fi

command_key="getpub -k $key_ecc"
"$imgtool" $command_key > $keys_c_path
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" > $projectdir"/keygen.txt"
exit 1
fi

#priv key to encode images
key_ecc_enc_priv=$keys_dir"/OEMiRoT_Encryption.pem"
key_ecc_enc_pub=$keys_dir"/OEMiRoT_Encryption_Pub.pem"
command_key="keygen -k $key_ecc_enc_priv -t ecdsa-p256 -e $key_ecc_enc_pub"
"$imgtool" $command_key
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" > $projectdir"/keygen.txt"
exit 1
fi

command_key="getpriv --minimal -k $key_ecc_enc_priv"
"$imgtool" $command_key >> $keys_c_path
ret=$?
if [ $ret != 0 ]; then
echo $command_key" : failed" > $projectdir"/keygen.txt"
exit 1
fi
