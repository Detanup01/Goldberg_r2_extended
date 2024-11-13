del /Q /S debug64\*
rmdir /S /Q debug64
mkdir debug64
call build_env_x64.bat
cl /std:c++20 /LD /DNDEBUG emu.cpp upc.h User32.lib Shell32.lib Ole32.lib /EHsc /MP12 /Ox /link /OUT:debug64\upc_r2_loader64.dll
echo Built
pause
echo Done