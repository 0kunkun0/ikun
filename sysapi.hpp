//Dependencies: ikun_stderr
// ikun库对操作系统API的跨平台封装
// 支持Windows API和POSIX API
// 版本: 7.1.2 Release Build 2026.2

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#ifndef IKUN_SYSAPI_HPP
#define IKUN_SYSAPI_HPP

#include <string>
#include <vector>
#include <optional>
#include "ikun_stderr.hpp"
#include <cstdlib>
#include <cstring>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#endif

namespace sysapi
{

using std::string;
using std::vector;
using std::optional;

namespace internal
{
    const string LIB_NAME = "sysapi.hpp";
    
#ifdef _WIN32
    string get_last_error_str()
    {
        DWORD error = GetLastError();
        if (error == 0) return "No error";
        
        LPSTR buffer = nullptr;
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&buffer, 0, nullptr);
        
        string message(buffer, size);
        LocalFree(buffer);
        return message;
    }
#endif
}

/**
 * @brief 执行Shell命令并获取输出
 * @param command 要执行的命令
 * @return optional<string> 命令输出（如果执行成功）
 * @throws 运行时错误 (错误类型: ikun_sysapi 错误码: 001)
 */
optional<string> exec_cmd(const string& command)
{
#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        ikun_error::throw_re(
            "Failed to execute command: " + command,
            internal::LIB_NAME,
            "execute_command",
            "ikun_sysapi 001"
        );
    }
    
    char buffer[128];
    string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        result += buffer;
    }
    
    int return_code = _pclose(pipe);
    if (return_code != 0)
    {
        return std::nullopt;
    }
    
    return result;
#else
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        ikun_error::throw_re(
            "Failed to execute command: " + command,
            internal::LIB_NAME,
            "execute_command",
            "ikun_sysapi 001"
        );
    }
    
    char buffer[128];
    string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        result += buffer;
    }
    
    int return_code = pclose(pipe);
    if (return_code != 0)
    {
        return std::nullopt;
    }
    
    return result;
#endif
}

/**
 * @brief 执行Shell命令并返回退出码
 * @param command 要执行的命令
 * @return int 命令退出码
 * @throws 运行时错误 (错误类型: ikun_sysapi 错误码: 002)
 */
int execmd_with_ret(const string& command)
{
#ifdef _WIN32
    return system(command.c_str());
#else
    int status = system(command.c_str());
    if (status == -1)
    {
        ikun_error::throw_re(
            "Failed to execute command: " + command,
            internal::LIB_NAME,
            "execute_command_with_exitcode",
            "ikun_sysapi 002"
        );
    }
    return WEXITSTATUS(status);
#endif
}

/**
 * @brief 获取当前工作目录
 * @return string 当前工作目录路径
 * @throws 运行时错误 (错误类型: ikun_sysapi 错误码: 003)
 */
string get_cwd()
{
    char buffer[4096];
    if (getcwd(buffer, sizeof(buffer)) == nullptr)
    {
        ikun_error::throw_re(
#ifdef _WIN32
            "GetCurrentDirectory failed: " + internal::get_last_error_str(),
#else
            "getcwd failed: " + string(strerror(errno)),
#endif
            internal::LIB_NAME,
            "get_current_directory",
            "ikun_sysapi 003"
        );
    }
    return string(buffer);
}

/**
 * @brief 跨平台获取本地时间（安全版本）
 * @param time 要转换的time_t时间值
 * @return tm 本地时间结构体
 * @throws 运行时错误 (错误类型: ikun_sysapi 错误码: 007)
 */
std::tm get_localtime_from_sysapi(std::time_t time_val)
{
    std::tm local_time = {};
    
#ifdef _WIN32
    // Windows 使用 localtime_s
    errno_t err = localtime_s(&local_time, &time_val);
    if (err != 0)
    {
        ikun_error::throw_re(
            "Failed to convert time to local time (localtime_s failed)",
            internal::LIB_NAME,
            "get_localtime_from_sysapi",
            "ikun_sysapi 006"
        );
    }
#else
    // POSIX 系统使用 localtime_r
    std::tm* result = localtime_r(&time_val, &local_time);
    if (result == nullptr)
    {
        ikun_error::throw_re(
            "Failed to convert time to local time (localtime_r failed)",
            internal::LIB_NAME,
            "get_localtime_from_sysapi",
            "ikun_sysapi 006"
        );
    }
#endif
    
    return local_time;
}

/**
 * @brief 更改当前工作目录
 * @param path 目标目录路径
 * @return bool 是否成功
 * @throws 运行时错误 (错误类型: ikun_sysapi 错误码: 004)
 */
bool cd(const string& path)
{
    if (chdir(path.c_str()) != 0)
    {
        ikun_error::throw_re(
            "Failed to change directory to: " + path,
            internal::LIB_NAME,
            "change_directory",
            "ikun_sysapi 004"
        );
        return false;
    }
    return true;
}

/**
 * @brief 获取环境变量值
 * @param name 环境变量名
 * @return optional<string> 环境变量值（如果存在）
 */
optional<string> get_env_var(const string& name)
{
#ifdef _WIN32
    char buffer[4096];
    DWORD length = GetEnvironmentVariableA(name.c_str(), buffer, sizeof(buffer));
    if (length == 0)
    {
        return std::nullopt;
    }
    return string(buffer, length);
#else
    char* value = getenv(name.c_str());
    if (value == nullptr)
    {
        return std::nullopt;
    }
    return string(value);
#endif
}

/**
 * @brief 设置环境变量
 * @param name 环境变量名
 * @param value 环境变量值
 * @return bool 是否成功
 * @throws 运行时错误 (错误类型: ikun_sysapi 错误码: 006)
 */
bool set_env_var(const string& name, const string& value)
{
#ifdef _WIN32
    if (SetEnvironmentVariableA(name.c_str(), value.c_str()) == 0)
    {
        ikun_error::throw_re(
            "Failed to set environment variable: " + name,
            internal::LIB_NAME,
            "set_environment_variable",
            "ikun_sysapi 006"
        );
        return false;
    }
#else
    if (setenv(name.c_str(), value.c_str(), 1) != 0)
    {
        ikun_error::throw_re(
            "Failed to set environment variable: " + name + " - " + string(strerror(errno)),
            internal::LIB_NAME,
            "set_environment_variable",
            "ikun_sysapi 005"
        );
        return false;
    }
#endif
    return true;
}

/**
 * @brief 检查是否为目录
 * @param path 路径
 * @return bool 是否为目录
 */
bool is_dir(const string& path)
{
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(path.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    ikun_error::throw_logic_error("Error: is_dir() is not implemented on non-Windows platforms",
    "sysapi.hpp", "is_dir", "None");
    return false; // 这行代码永远不会被执行
#endif
}

} // namespace ikun_sysapi

#endif // IKUN_SYSAPI_HPP