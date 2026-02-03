// ikun库错误分析器
// 支持ikun库抛出的错误代码分析

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#include <print>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const string default_message =
"非标准错误码, 请向开发者反馈错误码\n"
"ikun_error.log文件中记录了错误码, 正确的错误码应该是类型(如ikun_input)和数字(如001, 在读取时会被处理为1)\n"
"或者错误分析器版本不是最新的(如果安装了新版的库, 请运行build_lib.py重新生成错误分析器)\n"
"再或者使用了版本分支不同的库, 如:错误分析器使用了main分支, 而要分析的库使用了preview分支";

void print_error(string error_message)
{
    print("{}", error_message);
    ofstream log_file = ofstream("ikun_error_analyse.log", ios::app);
    auto timestamp = time(nullptr);
    log_file << "分析时间: " << ctime(&timestamp) << endl << error_message << endl;
    log_file.close();
    println("请向开发者反馈分析内容, 可在当前目录找到ikun_error_analyse.log找到输出日志\n");
}

void ikun_input_error_analyzer(int error_code)
{
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message = 
            "分析结果: \n"
            "  类型: 参数不合法\n"
            "  原因: 在开发时, 在input_from_file()函数中没有提供文件名\n"
            "  解决方案: \n"
            "    1. 检查input_from_file()函数的参数是否正确\n"
            "    2. 确保在调用input_from_file()函数时提供了文件名\n"
            "    3. 如果不需要使用文件输入, 请使用input()而非input_from_file()\n"
            "    4. 如果需要使用文件输入, 请确保文件名正确\n";
            break;
        case 2:
            error_message = 
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: input_from_file()函数打开文件失败\n"
            "  解决方案: \n"
            "    1. 检查文件名是否正确\n"
            "    2. 确保文件存在\n"
            "    3. 确保文件路径正确\n"
            "    4. 确保程序有权限访问文件\n";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

void ikun_hpd_error_analyzer(int error_code) // 高精度整数库错误分析器
{
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message =
            "分析结果: \n"
            "  类型: 参数不合法\n"
            "  原因: 在使用high_precision_digit库bigint类构造函数时, 提供的字符串中包含非法字符\n"
            "  如: 非数字字符, 非法字符等\n"
            "  解决方案: \n"
            "    2. 确保字符串中不包含非法字符\n"
            "    2. 如果需要使用非数字字符(如科学计数法), 请使用其他方式, 本库高精度整数库暂不支持\n";
            break;
        case 2:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用high_precision_digit库bigint类除法运算时, 除数为0\n"
            "  解决方案: \n"
            "    确保除数不为0\n";
            break;
        case 3:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用high_precision_digit库bigint类取模运算时, 模数为0\n"
            "  解决方案: \n"
            "    确保模数不为0\n";
            break;
        case 4:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用high_precision_digit库bigint类pow()/big_pow()函数时, 提供的指数为负数\n"
            "  解决方案: \n"
            "    确保指数为非负数\n";
            break;
        case 5:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用high_precision_digit库bigint类sqrt()函数时, 提供的数为负数\n"
            "  解决方案: \n"
            "    不要对负数开平方根, 本库不支持虚数返回值\n";
            break;
        case 6:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用high_precision_digit.hpp的fac()函数时, 提供的数为负数\n"
            "  解决方案: \n"
            "    不要对负数求阶乘\n";
            break;
        case 7:
            error_message =
            "分析结果: \n"
            "  类型: 数值超出范围\n"
            "  原因: 在使用high_precision_digit.hpp的to_long_long()或to_int()函数时, 提供的数 > INT64_MAX或 < INT64_MIN\n"
            "  解决方案: \n"
            "    确保提供的数在long long范围内(-9223372036854775808 ~ 9223372036854775807)\n";
            break;
        case 8:
            error_message =
            "分析结果: \n"
            "  类型: 数值超出范围\n"
            "  原因: 在使用high_precision_digit.hpp的to_int()函数时, 提供的数 > INT32_MAX或 < INT32_MIN\n"
            "  解决方案: \n"
            "    确保提供的数在int范围内(-2147483648 ~ 2147483647)\n";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

void ikun_file_error_analyzer(int error_code)
{
    // 1. 无法打开文件(rfile()读取文件)
    // 2. 无法创建/打开文件(wfile()写入文件)
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用files.hpp库rfile()函数时, 无法打开文件\n"
            "  解决方案: \n"
            "    1. 确保文件存在\n"
            "    2. 确保文件路径正确\n"
            "    3. 确保程序有权限访问文件\n";
            break;
        case 2:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用files.hpp库wfile()函数时, 无法创建/打开文件\n"
            "  解决方案: \n"
            "    1. 确保文件路径合法\n"
            "    2. 确保程序有权限创建文件\n"
            "    3. 确保文件名合法(如在Windows上不能用CON等保留字, 特殊字符等作为文件名)\n";
            break;
        case 3:case 4:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 在使用files.hpp库filedir()函数时, 发生未知错误(错误由filesystem库抛出)\n";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

void core_error_analyzer(int error_code) // 库管理程序错误
{
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 下载模块时, ikun库管理程序无法初始化git\n"
            "  解决方案: \n"
            "    1. 确保git已经安装\n"
            "    2. 确保git已经正确配置(如环境变量)\n"
            "    3. 尝试删除ikun(.exe)和ikun_core.cpp, 并重新运行build_lib.py构建库管理程序\n";
            break;
        case 2:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 下载模块时, ikun库管理程序无法添加git远程仓库\n"
            "  解决方案: \n"
            "    1. 确保git已经正确配置(如环境变量)\n"
            "    2. 确保有正确的网络环境可以访问GitHub\n"
            "提示: \n"
            "  可以从Microsoft Store下载Watt Toolkit后, 在加速界面勾选GitHub开始加速"
            "  Linux环境自己解决科学上网";
            break;
        case 3:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 下载模块时, ikun库管理程序无法启用git稀疏检出\n"
            "  解决方案: \n"
            "    确保git已经正确配置(如环境变量)\n";
            break;
        case 4:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 下载模块时, ikun库管理程序无法创建git稀疏检出文件\n"
            "  解决方案: \n"
            "    确保程序有权限创建文件\n";
            break;
        case 5:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 下载模块时, ikun库管理程序无法通过git拉取文件\n"
            "  解决方案: \n"
            "    1. 确保git已经正确配置(如环境变量)\n"
            "    2. 确保有正确的网络环境可以访问GitHub\n"
            "提示: \n"
            "  输入core_error 002查看提示";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

void ikun_maths_error_analyzer(int error_code) // 数学库错误
{
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message =
            "分析结果: \n"
            "  类型: 参数不合法\n"
            "  原因: 在使用maths.hpp库的fac()函数时, 提供的数为负数\n"
            "  解决方案: \n"
            "    不要对负数求阶乘\n";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

void ikun_random_error_analyzer(int error_code)
{
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message =
            "分析结果: \n"
            "  类型: 参数不合法\n"
            "  原因: 在使用random.hpp库的Random::random_int()或Random::random_double()函数时, 提供的min超过了max\n"
            "  解决方案: \n"
            "    确保min小于等于max\n";
            break;
        case 2:
            error_message =
            "分析结果: \n"
            "  类型: 参数不合法\n"
            "  原因: 在使用random.hpp库的Random::random_string()函数时, 提供的长度为负数\n"
            "  解决方案: \n"
            "    确保长度大于等于0\n";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

void ikun_sysapi_error_analyzer(int error_code)
{
    string error_message;
    switch (error_code)
    {
        case 1:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 执行Shell命令时失败，无法创建管道或进程\n"
            "  解决方案: \n"
            "    1. 检查命令语法是否正确\n"
            "    2. 确保系统资源充足（如文件描述符）\n"
            "    3. 检查程序是否有足够权限执行命令\n"
            "    4. 确保命令存在于系统PATH中或使用绝对路径\n";
            break;
        case 2:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 执行Shell命令时，系统调用失败\n"
            "  解决方案: \n"
            "    1. 检查命令是否存在或语法正确\n"
            "    2. 确保程序有执行权限\n"
            "    3. 检查系统资源是否充足\n";
            break;
        case 3:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 获取当前工作目录失败\n"
            "  解决方案: \n"
            "    1. 检查程序是否有读取当前目录的权限\n"
            "    2. 确保目录未被删除或移动\n"
            "    3. 检查系统文件系统状态\n";
            break;
        case 4:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 更改工作目录失败\n"
            "  解决方案: \n"
            "    1. 检查目标路径是否存在\n"
            "    2. 确保程序有访问目标目录的权限\n"
            "    3. 检查路径语法是否正确\n";
            break;
        case 5:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 设置环境变量失败\n"
            "  解决方案: \n"
            "    1. 检查环境变量名是否合法\n"
            "    2. 确保程序有权限设置环境变量\n"
            "    3. 检查系统环境变量空间是否充足\n";
            break;
        case 6:
            error_message =
            "分析结果: \n"
            "  类型: 运行时错误\n"
            "  原因: 获取本地时间失败(localtime_s/localtime_r调用失败)\n"
            "  解决方案: \n"
            "    1. 检查提供的时间值是否有效（非负值）\n"
            "    2. 确保系统时区设置正确\n"
            "    3. 检查系统时间设置是否正常\n"
            "    4. 确保程序有访问系统时间信息的权限\n"
            "注意: 此函数内部使用localtime_s(Win)或localtime_r(POSIX)安全函数\n";
            break;
        default:
            error_message = default_message;
            break;
    }

    print_error(error_message);
}

int main()
{
    println("-------------------------------");
    println("ikun库错误分析器");
    println("版本: 7.1.2 Release Build 2026.2");
    println("-------------------------------");
    println("如何获取错误代码:");
    println("  1. 在使用ikun库开发的程序中, 当出现错误时, 程序会输出错误代码到控制台和ikun_error.txt文件中");
    println("  2. 打开ikun_error.txt文件, 复制错误代码");
    println("  3. 将错误代码粘贴到本程序中");
    println("为什么没有错误代码:");
    println("  1. 程序没有使用ikun库");
    println("  2. 程序没有出现基于ikun_stderr.hpp定义的错误(如使用throw关键字抛出异常而不是使用ikun库的throw_re/throw_inv_arg等函数抛出异常)");

    while (1)
    {
        print("请输入错误代码, 输入exit退出程序: ");
        string error_type;
        int error_code;
        cin >> error_type >> error_code;
        if (error_type == "ikun_input")
        {
            println("\n正在分析中...");
            ikun_input_error_analyzer(error_code);
        }
        else if (error_type == "ikun_hpd")
        {
            println("\n正在分析中...");
            ikun_hpd_error_analyzer(error_code);
        }
        else if (error_type == "ikun_file")
        {
            println("\n正在分析中...");
            ikun_file_error_analyzer(error_code);
        }
        else if (error_type == "core_error")
        {
            println("\n正在分析中...");
            core_error_analyzer(error_code);
        }
        else if (error_type == "ikun_maths")
        {
            println("\n正在分析中...");
            ikun_maths_error_analyzer(error_code);
        }
        else if (error_type == "ikun_sysapi")
        {
            println("\n正在分析中...");
            ikun_sysapi_error_analyzer(error_code);
        }
        else if (error_type == "exit")
        {
            return 0;
        }
        else
        {
            println("错误: 错误代码不正确");
        }
    }
    return 0;
}