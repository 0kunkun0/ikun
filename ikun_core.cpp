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
    自带内容:
    ikun_core.cpp // 主程序的原代码
    build.bat // Windows下的编译脚本
    build_lib.py // 管理脚本
    
    别的内容请访问 https://github.com/0kunkun0/ikun
    或者查看库中的 说明.txt
*/

#define IKUN_VERSION "7.1.1"
#define IKUN_OS_PLATFORM "Windows x64"
#define IKUN_CPP_VERSION_REQUIRED 202303L
#define IKUN_LANGUAGE_PLATFORM "C++"

#ifdef _MSC_VER // 判断编译器
    #define MSVC
#elifdef __clang__
    #define CLANG
#elifdef __GNUC__
    #define GCC
#endif

#include <print>
#include <regex>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <processthreadsapi.h>

using namespace std;
namespace fs = std::filesystem;

inline namespace core_color
{
    constexpr const char* reset = "\033[0m";
    constexpr const char* red = "\033[31m";
    constexpr const char* green = "\033[32m";
    constexpr const char* blue = "\033[34m";
    constexpr const char* bold = "\033[1m";
}

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

namespace ikun_core_cpp_functions_do_not_use
{
    bool downloadFileFromGitHub
    (
        const string& repoUrl,
        const string& filePath,
        const string& branch = "main",
        const string& outputDir = "."
    )
    {
        // 创建临时目录
        string tempDir = fs::temp_directory_path().string() + "\\github_download_" + to_string(GetCurrentProcessId());
        
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
                println(cerr, "错误: git init失败");
                throw runtime_error("git init failed");
            }

            // 添加远程仓库
            cmd = "git remote add origin \"" + repoUrl + "\"";
            println("执行: {}", cmd);
            rst = system(cmd.c_str());
            if (rst != 0)
            {
                println(cerr, "错误：添加远程仓库失败");
                throw runtime_error("git remote add failed");
            }

            // 启用稀疏检出
            cmd = "git config core.sparseCheckout true";
            println("执行: {}", cmd);
            rst = system(cmd.c_str());
            if (rst != 0)
            {
                println(cerr, "错误：启用稀疏检出失败");
                throw runtime_error("git config failed");
            }

            // 指定要下载的文件路径
            string sparseCheckoutFile = ".git/info/sparse-checkout";
            fs::path sparseCheckoutPath(sparseCheckoutFile);
            
            // 确保目录存在
            fs::create_directories(sparseCheckoutPath.parent_path());
            
            // 写入要下载的文件路径
            {
                ofstream file(sparseCheckoutPath);
                if (!file.is_open())
                {
                    println(cerr, "错误：无法创建稀疏检出配置文件");
                    throw runtime_error("无法创建稀疏检出文件");
                }
                file << filePath << endl;
            }

            // 5. 拉取文件
            cmd = "git pull origin " + branch;
            println("执行: {}", cmd);
            rst = system(cmd.c_str());
            if (rst != 0)
            {
                println(cerr, "错误：拉取文件失败");
                throw runtime_error("git pull failed");
            }

            // 7. 复制文件到输出目录
            fs::path oP = fs::path(outputDir) / fs::path(filePath).filename();
            
            // 确保输出目录存在
            fs::create_directories(outputPath.parent_path());
            
            println("复制文件从 {} 到 {}", dF, oP);
            string operation = "copy" + dF.string() + oP.string();
            println("文件下载成功: {}", oP);
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
                                    [](char a, char b) {
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
        println("  -li, --list-libs: 列出所有安装了的库");
        println("  -i, --install: 安装指定的库");
        println("  -ih, --install-header: 安装指定的头文件");
        println("  -u, --uninstall: 卸载指定的库/头文件");
        println("  -c, --check: 检查指定的库是否已安装");
        println("  -ic, --install-core: 安装核心程序");
        println("参数:");
        println("  库名: 指定要安装/卸载/检查的库的名称");
    }

    void core_list_libs()
    {
        // 访问作者GitHub
        system("start https://github.com/0kunkun0/ikun");
    }

    void list_installed_libs()
    {
        println("已安装的库:");
        core_filedir("./", ".hpp");
    }

    void install_header(string lib_name) // 从GitHub上下载单个头文件
    {
        println("从GitHub上下载库");
        println("库名: {}", lib_name);
        println("请保证库名正确");

        ifstream file("libs_download_url.txt");
        string url;
        getline(file, url);
        file.close();

        bool a = downloadFileFromGitHub
        (
            url,
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

        ifstream file("libs_download_url.txt");
        string url;
        getline(file, url);
        file.close();

        bool a = downloadFolderFromGitHub
        (
            url,
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
        if ((lib_name == "ikunn_core") || (lib_name == "build") || (lib_name == "build_lib"))// 防误删逻辑
        {
            println("{}关键错误: 无法卸载核心库{}", core_color::red, core_color::reset);
            return;
        }

        println("库名: {}", lib_name);
        println("{}请保证库名正确, 否则可能误删文件", core_color::red);
        println("删除时不会经过回收站, 按下回车键继续{}", core_color::reset);
        cin.ignore();
        string a = "del /S /F /Q " + lib_name + ".hpp";
        system(a.c_str());
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

        return core_fileexists(lib_name + ".hpp");
    }
}

int main(int argc, char* argv[])
{
    using namespace ikun_core_cpp_functions_do_not_use;
    println("ikun库核心管理器");

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
        else if (arg == "-li" || arg == "--list-libs")
        {
            list_installed_libs();
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
                println("{}错误: 请指定要安装的头文件名{}", core_color::red, core_color::reset);
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
                println("{}错误: 请指定要安装的库名{}", core_color::red, core_color::reset);
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
                println("{}错误: 请指定要卸载的库名{}", core_color::red, core_color::reset);
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
                println("{}错误: 请指定要检查的库名{}", core_color::red, core_color::reset);
                return 1;
            }
        }
        else if (arg == "-ic" || arg == "--install-core")
        {
            install_header("core");
            install_header("functions");
        }
        else
        {
            println("{}关键错误: 未知选项: {}{}", arg, core_color::red, core_color::reset);
            help();
            return 1;
        }
    }

    return 0;
}
