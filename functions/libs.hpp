// ikun库functions模块工具库libs.hpp
// 提供文件操作功能

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_FUNCTIONS_LIBS_HPP
#define IKUN_FUNCTIONS_LIBS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>

namespace libs
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

        for (const auto & entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                int fextnl = fileextname.length();
                if (filename.size() >= fextnl && filename.substr(filename.size() - fextnl) == fileextname)
                {
                    files.push_back(filename);
                }
            }
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
            throw std::runtime_error("无法打开文件: " + filename);
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
            throw std::runtime_error("无法创建文件: " + filename);
        }
        file.write(content.c_str(), content.size());
        file.close();
    }

    void mkdir(const std::string& path) // 创建目录
    {
        std::filesystem::create_directory(path);
    }
}
#endif // IKUN_FUNCTIONS_LIBS_HPP