// ikun库functions模块工具库libs.hpp
// 提供文件系统操作和部分Windows API功能
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据许可证和用户协议规定, 禁止商业用途, 违者依法追究法律责任

#ifndef IKUN_FUNCTIONS_LIBS_HPP
#define IKUN_FUNCTIONS_LIBS_HPP

#ifndef IKUN_CORE
    #error "请先使用#include "ikun/core.hpp"包含ikun库核心"
    #error "否则无法使用版本管理功能"
#endif // IKUN_CORE

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace libs
{
    bool fileexists(const std::string& filename) // 检查文件是否存在
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            file.close();
            return true;
        }
        
#ifdef _WINDOWS_
        DWORD attributes = GetFileAttributesA(filename.c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES) 
        {
            return false;
        }
        
        return !(attributes & FILE_ATTRIBUTE_DIRECTORY);
#endif
        std::cerr << "未包含系统API, 无法检查文件是否存在" << std::endl;
        return false;
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

#ifdef _WINDOWS_
    bool mkdir(const std::string& path) // 创建目录
    {
            return CreateDirectoryA(path.c_str(), NULL) != 0;
    }
#
    DWORD get_current_process_id() // 获取当前进程ID
    {
        return GetCurrentProcessId();
    }
#endif

    void speedup_io() // 关闭iostream和stdio.h的同步, 关闭cin和cout的绑定, 加快IO
    {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(0);
        std::cout.tie(0);
    }

#ifdef _WINDOWS_
    /**
     * @brief 显示消息框
     * @param title 标题
     * @param message 消息内容
     * @param type 消息框类型（MB_OK, MB_YESNO等）
     * @return int 用户响应
     */
    int show_message_box(const std::string& title, const std::string& message, UINT type = MB_OK)
    {
        return MessageBoxA(NULL, message.c_str(), title.c_str(), type);
    }
#endif
}
#endif // IKUN_FUNCTIONS_LIBS_HPP