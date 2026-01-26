// ikun库数学计算库math.hpp
// 提供cmath和cmath之外的数学函数
// 版本7.1.2 Release

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#ifndef IKUN_MATHS_HPP
#define IKUN_MATHS_HPP

#include "ikun_stderr.hpp"
#include <cmath>

const long double PI = 3.1415926535897932384626433832795028841971L; // 圆周率(受精度限制, Windows平台小数点后最多保留17位, 不过由于某些平台能到40位, 所以这里也保留40位)
typedef unsigned long long ull; // 无符号长整型简写
typedef long long ll; // 长整型简写

template <int N>
struct Fac // 阶乘(模板元编程)
{
    static constexpr ull value = N * Fac<N - 1>::value;
};

template <>
struct Fac<1>
{
    static constexpr ull value = 1;
};

template <int N>
struct Fib // 斐波那契数列(模板元编程)
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

template<int B, int E>
struct Pow // 幂(模板元编程)
{
    static constexpr long long value = B * Pow<B, E - 1>::value;
};

template<int B>
struct Pow<B, 0>
{
    static constexpr long long value = 1;
};

namespace maths
{
    ll fac(int n) // 计算阶乘 (对于常量, 可使用模板元编程Fac<n>::value计算)
    {
        if (n < 0) ikun_error::throw_inv_arg("Factorial of negative number is undefined",
        "functions.hpp", "fac()", "ikun_maths 001"
        );
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

#endif // IKUN_MATHS_HPP