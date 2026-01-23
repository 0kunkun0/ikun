@echo off
chcp 65001 > nul

echo ikun库构建脚本, 基于Windows Batch实现
echo 请确保已安装Python 3.10+, 可用的C++编译器以及Git, 并已配置好环境变量
echo.
pause

cls
echo 正在查找运行依赖项...
echo Python:
where python
echo.

echo GCC:
where g++
echo.

echo Clang:
where clang++

echo MSVC:
where cl.exe
echo.

echo Git:
where git
pause
cls

echo 正在检查版本...
echo Python需要3.8+, GCC需要11+, Clang需要14+, MSVC需要19.30+, 且有一个可以连接GitHub的网络环境
echo 由于本库大量使用C++23 print, 故
echo 请保证使用的标准库, 如GNU libstdc++, LLVM libc++, MSVC STL支持C++23
echo 注意: 如果使用MSVC, 请在VS开发者命令提示符中运行此脚本
pause

python.exe --version
g++.exe --version
clang++.exe --version
cl.exe
pause

echo 正在检查build_lib.py是否存在于当前目录...
if exist ".\build_lib.py" (
    echo.
) else (
    echo build_lib.py不存在, 尝试从GitHub下载...
    curl -L -o "build_lib.py" "https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/build_lib.py"
)

python .\build_lib.py