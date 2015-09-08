rmdir /s /q "build"
rmdir /s /q "libs/win32release"
mkdir "build"
mkdir "libs/win32release"
cd "build"
call vcvars32.bat
cmake ../ -DCMAKE_BUILD_TYPE=RELWITHDEBINFO -G "NMake Makefiles"
nmake
cd "../libs/boost/"
start /wait b2 -j8 --without-python --build-dir="../../build"  --toolset=msvc architecture=x86 link=static threading=multi release inlining=off debug-symbols=on
cd "../../build/"
for /r %%f in (*.lib) do @xcopy /yq "%%f" "../libs/win32release"
cd "../"