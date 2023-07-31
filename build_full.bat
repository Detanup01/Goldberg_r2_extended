del /Q /S full_nodebug\*
rmdir /S /Q full_nodebug
mkdir full_nodebug
call build_env_x86.bat
cl /std:c++17 /LD /DEMU_RELEASE_BUILD /DNDEBUG emu.cpp User32.lib Shell32.lib /EHsc /MP12 /Ox /link /OUT:full_nodebug\upc_r2_loader.dll
echo.
call build_env_x64.bat
cl /std:c++17 /LD /DEMU_RELEASE_BUILD /DNDEBUG emu.cpp User32.lib Shell32.lib /EHsc /MP12 /Ox /link /OUT:full_nodebug\uplay_r2_loader64.dll
echo.
pause
echo.