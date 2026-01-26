// ikun库随机数相关模块random.hpp
// 为了Linux上的兼容性, 此处使用random_作为命名空间
// 版本7.1.2 Release Candidate

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#ifndef IKUN_RANDOM_HPP
#define IKUN_RANDOM_HPP

#include <random>
#include <ctime>
#include "ikun_stderr.hpp"

namespace random_
{
    using ikun_error::throw_inv_arg;
    using ikun_error::throw_out_of_range;

    /**
     * @brief 随机数生成器类
     */
    class Random
    {
        private:
            std::random_device rd;
            std::mt19937 gen;
        
        public:
            Random() : gen(rd()) {}
            Random& operator=(const Random&) = delete; // 禁止拷贝构造

            /**
             * @brief 区间随机数生成器
             * @param min 最小值
             * @param max 最大值
             * @return int 随机数
             */
            int random_int(int min, int max)
            {
                if (min > max)
                {
                    throw_inv_arg("min不能大于max",
                    "random.hpp", "Random::random_int()", "ikun_random 001"
                    );
                }

                std::uniform_int_distribution<> dis(min, max);
                return dis(gen);
            }

            /**
             * @brief 随机字符生成器
             * @return char 随机字符
             */
            char random_char()
            {
                return static_cast<char>(random_int(32, 126));
            }

            /**
             * @brief 随机字符串生成器
             * @brief 仅支持可显示ASCII字符
             * @param length 字符串长度(无符号整数)
             * @return std::string 字符串内容    
            */
            std::string random_string(int length)
            {
                if (length < 0)
                {
                    throw_out_of_range("length不能小于0",
                    "random.hpp", "Random::random_string()", "ikun_random 002"
                    );
                }

                std::string str;
                str.reserve(length);
                for (int i = 0; i < length; i ++)
                {
                    str += random_char();
                }

                return str;
            }

            /**
             * @brief 随机浮点数生成器
             * @param min 最小值
             * @param max 最大值
             * @return double 随机浮点数
             */
            double random_double(double min, double max)
            {
                if (min > max)
                {
                    throw_inv_arg("min不能大于max",
                    "random.hpp", "Random::random_double()", "ikun_random 001"
                    );
                }
                
                std::uniform_real_distribution<> dis(min, max);
                return dis(gen);
            }
            
            bool random_bool()
            {
                std::uniform_int_distribution<> dis(0, 1);
                return dis(gen) == 1;
            }

            /**
             * @brief 重新生成随机数种子
             */
            void reseed()
            {
                gen.seed(rd());
            }
    };
}

#endif // IKUN_RANDOM_HPP