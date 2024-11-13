del /Q /S release64\*
rmdir /S /Q release64
mkdir release64
call build_env_x64.bat
cl /std:c++20 /LD /DEMU_RELEASE_BUILD /DNDEBUG emu.cpp upc.h User32.lib Shell32.lib Ole32.lib /EHsc /MP12 /Ox /link /OUT:release64\upc_r2_loader64.dll
echo Built
pause
echo Done