// ikun库错误信息库ikun_stderr.hpp
// 用于为库中的其它头文件提供错误信息
// 版本7.1.2 Preview Build 2026.1.24

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_STDERR_HPP
#define IKUN_STDERR_HPP

#include <iostream>
#include <print>
#include <string>
#include <stdexcept>
#include <fstream>
#include <regex>

namespace stderr_color
{
    constexpr const char* reset = "\033[0m";
    constexpr const char* red = "\033[31m";
}

namespace ikun_error
{
    using std::print;
    using std::println;
    using std::string;
    using std::runtime_error;
    using std::overflow_error;
    using std::out_of_range;
    using std::logic_error;
    using std::ofstream;
    using std::to_string;
    using std::cout;
    using std::cerr;
    using std::ifstream;
    using std::regex;
    using std::regex_replace;
    using namespace stderr_color;

    void remove_ansi_color()
    {
        ifstream in("ikun_temp_log.txt");
        ofstream out("ikun_error.log");
        regex ansi_regex("\\x1b\\[[0-9;]*m");
        string line;
        
        while (getline(in, line))
        {
            string cleaned = regex_replace(line, ansi_regex, "");
            out << cleaned << "\n";
        }
    }

    void throw_re(string error_info, string where_lib, string where_func, string err_code) // 抛出运行时错误
    {
        string output_err_info = 
            string(ikun_error::red) + "ikun lib Runtime Error!\n"
            "Traceback(most recent call last):\n" + string(ikun_error::reset) +
            "From library" + where_lib + ":\n"
            "    In function " + where_func + ":\n"
            "        " + string(ikun_error::red) + "Runtime Error: " + error_info + "\n" + string(ikun_error::reset) +
            "If you are the developer, please check your code.\n"
            "If you are the user, please report this error to the developer.\n"
            + string(ikun_error::red) + "Error Code: " + err_code + "\n" + string(ikun_error::reset);
        ofstream error_log = ofstream("ikun_temp_log.txt");
        error_log << output_err_info;
        error_log.close();
        remove_ansi_color();
        println("{}\nLog file ikun_error.log has been created in the current directory.", output_err_info);
        cout.flush();
        cerr.flush();
        throw runtime_error(output_err_info);
    }

    void throw_inv_arg(string error_info, string where_lib, string where_func, string err_code) // 抛出无效参数错误
    {
        string output_err_info = 
            string(ikun_error::red) + "ikun lib Invalid Argument Error!\n"
            "Traceback(most recent call last):\n" + string(ikun_error::reset) +
            "From library" + where_lib + ":\n"
            "    In function " + where_func + ":\n"
            "        " + string(ikun_error::red) + "Invalid Argument Error: " + error_info + "\n" + string(ikun_error::reset) +
            "If you are the developer, please check your code.\n"
            "If you are the user, please report this error to the developer, but you shouldn't see this error because it's a code error.\n"
            + string(ikun_error::red) + "Error Code: " + err_code + "\n" + string(ikun_error::reset);
        ofstream error_log = ofstream("ikun_temp_log.txt");
        error_log << output_err_info;
        error_log.close();
        remove_ansi_color();
        println("{}\nLog file ikun_error.log has been created in the current directory.", output_err_info);
        cout.flush();
        cerr.flush();
        throw runtime_error(output_err_info);
    }

    void throw_out_of_range(string error_info, string where_lib, string where_func, string err_code) // 抛出越界错误
    {
        string output_err_info = 
            string(ikun_error::red) + "ikun lib Range Error!\n"
            "Traceback(most recent call last):\n" + string(ikun_error::reset) +
            "From library " + where_lib + ":\n"
            "    In function " + where_func + ":\n"
            "        " + string(ikun_error::red) + "Range Error: " + error_info + "\n" + string(ikun_error::reset) +
            "If you are the developer, please check your code.\n"
            "If you are the user, please report this error to the developer.\n"
            + string(ikun_error::red) + "Error Code: " + err_code + "\n" + string(ikun_error::reset);
        ofstream error_log = ofstream("ikun_temp_log.txt");
        error_log << output_err_info;
        error_log.close();
        remove_ansi_color();
        println("{}\nLog file ikun_error.log has been created in the current directory.", output_err_info);
        cout.flush();
        cerr.flush();
        throw out_of_range(output_err_info);
    }

    void throw_stack_overflow(string error_info, string where_lib, string where_func, string err_code) // 抛出栈溢出错误
    {
        string output_err_info = 
            string(ikun_error::red) + "ikun lib Stack Overflow Error!\n"
            "Traceback(most recent call last):\n" + string(ikun_error::reset) +
            "From library " + where_lib + ":\n"
            "    In function " + where_func + ":\n"
            "        " + string(ikun_error::red) + "Stack Overflow Error: " + error_info + "\n" + string(ikun_error::reset) +
            "If you are the developer, please check your code.\n"
            "If you are the user, please report this error to the developer.\n"
            + string(ikun_error::red) + "Error Code: " + err_code + "\n" + string(ikun_error::reset);
        ofstream error_log = ofstream("ikun_temp_log.txt");
        error_log << output_err_info;
        error_log.close();
        remove_ansi_color();
        println("{}\nLog file ikun_error.log has been created in the current directory.", output_err_info);
        cout.flush();
        cerr.flush();
        throw overflow_error(output_err_info);
    }

    void throw_logic_error(string error_info, string where_lib, string where_func, string err_code) // 抛出逻辑错误
    {
        string output_err_info = 
            string(ikun_error::red) + "ikun lib Logic Error!\n"
            "Traceback(most recent call last):\n" + string(ikun_error::reset) +
            "From library" + where_lib + ":\n"
            "    In function " + where_func + ":\n"
            "        " + string(ikun_error::red) + "Logic Error: " + error_info + "\n" + string(ikun_error::reset) +
            "Please check your code.\n"
            + string(ikun_error::red) + "Error Code: " + err_code + "\n" + string(ikun_error::reset);
        ofstream error_log = ofstream("ikun_temp_log.txt");
        error_log << output_err_info;
        error_log.close();
        remove_ansi_color();
        println("{}\nLog file ikun_error.log has been created in the current directory.", output_err_info);
        cout.flush();
        cerr.flush();
        throw logic_error(output_err_info);
    }
}

#endif // IKUN_STDERR_HPP