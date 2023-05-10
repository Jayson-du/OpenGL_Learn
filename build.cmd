@echo off
echo cmake build vs2022 x64

if not exist build (
   mkdir build
) 

cd ./build

if exist CMakeCache.txt (
    echo "ɾ������CMakeCache.txt"
    rm CMakeCache.txt
) 

set builderDir=%~dp0

if exist %builderDir%\CMakeCache.txt rm %builderDir%\CMakeCache.txt

:: -S: ָ��Դ������·��
:: -B: ָ��������Ŀ����·��
:: -G: ʹ���ĸ�SDK������
:: -A: ָ��ƽ̨
:: -D: ���ݲ�����CMakeLists.txt
:: CMAKE_CONFIGURATION_TYPES: ָ��Release or Debug
cmake -S %builderDir% -B %builderDir%\build -G "Visual Studio 16 2019"  -A x64 -DPLATFORM=Windows -DCMAKE_CONFIGURATION_TYPES=Debug

pause
