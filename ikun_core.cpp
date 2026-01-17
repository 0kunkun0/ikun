// ikun库主程序, 用于管理库的功能

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

/*
库结构:
{workspace floder}/ 
项目文件...
/ikun
    标准库:
    all_libs.hpp // 包含所有库和命名空间
    stdc++lib.hpp // 包含所有C++标准库
    core.hpp // 核心库, 包含版本信息
    ikun_core.cpp // 主程序的原代码
    build.py // 编译脚本
    high_precision_digit.hpp // 高精度数字库
    functions.hpp // 函数库
    github.hpp // GitHub相关操作
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
    #elif defined(CLANG)
        #error "请在编译时显式指定-std=c++23(clang-cl使用/std:c++23)"
    #elif defined(GCC)
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
    println("版本: {}", IKUN_VERSION);
    println("编译时使用的编译器: {}", compiler_status);
    println("操作系统: {}", os_status);
    println("编程语言: 编译时使用{}", IKUN_LANGUAGE_PLATFORM);
    println("- 语言扩展: 编译时使用的C++标准: {}", __cplusplus);
}

void help()
{
    println("使用方法: ikun [选项] [参数]");
    println("使用本库时请保证电脑有一个可正常连接GitHub的网络和Git");
    println("选项:");
    println("  -v, --version: 显示版本信息");
    println("  -h, --help: 显示帮助信息");
    println("  -l, --list: 列出所有可用的库");
    println("  -i, --install: 安装指定的库");
    println("  -ih, --install-header: 安装指定的头文件");
    println("  -u, --uninstall: 卸载指定的库/头文件");
    println("  -c, --check: 检查指定的库是否已安装");
    println("参数:");
    println("  库名: 指定要安装/卸载/检查的库的名称");
}

void list_libs()
{
    // 访问作者GitHub
    system("start https://github.com/0kunkun0/ikun");
}

void install_header(string lib_name) // 从GitHub上下载单个头文件
{
    println("从GitHub上下载库");
    println("库名: {}", lib_name);
    println("请保证库名正确");
    bool a = downloadFileFromGitHub
    (
        "https://github.com/0kunkun0/ikun.git",
        lib_name + ".hpp",
        "main",
        "./"
    );
    if (a)
    {
        println("下载失败");
        return;
    }
}

void install_lib(string lib_name) // 从GitHub上下载单个库
{
    println("从GitHub上下载库");
    println("库名: {}", lib_name);
    println("请保证库名正确");
    bool a = downloadFolderFromGitHub
    (
        "https://github.com/0kunkun0/ikun.git",
        lib_name,
        "main",
        "./",
        true
    );
    bool b = downloadFileFromGitHub
    (
        "https://github.com/0kunkun0/ikun.git",
        lib_name + ".hpp",
        "main",
        "./"
    );
    if (a || b)
    {
        println("下载失败");
        return;
    }
}

void uninstall_lib(string lib_name)
{
    println("卸载库");
    if ((lib_name == "core") || (lib_name == "functions")
    || (lib_name == "all_libs") || (lib_name == "stdc++lib")
    || (lib_name == "high_precision_digit") || (lib_name == "github")
    || (lib_name == "github") || (lib_name == "test_high_precision_digit"))// 防误删逻辑
    {
        println("关键错误: 无法卸载核心库");
        return;
    }

    println("库名: {}", lib_name);
    println("请保证库名正确, 否则可能误删文件, 按下回车键继续");
    cin.ignore();
    string a = "del /S /F /Q " + lib_name + "\\";
    string b = "del /S /F /Q " + lib_name + ".hpp";
    string c = "rmdir /S /Q " + lib_name;
    system(a.c_str());
    system(b.c_str());
    system(c.c_str());
}

bool check_lib_install(string lib_name)
{
    if ((lib_name == "core") || (lib_name == "functions")
    || (lib_name == "all_libs") || (lib_name == "stdc++lib")
    || (lib_name == "high_precision_digit") || (lib_name == "github")
    || (lib_name == "github") || (lib_name == "test_high_precision_digit")) // 防止额外开销
    {
        return true;
    }
    return fileexists(lib_name + ".hpp");
}

int main(int argc, char* argv[])  // 修复：应该是 char* argv[]，而不是 char* *argv[]
{
    println("ikun库核心管理器");
    if (argc < 2)
    {
        println("关键错误: 未传递选项");
        help();
        return 1;
    }

    for (int i = 1; i < argc; i ++)
    {
        string arg = argv[i];
        
        if (arg == "-v" || arg == "--version")
        {
            version();
            return 0;
        }
        else if (arg == "-h" || arg == "--help")
        {
            help();
            return 0;
        }
        else if (arg == "-l" || arg == "--list")
        {
            list_libs();
            return 0;
        }
        else if (arg == "-ih" || arg == "--install-header")
        {
            if (i + 1 < argc)  // 检查是否有足够的参数
            {
                install_header(argv[i + 1]);
                i ++;  // 跳过下一个参数(库名)
            }
            else
            {
                println("错误: 请指定要安装的头文件名");
                return 1;
            }
        }
        else if (arg == "-i" || arg == "--install")
        {
            if (i + 1 < argc)
            {
                install_lib(argv[i + 1]);
                i ++;
            }
            else
            {
                println("错误: 请指定要安装的库名");
                return 1;
            }
        }
        else if (arg == "-u" || arg == "--uninstall")
        {
            if (i + 1 < argc)
            {
                uninstall_lib(argv[i + 1]);
                i ++;
            }
            else
            {
                println("错误: 请指定要卸载的库名");
                return 1;
            }
        }
        else if (arg == "-c" || arg == "--check")
        {
            if (i + 1 < argc)
            {
                if (check_lib_install(argv[i + 1]))
                {
                    println("{} 库已安装", argv[i + 1]);
                }
                else
                {
                    println("{} 库未安装或不存在", argv[i + 1]);
                }
                i ++;
            }
            else
            {
                println("错误: 请指定要检查的库名");
                return 1;
            }
        }
        else
        {
            println("未知选项: {}", arg);
            help();
            return 1;
        }
    }

    return 0;
}