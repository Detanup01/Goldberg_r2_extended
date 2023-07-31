del /Q /S debug_full\*
rmdir /S /Q full
mkdir full
call build_env_x86.bat
cl /std:c++17 /LD /DNDEBUG emu.cpp User32.lib Shell32.lib /EHsc /MP12 /Ox /link /OUT:debug_full\upc_r2_loader.dll
echo.
call build_env_x64.bat
cl /std:c++17 /LD /DNDEBUG emu.cpp User32.lib Shell32.lib /EHsc /MP12 /Ox /link /OUT:debug_full\upc_r2_loader64.dll
echo Built
pause
echo Done