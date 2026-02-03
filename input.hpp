//Dependencies: ikun_stderr
// ikun库处理输入库input.hpp
// 格式类似Python的input函数, 基于模板实现, 需要在输入时指定类型
// 命名空间ikun_input

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_INPUT_HPP
#define IKUN_INPUT_HPP

#ifndef WIN32_LEAD_AND_MEAN
#define WIN32_LEAD_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <print>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "ikun_stderr.hpp"

namespace ikun_input
{
    using std::cin;
    using std::string;
    using std::println;
    using std::getline;
    using std::ifstream;
    using std::ofstream;
    using std::fstream;
    using std::stringstream;
    using std::runtime_error;
    using ikun_error::throw_re;
    using ikun_error::throw_inv_arg;

    // 常规输入
    /**
     * @brief 从标准输入获取数据
     * @param output 输出提示信息, 默认为空字符串
     * @return 输入的数据
     */
    template <typename T>
    T input(string output = "")
    {
        T i;
        if (!output.empty())
        {
            println("{}", output);
        }
        cin >> i;
        return i;
    }

    /**
     * @brief 从标准输入获取数据
     * @param output 输出提示信息, 默认为空字符串
     * @return 输入的数据
     */
    template <>
    string input<string>(string output)
    {
        string i;
        if (!output.empty())
        {
            println("{}", output);
        }
        getline(cin, i);
        return i;
    }

    /**
     * @brief 从文件获取数据
     * @param file_name 文件名, 默认为空字符串
     * @param getline_ 是否获取一整行内容, 默认为true
     * @return 输入的数据
     */
    string input_from_file(string file_name = "", bool getline_ = true)
    {
        string in;
        if (file_name == "")
        {
            throw_inv_arg(
                "The argument file_name is required, but it is empty.", 
                "input.hpp", "input_from_file()", "ikun_input 001"
            );

        }

        ifstream if_(file_name);
        if (!if_.is_open())
        {
            throw_re("The file " + file_name + " does not exist.",
            "input.hpp", "input_from_file()", "ikun_input 002");
        }
        
        if (getline_)
        {
            getline(if_, in);
        }
        else
        {
            if_ >> in;
        }

        return in;
    }

    template <typename T>
    /**
     * @brief 从stringstream获取数据
     * @param i stringstream对象
     * @param getline_ 是否获取一整行内容, 默认为true
     * @return 输入的数据
     */
    T input_from_ss(stringstream &i, bool getline_ = true)
    {
        T in;
        if (getline_)
        {
            getline(i, in);
        }
        else
        {
            i >> in;
        }
        return in;
    }
}

#endif // IKUN_INPUT_HPP