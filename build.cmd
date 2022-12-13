@echo off
echo cmake build vs2022 x64

set builderDir=%~dp0

if exist %builderDir%\CMakeCache.txt rm %builderDir%\CMakeCache.txt

:: -S: 指定源码所在路径
:: -B: 指定生成项目所在路径
:: -G: 使用哪个SDK生成器
:: -A: 指定平台
:: -D: 传递参数给CMakeLists.txt
:: CMAKE_CONFIGURATION_TYPES: 指定Release or Debug
cmake -S %builderDir% -B %builderDir% -G "Visual Studio 16 2019"  -A x64 -DPLATFORM=Windows -DCMAKE_CONFIGURATION_TYPES=Debug

pause
