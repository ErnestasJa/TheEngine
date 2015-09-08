@echo off
rmdir /s /q "build"
rmdir /s /q "libs/win32debug"
mkdir "build"
mkdir "libs/win32debug"
cd "build"
call vcvars32.bat
cmake ../ -DCMAKE_BUILD_TYPE=DEBUG -G "NMake Makefiles"
nmake
cd "../libs/boost/"
start /wait b2 -j8 --without-python --build-dir="../../build" toolset=msvc link=static threading=multi debug
cd "../../build/"
for /r %%f in (*.lib) do @xcopy /yq "%%f" "../libs/win32debug"
cd "../"