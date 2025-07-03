#!/bin/bash -
echo "regression script started"
source ../env.sh

connect_no_reset="-c port=SWD speed=fast mode=Hotplug"
connect_reset="-c port=SWD mode=UR"

rdp_0="-ob RDP=0xAA"
remove_bank1_protect="-ob WRP1A_STRT=0x1f WRP1A_END=0"
remove_bank2_protect="-ob WRP1B_STRT=0x1f WRP1B_END=0"
erase_all="-e all"
remove_hdp_protection="-ob HDP1_PEND=0 HDP1EN=0xB4"
default_ob="-ob BOOT_LOCK=0"


echo "Regression to RDP 0"
#RDP Regression
echo "Regression to RDP 0"
# Unlock RDP 2 to switch in RDP 1
"$stm32programmercli" $connect_reset -hardRst -unlockRDP2 $oem2_key
echo "Please unplug USB cable and plug it again to recover SWD Connection."
echo "Press any key to continue..."
echo
if [ "$1" != "AUTO" ]; then read -p "" -n1 -s; fi

# Switch RDP 1 to RDP 0
"$stm32programmercli" $connect_no_reset $rdp_0
ret=$?
if [ $ret != 0 ]; then
  "$stm32programmercli" $connect_reset $rdp_0
  ret=$?
  if [ $ret != 0 ]; then
    if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
    exit 1
  fi
fi

echo "Provision default OEM2 key"
"$stm32programmercli" $connect_reset -lockRDP2 $oem2_key
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Remove bank1 protection"
"$stm32programmercli" $connect_reset $remove_bank1_protect
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Remove bank2 protection and erase all"
"$stm32programmercli" $connect_no_reset $remove_bank2_protect $erase_all
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Remove HDP protection"
"$stm32programmercli" $connect_no_reset $remove_hdp_protection
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi

echo "Set default OB"
"$stm32programmercli" $connect_no_reset $default_ob
ret=$?
if [ $ret != 0 ]; then
  if [ "$1" != "AUTO" ]; then read -p "regression script failed, press a key" -n1 -s; fi
  exit 1
fi
if [ "$1" != "AUTO" ]; then read -p "regression script Done, press a key" -n1 -s; fi
exit 0
