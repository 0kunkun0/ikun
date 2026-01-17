// ikun库all_libs.hpp
// 本文件为ikun库的整合文件, 包含了ikun库的所有头文件
// 包括万能头文件stdc++lib.hpp, 实用函数库functions.hpp, 高精度整数运算high_precision_int.hpp, GitHub相关操作github.hpp等
// 本库的所有头文件均为互相包含, 使用前请下载完整库, 并确保所有头文件在ikun文件夹下
// 注意: 此头文件会自动包含所有命名空间, 如果不希望自动使用, 请定义IKUN_NONAMESPACE宏
// 注意: 由于自C++17后, Windows API会和C++标准库命名冲突, 请定义IKUN_NONAMESPACE_STD宏

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

// 联系作者 (匿名地址, 非作者本人真实邮件地址) : trill.hokey1p@icloud.com
#ifndef IKUN_ALL_LIBS_HPP
#define IKUN_ALL_LIBS_HPP
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "core.hpp"
#include "stdc++lib.hpp"
#include "functions.hpp"
#include "high_precision_digit.hpp"
#include "github.hpp"

#ifndef IKUN_NONAMESPACE

#ifndef IKUN_NONAMESPACE_STD
using namespace std; // C++ 标准库
#endif

#if __has_include (<chrono>) // C++ 标准库
using namespace std::chrono; // C++ 标准库
#endif

#if __has_include (<thread>) // C++ 标准库
using namespace std::this_thread; // C++ 标准库
#endif

#if __has_include (<filesystem>) // C++ 标准库
using namespace std::filesystem; // C++ 标准库
#endif

using namespace libs; // functions/libs.hpp
using namespace maths; // functions/mathematics.hpp
using namespace times; // functions/times.hpp
using namespace random; // functions/random.hpp
using namespace high_precision_digit; // high_precision_digit.hpp
#endif // _IKUN_NONAMESPACE

#endif // ALL_LIBS_HPP