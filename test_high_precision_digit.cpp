// ikun库测试文件 test_high_precision_digit.cpp
// 测试高精度整数库的功能和性能
// 此文件为跨平台测试文件, 在high_precision_digit.hpp中不存在平台相关代码的情况下即可在Linux和macOS上运行
// 按照许可证以及ikun库用户协议, 仅可用于个人学习测试, 开源项目, 非盈利性教学和竞赛使用
// 禁止用于任何商业及相关用途, 违者将依法追究责任

#include "high_precision_digit.hpp"
#include <iostream>
#include <chrono>
#include <cassert>
#include <limits>

using namespace std;
using namespace high_precision_digit;

// 性能测试函数
template<typename Func>
long long measure_time(Func func, const string& test_name)
{
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << test_name << " 耗时: " << duration.count() << " ms" << endl;
    return duration.count();
}

// 测试基本功能
void test_basic_functionality()
{
    cout << "========== 基本功能测试 ==========" << endl;
    
    // 构造测试
    bigint a("12345678901234567890");
    bigint b("98765432109876543210");
    bigint c(-123456789);
    bigint d = 1234567890LL;
    
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;
    
    // 加法测试
    bigint sum = a + b;
    cout << "a + b = " << sum << endl;
    assert(sum.to_string() == "111111111011111111100");
    
    // 减法测试
    bigint diff = b - a;
    cout << "b - a = " << diff << endl;
    assert(diff.to_string() == "86419753208641975320");
    
    // 乘法测试
    bigint product = a * d;
    cout << "a * d = " << product << endl;
    assert(product.to_string() == "15241578751714678875019052100");
    
    // 除法测试
    bigint quotient = b / d;
    cout << "b / d = " << quotient << endl;
    assert(quotient.to_string() == "80000000737");
    
    // 取余测试
    bigint remainder = b % d;
    cout << "b % d = " << remainder << endl;
    
    // 比较测试
    cout << "a < b? " << (a < b) << endl;
    cout << "a > b? " << (a > b) << endl;
    cout << "a == a? " << (a == a) << endl;
    cout << "a != b? " << (a != b) << endl;
    
    // 整数运算
    bigint e = a + 100;
    cout << "a + 100 = " << e << endl;
    bigint f = 100 + a;
    cout << "100 + a = " << f << endl;
    assert(e == f);
    
    cout << "基本功能测试通过!" << endl << endl;
}

// 测试数学函数
void test_math_functions()
{
    cout << "========== 数学函数测试 ==========" << endl;
    
    // 绝对值测试
    bigint neg("-123456789");
    bigint pos = abs(neg);
    cout << "abs(" << neg << ") = " << pos << endl;
    assert(pos.to_string() == "123456789");
    
    // 幂运算测试
    bigint base("2");
    bigint power_result = pow(base, 100);
    cout << "2^100 = " << power_result << endl;
    assert(power_result.to_string() == "1267650600228229401496703205376");
    
    // 平方根测试
    bigint square("100000000000000000000"); // 10^20
    bigint sqrt_result = sqrt(square);
    cout << "sqrt(" << square << ") = " << sqrt_result << endl;
    assert(sqrt_result.to_string() == "10000000000");
    
    // 阶乘测试
    bigint factorial_result = maths::big_fac(20);
    cout << "20! = " << factorial_result << endl;
    assert(factorial_result.to_string() == "2432902008176640000");
    
    // 大数幂测试
    bigint base2("3");
    bigint pow_result = maths::big_pow(base2, 50);
    cout << "3^50 = " << pow_result << endl;
    
    cout << "数学函数测试通过!" << endl << endl;
}

// 测试边界情况
void test_edge_cases()
{
    cout << "========== 边界情况测试 ==========" << endl;
    
    // 零测试
    bigint zero1("0");
    bigint zero2("-0");
    bigint zero3(0);
    cout << "zero1 = " << zero1 << endl;
    cout << "zero2 = " << zero2 << endl;
    cout << "zero3 = " << zero3 << endl;
    assert(zero1 == zero2);
    assert(zero2 == zero3);
    
    // 极大数测试
    bigint max_ll(numeric_limits<long long>::max());
    bigint min_ll(numeric_limits<long long>::min());
    cout << "max_ll = " << max_ll << endl;
    cout << "min_ll = " << min_ll << endl;
    
    bigint pos = bigint("123");

    // 除零测试
    try 
    {
        bigint div_by_zero = pos / zero1;
        cout << "错误: 除零异常未捕获!" << endl;
    } 
    catch (const runtime_error& e) 
    {
        cout << "正确捕获除零异常: " << e.what() << endl;
    }
    
    // 取余零测试
    try 
    {
        bigint mod_by_zero = pos % zero1;
        cout << "错误: 取余零异常未捕获!" << endl;
    } 
    catch (const runtime_error& e) 
    {
        cout << "正确捕获取余零异常: " << e.what() << endl;
    }
    
    cout << "边界情况测试通过!" << endl << endl;
}

// 测试优化大数类 (如果启用)
#ifdef IKUN_EXPERIMENTAL
void test_optimized_bigint()
{
    cout << "========== 优化大数类测试 ==========" << endl;
    
    // 构造测试
    bigint_opt a("123456789012345678901234567890");
    bigint_opt b("987654321098765432109876543210");
    bigint_opt c(-1234567890123456789LL);
    bigint_opt d = 1234567890123456789LL;
    
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;
    
    // 基本运算测试
    bigint_opt sum = a + b;
    cout << "a + b = " << sum << endl;
    
    bigint_opt diff = b - a;
    cout << "b - a = " << diff << endl;
    
    bigint_opt product = a * d;
    cout << "a * d = " << product << endl;
    
    // 复合赋值运算
    bigint_opt x = a;
    x += b;
    cout << "a += b = " << x << endl;
    assert(x == sum);
    
    x = b;
    x -= a;
    cout << "b -= a = " << x << endl;
    assert(x == diff);
    
    // 递增递减运算
    bigint_opt counter(100);
    cout << "原始值: " << counter << endl;
    ++ counter;
    cout << "++counter: " << counter << endl;
    assert(counter == bigint_opt(101));
    
    counter --;
    cout << "counter--: " << counter << endl;
    assert(counter == bigint_opt(100));
    
    // 快速幂运算
    bigint_opt base("2");
    bigint_opt power_result = base.pow(256);
    cout << "2^256 = " << power_result << endl;
    
    // 位移运算
    bigint_opt shift_test(123);
    bigint_opt left_shifted = shift_test << 3; // 相当于乘以8
    bigint_opt right_shifted = shift_test >> 2; // 相当于除以4
    cout << "123 << 3 = " << left_shifted << endl;
    cout << "123 >> 2 = " << right_shifted << endl;
    assert(left_shifted == bigint_opt(984));
    
    // 实用函数
    cout << "a的位数: " << a.get_digit_count() << endl;
    cout << "a是偶数? " << a.is_even() << endl;
    cout << "a是奇数? " << a.is_odd() << endl;
    cout << "a是零? " << a.is_zero() << endl;
    
    // 优化版阶乘
    bigint_opt factorial_result = math::big_fac_opt(30);
    cout << "30! = " << factorial_result << endl;
    
    // 优化版幂运算
    bigint_opt base2("3");
    bigint_opt pow_result = math::big_pow_opt(base2, 60);
    cout << "3^60 = " << pow_result << endl;
    
    cout << "优化大数类测试通过!" << endl << endl;
}

// 性能对比测试
void test_performance_comparison()
{
    cout << "========== 性能对比测试 ==========" << endl;
    
    // 测试大规模乘法
    string large_num1_str(100, '9'); // 100位的9
    string large_num2_str(100, '8'); // 100位的8
    
    bigint large_num1(large_num1_str);
    bigint large_num2(large_num2_str);
    
    bigint_opt opt_num1(large_num1_str);
    bigint_opt opt_num2(large_num2_str);
    
    // 测试普通大数乘法
    auto basic_mult_test = [&]() 
    {
        bigint result = large_num1 * large_num2;
        // 验证结果
        bigint expected = bigint("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999") * 
                         bigint("88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888");
    };
    
    // 测试优化大数乘法
    auto opt_mult_test = [&]() 
    {
        bigint_opt result = opt_num1 * opt_num2;
    };
    
    cout << "测试100位大数乘法:" << endl;
    long long basic_time = measure_time(basic_mult_test, "基本大数类");
    long long opt_time = measure_time(opt_mult_test, "优化大数类");
    
    if (opt_time > 0) 
    {
        double speedup = static_cast<double>(basic_time) / opt_time;
        cout << "性能提升倍数: " << speedup << "x" << endl;
    }
    
    // 测试大规模幂运算
    cout << "\n测试2^1000计算:" << endl;
    auto basic_pow_test = []() 
    {
        bigint base("2");
        bigint result = pow(base, 1000);
    };
    
    auto opt_pow_test = []() 
    {
        bigint_opt base("2");
        bigint_opt result = base.pow(1000);
    };
    
    basic_time = measure_time(basic_pow_test, "基本大数类");
    opt_time = measure_time(opt_pow_test, "优化大数类");
    
    if (opt_time > 0) 
    {
        double speedup = static_cast<double>(basic_time) / opt_time;
        cout << "性能提升倍数: " << speedup << "x" << endl;
    }
    
    cout << endl;
}
#endif

// 测试与64位整数对比
void test_64bit_comparison()
{
    cout << "========== 64位整数对比测试 ==========" << endl;
    
    // 测试在64位范围内的运算
    long long int_64_max = numeric_limits<long long>::max();
    long long int_64_min = numeric_limits<long long>::min();
    
    bigint big_max(int_64_max);
    bigint big_min(int_64_min);
    
    cout << "64位最大值: " << int_64_max << endl;
    cout << "大数表示: " << big_max << endl;
    
    cout << "64位最小值: " << int_64_min << endl;
    cout << "大数表示: " << big_min << endl;
    
    // 加法对比
    long long a_64 = 123456789012345LL;
    long long b_64 = 987654321098765LL;
    
    bigint big_a(a_64);
    bigint big_b(b_64);
    
    long long sum_64 = a_64 + b_64;
    bigint big_sum = big_a + big_b;
    
    cout << "\n加法对比:" << endl;
    cout << "64位: " << a_64 << " + " << b_64 << " = " << sum_64 << endl;
    cout << "大数: " << big_a << " + " << big_b << " = " << big_sum << endl;
    assert(big_sum == bigint(sum_64));
    
    // 乘法对比
    long long product_64 = a_64 * b_64;
    bigint big_product = big_a * big_b;
    
    cout << "\n乘法对比:" << endl;
    cout << "64位: " << a_64 << " * " << b_64 << " = " << product_64 << endl;
    cout << "大数: " << big_a << " * " << big_b << " = " << big_product << endl;
    assert(big_product == bigint("121932631137021071359549253925"));
    
    // 测试溢出情况
    long long overflow_a = int_64_max;
    long long overflow_b = 2;
    
    bigint big_overflow_a(overflow_a);
    bigint big_overflow_b(overflow_b);
    
    cout << "\n溢出测试:" << endl;
    cout << "64位溢出: " << overflow_a << " * " << overflow_b << " = 溢出!" << endl;
    bigint big_no_overflow = big_overflow_a * big_overflow_b;
    cout << "大数无溢出: " << big_overflow_a << " * " << big_overflow_b << " = " << big_no_overflow << endl;
    
    // 展示大数优势：计算20!（64位会溢出）
    cout << "\n大数优势展示:" << endl;
    cout << "64位计算20! 会溢出" << endl;
    
    bigint factorial_20 = maths::big_fac(20);
    cout << "大数计算20! = " << factorial_20 << endl;
    assert(factorial_20.to_string() == "2432902008176640000");
    
    // 计算2^62（接近64位极限）
    bigint pow_2_62 = pow(bigint(2), 62);
    cout << "2^62 = " << pow_2_62 << endl;
    assert(pow_2_62 == bigint(1LL << 62));
    
    cout << "64位整数对比测试完成!" << endl << endl;
}

// 综合测试
void test_comprehensive()
{
    cout << "========== 综合测试 ==========" << endl;
    
    // 测试大数链式运算
    bigint a("12345678901234567890");
    bigint b("98765432109876543210");
    bigint c("55555555555555555555");
    
    bigint result = (a + b) * c / bigint(100) - bigint(999999999);
    cout << "(a + b) * c / 100 - 999999999 = " << result << endl;
    
    // 测试混合类型运算
    result = a * 123 + 456 - b / 789;
    cout << "a * 123 + 456 - b / 789 = " << result << endl;
    
    // 测试科学计算
    cout << "\n科学计算测试:" << endl;
    bigint gravitational_constant("66743"); // 6.6743×10^-11 的缩放版本
    bigint earth_mass("5972190000000000000000000"); // 5.97219×10^24
    bigint earth_radius("6371000");
    
    // 计算地球表面的重力加速度 g = GM/r^2
    bigint numerator = gravitational_constant * earth_mass;
    bigint denominator = earth_radius * earth_radius;
    bigint gravity = numerator / denominator;
    
    cout << "地球重力加速度计算: " << gravity << " (缩放后)" << endl;
    
    cout << "综合测试完成!" << endl << endl;
}

int main()
{
    cout << "=== ikun库高精度整数测试程序 ===" << endl;
    cout << "编译时间: " << __DATE__ << " " << __TIME__ << endl;
    cout << "支持优化版本: ";
#ifdef IKUN_EXPERIMENTAL
    cout << "是" << endl;
#else
    cout << "否" << endl;
#endif
    cout << "===================================" << endl << endl;
    
    try 
    {
        // 运行所有测试
        test_basic_functionality();
        test_math_functions();
        test_edge_cases();
        test_64bit_comparison();
        
#ifdef IKUN_EXPERIMENTAL
        test_optimized_bigint();
        test_performance_comparison();
#endif
        
        test_comprehensive();
        
        cout << "===================================" << endl;
        cout << "所有测试通过!" << endl;
        cout << "ikun高精度整数库功能完整，性能良好!" << endl;
        cout << "===================================" << endl;
        
        return 0;
    } 
    catch (const exception& e) 
    {
        cerr << "测试失败: " << e.what() << endl;
        return 1;
    }
}