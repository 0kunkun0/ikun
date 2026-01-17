// ikun库万能头文件stdc++lib.hpp
// 类似于libstdc++的bits/stdc++.h, 但通过预处理器指令实现了更多的控制, 而且去掉了很多不必要的头文件
// 对比更适用于竞赛的bits/stdc++.h, 本头文件适用于任何用途
// 包括但不限于开发, 竞赛编程, 快速原型测试等
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据许可证和用户协议规定, 禁止商业用途, 违者依法追究法律责任

#ifndef IKUN_STDCPPLIB_HPP
#define IKUN_STDCPPLIB_HPP

#ifndef IKUN_CORE
    #error "请先使用#include "ikun/core.hpp"包含ikun库核心"
    #error "否则无法使用版本管理功能"
#endif // IKUN_CORE

// 最小化的库包含

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

// 以上适用于竞赛编程

#ifndef DISABLE_UNESSENTIAL_C_HEADER

    #include <cstring> // string
    #include <cstdio> // iostream + 禁用缓冲区同步
                    // 或者format, print, fstream, filesystem
    #include <ctime> // chrono

#endif

#ifndef MIN_HEADER_INCLUDE

#include <fstream>
#include <bitset>
#include <sstream>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <future>
#include <deque>
#include <list>
#include <array>
#include <tuple>
#include <utility>
#include <type_traits>
#include <limits>
#include <cwchar>
#include <locale>
#include <codecvt>
#include <cctype>
#include <cstdint>
#include <cfloat>
#include <climits>
#include <complex>
#include <valarray>
#include <exception>
#include <stdexcept>
#include <new>
#include <memory>

// C++17 特性检测
#if __cplusplus >= 201703L
    #include <filesystem>
    using namespace std::filesystem;
#elif defined(_WIN32) && !defined(__MINGW32__)
    // 对于Windows且不是MinGW的早期版本，可以使用<filesystem>的实验性版本
    #if _MSC_VER >= 1900
        #include <experimental/filesystem>
        using namespace std::experimental::filesystem;
    #endif
#endif

// C++20 特性检测
#if __cplusplus >= 202002L
    #include <format>
#endif

// C++23 特性检测
#if __cplusplus >= 202302L 
    #include <print>
#endif

#endif // MIN_HEADER_INCLUDE

#endif // IKUN_STDCPPLIB_HPP