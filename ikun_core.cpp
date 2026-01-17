// ikun库主程序, 用于管理库的功能
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据许可证和用户协议规定, 禁止商业用途, 违者依法追究法律责任

/*
库结构:
{workspace floder}/ 
项目文件...
/ikun
    all_libs.hpp // 包含所有库和命名空间
    stdc++lib.hpp // 包含所有C++标准库
    core.hpp // 核心库, 包含版本信息
    ikun_core.cpp // 主程序的原代码
    high_precision_digit.hpp // 高精度数字库
    functions.hpp // 函数库
    /functions
        libs.hpp // 工具库, 命名空间libs
        mathematics.hpp // 数学库, 命名空间maths
        random.hpp // 随机库, 命名空间random
        times.hpp // 时间库, 命名空间times
*/

#ifdef _MSC_VER // 判断编译器
    #define MSVC
#elifdef __clang__
    #define CLANG
#elifdef __GNUC__
    #define GCC
#endif

#if __cplusplus < IKUN_CPP_VERSION_REQUIRED
    #ifdef MSVC
        #error "请在编译时显式指定/std:c++23或在项目配置中指定C++23标准"
    #elifdef CLANG
        #error "请在编译时显式指定-std=c++23(clang-cl使用/std:c++23)"
    #elifdef GCC
        #error "请在编译时显式指定-std=c++23"
    #else
        #error "不支持的编译器"
    #endif
#endif

#include "all_libs.hpp"

#ifndef IKUN_VERSION
    #define IKUN_VERSION "Unsupported"
#endif

string compiler_status = 
#ifdef MSVC
"Microsoft Visual C++"
#elifdef CLANG
"LLVM Clang"
#elifdef GCC
"GNU C Compiler"
#else
"Unknown"
#endif
;

string os_status = 
#ifdef _WIN64
"Windows"
#elifdef __linux__
"Linux"
#elifdef __APPLE__
"Apple macOS"
#else
"Unsupported"
#endif
;

void version()
{
    println("ikun库核心管理器");
    println("版本: {}", IKUN_VERSION);
    println("编译器: {}", compiler_status);
    println("操作系统: {}", os_status);
    println("编程语言: 当前版本库适用于{}", IKUN_LANGUAGE_PLATFORM);
    println("编译时使用的C++标准: {}", __cplusplus);
}

void help()
{
    println("ikun库核心管理器");
    println("使用方法: ikun [选项] [参数]");
    println("选项:");
    println("  -v, --version: 显示版本信息");
    println("  -h, --help: 显示帮助信息");
    println("  -l, --list: 列出所有可用的库");
    println("  -i, --install: 安装指定的库");
    println("  -u, --uninstall: 卸载指定的库");
    println("  -c, --check: 检查指定的库是否已安装");
    println("参数:");
    println("  库名: 指定要安装/卸载/检查的库的名称");
}

void list()
{
    println("ikun库核心管理器");
    println("列出所有可用的库");
    system("dir /b /a:d /s");
}

int main(string argv[])
{
}