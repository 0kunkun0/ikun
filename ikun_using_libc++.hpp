// ikun库自动包含命名空间模块 ikun_using_libc++.hpp
// 请在代码#include指令最下面包含本头文件
// 注: 由于命名空间std可能导致某些冲突, 故本文件只添加常用的std成员
// 本文件使用的是libc++标准库, 需要编译器为Clang(非clang-cl模式)

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

// 仅在Clang编译器下启用
#ifndef _LIBCPP_
    #error 错误的编译器
    #ifdef _GLIBCXX_
        #error 请使用ikun_using_libstdc++.hpp
    #elifdef _MSC_VER
        #error 请使用ikun_using_msvc.hpp
    #endif
#endif

#ifndef IKUN_USING_LIBCPP_HPP
#define IKUN_USING_LIBCPP_HPP

// 基础I/O
#ifdef _LIBCPP_IOSTREAM_
#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::clog;
using std::endl;
using std::flush;
using std::wcin;
using std::wcout;
using std::wcerr;
using std::wclog;
using std::wendl;
using std::getline;
#endif

#ifdef _LIBCPP_FORMAT_
#include <format>
using std::format;
#endif

#ifdef _LIBCPP_PRINT_
#include <print>
using std::print;
using std::println;
#endif

// 字符串
#ifdef _LIBCPP_STRING_
#include <string>
using std::string;
using std::wstring;
using std::getline;
#endif

// 向量
#ifdef _LIBCPP_VECTOR_
#include <vector>
using std::vector;
#endif

// 列表
#ifdef _LIBCPP_LIST_
#include <list>
using std::list;
#endif

// 双端队列
#ifdef _LIBCPP_DEQUE_
#include <deque>
using std::deque;
#endif

// 数组
#ifdef _LIBCPP_ARRAY_
#include <array>
using std::array;
#endif

// 映射
#ifdef _LIBCPP_MAP_
#include <map>
using std::map;
using std::multimap;
#endif

// 无序映射
#ifdef _LIBCPP_UNORDERED_MAP_
#include <unordered_map>
using std::unordered_map;
using std::unordered_multimap;
#endif

// 集合
#ifdef _LIBCPP_SET_
#include <set>
using std::set;
using std::multiset;
#endif

// 无序集合
#ifdef _LIBCPP_UNORDERED_SET_
#include <unordered_set>
using std::unordered_set;
using std::unordered_multiset;
#endif

// 栈
#ifdef _LIBCPP_STACK_
#include <stack>
using std::stack;
#endif

// 队列
#ifdef _LIBCPP_QUEUE_
#include <queue>
using std::queue;
using std::priority_queue;
#endif

// 算法
#ifdef _LIBCPP_ALGORITHM_
#include <algorithm>
using std::sort;
using std::reverse;
using std::find;
using std::count;
// using std::max; // 与Windows API的max冲突
// using std::min; // 与Windows API的min冲突
using std::swap;
using std::fill;
using std::copy;
using std::transform;
using std::accumulate;
using std::for_each;
using std::binary_search;
#endif

// 数值算法
#ifdef _LIBCPP_NUMERIC_
#include <numeric>
using std::accumulate;
using std::inner_product;
using std::partial_sum;
using std::iota;
#endif

// 函数对象
#ifdef _LIBCPP_FUNCTIONAL_
#include <functional>
using std::function;
using std::less;
using std::greater;
using std::equal_to;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;
#endif

// 迭代器
#ifdef _LIBCPP_ITERATOR_
#include <iterator>
using std::begin;
using std::end;
using std::advance;
using std::distance;
#endif

// 工具
#ifdef _LIBCPP_UTILITY_
#include <utility>
using std::pair;
using std::make_pair;
using std::move;
using std::forward;
#endif

// 智能指针
#ifdef _LIBCPP_MEMORY_
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::make_shared;
using std::make_unique;
#endif

// 动态数组
#ifdef _LIBCPP_VALARRAY_
#include <valarray>
using std::valarray;
#endif

// 复数
#ifdef _LIBCPP_COMPLEX_
#include <complex>
using std::complex;
#endif

// 随机数
#ifdef _LIBCPP_RANDOM_
#include <random>
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
#endif

// 类型特性
#ifdef _LIBCPP_TYPE_TRAITS_
#include <type_traits>
using std::is_integral;
using std::is_floating_point;
using std::is_pointer;
using std::remove_reference;
using std::remove_pointer;
#endif

// 时间
#ifdef _LIBCPP_CHRONO_
#include <chrono>
using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::time_point;
#endif

// 线程
#ifdef _LIBCPP_THREAD_
#include <thread>
using std::thread;
using std::this_thread::sleep_for;
using std::this_thread::sleep_until;
using std::this_thread::yield;
#endif

// 互斥锁
#ifdef _LIBCPP_MUTEX_
#include <mutex>
using std::mutex;
using std::recursive_mutex;
using std::lock_guard;
using std::unique_lock;
#endif

// 条件变量
#ifdef _LIBCPP_CONDITION_VARIABLE_
#include <condition_variable>
using std::condition_variable;
using std::condition_variable_any;
#endif

// 原子操作
#ifdef _LIBCPP_ATOMIC_
#include <atomic>
using std::atomic;
using std::atomic_int;
using std::atomic_bool;
using std::atomic_flag;
#endif

// 异常
#ifdef _LIBCPP_EXCEPTION_
#include <exception>
using std::exception;
#endif

// 类型信息
#ifdef _LIBCPP_TYPEINFO_
#include <typeinfo>
using std::type_info;
#endif

// 可选值
#ifdef _LIBCPP_OPTIONAL_
#include <optional>
using std::optional;
using std::nullopt;
#endif

// 变体
#ifdef _LIBCPP_VARIANT_
#include <variant>
using std::variant;
using std::visit;
#endif

// 任意类型
#ifdef _LIBCPP_ANY_
#include <any>
using std::any;
using std::any_cast;
#endif

// 文件流
#ifdef _LIBCPP_FSTREAM_
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::fstream;
#endif

// 字符串流
#ifdef _LIBCPP_SSTREAM_
#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;
#endif

// C风格字符串操作
#ifdef _LIBCPP_CSTRING_
#include <cstring>
#endif

// C风格数学函数
#ifdef _LIBCPP_CMATH_
#include <cmath>
#endif

// C风格标准库
#ifdef _LIBCPP_CSTDLIB_
#include <cstdlib>
#endif

// 警告: 这个宏会引入整个std命名空间, 慎用!
#define USE_STD using namespace std;

#endif // IKUN_USING_MSVC_HPP