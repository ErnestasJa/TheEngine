@echo off
cd ../../
rmdir /s /q "build"
rmdir /s /q "libs/win64release"
mkdir "build"
mkdir "libs/win64release"
cd "build"
call ../build_tools/windows/vcvars64.bat
cmake ../libs/ -DCMAKE_BUILD_TYPE=RELEASE -G "NMake Makefiles"
nmake
cd "../libs/boost/"
start /wait bootstrap.bat
start /wait b2 -j8 --without-python --build-dir="../../build"  --toolset=msvc architecture=x86 address-model=64  link=static threading=multi release
cd "../../build/"
for /r %%f in (*.lib) do @xcopy /yq "%%f" "../libs/win64release"
cd "../"
rmdir /s /q "build"