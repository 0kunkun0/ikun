// ikun库all_libs.hpp
// 本文件为ikun库的整合文件, 包含了ikun库的所有自带头文件
// 包括万能头文件stdc++lib.hpp, 实用函数库functions.hpp
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

#if __has_include ("stdc++lib.hpp")
#include "stdc++lib.hpp"
#endif

#if __has_include ("functions.hpp")
#include "functions.hpp"
#endif

#if __has_include ("high_precision_digit.hpp")
#include "high_precision_digit.hpp"
#endif

#if __has_include ("github.hpp")
#include "github.hpp"
#endif

#if __has_include ("print.hpp") // 不是C++标准库别理解错了
#include "print.hpp"
#endif

#if __has_include ("console_color.hpp")
#include "console_color.hpp"
#endif

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

#ifdef IKUN_HIGH_PRECISION_DIGIT_HPP
using namespace high_precision_digit; // high_precision_digit.hpp
#endif

#ifdef IKUN_FUNCTIONS_HPP
using namespace libs; // functions.hpp
using namespace maths; // functions.hpp
using namespace times; // functions.hpp
using namespace random; // functions.hpp
#endif

#ifdef IKUN_CONSOLE_COLOR_HPP
using namespace color; // console_color.hpp
#endif

#ifdef IKUN_HIGH_PRECISION_DIGIT_HPP
using namespace high_precision_digit; // high_precision_digit.hpp
#endif

#endif // IKUN_NONAMESPACE

#endif // ALL_LIBS_HPP