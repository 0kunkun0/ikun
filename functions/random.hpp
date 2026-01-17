// ikun库functions模块随机数库random.hpp
// 提供基于mt19937算法的随机数生成器
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据许可证和用户协议规定, 禁止商业用途, 违者依法追究法律责任

#ifndef IKUN_FUNCTIONS_RANDOM_HPP
#define IKUN_FUNCTIONS_RANDOM_HPP

#ifndef IKUN_CORE
    #error "请先使用#include "ikun/core.hpp"包含ikun库核心"
    #error "否则无法使用版本管理功能"
#endif // IKUN_CORE

#include <random>
#include <string>

namespace random
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

#endif // IKUN_FUNCTIONS_RANDOM_HPP