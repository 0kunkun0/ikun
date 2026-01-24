// ikun库主程序, 用于管理库的功能
// 操作系统要求: Windows 10 x64, macOS Catalina 10.15+, Linux内核5.10+
// 终端要求(ANSI转义序列): 
//     Windows: Windows Terminal/PowerShell 5.1+, cmd.exe默认不支持
//     macOS: 所有
//     Linux: 所有
// 版本: 7.1.2 Preview Build 2026.1.24
// C++版本要求: C++23或更高版本
// 编译器选项: /std:c++23preview(将来可能变为/std:c++23)或/std:c++latest, -std=c++23

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

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
    或者查看库中的 说明.txt
*/

#ifdef _MSC_VER
#define _AMD64_
#endif

#define IKUN_VERSION "7.1.2 Preview Build 2026.1.24"
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
    #undef __cplusplus // 取消定义
    #define __cplusplus _MSVC_LANG // MSVC上需要用_MSVC_LANG宏来代替, 此处为了兼容性重新定义__cplusplus
#endif

#if __cplusplus < IKUN_CPP_VERSION_REQUIRED // 判断C++版本
    #error "此版本的ikun库需要C++23或更高版本"
    #error "请在编译时指定最低-std=c++23或/std:c++23"
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

#include <print>
#include <format>
#include <regex>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
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

string tips =
#ifdef _WIN64
"请保证使用的终端为Windows Terminal和PowerShell 5.1+, cmd.exe可能无法显示ANSI转义序列(即带颜色的字符)"
#elifdef __linux__
"请注意: ikun库在Linux下的支持为实验性, 由于作者懒得制作Linux虚拟机, 测试均在WSL2下测试, 可能会有各种bug"
#elifdef __APPLE__
"请注意: ikun库在macOS的支持不完整, 由于作者没有Mac设备, 操作系统API相关特性均使用POSIX兼容性实现, 某些极端情况可能无法正常使用"
#endif
;

namespace ikun_core_cpp
{
    int core_get_cppversion() // 获取C++版本
    {
        int cppversion = (__cplusplus / 100) % 100;
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
        println("参数:");
        println("  (-i, -u, -c, --install-from-local) 库名: 指定要安装/卸载/检查的库的名称");
        println("  (--install-from-local) 本地库路径: 指定要安装的本地库的相对/绝对路径(以/作为分隔符, 如D:/workspace/dev/ikun/ikun_core.hpp");
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
}

int main(int argc, char* argv[])
{
    using namespace ikun_core_cpp;
    println("ikun库核心管理器");
    println("版本: {}", IKUN_VERSION);
    println("{}", tips);

    if (!core_fileexists("libs_download_url.txt"))
    {
        ofstream file("libs_download_url.txt"); // 创建日志文件
        file << "https://github.com/0kunkun0/ikun.git";
        file.close();
    }

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
        else if (arg == "-i" || arg == "--install")
        {
            if (i + 1 < argc)
            {
                install(argv[i + 1]);
                i ++;
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
        }
        else if (arg == "-u" || arg == "--uninstall")
        {
            if (i + 1 < argc)
            {
                uninstall(argv[i + 1]);
                i ++;
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
                i ++;
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
                i += 2;
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
