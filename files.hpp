// ikun库files.hpp
// 包括文件操作相关函数
// 命名空间: files
// 版本: 7.1.2 Release Candidate

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#ifndef IKUN_FILES_HPP
#define IKUN_FILES_HPP

#include <string>
#include <fstream>
#include <print>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <random>
#include "ikun_stderr.hpp"

namespace files
{
    bool fileexists(const std::string& filename) // 检查文件是否存在
    {
        return std::filesystem::exists(filename); 
    }

    /** @brief 获取目录下所有文件名
     *  @param path 路径
     *  @param fileextname 文件扩展名, 默认为.
     *  @return std::vector<std::string> 文件名列表
     */
    std::vector<std::string> filedir(std::string path = ".", std::string fileextname = ".")
    {
        std::vector<std::string> files;

        // 检查目录是否存在
        if (!std::filesystem::exists(path))
        {
            std::println(stderr, "错误: '{}' 不存在", path);
            return files; // 返回空向量
        }

        // 检查是否是目录
        if (!std::filesystem::is_directory(path))
        {
            std::println(stderr ,"错误: '{}' 不是目录", path);
            return files;
        }

        // 确保文件扩展名以点开头
        if (!fileextname.empty() && fileextname != ".")
        {
            if (fileextname[0] != '.')
            {
                fileextname = "." + fileextname; // 自动添加点
            }
        }

        try
        {
            // 遍历目录
            for (const auto &entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    std::string filename = entry.path().filename().string();
                    
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
                        // 获取文件扩展名
                        std::string ext = entry.path().extension().string();
                        
                        // 比较扩展名(不区分大小写)
                        if (!ext.empty() && 
                            std::equal(ext.begin(), ext.end(), 
                                    fileextname.begin(), fileextname.end(),
                                    [](char a, char b) {
                                        return std::tolower(a) == std::tolower(b);
                                    }))
                        {
                            files.push_back(filename);
                        }
                    }
                }
            }
            std::sort(files.begin(), files.end());
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            ikun_error::throw_re(e.what(), "files.hpp", "filedir()", "ikun_file 003");
        }
        catch (const std::exception& e)
        {
            ikun_error::throw_re(e.what(), "files.hpp", "filedir()", "ikun_file 004");
        }

        return files;
    }

    /**
     * @brief 读取文件全部内容
     * @param filename 文件名
     * @return std::string 文件内容
     */
    std::string rfile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            ikun_error::throw_re("Cannot open file: " + filename,
            "functions.hpp", "rfile()", "ikun_file 001"
            );
        }
        
        std::string content;
        file.seekg(0, std::ios::end);
        content.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&content[0], content.size());
        file.close();
        
        return content;
    }

    /**
     * @brief 写入文件
     * @param filename 文件名
     * @param content 内容
     */
    void wfile(const std::string& filename, const std::string& content)
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            ikun_error::throw_re("Cannot open/create file: " + filename,
            "functions.hpp", "wfile()", "ikun_file 002"
            );
        }
        file.write(content.c_str(), content.size());
        file.close();
    }

    bool check_file_name(std::string file_name) // 检查文件名是否合法
    {
        if (file_name.find_first_of("/\\:*?\"<>|") != std::string::npos)
        {
            return false;
        }

        return true;
    }

    bool check_dir_name(std::string dir_name) // 检查目录名是否合法
    {
        if (dir_name.find_first_of(":*?\"<>|") != std::string::npos)
        {
            return false;
        }

        return true;
    }

    void mkdir(const std::string& path) // 创建目录
    {
        std::filesystem::create_directory(path);
    }
}

#endif // IKUN_FILES_HPP