@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Initial configuration
set connect_no_reset=-c port=SWD mode=Hotplug
set connect_reset=-c port=SWD mode=UR

::Remove Protections and Initialize option bytes
set remove_bank1_protect=-ob WRP1A_STRT=0x7f WRP1A_END=0
set remove_bank2_protect=-ob WRP1B_STRT=0x7f WRP1B_END=0
set rdp_0=-ob RDP=0xAA
set remove_hdp_protect=-ob HDP1_PEND=0 HDP1EN=0xB4
set default_ob=-ob BOOT_LOCK=0
set erase_all=-e all


::RDP Regression
echo Regression to RDP 0
:: Unlock RDP 2 to switch in RDP 1
%stm32programmercli% %connect_reset% -hardRst -unlockRDP2 %oem2_key%
echo Please unplug USB cable and plug it again to recover SWD Connection.
echo Press any key to continue...
echo.
IF [%1] neq [AUTO] pause >nul

:: Switch RDP 1 to RDP 0
%stm32programmercli% %connect_no_reset% %rdp_0%
IF %errorlevel% NEQ 0 %stm32programmercli% %connect_reset% %rdp_0%
IF !errorlevel! NEQ 0 goto :step_error

::Provisioning default OEM2 key
echo Provisioning of OEM2 key
%stm32programmercli% %connect_reset% -lockRDP2 %oem2_key%
IF !errorlevel! NEQ 0 goto :step_error

::Remove Bank1 Protections
echo  Remove Bank1 protection
%stm32programmercli% %connect_reset% %remove_bank1_protect%
IF !errorlevel! NEQ 0 goto :step_error

::Remove Bank2 Protections and erase all
echo  Remove Bank2 protection
%stm32programmercli% %connect_reset% %remove_bank2_protect% %erase_all%
IF !errorlevel! NEQ 0 goto :step_error

::Remove HDP protection
echo Remove HDP protection
%stm32programmercli% %connect_no_reset% %remove_hdp_protect%
IF !errorlevel! NEQ 0 goto :step_error

::Set Default OB
echo Set Default OB (BOOT_LOCK)
%stm32programmercli% %connect_no_reset% %default_ob%
IF !errorlevel! NEQ 0 goto :step_error

echo regression script done, press key
IF [%1] NEQ [AUTO] pause
exit 0

:step_error
echo regression script failed, press key
IF [%1] NEQ [AUTO] pause
exit 1