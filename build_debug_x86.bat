del /Q /S release86\*
rmdir /S /Q release86
mkdir release86
call build_env_x86.bat
cl /std:c++17 /LD /DNDEBUG emu.cpp User32.lib Shell32.lib /EHsc /MP12 /Ox /link /OUT:release86\upc_r2_loader.dll
echo Built
pause
echo Done