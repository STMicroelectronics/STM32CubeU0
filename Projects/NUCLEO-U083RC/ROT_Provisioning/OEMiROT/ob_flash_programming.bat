call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Data updated with the postbuild of OEMiROT-Boot
set wrp1a_end=0x20
set wrp1b_start=0x30
set wrp1b_end=0x37
set hdp_end=0x2F
set appliaddress=0x8030000
set dataaddress=0x801E000
set loaderaddress=0x8018000
set data_image_number=1
set ext_loader=1
set code_image=%oemirot_appli%

:: STM32CubeProgammer connection
set connect_no_reset=-c port=SWD mode=Hotplug
set connect_reset=-c port=SWD mode=UR

:: ============================================= Remove protections and initialize Option Bytes =============================================
set remove_protect_init=-ob WRP1A_STRT=0x7f WRP1A_END=0 WRP1B_STRT=0x7f WRP1B_END=0 HDP1_PEND=0 HDP1EN=0xB4 BOOT_LOCK=0 BKPSRAM_HW_ERASE_DISABLE=0
:: ========================================================== Erase the user flash ==========================================================
set erase_all=-e all
:: =============================================================== Hardening ================================================================
set hide_protect=HDP1_PEND=%hdp_end% HDP1EN=1
set boot_write_protect=WRP1A_STRT=0x0 WRP1A_END=%wrp1a_end%
set loader_write_protect=WRP1B_STRT=%wrp1b_start% WRP1B_END=%wrp1b_end%
set boot_lock=BOOT_LOCK=1
:: ========================================================= Configure Option Bytes =========================================================
set "action=Remove protections and erase all"
echo %action%
%stm32programmercli% %connect_reset% %remove_protect_init% %erase_all%
IF !errorlevel! NEQ 0 goto :error
:: ============================================================ Download images =============================================================
echo "Images programming"

set "action=Write OEMiROT_Boot"
echo %action%
%stm32programmercli% %connect_no_reset% -d %cube_fw_path%\Projects\NUCLEO-U083RC\Applications\ROT\OEMiROT_Boot\Binary\OEMiROT_Boot.bin 0x8000000 -v
IF !errorlevel! NEQ 0 goto :error
echo "OEMiROT_Boot Written"

set "action=Write OEMiROT_Appli"
echo %action%
%stm32programmercli% %connect_no_reset% -d ../../%oemirot_boot_path_project%/Binary/%code_image% %appliaddress% -v
IF !errorlevel! NEQ 0 goto :error
echo "OEMiROT_Appli Written"

if  "%data_image_number%" == "1" (
set "action=Write OEMiROT_Data"
echo %action%
%stm32programmercli% %connect_no_reset% -d %cube_fw_path%\Projects\NUCLEO-U083RC\ROT_Provisioning\OEMiROT\Binary\data_enc_sign.bin %dataaddress% -v
IF !errorlevel! NEQ 0 goto :error
echo "OEMiROT_Data Written"
)

if  "%ext_loader%" == "1" (
set "action=Write OEMiROT_Loader"
echo %action%
%stm32programmercli% %connect_no_reset% -d %cube_fw_path%\Projects\NUCLEO-U083RC\Applications\ROT\OEMiROT_Loader\Binary\OEMiROT_Loader.bin %loaderaddress% -v
IF !errorlevel! NEQ 0 goto :error
echo "OEMiROT_Loader Written"
)

:: ========================================================= Extra board protections ========================================================
set "action=Configure Option Bytes: Write Protection, Hide Protection and Boot Lock"
echo %action%
%stm32programmercli% %connect_no_reset% -ob %boot_write_protect% %loader_write_protect% %hide_protect% %boot_lock%
IF !errorlevel! NEQ 0 goto :error

echo Programming success
IF [%1] NEQ [AUTO] cmd /k
exit 0

:error
echo      Error when trying to "%action%" >CON
echo      Programming aborted >CON
IF [%1] NEQ [AUTO] cmd /k
exit 1

