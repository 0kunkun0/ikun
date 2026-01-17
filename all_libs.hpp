// ikun库all_libs.hpp
// 本文件为ikun库的整合文件, 包含了ikun库的所有头文件
// 包括万能头文件stdc++lib.hpp, 实用函数库functions.hpp, 高精度整数运算high_precision_int.hpp, Windows API相关函数winapi_func.hpp等
// 本库的所有头文件均为互相包含, 使用前请下载完整库, 并确保所有头文件在ikun文件夹下
// 注意: 此头文件会自动包含所有命名空间, 如果不希望自动使用, 请定义IKUN_NONAMESPACE宏
// 注意: 由于自C++17后, Windows API会和C++标准库命名冲突, 如需使用winapi_func.hpp请定义IKUN_NONAMESPACE_STD宏
#ifndef IKUN_ALL_LIBS_HPP
#define IKUN_ALL_LIBS_HPP
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "core.hpp"
#include "stdc++lib.hpp"
#include "functions.hpp"
#include "high_precision_digit.hpp"
#include "winapi_func.hpp"

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