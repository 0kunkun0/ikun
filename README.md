## ikun库概述
- ikun库是一个跨平台的C++编程工具库, 支持Windows, Linux和macOS系统, 遵循GPL 3.0许可证, 此版本是7.1.2 Release Build 2026.2正式版
 - 提供了丰富的功能, 包括但不限于:
 文件操作(files.hpp)
 数学运算(maths.hpp)
 随机数生成(random.hpp)
 时间处理(times.hpp)
 错误处理(ikun_stderr.hpp)
 控制台颜色输出(console_color.hpp)
 输入输出整合(io.hpp)
 高精度整数运算(high_precision_digit.hpp)
 C++标准库整合(stdc++lib.hpp)
 跨平台操作系统API调用(sysapi.hpp)等
 - 允许在开源的情况下向ikun库添加扩展功能, 但必须遵循GPL 3.0许可证规定

- 开发工具方面, 对开发者提供错误分析器(仅限本库的代码)`ikun_error_analyzer.cpp`和库/项目管理工具`ikun_core.cpp`(编译后应为`ikun(.exe)`)

- 法律方面, ikun库遵循[GPL 3.0](https://www.gnu.org/licenses/gpl-3.0.html)许可证, **禁止使用本库进行闭源用途**
[本库GitHub仓库](https://github.com/0kunkun0/ikun)
---
## 🚀 快速开始
### 1. 获取库文件
- 可以通过克隆GitHub仓库获取最新Release版源码:
```bash
git clone https://github.com/0kunkun0/ikun.git
```

### 2. 构建库
- 在ikun库目录下运行`build.bat`或`build.sh`脚本, 根据系统选择合适的脚本
---
```text
./(你的工作区路径)
你的项目文件...
ikun/
    ikun_core.hpp   // ikun.exe原代码
    build.bat       // 构建库的批处理文件(Windows)
    build.sh        // 构建库的shell脚本(Unix-like)
    build_lib.py    // 构建库的脚本(不建议直接使用, 而是通过build.bat检查环境再使用)
    ikun(.exe)      // 通过build*.*预构建的可执行文件 (通常不建议下载预构建版本, 目前提供Windows和Linux版本)

推荐的一些库:
    files.hpp       // 文件相关功能   (namespace files)
    times.hpp       // 时间相关功能   (namespace times)
    maths.hpp       // 数学相关功能   (namespace maths)
    random.hpp      // 随机数相关功能 (namespace random_)
    all_libs.hpp    // 包含所有库的头文件
    high_precision_digit.hpp         (namespace high_precision_digit)
                    // 高精度数字相关功能
    stdc++lib.hpp   // 完整的C++标准库
    console_color.hpp                (namespace color)
                    // 控制台颜色
    input.hpp       // 输入相关功能   (namespace ikun_input)
    io.hpp          // 输入输出相关功能
    sysapi.hpp      // 系统API相关功能 (namespace sysapi)

推荐的工具:
    ikun_error_analyzer.cpp
                    // 本库运行时错误分析工具
    test.cpp        // 本库测试工具

其它详见ikun库GitHub仓库:
    https://github.com/0kunkun0/ikun
```
---
## ⚙️ ikun库错误分析(可直接用于AI或发送给开发者, 详见ikun_error_analyzer.cpp):

- ikun_input: (输入相关错误, `input.hpp`)
 - 001: 在`input_from_file()`**函数中没有提供文件名**(Invalid Argument)
 - 002: `input_from_file()`**打开文件失败**(Runtime Error)

- ikun_hpd: (高精度整数库错误, `high_precision_digit.hpp`)
 - 001: `high_precision_digit::bigint`类**构造函数提供的字符串中包含非法字符**(Invalid Argument)
 - 002: **除数为0**(Runtime Error)
 - 003: **模数为0**(Runtime Error)
 - 004: 使用`high_precision_digit::bigint::pow()/big_pow()`函数时, **提供的指数为负数**(Runtime Error)
 - 005: 使用`high_precision_digit::bigint::sqrt()`函数**对负数开平方根(不支持虚数返回值)**(Runtime Error)
 - 006: 在使用`high_precision_digit::bigint::fac()`函数时, **对负数求阶乘**
 - 007: 在使用`high_precision_digit::bigint::to_long_long()`或`to_int()`函数时, **整数超出int64_t范围**(Out of Range)
 - 008: 在使用`high_precision_digit::bigint::to_int()`函数时, **整数超出int32_t范围**(Out of Range)

- ikun_file: (文件相关错误, `files.hpp`)
 -  001: `files::rfile()`(读取整个文件)**无法打开文件**(Runtime Error)
 - 002: `files::wfile()`(写入整个文件)**无法打开文件**(Runtime Error)
 - 003:004: `files::filedir()`函数发生未知错误 **(错误由filesystem库抛出)** (Runtime Error)

- core_error: (管理程序运行时错误, `ikun_core.cpp`或编译后的`ikun(.exe)`)
 - 001: 无法初始化git
 - 002: 无法添加git远程仓库
 - 003: 无法启用git稀疏检出
 - 004: 无法创建git稀疏检出文件
 - 005: 无法通过git拉取文件
 - 006: 创建项目时不支持目标平台

- ikun_maths: (数学相关错误, `maths.hpp`)
 - 001: 在使用maths::fac()函数时, 对负数求阶乘 (Invalid Argument)

- ikun_random: (随机数相关错误, `random.hpp`)
 - 001: 在使用`random_::Random::random_int()`或`random_double()`时, min大于max (Invalid Argument)
 - 002: 在使用`random_::Random::random_string()`函数生成随机字符串时, 提供的长度为负数(Invalid Argument)

- ikun_sysapi: (系统API相关错误, `sysapi.hpp`)
 - 001: 执行Shell命令时失败, 无法创建管道或进程
 - 002: 执行Shell命令时, 系统调用失败
 - 003: 获取当前工作目录失败
 - 004: 更改工作目录失败
 - 005: 设置环境变量失败
 - 006: 获取本地时间失败(localtime_s/localtime_r调用失败)
---
## ikun库使用示例:

### files.hpp:
```cpp
namespace files
    bool fileexists(const string& filename)// 判断文件是否存在
    vector<string> filedir(string path = ".", string fileextname = ".")                          // 获取指定目录下的所有文件名
    string rfile(const string& filename)
                                           // 读取整个文件
    void wfile(const string& filename, const string& content)
                                           // 写入整个文件
    bool check_file_name(string file_name) // 检查文件名是否合法
    bool check_dir_name(string dir_name)   // 检查目录名是否合法
    void mkdir(const string& path)    // 创建目录
```
### high_precision_digit.hpp
性能提示：
- 对于小数字(< 2^63), 直接使用 long long 可能更快
- 频繁的字符串转换会影响性能
- 除法是最慢的操作, 尽量避免
- Karatsuba乘法在数字位数 > 64时自动启用
- 使用移动语义避免不必要的拷贝
```cpp
namespace high_precision_digit
    class bigint
        bigint(const string& str)                    // 构造函数
        bigint(int64_t num)                          // 构造函数
        重载运算符(+-*/%)等
        重载比较运算符(==, !=, <, <=, >, >=)
        重载输入输出运算符(>>, <<)
        static bigint fac(int64_t n)                  // 阶乘
        static bigint sqrt(const bigint& n)           // 开平方根
        static bigint pow(const bigint& n, int64_t p) // 幂
        long long to_long_long()                      // 转换为long long
        使用方法: long long a = 类.to_long_long()
        int to_int()                                  // 转换为int
        使用方法: int a = 类.to_int()
```
---
### random.hpp
```cpp
namespace random_
    class Random
        int random_int(int min, int max)              // 生成[min, max]范围内的随机整数
        double random_double(double min, double max)  // 生成[min, max]范围内的随机浮点数
        string random_string(int length)              // 生成指定长度的随机字符串
        char random_char()                            // 生成随机字符
        bool random_bool()                            // 生成随机布尔值
        void reseed()                                 // 重新播种随机数生成器
```
---
## 🔧 错误代码格式:
```text
    ikun lib 错误类型(如Runtime Error, Invalid Argument等)!
    Traceback(most recent call last):
    From library 抛出错误的库
        In function 抛出错误的函数:
            错误类型: 错误信息\n
    If you are the developer, please check your code.
    If you are the user, please report this error to the developer.
    Error Code: 错误代码
    Log file ikun_error.log has been created in the current directory.
```
---
## 预编译二进制文件编译环境:
| 平台 | 编译后的二进制文件名 | 编译器 | 编译时指定的C++版本 | 运行依赖 |
| --- | --- | --- | --- | --- |
| Windows | ikun.exe | MSVC | /std:c++latest(C++26) | 静态链接 |
| Windows | ikun_error_analyzer.exe | MSVC | /std:c++latest(C++26) | 静态链接 |
| Windows | ikun.exe | GCC | std=c++23 | 静态链接 |
| Windows | ikun_error_analyzer.exe | GCC | std=c++23 | 静态链接 |
| Windows | ikun.exe | Clang/LLVM | std=c++2b | `libc++.dll`<br>`libgcc_s_seh-1.dll` |
| Windows | ikun_error_analyzer.exe | Clang/LLVM | std=c++2b | `libc++.dll`<br>`libgcc_s_seh-1.dll` |
| Linux | ikun | GCC | -std=c++2b | 静态链接 |
| Linux | ikun_error_analyzer | GCC | -std=c++2b | 静态链接 |
---
**(可将本文件直接喂给AI)**