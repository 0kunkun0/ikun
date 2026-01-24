#!/bin/bash
# ikun库构建脚本Linux/macOS版
# 对应 Windows 端的build.bat

# 本库开源GitHub地址: https://github.com/0kunkun0/ikun
# 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
# 仅供个人, 非营利性组织, 开源项目以及竞赛使用
# 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

# 设置 UTF-8 编码
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8

echo "ikun库构建脚本, 基于 Bash 实现"
echo "请确保已安装 Python 3.10+, 可用的 C++ 编译器以及 Git, 并已配置好环境变量"
echo ""
read -p "按任意键继续..." -n1 -s
echo -e "\n"

clear
echo "正在查找运行依赖项..."
echo "Python:"
which python3 || which python || echo "未找到 Python"
echo ""

echo "GCC:"
which g++ || echo "未找到 GCC"
echo ""

echo "Clang:"
which clang++ || echo "未找到 Clang"
echo ""

echo "Git:"
which git || echo "未找到 Git"
read -p "按回车键继续..." -n1 -s
echo -e "\n"

clear
echo "正在检查版本..."
echo "Python需要3.8+, GCC需要11+, Clang需要14+, 且有一个可以连接GitHub的网络环境"
echo "由于本库大量使用C++23 print库, 故"
echo "请保证使用的标准库, 如GNU libstdc++, LLVM libc++ 支持 C++23"
echo "如果使用 macOS, 请确保 Xcode Command Line Tools 已安装"
echo ""
read -p "按任意键继续..." -n1 -s
echo -e "\n"

echo "版本信息:"
echo "Python:"
python3 --version 2>/dev/null || python --version 2>/dev/null || echo "Python 未安装"
echo ""

echo "GCC:"
g++ --version 2>/dev/null | head -1 || echo "GCC 未安装"
echo ""

echo "Clang:"
clang++ --version 2>/dev/null | head -1 || echo "Clang 未安装"
echo ""

read -p "按任意键继续..." -n1 -s
echo -e "\n"

echo "正在检查 build_lib.py 是否存在于当前目录..."
if [ -f "./build_lib.py" ]; then
    echo "build_lib.py 已存在"
    echo ""
else
    echo "build_lib.py 不存在, 尝试从 GitHub 下载..."
    
    # 尝试不同的下载方式
    if command -v curl &> /dev/null; then
        curl -L -o "build_lib.py" "https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/build_lib.py"
    elif command -v wget &> /dev/null; then
        wget -O "build_lib.py" "https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/build_lib.py"
    else
        echo "错误: 需要 curl 或 wget 来下载文件"
        echo "请手动下载: https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/build_lib.py"
        exit 1
    fi
    
    if [ ! -f "./build_lib.py" ]; then
        echo "下载失败，请检查网络连接"
        exit 1
    fi
    echo "下载完成"
fi

# 执行 Python 脚本
echo "正在执行 build_lib.py..."
echo ""

# 优先使用 python3
if command -v python3 &> /dev/null; then
    python3 ./build_lib.py
elif command -v python &> /dev/null; then
    python ./build_lib.py
else
    echo "错误: 未找到 Python 解释器"
    exit 1
fi

# 添加执行权限
if [ -f "./build_lib.py" ]; then
    chmod +x ./build_lib.py 2>/dev/null
fi

echo ""
read -p "构建完成，按回车键退出..." -n1 -s
echo -e "\n"