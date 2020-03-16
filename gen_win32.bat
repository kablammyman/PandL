if not exist "build_win32" mkdir build_win32
if not exist "x64\Debug" mkdir x64\Debug
if not exist "x64\Release" mkdir x64\Release

cd build_win32

cmake -G "Visual Studio 16 2019" ..

cd ..