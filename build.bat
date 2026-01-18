@echo off
chcp 65001 > nul

echo ikun库构建脚本, 基于Windows Batch实现
echo 请确保已安装Python 3.8+, GCC或Clang编译器以及Git, 并已配置好环境变量
echo.
pause

echo 正在查找运行依赖项...
where python
where g++
where clang++
where git

echo 正在检查版本...
echo Python需要>=3.8, GCC需要>=11, Clang需要>=14, 且有一个可以连接GitHub的网络环境
echo.

python.exe --version
g++.exe --version
clang++.exe --version
pause

echo 正在检查build_lib.py是否存在于当前目录...
if exist ".\build_lib.py" (
    python.exe .\build_lib.py
) else (
    echo build_lib.py不存在, 尝试从GitHub下载...
    curl -L -o "build_lib.py" "https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/build.py"
    python.exe .\build_lib.py
)