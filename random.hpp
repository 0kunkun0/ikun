// ikun库随机数相关模块random.hpp
// 为了Linux上的兼容性, 此处使用random_作为命名空间

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_RANDOM_HPP
#define IKUN_RANDOM_HPP

#include <random>
#include <ctime>

namespace random_
{
    int random_int(int min, int max) // 区间随机数生成器
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    char random_char() // 随机可显示ASCII字符生成器
    {
        return static_cast<char>(random_int(32, 126));
    }

    /**
     * @brief 随机字符串生成器
     * @brief 仅支持可显示ASCII字符
     * @param length 字符串长度
     * @return std::string 字符串内容    
    */
    std::string random_string(int length)
    {
        std::string str;
        for (int i = 0; i < length; i ++)
        {
            str += random_char();
        }

        return str;
    }
}

#endif // IKUN_RANDOM_HPP