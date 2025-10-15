@echo off
echo build_before_special bat
bash.exe ..\..\..\..\..\tools\version_generation\git_generate_version_sdk_lib.sh bee4-sdk- ..\..\..\..\..\tools\version_generation\ ..\..\..\inc_int\lib_version.h

:: modify for version management
git checkout -- ..\sdk_lib_version.h