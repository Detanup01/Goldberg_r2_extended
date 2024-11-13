del /Q /S debug86\*
rmdir /S /Q debug86
mkdir debug86
call build_env_x86.bat
cl /std:c++20 /LD /DNDEBUG emu.cpp User32.lib Shell32.lib Ole32.lib /EHsc /MP12 /Ox /link /OUT:debug86\upc_r2_loader.dll
echo Built
pause
echo Done