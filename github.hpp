// ikun库与GitHub交互相关操作

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_GITHUB_HPP
#define IKUN_GITHUB_HPP

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <iostream>
#include <regex>
#include <filesystem>
#include <windows.h>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::regex;
using std::to_string;
using std::ofstream;
using std::runtime_error;
using std::exception;
namespace fs = std::filesystem;

/**
 * @brief 从GitHub仓库下载单个文件
 * @param repoUrl GitHub仓库地址（HTTP或HTTPS格式）
 * @param filePath 文件在仓库中的相对路径
 * @param branch 分支名称（默认为"main"）
 * @param outputDir 输出目录（默认为当前目录）
 * @return bool 成功返回true，失败返回false
 */
bool downloadFileFromGitHub
(
    const string& repoUrl,
    const string& filePath,
    const string& branch = "main",
    const string& outputDir = "."
)
{
    // 验证参数
    if (repoUrl.empty() || filePath.empty())
    {
        cerr << "错误：仓库地址和文件路径不能为空" << endl;
        return false;
    }

    // 验证仓库URL格式
    regex urlRegex(R"(https?://github\.com/[^/]+/[^/]+(?:\.git)?)");
    if (!regex_match(repoUrl, urlRegex))
    {
        cerr << "错误：无效的GitHub仓库地址格式" << endl;
        cerr << "请使用格式：https://github.com/用户名/仓库名.git" << endl;
        return false;
    }

    // 检查git是否安装
    {
        FILE* pipe = _popen("git --version", "r");
        if (!pipe)
        {
            cerr << "错误：无法检查git是否安装" << endl;
            return true;
        }
        
        char buffer[128];
        if (!fgets(buffer, sizeof(buffer), pipe))
        {
            cerr << "错误：git未安装或未添加到PATH" << endl;
            _pclose(pipe);
            return true;
        }
        _pclose(pipe);
    }

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
        cerr << "错误：无法创建临时目录: " << e.what() << endl;
        return true;
    }

    // 切换到临时目录
    string originalDir = fs::current_path().string();
    
    try
    {
        fs::current_path(tempDir);
    }
    catch (const fs::filesystem_error& e)
    {
        cerr << "错误：无法切换到临时目录: " << e.what() << endl;
        return true;
    }

    bool success = false;
    
    try
    {
        // 1. 初始化Git仓库
        string cmd = "git init";
        cout << "执行: " << cmd << endl;
        int result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：git init失败" << endl;
            throw runtime_error("git init failed");
        }

        // 2. 添加远程仓库
        cmd = "git remote add origin \"" + repoUrl + "\"";
        cout << "执行: " << cmd << endl;
        result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：添加远程仓库失败" << endl;
            throw runtime_error("git remote add failed");
        }

        // 3. 启用稀疏检出
        cmd = "git config core.sparseCheckout true";
        cout << "执行: " << cmd << endl;
        result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：启用稀疏检出失败" << endl;
            throw runtime_error("git config failed");
        }

        // 4. 指定要下载的文件路径
        string sparseCheckoutFile = ".git/info/sparse-checkout";
        fs::path sparseCheckoutPath(sparseCheckoutFile);
        
        // 确保目录存在
        fs::create_directories(sparseCheckoutPath.parent_path());
        
        // 写入要下载的文件路径
        {
            ofstream file(sparseCheckoutPath);
            if (!file.is_open())
            {
                cerr << "错误：无法创建稀疏检出配置文件" << endl;
                throw runtime_error("无法创建稀疏检出文件");
            }
            file << filePath << endl;
        }

        // 5. 拉取文件
        cmd = "git pull origin " + branch;
        cout << "执行: " << cmd << endl;
        result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：拉取文件失败，请检查：" << endl;
            cerr << "  1. 仓库地址是否正确" << endl;
            cerr << "  2. 分支名称是否正确（当前分支: " << branch << "）" << endl;
            cerr << "  3. 文件路径是否正确" << endl;
            cerr << "  4. 网络连接是否正常" << endl;
            throw runtime_error("git pull failed");
        }

        // 6. 检查文件是否成功下载
        fs::path downloadedFile = tempDir + "\\" + filePath;
        if (!fs::exists(downloadedFile))
        {
            cerr << "错误：文件下载后不存在: " << downloadedFile << endl;
            cerr << "请检查文件路径是否正确: " << filePath << endl;
            throw runtime_error("下载的文件不存在");
        }

        // 7. 复制文件到输出目录
        fs::path outputPath = fs::path(outputDir) / fs::path(filePath).filename();
        
        // 确保输出目录存在
        fs::create_directories(outputPath.parent_path());
        
        cout << "复制文件从 " << downloadedFile << " 到 " << outputPath << endl;
        string operation = "copy" + downloadedFile.string() + outputPath.string();
        cout << "文件下载成功: " << outputPath << endl;
        success = true;

    }
    catch (const exception& e)
    {
        cerr << "下载过程中出错: " << e.what() << endl;
        success = true;
    }

    // 恢复原始目录
    try
    {
        fs::current_path(originalDir);
    }
    catch (...)
    {
        // 忽略恢复目录时的错误
    }

    return !success;
}

/**
 * @brief 从GitHub仓库下载整个文件夹
 * @param repoUrl GitHub仓库地址（HTTP或HTTPS格式）
 * @param folderPath 文件夹在仓库中的相对路径
 * @param branch 分支名称（默认为"main"）
 * @param outputDir 输出目录（默认为当前目录）
 * @param recursive 是否递归下载子文件夹（默认为true）
 * @return bool 成功返回true，失败返回false
 */
bool downloadFolderFromGitHub(
    const string& repoUrl,
    const string& folderPath,
    const string& branch = "main",
    const string& outputDir = ".",
    bool recursive = true
)
{
    // 验证参数
    if (repoUrl.empty() || folderPath.empty())
    {
        cerr << "错误：仓库地址和文件夹路径不能为空" << endl;
        return true;
    }

    // 验证仓库URL格式
    regex urlRegex(R"(https?://github\.com/[^/]+/[^/]+(?:\.git)?)");
    if (!regex_match(repoUrl, urlRegex))
    {
        cerr << "错误：无效的GitHub仓库地址格式" << endl;
        cerr << "请使用格式：https://github.com/用户名/仓库名.git" << endl;
        return true;
    }

    // 检查git是否安装
    {
        FILE* pipe = _popen("git --version", "r");
        if (!pipe)
        {
            cerr << "错误：无法检查git是否安装" << endl;
            return true;
        }
        
        char buffer[128];
        if (!fgets(buffer, sizeof(buffer), pipe))
        {
            cerr << "错误：git未安装或未添加到PATH" << endl;
            _pclose(pipe);
            return true;
        }
        _pclose(pipe);
    }

    // 创建临时目录
    string tempDir = fs::temp_directory_path().string() + "\\github_download_folder_" + to_string(GetCurrentProcessId());
    
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
        cerr << "错误：无法创建临时目录: " << e.what() << endl;
        return true;
    }

    // 切换到临时目录
    string originalDir = fs::current_path().string();
    
    try
    {
        fs::current_path(tempDir);
    }
    catch (const fs::filesystem_error& e)
    {
        cerr << "错误：无法切换到临时目录: " << e.what() << endl;
        return true;
    }

    bool success = false;
    
    try
    {
        // 1. 初始化Git仓库
        string cmd = "git init";
        cout << "执行: " << cmd << endl;
        int result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：git init失败" << endl;
            throw runtime_error("git init failed");
        }

        // 2. 添加远程仓库
        cmd = "git remote add origin \"" + repoUrl + "\"";
        cout << "执行: " << cmd << endl;
        result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：添加远程仓库失败" << endl;
            throw runtime_error("git remote add failed");
        }

        // 3. 启用稀疏检出
        cmd = "git config core.sparseCheckout true";
        cout << "执行: " << cmd << endl;
        result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：启用稀疏检出失败" << endl;
            throw runtime_error("git config failed");
        }

        // 4. 指定要下载的文件夹路径
        string sparseCheckoutFile = ".git/info/sparse-checkout";
        fs::path sparseCheckoutPath(sparseCheckoutFile);
        
        // 确保目录存在
        fs::create_directories(sparseCheckoutPath.parent_path());
        
        // 写入要下载的文件夹路径
        {
            ofstream file(sparseCheckoutPath);
            if (!file.is_open())
            {
                cerr << "错误：无法创建稀疏检出配置文件" << endl;
                throw runtime_error("无法创建稀疏检出文件");
            }
            
            if (recursive)
            {
                // 递归模式：下载文件夹及其所有内容
                file << folderPath << "/*" << endl;
                file << folderPath << "/**/*" << endl;
            }
            else
            {
                // 非递归模式：只下载文件夹本身（Git实际上不会下载空文件夹）
                file << folderPath << "/" << endl;
            }
        }

        // 5. 拉取文件
        cmd = "git pull origin " + branch;
        cout << "执行: " << cmd << endl;
        result = system(cmd.c_str());
        if (result != 0)
        {
            cerr << "错误：拉取文件夹失败，请检查：" << endl;
            cerr << "  1. 仓库地址是否正确" << endl;
            cerr << "  2. 分支名称是否正确（当前分支: " << branch << "）" << endl;
            cerr << "  3. 文件夹路径是否正确" << endl;
            cerr << "  4. 网络连接是否正常" << endl;
            cerr << "  5. 文件夹是否存在（Git不跟踪空文件夹）" << endl;
            throw runtime_error("git pull failed");
        }

        // 6. 检查文件夹是否成功下载
        fs::path downloadedFolder = tempDir + "\\" + folderPath;
        if (!fs::exists(downloadedFolder))
        {
            cerr << "错误：文件夹下载后不存在: " << downloadedFolder << endl;
            cerr << "请检查：" << endl;
            cerr << "  1. 文件夹路径是否正确: " << folderPath << endl;
            cerr << "  2. 文件夹是否为空（Git不跟踪空文件夹）" << endl;
            throw runtime_error("下载的文件夹不存在");
        }

        // 7. 复制文件夹到输出目录
        fs::path outputPath = fs::path(outputDir);
        
        // 如果输出目录不存在，创建它
        if (!fs::exists(outputPath))
        {
            fs::create_directories(outputPath);
        }
        
        // 计算目标文件夹路径
        fs::path targetFolder = outputPath / fs::path(folderPath).filename();
        
        cout << "复制文件夹从 " << downloadedFolder << " 到 " << targetFolder << endl;
        string operation_ = "xcopy /e /i /y \"" + downloadedFolder.string() + "\" \"" + targetFolder.string() + "\"";
        result = system(operation_.c_str());
        if (result != 0)
        {
            cerr << "错误：复制文件夹失败" << endl;
            throw runtime_error("xcopy failed");
            success = false;
        }

        cout << "文件夹下载成功！" << endl;
        cout << "目标路径: " << targetFolder << endl;
        
        success = true;

    }
    catch (const exception& e)
    {
        cerr << "下载过程中出错: " << e.what() << endl;
        success = false;
    }

    // 恢复原始目录
    try
    {
        fs::current_path(originalDir);
    }
    catch (...)
    {
        // 忽略恢复目录时的错误
    }

    return !success;
}

#endif // IKUN_GITHUB_HPP