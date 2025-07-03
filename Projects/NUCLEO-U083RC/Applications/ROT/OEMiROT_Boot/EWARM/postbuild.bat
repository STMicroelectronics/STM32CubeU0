@ECHO OFF
:: Getting the Trusted Package Creator and STM32CubeProgammer CLI path
set "projectdir=%~dp0"
pushd %projectdir%\..\..\..\..\ROT_Provisioning
set provisioningdir=%cd%
popd
call "%provisioningdir%\env.bat"

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Environment variable for log file
set current_log_file="%projectdir%\postbuild.log"
echo. > %current_log_file%

:start
goto exe:
goto py:
:exe
::line for window executable
set applicfg="%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\dist\AppliCfg.exe"
set "python="
if exist %applicfg% (
echo run config Appli with windows executable
goto postbuild
)
:py
::line for python
echo run config Appli with python script
set applicfg="%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\AppliCfg.py"
set "python= "

:postbuild
set preprocess_bl2_file="%projectdir%\image_macros_preprocessed_bl2.c"
set "appli_dir=..\..\..\..\%oemirot_appli_path_project%"
set "loader_dir=..\..\..\..\Applications\ROT\OEMiROT_Loader"
set option_bytes="%projectdir%\..\..\..\..\ROT_Provisioning\OEMiROT\ob_flash_programming.bat"
set provisioning="%projectdir%\..\..\..\..\ROT_Provisioning\OEMiROT\provisioning.bat"
set postbuild_file="%appli_dir%\EWARM\postbuild.bat"

:: Environment variable for AppliCfg
set appli_system_file="%appli_dir%\Src\system_stm32u0xx.c"
set loader_system_file="%loader_dir%\Src\system_stm32u0xx.c"

set appli_flash_layout="%appli_dir%\Inc\appli_flash_layout.h"
set loader_flash_layout="%loader_dir%\Inc\loader_flash_layout.h"

set appli_linker_file="%appli_dir%\EWARM\stm32u083xx_flash.icf"
set loader_linker_file="%loader_dir%\EWARM\stm32u083xx_flash.icf"

set code_xml="%projectdir%\..\..\..\..\ROT_Provisioning\OEMiROT\Images\OEMiRoT_Code_image.xml"
set data_xml="%projectdir%\..\..\..\..\ROT_Provisioning\OEMiROT\Images\OEMiRoT_Data_Image.xml"

set init_code_xml="%projectdir%\..\..\..\..\ROT_Provisioning\OEMiROT\Images\OEMiRoT_Code_Init_Image.xml"
set init_data_xml="%projectdir%\..\..\..\..\ROT_Provisioning\OEMiROT\Images\OEMiRoT_Data_Init_Image.xml"

set code_size="Firmware area size"
set data_size="Data area size"
set scratch_sector_number="Number of scratch sectors"
set init_app_bin="..\..\..\Applications\ROT\OEMiROT_Appli\Binary\rot_app_init_sign.bin"
set init_data_bin="..\Binary\data_init_sign.bin"
set fw_out_bin="Image output file"

:: Bypass configuration of appli linker file if not present
if not exist %appli_linker_file% goto :loader_linker_file
set "command=%python%%applicfg% linker --layout %preprocess_bl2_file% -m RE_AREA_0_OFFSET -n CODE_OFFSET %appli_linker_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% linker --layout %preprocess_bl2_file% -m RE_CODE_IMAGE_SIZE -n CODE_SIZE %appli_linker_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:loader_linker_file
set "command=%python%%applicfg% linker --layout %preprocess_bl2_file% -m RE_LOADER_CODE_START -n LOADER_CODE_START %loader_linker_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% linker --layout %preprocess_bl2_file% -m RE_LOADER_CODE_SIZE -n LOADER_CODE_SIZE %loader_linker_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:appli_system_file
:: Bypass configuration of application system file if not present
if not exist %appli_system_file% goto :loader_system_file
set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_IMAGE_PRIMARY_AREA_OFFSET -n VECT_TAB_OFFSET %appli_system_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:loader_system_file
set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_LOADER_CODE_OFFSET -n VECT_TAB_OFFSET %loader_system_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:appli_flash_layout
:: Bypass configuration of appli flash layout file if not present
if not exist %appli_flash_layout% goto :loader_flash_layout
set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_PRIMARY_ONLY -n PRIMARY_ONLY %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_OVER_WRITE -n OVERWRITE_ONLY %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_IMAGE_NUMBER -n DATA_IMAGE_NUMBER %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_AREA_0_OFFSET -n IMAGE_PRIMARY_PARTITION_OFFSET %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_AREA_2_OFFSET -n IMAGE_SECONDARY_PARTITION_OFFSET %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_AREA_5_OFFSET -n DATA_IMAGE_PRIMARY_PARTITION_OFFSET %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_AREA_7_OFFSET -n DATA_IMAGE_SECONDARY_PARTITION_OFFSET %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_CODE_IMAGE_SIZE -n IMAGE_PARTITION_SIZE %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_IMAGE_SIZE -n DATA_PARTITION_SIZE %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_OFFSET -n DATA_HEADER_OFFSET %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_SIZE -n DATA_SIZE %appli_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:loader_flash_layout
set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_PRIMARY_ONLY -n PRIMARY_ONLY %loader_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_IMAGE_NUMBER -n DATA_IMAGE_NUMBER %loader_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_IMAGE_PARTITION_OFFSET -n IMAGE_PARTITION_OFFSET %loader_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_IMAGE_PARTITION_OFFSET -n DATA_IMAGE_PARTITION_OFFSET %loader_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_CODE_IMAGE_SIZE -n IMAGE_PARTITION_SIZE %loader_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% definevalue --layout %preprocess_bl2_file% -m RE_DATA_IMAGE_SIZE -n DATA_PARTITION_SIZE %loader_flash_layout% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:option_bytes
set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp1a_end -m RE_BL2_WRP_END -d 0x800 %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp1b_start -m RE_LOADER_CODE_OFFSET -d 0x800 %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b wrp1b_end -m RE_LOADER_WRP_END -d 0x800 %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b hdp_end -m RE_BL2_HDP_END -d 0x800 %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:: Application, data and loader addresses
set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b appliaddress -m RE_INIT_IMAGE_PARTITION_ADDRESS %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b dataaddress -m RE_INIT_DATA_IMAGE_PARTITION_ADDRESS %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b loaderaddress -m RE_LOADER_CODE_START %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b data_image_number -m RE_DATA_IMAGE_NUMBER --decimal %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b ext_loader -m RE_LOADER --decimal %option_bytes% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b data_image_number -m RE_DATA_IMAGE_NUMBER --decimal %provisioning% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b ext_loader -m RE_LOADER --decimal %provisioning% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% flash --layout %preprocess_bl2_file% -b primary_only -m RE_PRIMARY_ONLY --decimal %postbuild_file% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

:: xml files used for image generation
set "command=%python%%applicfg% xmlval --layout %preprocess_bl2_file% -m RE_CODE_IMAGE_SIZE -c S %code_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval --layout %preprocess_bl2_file% -m RE_DATA_IMAGE_SIZE -c S %data_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --layout  %preprocess_bl2_file% -m RE_PRIMARY_ONLY -n "Slot Option" -t Data -c --primary-only -h 1 -d "" --vb %code_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --layout  %preprocess_bl2_file% -m RE_PRIMARY_ONLY -n "Slot Option" -t Data -c --primary-only -h 1 -d "" --vb %data_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --layout  %preprocess_bl2_file% -m RE_OVER_WRITE -n "Write Option" -t Data -c --overwrite-only -h 1 -d "" --vb %code_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --layout  %preprocess_bl2_file% -m RE_OVER_WRITE -n "Write Option" -t Data -c --overwrite-only -h 1 -d "" --vb %data_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval --layout %preprocess_bl2_file% -m RE_FLASH_AREA_SCRATCH_SIZE -n %scratch_sector_number% --decimal %code_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval -xml %code_xml% -nxml %code_size% -nxml %scratch_sector_number% --decimal -e (((val1+1)/val2)+1) -cond val2 -c M %code_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval --layout %preprocess_bl2_file% -m RE_FLASH_AREA_SCRATCH_SIZE -n %scratch_sector_number% --decimal %data_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval -xml %data_xml% -nxml %data_size% -nxml %scratch_sector_number% --decimal -e (((val1+1)/val2)+1) -cond val2 -c M %data_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --layout  %preprocess_bl2_file% -m RE_ENCRYPTION -n "Encryption key" -t File -c -E -h 1 -d "../Keys/OEMiRoT_Encryption.pem" --vb %code_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --layout  %preprocess_bl2_file% -m RE_ENCRYPTION -n "Encryption key" -t File -c -E -h 1 -d "../Keys/OEMiRoT_Encryption.pem" --vb %data_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

::xml used for init image generation

copy %code_xml% %init_code_xml%
IF !errorlevel! NEQ 0 goto :error

copy %data_xml% %init_data_xml%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --option add -n "Clear Image" -t Data -c -c -h 1 -d "" %init_code_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --option add -n "Confirm" -t Data -c --confirm -h 1 -d "" %init_code_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval -v %init_app_bin% --string -n %fw_out_bin% %init_code_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --option add -n "Clear Image" -t Data -c -c -h 1 -d "" %init_data_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlparam --option add -n "Confirm" -t Data -c --confirm -h 1 -d "" %init_data_xml% >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

set "command=%python%%applicfg% xmlval -v %init_data_bin% --string -n %fw_out_bin% %init_data_xml% --vb >> %current_log_file% 2>&1"
%command%
IF !errorlevel! NEQ 0 goto :error

exit 0

:error
echo.
echo =====
echo ===== Error occurred.
echo ===== See %current_log_file% for details. Then try again.
echo =====
exit 1

