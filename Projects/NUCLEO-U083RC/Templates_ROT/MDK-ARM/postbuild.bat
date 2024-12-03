@ECHO OFF
:: Getting the Trusted Package Creator CLI path
set "projectdir=%~dp0"
pushd %projectdir%\..\..\ROT_Provisioning
set provisioningdir=%cd%
popd
call "%provisioningdir%\env.bat"

:: Enable delayed expansion
setlocal EnableDelayedExpansion
:: Environment variable for log file
set current_log_file="%projectdir%\postbuild.log"
echo. > %current_log_file%

set appli_dir="..\..\..\Applications\ROT\OEMiROT_Appli"
set app_bin="%appli_dir%\Binary\rot_app.bin"
set app_enc_sign_bin="%appli_dir%\Binary\rot_app_enc_sign.bin"
set code_image_file="%provisioningdir%\OEMiROT\Images\OEMiRoT_Code_Image.xml"
set data_image_file="%provisioningdir%\OEMiROT\Images\OEMiRoT_Data_Image.xml"

goto exe:
goto py:
:exe
::line for window executable
set "applicfg=%cube_fw_path%/Utilities/PC_Software/ROT_AppliConfig/dist/AppliCfg.exe"
set "python="
if exist %applicfg% (
echo run config Appli with windows executable
goto postbuild
)
:py
::line for python
echo run config Appli with python script
set "applicfg=%cube_fw_path%/Utilities/PC_Software/ROT_AppliConfig/AppliCfg.py"
set "python=python "

:postbuild
::update xml file
set "command=%python%%applicfg% xmlval -v %app_bin% --string -n "Firmware binary input file" %code_image_file%"
%command%
IF !errorlevel! NEQ 0 goto :error
set "command=%python%%applicfg% xmlval -v %app_enc_sign_bin% --string -n "Image output file" %code_image_file%"
%command%
IF !errorlevel! NEQ 0 goto :error

echo Creating OEMiROT image  >> %current_log_file% 2>&1
%stm32tpccli% -pb %code_image_file% >> %current_log_file% 2>&1
IF !errorlevel! NEQ 0 goto :error

echo Creating OEMiROT data  >> %current_log_file% 2>&1
%stm32tpccli% -pb %data_image_file% >> %current_log_file% 2>&1
IF !errorlevel! NEQ 0 goto :error
exit 0

:error
echo.
echo =====
echo ===== Error occurred.
echo ===== See %current_log_file% for details. Then try again.
echo =====
exit 1
