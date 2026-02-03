//Dependencies: ikun_stderr times
// ikun库主程序, 用于管理库的功能
// 操作系统要求: Windows 10 x64, macOS Catalina 10.15+, Linux内核5.10+
// 终端要求(ANSI转义序列): 
//     Windows: Windows Terminal/PowerShell 5.1+, cmd.exe默认不支持
//     macOS: 所有
//     Linux: 所有
// 版本: 7.1.2 Release Candidate
// C++版本要求: C++23或更高版本, 否则将无法使用print和#elifdef
// 编译器选项:
//     MSVC: 
//     /std:c++23preview(将来可能变为/std:c++23)或/std:c++latest
//     GCC/Clang:
//     -std=c++23

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

/*
库结构:
{workspace floder}/ 
项目文件...
/ikun
    自带内容:
    ikun_core.cpp   // 主程序的原代码
    ikun_stderr.hpp // ikun错误库
    build.bat       // Windows下的编译脚本
    build.sh        // Linux/macOS下的编译脚本
    build_lib.py    // 管理脚本
    
    别的内容请访问 https://github.com/0kunkun0/ikun
    或者查看库中的 发行说明.md
*/

#ifdef _MSC_VER
#define _AMD64_
#endif

#define IKUN_VERSION "7.1.2 Release Build 2026.2"
#define IKUN_CPP_VERSION_REQUIRED 202302L
#define IKUN_LANGUAGE_PLATFORM "C++"

#ifdef _MSC_VER // 判断编译器
    #define MSVC
#elifdef __clang__
    #define CLANG
#elifdef __GNUC__
    #define GCC
#endif

#ifdef MSVC // 处理MSVC上__cplusplus宏的问题
    #define CPPVER _MSVC_LANG // MSVC上需要用_MSVC_LANG宏来代替
#else
    #define CPPVER __cplusplus // 其他编译器上直接使用__cplusplus宏
#endif

#if CPPVER < IKUN_CPP_VERSION_REQUIRED // 判断C++版本
    #error 此版本的ikun库需要C++23或更高版本
    #error 请在编译时指定最低-std=c++23或/std:c++23
    #error 并在IDE的IntelliSense引擎中选择C++23或更高版本
#endif

#ifdef _WIN32 // 简化Windows API的调用
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #define NOGDICAPMASKS
    #define NOSYSMETRICS
    #define NOMENUS
    #define NOICONS
    #define NOATOM
#endif

#ifdef _WIN32
#define OUTPUT_TO_NUL " > nul 2>&1"
#else
#define OUTPUT_TO_NUL " > /dev/null 2>&1"
#endif

#include <print>
#include <format>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <string_view>
#include <algorithm>
#include "times.hpp"
#include "ikun_stderr.hpp"

#ifdef _WIN32
#include <processthreadsapi.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif

using namespace std;
using namespace ikun_error;
namespace fs = std::filesystem;

namespace core_color
{
    constexpr const char* reset = "\033[0m";
    constexpr const char* red = "\033[31m";
    constexpr const char* green = "\033[32m";
    constexpr const char* blue = "\033[34m";
    constexpr const char* bold = "\033[1m";
}

string compiler_status = // 编译器信息
#ifdef MSVC
"Microsoft Visual C++ Compiler"
#elifdef CLANG
"Clang/LLVM Compiler"
#elifdef GCC
"GNU Compiler Collection"
#else
"Unknown Compiler"
#endif
;

string os_status = // 操作系统信息
#ifdef _WIN64
"Windows x64"
#elifdef _WIN32
"Windows x86"
#elif defined(__linux__) && defined(__x86_64__)
"Linux x86_64"
#elif defined(__linux__) && defined(__i386__)
"Linux x86"
#elif defined(__APPLE__) && defined(__x86_64__)
"Apple macOS x86_64";compiler_status = "Apple Clang/LLVM Compiler"
#elif defined(__APPLE__) && defined(__arm64__)
"Apple macOS ARM64E";compiler_status = "Apple Clang/LLVM Compiler"
#else
"Unsupported OS or Architecture"
#endif
;

string tips = format(
#ifdef _WIN64
"{}请保证使用的终端为Windows Terminal和PowerShell 5.1+, cmd.exe可能无法显示ANSI转义序列(即带颜色的字符){}"
#elifdef __linux__
"{}请注意: ikun库在Linux上的支持为实验性, 本库在Linux下的测试均在WSL环境下测试, 可能会有各种bug{}"
#elifdef __APPLE__
"{}请注意: ikun库在macOS上的支持不完整, 操作系统API相关特性均使用POSIX兼容性实现, 某些极端情况可能无法正常使用{}"
#endif
, core_color::bold, core_color::reset);

namespace ikun_core_cpp
{
    int core_get_cppversion() // 获取C++版本
    {
        int cppversion = (CPPVER / 100) % 100;
        int temp = cppversion % 3;

        if (temp != 2)
        {
            if (temp == 1) cppversion += 1;
            if (temp == 0) cppversion += 2;
        }

        return cppversion;
    }

    auto getparentdir()
    {
        return fs::path(__FILE__).parent_path().string();
    }

    auto getpid_()
    {
        #ifdef _WIN32
        return GetCurrentProcessId();
        #else
        return getpid();
        #endif
    }

    vector<string_view> split(string_view s, string_view delimiter)
    {
        vector<string_view> tokens;
        size_t start = 0, end = 0;
        
        while ((end = s.find(delimiter, start)) != string_view::npos)
        {
            tokens.push_back(s.substr(start, end - start));
            start = end + delimiter.length();
        }
        
        tokens.push_back(s.substr(start));
        return tokens;
    }


    bool downloadFileFromGitHub
    (
        const string& repoUrl,
        const string& filePath,
        const string& branch = "main",
        const string& outputDir = "."
    )
    {
        // 创建临时目录
        string tempDir = fs::temp_directory_path().string() + "/github_download_" + to_string(getpid_());
        
        try
        {
            if (fs::exists(tempDir))
            {
                fs::remove_all(tempDir);
            }
            fs::create_directory(tempDir);
        }
        catch (const fs::filesystem_error& e)
        {
            println(cerr, "错误：无法创建临时目录: {}", e.what());
            return true;
        }

        // 切换到临时目录
        string oD = fs::current_path().string();
        
        try
        {
            fs::current_path(tempDir);
        }
        catch (const fs::filesystem_error& e)
        {
            println(cerr, "错误：无法切换到临时目录: {}", e.what());
            return true;
        }

        bool success = false;
        
        try
        {
            // 初始化
            string cmd = "git init";
            println("执行: {}", cmd);
            int rst = system(cmd.c_str());
            if (rst != 0)
            {
                throw_re("git init failed",
                "ikun_core.cpp", "downloadFileFromGitHub()", "core_error 001"
                );
            }

            // 添加远程仓库
            cmd = "git remote add origin \"" + repoUrl + "\"";
            println("执行: {}", cmd);
            rst = system(cmd.c_str());
            if (rst != 0)
            {
                throw_re("git remote add failed",
                "ikun_core.cpp", "downloadFileFromGitHub()", "core_error 002"
                );
            }

            // 启用稀疏检出
            cmd = "git config core.sparseCheckout true";
            println("执行: {}", cmd);
            rst = system(cmd.c_str());
            if (rst != 0)
            {
                throw_re("git config failed",
                "ikun_core.cpp", "downloadFileFromGitHub()", "core_error 003"
                );
            }

            // 指定要下载的文件路径
            string sCF = ".git/info/sparse-checkout";
            fs::path sCP(sCF);
            
            // 确保目录存在
            fs::create_directories(sCP.parent_path());
            
            // 写入要下载的文件路径
            {
                ofstream file(sCP);
                if (!file.is_open())
                {
                    println(cerr, "错误：无法创建稀疏检出配置文件");
                    throw_re("无法创建稀疏检出文件",
                    "ikun_core.cpp", "downloadFileFromGitHub()", "core_error 004"
                );
                }
                file << filePath << endl;
            }

            // 拉取文件
            cmd = "git pull origin " + branch;
            println("执行: {}", cmd);
            rst = system(cmd.c_str());
            if (rst != 0)
            {
                throw_re("git pull failed",
                "ikun_core.cpp", "downloadFileFromGitHub()", "core_error 005"
            );
            }

            // 构建源文件路径
            fs::path sF = fs::path(filePath).filename();
            fs::path oP = fs::path(outputDir) / sF;

            // 确保输出目录存在
            fs::create_directories(oP.parent_path());

            println("复制文件从 {} 到 {}", sF.string(), oP.string());
            #ifdef _WIN32
            string operation = "copy " + sF.string() + " " + oP.string();
            #else
            string operation = "cp " + sF.string() + " " + oP.string();
            #endif
            system(operation.c_str());
            println("文件下载成功: {}", oP.string());
            success = true;


        }
        catch (const exception& e)
        {
            println(cerr, "下载过程中出错: {}", e.what());
            success = true;
        }

        // 恢复原始目录
        try
        {
            fs::current_path(oD);
        }
        catch (...)
        {
            // 忽略恢复目录时的错误
        }

        return !success;
    }

    vector<string> core_filedir(string path = ".", string fileextname = ".")
    {
        vector<string> files;
        try
        {
            // 遍历目录
            for (const auto &entry : filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    string filename = entry.path().filename().string();
                    
                    // 根据 fileextname 参数进行过滤
                    if (fileextname == ".") // 默认值，匹配所有文件
                    {
                        files.push_back(filename);
                    }
                    else if (fileextname.empty()) // 空字符串也匹配所有文件
                    {
                        files.push_back(filename);
                    }
                    else
                    {
                        // 检查文件扩展名
                        string ext = entry.path().extension().string();
                        if (!ext.empty() && 
                            equal(ext.begin(), ext.end(), 
                                    fileextname.begin(), fileextname.end(),
                                    [](char a, char b)
                                    {
                                        return tolower(a) == tolower(b);
                                    }))
                        {
                            files.push_back(filename);
                        }
                    }
                }
            }
            sort(files.begin(), files.end());
        }
        catch (const filesystem::filesystem_error& e)
        {
            println(cerr, "文件系统错误: {}", e.what());
        }
        catch (const exception& e)
        {
            println(cerr, "错误: {}", e.what());
        }

        return files;
    }
        
    bool core_fileexists(const string& filename) // 检查文件是否存在
    {
        return filesystem::exists(filename); 
    }

    void version()
    {
        int cppversion = core_get_cppversion();

        println("编译时使用的编译器: {}", compiler_status);
        println("操作系统: {}", os_status);
        println("编程语言: 编译时使用{}", IKUN_LANGUAGE_PLATFORM);
        println("- 语言扩展: 编译时使用的C++标准: C++{}", cppversion);
        println("提示: 上面的选项只要有一个是Unknown本库就无法正常使用");
    }

    void help()
    {
        println("使用方法: ikun [选项] [参数]");
        println("使用本库时请保证电脑有一个可正常连接GitHub的网络和Git");
        println("选项:");
        println("  -v, --version:               显示版本信息");
        println("  -h, --help:                  显示帮助信息");
        println("  -l, --list:                  列出所有可用的模块");
        println("  -ll, --list-libs:            列出所有安装了的模块");
        println("  -i, --install:               安装指定的模块");
        println("  -ip, --install-preview:      安装指定模块的预览版");
        println("  -ifl, --install-from-local:  从本地ikun库安装副本安装模块(此方法适用于基于ikun库扩展的模块)");
        println("  -u, --uninstall:             卸载指定的模块");
        println("  -c, --check:                 检查指定的模块是否已安装");
        println("  -cp, --create-project:       创建基于ikun库开发的C++项目");
        println("  --compile                    编译C++源文件");
        println("参数:");
        println("  (-i, -u, -c, --install-from-local) 库名: 指定要安装/卸载/检查的库的名称");
        println("  (--install-from-local) 本地库路径: 指定要安装的本地库的相对/绝对路径\n    (以/作为分隔符, 如D:/workspace/dev/ikun/ikun_core.hpp");
        println("    备注: -ifl选项格式为 ikun -ifl [库名] [本地库路径]");
    }

    void core_list_libs()
    {
        // 访问作者GitHub
        #ifdef _WIN32
        system("start https://github.com/0kunkun0/ikun");
        #else
        system("xdg-open https://github.com/0kunkun0/ikun");
        #endif
    }

    void list_installed_libs()
    {
        println("已安装的库:");
        vector<string> libs = core_filedir("./", ".hpp");
        for (const auto &lib : libs)
        {
            println("  {}", lib);
        }
    }

    void install(string lib_name, bool ispreview = false) // 从GitHub上下载库
    {
        println("从GitHub上下载库");
        println("库名: {}", lib_name);
        println("请保证库名正确");

        ifstream file("libs_download_url.txt");
        string url;
        getline(file, url);
        file.close();

        string branch;

        if (ispreview)
        {
            branch = "preview";
        }
        else
        {
            branch = "main";
        }

        bool a = downloadFileFromGitHub
        (
            url,
            lib_name + ".hpp",
            branch,
            "./"
        );
        if (a)
        {
            println("下载失败");
            return;
        }
    }

    void uninstall(string lib_name)
    {
        println("卸载库");
        if ((lib_name == "ikun_core") || (lib_name == "build") || (lib_name == "build_lib") || (lib_name == "ikun_stderr"))// 防误删逻辑
        {
            println("{}关键错误: 无法卸载核心库{}", core_color::red, core_color::reset);
            return;
        }

        println("库名: {}", lib_name);
        println("{}请保证库名正确, 否则可能误删文件", core_color::red);
        println("删除时不会经过回收站, 按下回车键继续{}", core_color::reset);
        cin.ignore();
        #ifdef _WIN32
        string a = "del /S /F /Q " + lib_name + ".hpp";
        #else
        string a = "rm -rf " + lib_name + ".hpp";
        #endif
        system(a.c_str());
    }

    bool check_install(string lib_name)
    {
        return core_fileexists(lib_name + ".hpp");
    }
    
    void install_from_local(string lib_name, string path)
    {
        println("从本地库安装副本");
        // 检查路径是否正确
        if (!core_fileexists(path))
        {
            println("{}错误: 路径不存在{}", core_color::red, core_color::reset);
            return;
        }
        // 检查文件是否存在
        if (!core_fileexists(path + "/" + lib_name + ".hpp"))
        {
            println("{}错误: 文件不存在{}", core_color::red, core_color::reset);
            return;
        }
        #ifdef _WIN32
            string a = "copy " + path + " " + lib_name + ".hpp .\\";
        #elif defined(__linux__) || defined(__APPLE__)
            string a = "cp " + path + "/" + lib_name + ".hpp ./";
        #endif
        system(a.c_str());
        if (!core_fileexists(lib_name + ".hpp"))
        {
            println("{}错误: 安装失败{}", core_color::red, core_color::reset);
            return;
        }
    }

    bool core_check_file_name(string dir_name) // 检查文件名是否合法
    {
        if (dir_name.find_first_of("/\\:*?\"<>|") != string::npos)
        {
            return false;
        }

        return true;
    }

    bool core_check_dir_name(string dir_name) // 检查目录名是否合法
    {
        #ifdef _WIN32
        if (dir_name.find_first_of("*?\"<>|") != string::npos)
        {
            return false;
        }
        #elif defined(__linux__) || defined(__APPLE__)
        if (dir_name.find_first_of(":*?\"<>|") != string::npos)
        {
            return false;
        }
        #endif

        return true;
    }

    void create_project()
    {
        println("创建基于ikun库开发的C++项目");
        println("测试版功能, 实现暂不完善, 如出现bug, 请访问 https://github.com/0kunkun0/ikun 并按照README.md的指示进行提交反馈");
        println("注意: 作为当前版本在preview分支新增的功能, 该功能不受支持, 并可能在未来{}的Release版本发布前受到修改或移除", IKUN_VERSION);
        println("");

        print("请输入项目名称: ");
        string project_name;
        getline(cin, project_name);
        if (!core_check_file_name(project_name))
        {
            println("{}错误: 项目名称不合法{}", core_color::red, core_color::reset);
            return;
        }

        print("\n请输入项目路径: ");
        string project_path;
        getline(cin, project_path);
        println("");
        if (!core_check_dir_name(project_path))
        {
            println("{}错误: 项目路径不合法{}", core_color::red, core_color::reset);
            return;
        }

        string md = "mkdir " + project_path + "\\" + project_name;
        vector<string> dirs = {"\\src", "\\include", "\\build", "\\include\\ikun"};
        for (string dir : dirs)
        {
            string cmd = md + dir;
            println("\n执行操作{}", cmd);
            system(cmd.c_str());
        }

        #ifdef _WIN32
        println("如有提示询问是文件名还是目录名, 请选择目录名(D)");
        string cmd = "xcopy *.hpp " + project_path + "\\" + project_name + "\\include\\ikun/";
        string cmd1 = "xcopy *.cpp " + project_path + "\\" + project_name + "\\include\\ikun/";
        string cmd2 = "xcopy ikun.exe " + project_path + "\\" + project_name + "\\include\\ikun/";
        string cmd3 = "xcopy ikun_error_analyzer.exe" + project_path + "\\" + project_name + "\\include\\ikun/";
        #elif defined(__linux__) || defined(__APPLE__)
        string cmd = "cp *.hpp " + project_path + "/" + project_name + "/include/ikun/";
        string cmd1 = "cp *.cpp " + project_path + "/" + project_name + "/include/ikun/";
        string cmd2 = "cp ikun " + project_path + "/" + project_name + "/include/ikun/";
        string cmd3 = "cp ikun_error_analyzer " + project_path + "/" + project_name + "/include/ikun/";
        #else
        throw_re("不支持的平台",
        "ikun(.exe)", "create_project, in line" + to_string(__LINE__), "core_error 006"
        );
        #endif
        
        println("正在执行命令: {}", cmd);
        println("正在执行命令: {}", cmd1);
        println("正在执行命令: {}", cmd2);
        println("正在执行命令: {}", cmd3);

        system(cmd.c_str());
        system(cmd1.c_str());
        system(cmd2.c_str());
        system(cmd3.c_str());

        string main_cpp_content =
R"(#include "ikun/all_libs.hpp"
int main()
{
    println("Hello, World!");
    return 0;
})";

        println("正在创建main.cpp...");
        ofstream main_cpp(project_path + "\\" + project_name + "\\src\\main.cpp");
        main_cpp << main_cpp_content;
        main_cpp.close();
        println("创建完成, 详见{}", project_path + "\\" + project_name + "\\src\\main.cpp");

        string cmakelist_txt_content = format(
R"(cmake_minimum_required(VERSION 3.10)
project({})

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable({} main.cpp))", project_name, project_name); // 现代C++神力, 此处羡慕Python的f-string, Swift的\()字符串插值和C#的${}字符串插值
        ofstream cmakelist_txt(project_path + "\\" + project_name + "\\CMakeLists.txt");
        cmakelist_txt << cmakelist_txt_content;
        cmakelist_txt.close();
        println("创建CMakeLists.txt成功, 详见{}", project_path + "\\" + project_name + "\\CMakeLists.txt");

        println("{}项目创建成功{}", core_color::green, core_color::reset);
    }

    string get_time_()
    {
        string month = to_string(times::get_month());
        string day = to_string(times::get_day());
        string hour = to_string(times::get_hour());
        string minute = to_string(times::get_minute());
        string second = to_string(times::get_second());
        return format("{}月{}日 {}:{}:{}  ", month, day, hour, minute, second);
    }

    void compile() // 封装更快速的编译程序
    {
        print("请输入文件位置(不包含文件名):");
        string file_path;
        getline(cin, file_path);
        if (core_check_dir_name(file_path))
        {
            if (!core_fileexists(file_path))
            {
                println("{}错误: 文件不存在{}", core_color::red, core_color::reset);
                return;
            }
        }
        else
        {
            println("{}错误: 文件路径不合法{}", core_color::red, core_color::reset);
            return;
        }

        print("请输入文件名(可以为多个):");
        string file_name;
        getline(cin, file_name);
        if (!core_check_file_name(file_name))
        {
            println("{}错误: 文件名不合法{}", core_color::red, core_color::reset);
            return;
        }

        print("请输入编译器名(或者直接输入地址):");
        string compiler;
        getline(cin, compiler);

        if (system((compiler + OUTPUT_TO_NUL).c_str()))
        {
            println("\n{}错误: 编译器不存在{}", core_color::red, core_color::reset);
            #ifdef _WIN32
            if (compiler == "cl" || compiler == "cl.exe")
            {
                println("{}备注: 请打开VS本机工具命令提示符后重启程序{}", core_color::blue, core_color::reset);
            }
            #endif
            return;
        }

        print("\n\n请输入C++版本(如17, 20, 23, MSVC可用latest, 但某些情况需要加上preview):");
        string version;
        cin >> version;

        print("\n请输入是否静态编译:(1是0否)");
        bool static_compile;
        cin >> static_compile;

        print("\n请输入要链接的静态库:");
        string static_libs;
        getline(cin, static_libs);

        print("\n请输入include路径:");
        string include_path;
        getline(cin, include_path);

        print("\n请输入是否为Release模式:(1是0否,否为Debug模式)");
        bool release_mode;
        cin >> release_mode;

        print("\n请输入是否启用优化:(1是0否)");
        bool optimize;
        cin >> optimize;

        print("\n请输入输出文件类型(1:可执行文件, 2:动态链接库, 3:静态链接库):");
        int output_file_type;
        cin >> output_file_type;

        print("\n请输入编译后的文件名(必须包含扩展名, 可包含地址):");
        string output_file_name;
        getline(cin, output_file_name);

        bool use_llvm_libcpp = false;
        if (compiler == "clang++" || compiler == "clang++.exe")
        {
            print("是否使用LLVM的C++标准库? (1是0否, 0则默认使用GNU的libstdc++)");
            bool use_llvm_libcpp;
            cin >> use_llvm_libcpp;
            println("\n注: 指定此项后再指定静态链接在Windows上会出问题");
            println("按下回车键继续...");
            cin.ignore();
        }

        println("\n\n生成开始于{}", get_time_());
        println("可在当前目录的ikun_compile.log中查看编译细节");

        ofstream log_file("ikun_compile.log", ios::app);
        println(log_file, "\n{}添加任务:编译{}", get_time_(), file_name);
        println(log_file, "\n{}正在拼接编译命令...", get_time_());

        println(log_file, "{}正在拼接编译命令", get_time_());
        println(log_file, "{}正在创建cmd_compiler字符串", get_time_());
        string cmd_compiler = "\"" + compiler + "\""; // 加上引号防止路径中有空格
        println(log_file, "{}(variable)cmd_compiler: {}", get_time_(), cmd_compiler);
        
        println(log_file, "{}正在获取编译器可执行文件名", get_time_());
        {
            string_view compiler_name = split(split(split(compiler, "\\").back(), "/").back(), ".").front(); // 简单粗暴
            // 思路:                     ^^^^^取按文件扩展名分割后的第一个元素(取按斜杠分割的最后一个元素(取按反斜杠分割的最后一个元素))
            stringstream ss;
            ss << compiler_name;
            ss >> compiler;
        }
        println(log_file, "{}(variable)compiler_name: {}", get_time_(), compiler);

        println(log_file, "{}正在创建cmd_args字符串", get_time_());
        string cmd_args = "";
        if (compiler == "cl" || compiler == "clang-cl")
        {
            println(log_file, "{}识别为Clang(clang-cl)/MSVC编译器", get_time_());
            cmd_args += "/EHsc /Zc:__cplusplus /W3 ";
            println(log_file, "{}添加/EHsc /Zc:__cplusplus /W3 到编译命令", get_time_());
            if (static_compile)
            {
                println(log_file, "{}添加/MT到编译命令, static_compile为true", get_time_());
                cmd_args += "/MT";
            }
            else
            {
                println(log_file, "{}添加/MD到编译命令, static_compile为false", get_time_());
                cmd_args += "/MD";
            }

            if (release_mode)
            {
                println(log_file, "{}添加 /GL /DNDEBUG 到编译命令, release_mode为true", get_time_());
                cmd_args += " /GL /DNDEBUG ";
            }
            else
            {
                println(log_file, "{}添加 d /Zi /DDEBUG 到编译命令, release_mode为false", get_time_());
                cmd_args += "d /Zi /DDEBUG ";
            }

            if (optimize && release_mode)
            {
                println(log_file, "{}添加/O2 到编译命令, optimize为true, release_mode为true", get_time_());
                cmd_args += "/O2 ";
            }
            else if (!release_mode)
            {
                println(log_file, "{}添加 /Od 到编译命令, 调试模式禁用优化", get_time_());
                cmd_args += "/Od ";
            }
            else
            {
                println(log_file, "{}添加 /Ox 到编译命令, release_mode为true, optimize为false", get_time_());
                cmd_args += "/Ox ";
            }
            
        }
        else
        {
            println(log_file, "{}识别为GCC/Clang编译器", get_time_());
            cmd_args = "-Wall -Wextra -Werror -lstdc++exp ";
            println(log_file, "{}添加-Wall -Wextra -Werror -lstdc++exp 到编译命令", get_time_());
            if (compiler == "clang++")
            {
                if (use_llvm_libcpp)
                {
                    println(log_file, "{}添加-stdlib=libc++ 到编译命令, use_llvm_libcpp为true", get_time_());
                    cmd_args += "-stdlib=libc++ ";
                }
            }

            if (static_compile)
            {
                println(log_file, "{}添加-static 到编译命令, static_compile为true", get_time_());
                cmd_args += "-static ";
            }

            if (release_mode)
            {
                println(log_file, "{}添加-DNDEBUG 到编译命令, release_mode为true", get_time_());
                cmd_args += "-DNDEBUG ";
            }

            if (optimize && release_mode)
            {
                println(log_file, "{}添加-O2 到编译命令, optimize为true, release_mode为true", get_time_());
                cmd_args += "-O2 ";
            }
            else if (!release_mode)
            {
                println(log_file, "{}添加-O0 -g 到编译命令, 调试模式启用调试信息禁用优化", get_time_());
                cmd_args += "-O0 -g ";
            }
            else
            {
                println(log_file, "{}添加-O0 到编译命令, release_mode为true, optimize为false", get_time_());
                cmd_args += "-O0 ";
            }
        }

        {
            println(log_file, "{}(variable)cmd_args: {}", get_time_(), cmd_args);

            println(log_file, "\n{}正在创建cmd_files字符串", get_time_());
            string cmd_files = file_name;
            println(log_file, "{}(variable)cmd_files/编译的文件: {}", get_time_(), cmd_files);

            println(log_file, "\n{}正在创建cmd_libs字符串", get_time_());
            string cmd_libs = static_libs;
            println(log_file, "{}(variable)cmd_libs/静态库: {}", get_time_(), cmd_libs);

            println(log_file, "{}正在创建cmd_include字符串", get_time_());
            string cmd_include = include_path;
            println(log_file, "{}(variable)cmd_include/头文件路径: {}", get_time_(), cmd_include);

            println(log_file, "\n{}正在创建cmd_out_type字符串", get_time_());
            int cmd_out_type = output_file_type; // 1:可执行文件 2:动态库 3:静态库
            println(log_file, "{}(variable)cmd_out_type/输出类型: {}", get_time_(), cmd_out_type);

            println(log_file, "\n{}正在创建cmd_out_path字符串", get_time_());
            string cmd_out_path = output_file_name;
            println(log_file, "{}(variable)cmd_out_path/输出路径: {}", get_time_(), cmd_out_path);

            println(log_file, "\n{}正在创建指定C++标准的标志", get_time_());
            string cppstandard;
            if (compiler == "g++" || compiler == "clang++")
            {
                cppstandard = " -std=c++" + version;
            }
            else if (compiler == "cl" || compiler == "clang-cl")
            {
                cppstandard = " /std:c++" + version + " ";
            }

            println(log_file, "\n{}正在添加到cmd_args字符串", get_time_());
            cmd_args += cmd_files + " " + cppstandard + cmd_libs + " /I:\"" + cmd_include + "\" ";
            
            if (compiler == "g++" || compiler == "clang++")
            {
                if (cmd_out_type == 1)
                {
                    cmd_args += "-o \"" + cmd_out_path + "\"";
                }
                else if (cmd_out_type == 2)
                {
                    cmd_args += "-shared -o \"" + cmd_out_path + "\"";
                }
                else if (cmd_out_type == 3)
                {
                    cmd_args += "-c -o \"" + cmd_out_path + "\"";
                }
            }
            else if (compiler == "cl" || compiler == "clang-cl")
            {
                if (cmd_out_type == 1)
                {
                    cmd_args += "/Fe\"" + cmd_out_path + "\"";
                }
                else if (cmd_out_type == 2)
                {
                    cmd_args += "/LD /Fe\"" + cmd_out_path + "\"";
                }
                else if (cmd_out_type == 3)
                {
                    cmd_args += "/c /Fo\"" + cmd_out_path + "\"";
                }
            }

            if (compiler == "cl")
            {
                cmd_args += " /nologo";
            }

            println(log_file, "\n{}(variable)cmd_args/命令行参数:", get_time_(), cmd_args);
        }

        string cmd = compiler + " " + cmd_args;

        #ifdef _WIN32
        FILE* fp = _popen(cmd.c_str(), "r");
        #else
        FILE* fp = popen(cmd.c_str(), "r");
        #endif
        println(log_file, "{}正在执行命令:{}\n", get_time_(), cmd);

        if (fp == NULL)
        {
            println(log_file, "{}错误: 无法执行命令{}", get_time_(), cmd_args);
            return ;
        }
        else
        {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), fp) != NULL)
            {
                print(log_file, "{}", buffer);
            }
        }
        println(log_file, "{}编译完成", get_time_());
        #ifdef _WIN32
        _pclose(fp);
        #else
        pclose(fp);
        #endif
        log_file.close();

        println("编译完成于{}", get_time_());
        return;
    }
}

int main(int argc, char* argv[])
{
    using namespace ikun_core_cpp;
    println("ikun库核心管理器");
    println("版本: {}", IKUN_VERSION);
    println("{}", tips);

    if (argc < 2)
    {
        println("{}关键错误: 未传递选项{}", core_color::red, core_color::reset);
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
            core_list_libs();
            return 0;
        }
        else if (arg == "-ll" || arg == "--list-libs")
        {
            list_installed_libs();
            return 0;
        }
        else if (arg == "--compile")
        {
            compile();
            return 0;
        }
        else if (arg == "-i" || arg == "--install")
        {
            if (i + 1 < argc)
            {
                install(argv[i + 1]);
                return 0;
            }
            else
            {
                println("{}错误: 请指定要安装的模块名{}", core_color::red, core_color::reset);
                return 1;
            }
        }
        else if (arg == "-ip" || arg == "--install-preview")
        {
            // 安装preview分支的库
            if (i + 1 < argc)
            {
                install(argv[i + 1], true);
            }
            return 0;
        }
        else if (arg == "-u" || arg == "--uninstall")
        {
            if (i + 1 < argc)
            {
                uninstall(argv[i + 1]);
                return 0;
            }
            else
            {
                println("{}错误: 请指定要卸载的模块名{}", core_color::red, core_color::reset);
                return 1;
            }
        }
        else if (arg == "-c" || arg == "--check")
        {
            if (i + 1 < argc)
            {
                if (check_install(argv[i + 1]))
                {
                    println("{} 模块已安装", argv[i + 1]);
                }
                else
                {
                    println("{} 模块未安装或不存在", argv[i + 1]);
                }
                return 0;
            }
            else
            {
                println("{}错误: 请指定要检查的库名{}", core_color::red, core_color::reset);
                return 1;
            }
        }
        else if (arg == "-ifl" || arg == "--install-from-local")
        {
            if (i + 2 < argc)
            {
                install_from_local(argv[i + 1], argv[i + 2]);
                return 0;
            }
            else
            {
                println("{}错误: 请指定要安装的模块名和路径{}", core_color::red, core_color::reset);
                println("示例: ./ikun -ifl ikun_core /usr/local/ikun/");
                return 1;
            }
        }
        else if (arg == "-cp" || arg == "--create-project")
        {
            create_project();
            return 0;
        }
        else
        {
            println("{}关键错误: 未知选项: {}{}", core_color::red, arg, core_color::reset);
            help();
            return 1;
        }
    }

    return 0;
}
