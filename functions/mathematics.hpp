// ikun库functions模块数学计算相关函数库mathematics.hpp
// 提供数学计算相关函数
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据许可证和用户协议规定, 禁止商业用途, 违者依法追究法律责任

#ifndef IKUN_FUNCTIONS_MATHEMATICS_HPP
#define IKUN_FUNCTIONS_MATHEMATICS_HPP

#ifndef IKUN_CORE
    #error "请先使用#include "ikun/core.hpp"包含ikun库核心"
    #error "否则无法使用版本管理功能"
#endif // IKUN_CORE

#include <cmath>
#include <stdexcept>

const long double PI = 3.141592653589793238L; // 圆周率(受精度限制, 小数点后最多保留17位)
typedef unsigned long long ull;
typedef long long ll;

template <int N>
struct Fac
{
    static constexpr ull value = N * Fac<N - 1>::value;
};

template <>
struct Fac<1>
{
    static constexpr ull value = 1;
};

template <int N>
struct Fib
{
    static constexpr ull value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template <>
struct Fib<1>
{
    static constexpr ull value = 1;
};

template <>
struct Fib<0>
{
    static constexpr ull value = 0;
};

template <int N>
struct Pow
{
    static constexpr ull value = N * Pow<N - 1>::pow;
};

template <>
struct Pow<1>
{
    static constexpr ull value = 1;
};

template <>
struct Pow<0>
{
    static constexpr ull value = 1;
};

namespace maths
{
    ll fac(int n) // 计算阶乘 (对于常量, 可使用模板元编程Fac<n>::value计算)
    {
        if (n < 0) throw std::invalid_argument("阶乘不能为负数");
        if (n == 0) return 1;
        if (n == 1) return 1;
        return fac(n - 1) * n;
    }

    ll fib(int n) // 计算斐波那契数列 (对于常量, 可使用模板元编程Fib<n>::value计算)
    {
        if (n <= 1) return n;
        ll a = 0, b = 1;
        for (int i = 2; i <= n; i ++)
        {
            ll temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }

    bool is_prime(int n) // 判断质数
    {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        
        for (int i = 5; i * i <= n; i += 6)
        {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    int gcd(int a, int b) // 计算最大公因数
    {
        while (b != 0)
        {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int lcm(int a, int b) // 计算最小公倍数
    {
        return a / gcd(a, b) * b;
    }
}
#endif