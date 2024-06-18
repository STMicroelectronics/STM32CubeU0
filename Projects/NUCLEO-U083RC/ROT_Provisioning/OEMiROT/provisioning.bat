@ECHO OFF

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Getting the CubeProgammer_cli path
call ../env.bat

::Updated by boot postbuild
set data_image_number=1
set ext_loader=1

:: CubeProgammer path and input files
set ob_flash_programming="ob_flash_programming.bat"

set ob_flash_log="ob_flash_programming.log"
set state_change_log="provisioning.log"
set "projectdir=%~dp0"
set appli_dir="../../%oemirot_boot_path_project%"
set app_bin="../%appli_dir%/Binary/rot_app.bin"
set app_enc_sign_bin="../%appli_dir%/Binary/rot_app_enc_sign.bin"
set code_image_file="%projectdir%Images\OEMiROT_Code_Image.xml"
set data_image_file="%projectdir%Images\OEMiRoT_Data_Image.xml"
set boot_cfg_h="%cube_fw_path%\Projects\NUCLEO-U083RC\Applications\ROT\OEMiROT_Boot\Inc\boot_hal_cfg.h"

:: Initial configuration
set connect_no_reset=-c port=SWD mode=Hotplug
set connect_reset=-c port=SWD mode=UR

:: OEM2 key
set oem2_key=0x11223344 0x55667788 0x99AABBCC 0xDDEEFF00

goto exe:
goto py:
:exe
::line for window executable
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\dist\AppliCfg.exe"
set "python="
if exist %applicfg% (
echo run config Appli with windows executable
goto prov
)
:py
::line for python
echo run config Appli with python script
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\AppliCfg.py"
set "python=python "

:prov
echo =====
echo ===== Provisioning of OEMiRoT boot path
echo ===== Application selected through env.bat:
echo =====   %oemirot_boot_path_project%
echo =====
echo.

:: Check if application path exists
if not exist %appli_dir% (
echo =====
echo ===== Wrong Boot path: %oemirot_boot_path_project%
echo ===== please modify the env.bat to set the right path
goto step_error
)

:: ============================================================= RDP selection =============================================================
:define_rdp_level
set "action=Define RDP level"
echo    * %action%
set /p "RDP_level=      %USERREG% [ 2 | 1 | 0 ]: "

if /i "%RDP_level%" == "0" (
echo.
set rdp_value=0xAA
set rdp_str="OB_RDP_LEVEL_0"
goto image_generation
)

if /i "%RDP_level%" == "1" (
echo.
set rdp_value=0xBB
set rdp_str="OB_RDP_LEVEL_1"
goto image_generation
)

if /i "%RDP_level%" == "2" (
echo.
set rdp_value=0xCC
set rdp_str="OB_RDP_LEVEL_2"
goto image_generation
)

echo        WRONG RDP level selected
set current_log_file="./*.log files "
echo;
goto define_rdp_level

:: ======================================================== Images generation steps ========================================================
:image_generation
:: Replace RDP level in boot_hal_cfg
%python%%applicfg% modifyfilevalue --variable OEMIROT_OB_RDP_LEVEL_VALUE --value %rdp_str% %boot_cfg_h% --str

echo    * Boot firmware image generation
echo        Open the OEMiROT_Boot project with preferred toolchain and rebuild all files.
echo        Press any key to continue...
echo.
if [%1] neq [AUTO] pause >nul

::update xml file
set "command=%python%%applicfg% xmlval -v %app_bin% --string -n "Firmware binary input file" %code_image_file%"
%command%
IF !errorlevel! NEQ 0 goto :step_error
set "command=%python%%applicfg% xmlval -v %app_enc_sign_bin% --string -n "Image output file" %code_image_file%"
%command%
IF !errorlevel! NEQ 0 goto :step_error

echo    * Application firmware image generation
echo        Open the OEMiROT_Appli project with preferred toolchain and rebuild all files.
echo        Press any key to continue...
echo.
if [%1] neq [AUTO] pause >nul

if  "%ext_loader%" == "0" (goto :no_loader)
echo    * Loader firmware image generation
echo        Open the OEMiROT_Loader project with preferred toolchain and rebuild all files.
echo        Press any key to continue...
echo.
if [%1] neq [AUTO] pause >nul

:no_loader


if  "%data_image_number%" == "0" (goto :no_data)
echo    * Data generation (if Data image is enabled)
echo        Select OEMiROT_Data_Image.xml(Default path is \ROT\Provisioning\OEMiROT\OEMiROT_Data_Image.xml)
echo        Generate the data_enc_sign.hex image
echo        Press any key to continue...
echo.
%stm32tpccli% -pb %data_image_file% > %state_change_log%
if !errorlevel! neq 0 goto :step_error

:no_data


:: ========================================================= OEM2 key provisioning ==========================================================
set "action=Provisioning of OEM2 key : %oem2_key%"
set current_log_file=%state_change_log%
echo    * %action%
echo.
set "command=%stm32programmercli% %connect_reset% -lockRDP2 %oem2_key%"
%command% >> %state_change_log%
IF !errorlevel! NEQ 0 goto :step_error

:: ================================================== Option Bytes and flash programming ===================================================
set "action=Programming the option bytes and flashing the images..."
set current_log_file=%ob_flash_log%
set "command=start /w /b call %ob_flash_programming% AUTO"
echo    * %action%
%command% > %ob_flash_log%

if !errorlevel! neq 0 goto :step_error

echo        Successful option bytes programming and images flashing
echo        (see %ob_flash_log% for details)
echo.

:: =========================================================== RDP level setting ===========================================================
:set_rdp_level
set "action=Setting RDP level %RDP_level%"
set current_log_file=%state_change_log%
echo    * %action%
set "command=%stm32programmercli% %connect_reset% -ob RDP=%rdp_value%"
%command% >> %state_change_log%
echo.
if "%rdp_value%" == "0xAA" ( goto final_execution )
echo    * Please unplug USB cable and plug it again to recover SWD Connection.
echo        Press any key to continue...
echo.
if [%1] neq [AUTO] pause >nul
:: In RDP 2, the connection with the board is lost and the return value of the command cannot be verified
goto final_execution

:: ============================================================= End functions =============================================================
:: All the steps to set the STM32U0 product were executed correctly
:final_execution
echo =====
echo ===== The board is correctly configured.
echo ===== Connect UART console (115200 baudrate) to get application menu.

:no_menu
echo =====
cmd /k
exit 0

:: Error when external script is executed
:step_error
echo.
echo =====
echo ===== Error while executing "%action%".
echo ===== See %current_log_file% for details. Then try again.
echo =====
cmd /k
exit 1


