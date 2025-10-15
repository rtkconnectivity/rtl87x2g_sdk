@echo off
echo after_build_special bat
1>NUL del  ..\..\..\lib\rtl87x2g\mdk\mbedtls.lib
1>NUL copy .\objects\mbedtls.lib    ..\..\..\lib\rtl87x2g\mdk\
