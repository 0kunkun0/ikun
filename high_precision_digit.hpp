// ikun库high_precision_digit.hpp
// 提供高精度整数类以及运算, 基于C++ STL实现
// 命名空间为high_precision_digit
// 注意: 高精度运算某些情况下会导致性能问题, 请根据需要使用

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_HIGH_PRECISION_DIGIT_HPP
#define IKUN_HIGH_PRECISION_DIGIT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <cstdlib>

namespace high_precision_digit
{

class bigint // 基本大数类, 使用动态数组存储数字
{
private:
    std::vector<int> digits;  // 存储数字，低位在前
    bool is_negative;    // 是否为负数
    
    // 移除前导零
    void trim()
    {
        while (digits.size() > 1 && digits.back() == 0)
        {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0)
        {
            is_negative = false;
        }
    }
    
    // 比较绝对值大小
    int compare_abs(const bigint& other) const
    {
        if (digits.size() != other.digits.size())
        {
            return digits.size() > other.digits.size() ? 1 : -1;
        }
        for (int i = digits.size() - 1; i >= 0; -- i)
        {
            if (digits[i] != other.digits[i]) {
                return digits[i] > other.digits[i] ? 1 : -1;
            }
        }
        return 0;
    }
    
    // 加法（假设两个数都是正数）
    static bigint add_abs(const bigint& a, const bigint& b)
    {
        bigint result;
        result.digits.clear();
        
        int carry = 0;
        size_t max_len = std::max(a.digits.size(), b.digits.size());
        
        for (size_t i = 0; i < max_len || carry; ++ i)
        {
            int sum = carry;
            if (i < a.digits.size()) sum += a.digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }
        
        return result;
    }
    
    // 减法（假设a的绝对值大于等于b的绝对值，且结果为正）
    static bigint sub_abs(const bigint& a, const bigint& b)
    {
        bigint result;
        result.digits.clear();
        
        int borrow = 0;
        for (size_t i = 0; i < a.digits.size(); ++i)
        {
            int diff = a.digits[i] - borrow;
            if (i < b.digits.size()) diff -= b.digits[i];
            
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.digits.push_back(diff);
        }
        
        result.trim();
        return result;
    }

public:
    // 构造函数
    bigint() : is_negative(false)
    {
        digits.push_back(0);
    }
    
    bigint(const std::string& s)
    {
        if (s.empty())
        {
            digits.push_back(0);
            is_negative = false;
            return;
        }
        
        size_t start = 0;
        is_negative = false;
        
        if (s[0] == '-')
        {
            is_negative = true;
            start = 1;
        } else if (s[0] == '+')
        {
            start = 1;
        }
        
        // 跳过前导零
        while (start < s.size() && s[start] == '0')
        {
            start ++;
        }
        
        if (start == s.size())
        {
            digits.push_back(0);
            is_negative = false;
            return;
        }
        
        for (int i = s.size() - 1; i >= static_cast<int>(start); -- i)
        {
            if (!isdigit(s[i]))
            {
                throw std::invalid_argument("Invalid character in number string");
            }
            digits.push_back(s[i] - '0');
        }
        
        trim();
    }
    
    bigint(long long n)
    {
        if (n == 0)
        {
            digits.push_back(0);
            is_negative = false;
            return;
        }
        
        is_negative = n < 0;
        n = std::abs(n);
        
        while (n > 0)
        {
            digits.push_back(n % 10);
            n /= 10;
        }
    }
    
    // 转换为字符串
    std::string to_string() const
    {
        if (digits.empty()) return "0";
        
        std::string result;
        if (is_negative) result += '-';
        
        for (int i = digits.size() - 1; i >= 0; -- i)
        {
            result += char(digits[i] + '0');
        }
        
        return result;
    }
    
    // 算术运算符重载
    bigint operator+(const bigint& other) const
    {
        // 同号相加
        if (is_negative == other.is_negative)
        {
            bigint result = add_abs(*this, other);
            result.is_negative = is_negative;
            return result;
        }
        
        // 异号相减
        int cmp = compare_abs(other);
        if (cmp == 0) return bigint(0);
        
        bigint result;
        if (cmp > 0)
        {
            result = sub_abs(*this, other);
            result.is_negative = is_negative;
        }
        else
        {
            result = sub_abs(other, *this);
            result.is_negative = other.is_negative;
        }
        
        return result;
    }
    
    bigint operator-(const bigint& other) const
    {
        // 改变符号后相加
        bigint temp = other;
        temp.is_negative = !temp.is_negative;
        return *this + temp;
    }
    
    bigint operator*(const bigint& other) const
    {
        bigint result;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        
        for (size_t i = 0; i < digits.size(); ++i)
        {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++ j)
            {
                long long cur = result.digits[i + j] + 
                               digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + 
                               carry;
                result.digits[i + j] = cur % 10;
                carry = static_cast<int>(cur / 10);
            }
        }
        
        result.is_negative = is_negative != other.is_negative;
        result.trim();
        return result;
    }
    
    bigint operator/(const bigint& other) const
    {
        if (other == bigint(0))
        {
            throw std::runtime_error("Division by zero");
        }
        
        bigint a = *this;
        bigint b = other;
        a.is_negative = false;
        b.is_negative = false;
        
        if (a < b) return bigint(0);
        
        bigint result;
        bigint current;
        result.digits.resize(a.digits.size());
        
        for (int i = a.digits.size() - 1; i >= 0; --i)
        {
            current.digits.insert(current.digits.begin(), a.digits[i]);
            current.trim();
            
            int x = 0;
            int left = 0, right = 10;
            while (left <= right)
            {
                int mid = (left + right) / 2;
                if (b * bigint(mid) <= current)
                {
                    x = mid;
                    left = mid + 1;
                }
                else
                {
                    right = mid - 1;
                }
            }
            
            result.digits[i] = x;
            current = current - b * bigint(x);
        }
        
        result.is_negative = is_negative != other.is_negative;
        result.trim();
        return result;
    }
    
    bigint operator%(const bigint& other) const
    {
        if (other == bigint(0))
        {
            throw std::runtime_error("Modulo by zero");
        }
        
        bigint quotient = *this / other;
        bigint result = *this - quotient * other;
        
        // 确保余数非负
        if (result.is_negative)
        {
            result = result + other;
        }
        
        return result;
    }
    
    // 比较运算符重载
    bool operator==(const bigint& other) const
    {
        return is_negative == other.is_negative && digits == other.digits;
    }
    
    bool operator!=(const bigint& other) const
    {
        return !(*this == other);
    }
    
    bool operator<(const bigint& other) const
    {
        if (is_negative != other.is_negative)
        {
            return is_negative;
        }
        
        if (is_negative)
        {
            return compare_abs(other) > 0;
        }
        else
        {
            return compare_abs(other) < 0;
        }
    }
    
    bool operator<=(const bigint& other) const
    {
        return *this < other || *this == other;
    }
    
    bool operator>(const bigint& other) const
    {
        return !(*this <= other);
    }
    
    bool operator>=(const bigint& other) const
    {
        return !(*this < other);
    }
    
    // 与整数类型的运算
    bigint operator+(long long n) const
    {
        return *this + bigint(n);
    }
    
    bigint operator-(long long n) const
    {
        return *this - bigint(n);
    }
    
    bigint operator*(long long n) const
    {
        return *this * bigint(n);
    }
    
    bigint operator/(long long n) const 
    {
        return *this / bigint(n);
    }
    
    bigint operator%(long long n) const
    {
        return *this % bigint(n);
    }
    
    // 友元函数，支持整数在左侧的运算
    friend bigint operator+(long long n, const bigint& b)
    {
        return bigint(n) + b;
    }
    
    friend bigint operator-(long long n, const bigint& b)
    {
        return bigint(n) - b;
    }
    
    friend bigint operator*(long long n, const bigint& b)
    {
        return bigint(n) * b;
    }
    
    friend bigint operator/(long long n, const bigint& b)
    {
        return bigint(n) / b;
    }
    
    friend bigint operator%(long long n, const bigint& b)
    {
        return bigint(n) % b;
    }
    
    // 重载cmath函数
    friend bigint abs(const bigint& n)
    {
        bigint result = n;
        result.is_negative = false;
        return result;
    }
    
    friend bigint pow(const bigint& base, int exponent)
    {
        if (exponent < 0)
        {
            throw std::runtime_error("Negative exponent not supported");
        }
        
        if (exponent == 0) return bigint(1);
        
        bigint result(1);
        bigint b = base;
        int e = exponent;
        
        while (e > 0)
        {
            if (e & 1)
            {
                result = result * b;
            }
            b = b * b;
            e >>= 1;
        }
        
        return result;
    }
    
    // 求平方根（整数部分）
    friend bigint sqrt(const bigint& n)
    {
        if (n.is_negative)
        {
            throw std::runtime_error("Square root of negative number");
        }
        
        if (n == bigint(0) || n == bigint(1)) return n;
        
        bigint left(1);
        bigint right = n;
        bigint result(1);
        
        while (left <= right)
        {
            bigint mid = (left + right) / 2;
            bigint square = mid * mid;
            
            if (square == n) return mid;
            
            if (square < n)
            {
                left = mid + bigint(1);
                result = mid;
            }
            else
            {
                right = mid - bigint(1);
            }
        }
        
        return result;
    }
};

// 输出运算符重载
std::ostream& operator<<(std::ostream& os, const bigint& n)
{
    os << n.to_string();
    return os;
}

// 输入运算符重载
std::istream& operator>>(std::istream& is, bigint& n)
{
    std::string s;
    is >> s;
    n = bigint(s);
    return is;
}

#ifdef IKUN_EXPERIMENTAL // 启用实验性功能
class bigint_opt // 性能更优的bigint类, 稳定性待验证
{
private:
    static const int BASE = 1000000000;      // 10亿进制
    static const int BASE_DIGITS = 9;        // 每个单元的数字位数
    
    std::vector<int> digits;  // 存储数字，低位在前
    bool is_negative;         // 是否为负数
    
    // 移除前导零
    void trim() noexcept
    {
        while (digits.size() > 1 && digits.back() == 0)
        {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0)
        {
            is_negative = false;
        }
    }
    
    // 比较绝对值大小（不修改对象）
    int compare_abs(const bigint_opt& other) const noexcept
    {
        if (digits.size() != other.digits.size())
        {
            return digits.size() > other.digits.size() ? 1 : -1;
        }
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i)
        {
            if (digits[i] != other.digits[i])
            {
                return digits[i] > other.digits[i] ? 1 : -1;
            }
        }
        return 0;
    }
    
    // 加法辅助函数（假设两个数都是正数）
    static void add_abs(const bigint_opt& a, const bigint_opt& b, bigint_opt& result) noexcept
    {
        result.digits.clear();
        int carry = 0;
        size_t max_len = std::max(a.digits.size(), b.digits.size());
        result.digits.reserve(max_len + 1);
        
        for (size_t i = 0; i < max_len || carry; ++i)
        {
            int sum = carry;
            if (i < a.digits.size()) sum += a.digits[i];
            if (i < b.digits.size()) sum += b.digits[i];
            
            if (sum >= BASE)
            {
                result.digits.push_back(sum - BASE);
                carry = 1;
            }
            else
            {
                result.digits.push_back(sum);
                carry = 0;
            }
        }
        result.is_negative = false;
    }
    
    // 减法辅助函数（假设a的绝对值大于等于b的绝对值）
    static void sub_abs(const bigint_opt& a, const bigint_opt& b, bigint_opt& result) noexcept
    {
        result.digits.clear();
        result.digits.reserve(a.digits.size());
        int borrow = 0;
        
        for (size_t i = 0; i < a.digits.size(); ++i)
        {
            int diff = a.digits[i] - borrow;
            if (i < b.digits.size()) diff -= b.digits[i];
            
            if (diff < 0)
            {
                diff += BASE;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.digits.push_back(diff);
        }
        
        result.trim();
        result.is_negative = false;
    }
    
    // 优化的试商函数（使用双精度估算）
    static int estimate_quotient_digit(const bigint_opt& a, const bigint_opt& b)
    {
        if (a.digits.size() < b.digits.size()) return 0;
        
        // 取被除数和除数的最高两位进行估算
        long long a_high = a.digits.back();
        if (a.digits.size() > 1)
        {
            a_high = a_high * BASE + a.digits[a.digits.size() - 2];
        }
        
        long long b_high = b.digits.back();
        if (b.digits.size() > 1)
        {
            b_high = b_high * BASE + b.digits[b.digits.size() - 2];
        }
        
        // 估算商位
        long long q = a_high / (b_high + 1);  // 保守估计
        return static_cast<int>(std::min(q, static_cast<long long>(BASE - 1)));
    }
    
    // 朴素乘法（用于小规模乘法）
    static void naive_multiply(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result)
    {
        size_t a_size = a.size();
        size_t b_size = b.size();
        result.assign(a_size + b_size, 0);
        
        for (size_t i = 0; i < a_size; ++i)
        {
            long long carry = 0;
            for (size_t j = 0; j < b_size || carry; ++j)
            {
                long long cur = result[i + j] + carry;
                if (j < b_size) cur += static_cast<long long>(a[i]) * b[j];
                result[i + j] = static_cast<int>(cur % BASE);
                carry = cur / BASE;
            }
        }
        
        while (result.size() > 1 && result.back() == 0)
        {
            result.pop_back();
        }
    }
    
    // Karatsuba乘法算法
    static void karatsuba_multiply(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result)
    {
        size_t n = std::max(a.size(), b.size());
        
        // 如果规模较小，使用朴素乘法
        if (n <= 32)
        {
            naive_multiply(a, b, result);
            return;
        }
        
        // 确保n是偶数
        if (n % 2 == 1) ++n;
        
        size_t m = n / 2;
        
        // 分割a和b为高低两部分
        std::vector<int> a_low(a.begin(), a.begin() + std::min(m, a.size()));
        std::vector<int> a_high;
        if (a.size() > m)
        {
            a_high.assign(a.begin() + m, a.end());
        }
        else
        {
            a_high.push_back(0);
        }
        
        std::vector<int> b_low(b.begin(), b.begin() + std::min(m, b.size()));
        std::vector<int> b_high;
        if (b.size() > m)
        {
            b_high.assign(b.begin() + m, b.end());
        }
        else
        {
            b_high.push_back(0);
        }
        
        // 递归计算三个乘积
        std::vector<int> z0, z1, z2;
        
        // z0 = a_low * b_low
        karatsuba_multiply(a_low, b_low, z0);
        
        // z2 = a_high * b_high
        karatsuba_multiply(a_high, b_high, z2);
        
        // 计算a_low + a_high 和 b_low + b_high
        std::vector<int> a_sum(m + 1, 0);
        std::vector<int> b_sum(m + 1, 0);
        
        for (size_t i = 0; i < m; ++i)
        {
            int av = (i < a_low.size()) ? a_low[i] : 0;
            int bv = (i < a_high.size()) ? a_high[i] : 0;
            a_sum[i] = av + bv;
        }
        
        for (size_t i = 0; i < m; ++i)
        {
            int av = (i < b_low.size()) ? b_low[i] : 0;
            int bv = (i < b_high.size()) ? b_high[i] : 0;
            b_sum[i] = av + bv;
        }
        
        // 处理进位
        for (size_t i = 0; i < m; ++i)
        {
            if (a_sum[i] >= BASE)
            {
                a_sum[i] -= BASE;
                if (i + 1 < a_sum.size()) a_sum[i + 1]++;
            }
            if (b_sum[i] >= BASE)
            {
                b_sum[i] -= BASE;
                if (i + 1 < b_sum.size()) b_sum[i + 1]++;
            }
        }
        
        // z1 = (a_low + a_high) * (b_low + b_high) - z0 - z2
        std::vector<int> z1_temp;
        karatsuba_multiply(a_sum, b_sum, z1_temp);
        
        // 从z1_temp中减去z0和z2
        int borrow = 0;
        size_t max_len = std::max(z1_temp.size(), std::max(z0.size(), z2.size()));
        z1.assign(max_len, 0);
        
        for (size_t i = 0; i < max_len; ++i)
        {
            int val = borrow;
            if (i < z1_temp.size()) val += z1_temp[i];
            if (i < z0.size()) val -= z0[i];
            if (i < z2.size()) val -= z2[i];
            
            if (val < 0)
            {
                val += BASE;
                borrow = -1;
            }
            else
            {
                borrow = 0;
            }
            z1[i] = val;
        }
        
        // 合并结果：result = z0 + z1 * BASE^m + z2 * BASE^(2m)
        size_t result_size = z0.size() + 2 * m;
        result.assign(result_size, 0);
        
        // 添加z0
        for (size_t i = 0; i < z0.size(); ++i)
        {
            result[i] = z0[i];
        }
        
        // 添加z1 * BASE^m
        for (size_t i = 0; i < z1.size(); ++i)
        {
            result[i + m] += z1[i];
            if (result[i + m] >= BASE)
            {
                result[i + m] -= BASE;
                if (i + m + 1 < result.size()) result[i + m + 1]++;
            }
        }
        
        // 添加z2 * BASE^(2m)
        for (size_t i = 0; i < z2.size(); ++i)
        {
            result[i + 2 * m] += z2[i];
            if (result[i + 2 * m] >= BASE)
            {
                result[i + 2 * m] -= BASE;
                if (i + 2 * m + 1 < result.size()) result[i + 2 * m + 1]++;
            }
        }
        
        // 移除前导零
        while (result.size() > 1 && result.back() == 0)
        {
            result.pop_back();
        }
    }

public:
    // 构造函数
    bigint_opt() : is_negative(false)
    {
        digits.push_back(0);
    }
    
    // 从字符串构造（优化版）
    bigint_opt(const std::string& s)
    {
        if (s.empty())
        {
            digits.push_back(0);
            is_negative = false;
            return;
        }
        
        size_t start = 0;
        is_negative = false;
        
        // 处理符号
        if (s[0] == '-')
        {
            is_negative = true;
            start = 1;
        }
        else if (s[0] == '+')
        {
            start = 1;
        }
        
        // 跳过前导零
        while (start < s.size() && s[start] == '0')
        {
            start++;
        }
        
        if (start == s.size())
        {
            digits.push_back(0);
            is_negative = false;
            return;
        }
        
        // 计算需要的digits数量
        size_t num_digits = (s.size() - start + BASE_DIGITS - 1) / BASE_DIGITS;
        digits.resize(num_digits, 0);
        
        // 从低位开始解析
        int digit_index = 0;
        long long current_value = 0;
        int current_power = 1;
        
        for (int i = static_cast<int>(s.size()) - 1; i >= static_cast<int>(start); --i)
        {
            if (!isdigit(s[i]))
            {
                throw std::invalid_argument("Invalid character in number string");
            }
            
            current_value += (s[i] - '0') * current_power;
            current_power *= 10;
            
            if (current_power == BASE)
            {
                digits[digit_index++] = static_cast<int>(current_value);
                current_value = 0;
                current_power = 1;
            }
        }
        
        if (current_value > 0)
        {
            digits[digit_index] = static_cast<int>(current_value);
        }
        
        trim();
    }
    
    // 从64位整数构造
    bigint_opt(long long n)
    {
        if (n == 0)
        {
            digits.push_back(0);
            is_negative = false;
            return;
        }
        
        is_negative = n < 0;
        unsigned long long un = static_cast<unsigned long long>(std::abs(n));
        
        while (un > 0)
        {
            digits.push_back(static_cast<int>(un % BASE));
            un /= BASE;
        }
    }
    
    // 从vector构造（内部使用）
    bigint_opt(std::vector<int>&& d, bool neg = false) noexcept
        : digits(std::move(d)), is_negative(neg)
    {
        trim();
    }
    
    // 移动构造函数
    bigint_opt(bigint_opt&& other) noexcept
        : digits(std::move(other.digits)), is_negative(other.is_negative)
    {
        other.digits = {0};
        other.is_negative = false;
    }
    
    // 复制构造函数
    bigint_opt(const bigint_opt& other) = default;
    
    // 移动赋值运算符
    bigint_opt& operator=(bigint_opt&& other) noexcept
    {
        if (this != &other)
        {
            digits = std::move(other.digits);
            is_negative = other.is_negative;
            other.digits = {0};
            other.is_negative = false;
        }
        return *this;
    }
    
    // 复制赋值运算符
    bigint_opt& operator=(const bigint_opt& other) = default;
    
    // 转换为字符串（优化版）
    std::string to_string() const
    {
        if (digits.empty()) return "0";
        
        std::string result;
        if (is_negative) result += '-';
        
        // 最高位不需要前导零
        result += std::to_string(digits.back());
        
        // 其余位需要补足9位
        for (int i = static_cast<int>(digits.size()) - 2; i >= 0; --i)
        {
            char buf[10];
            snprintf(buf, sizeof(buf), "%09d", digits[i]);
            result += buf;
        }
        
        return result;
    }
    
    // 算术运算符（优化版实现）
    bigint_opt operator+(const bigint_opt& other) const
    {
        // 同号相加
        if (is_negative == other.is_negative)
        {
            bigint_opt result;
            add_abs(*this, other, result);
            result.is_negative = is_negative;
            return result;
        }
        
        // 异号相减
        int cmp = compare_abs(other);
        if (cmp == 0) return bigint_opt(0);
        
        bigint_opt result;
        if (cmp > 0)
        {
            sub_abs(*this, other, result);
            result.is_negative = is_negative;
        }
        else
        {
            sub_abs(other, *this, result);
            result.is_negative = other.is_negative;
        }
        
        return result;
    }
    
    // 复合赋值运算符（提高性能）
    bigint_opt& operator+=(const bigint_opt& other)
    {
        *this = *this + other;
        return *this;
    }
    
    bigint_opt operator-(const bigint_opt& other) const
    {
        // 改变符号后相加
        bigint_opt temp = other;
        temp.is_negative = !temp.is_negative;
        return *this + temp;
    }
    
    bigint_opt& operator-=(const bigint_opt& other)
    {
        *this = *this - other;
        return *this;
    }
    
    // 优化乘法（自动选择算法）
    bigint_opt operator*(const bigint_opt& other) const
    {
        bigint_opt result;
        result.is_negative = is_negative != other.is_negative;
        
        // 如果有一个是0，直接返回0
        if ((digits.size() == 1 && digits[0] == 0) || 
            (other.digits.size() == 1 && other.digits[0] == 0))
        {
            return bigint_opt(0);
        }
        
        // 根据规模选择乘法算法
        if (digits.size() > 64 && other.digits.size() > 64)
        {
            // 使用Karatsuba算法
            karatsuba_multiply(digits, other.digits, result.digits);
        }
        else
        {
            // 使用朴素乘法
            naive_multiply(digits, other.digits, result.digits);
        }
        
        result.trim();
        return result;
    }
    
    bigint_opt& operator*=(const bigint_opt& other)
    {
        *this = *this * other;
        return *this;
    }
    
    // 优化除法（改进的试商算法）
    bigint_opt operator/(const bigint_opt& other) const
    {
        if (other == bigint_opt(0))
        {
            throw std::runtime_error("Division by zero");
        }
        
        bigint_opt a = *this;
        bigint_opt b = other;
        a.is_negative = false;
        b.is_negative = false;
        
        if (a < b) return bigint_opt(0);
        
        bigint_opt result;
        bigint_opt current;
        result.is_negative = is_negative != other.is_negative;
        
        // 结果的最大位数
        result.digits.resize(a.digits.size() - b.digits.size() + 1, 0);
        
        // 将除数左移，使其与被除数最高位对齐
        size_t shift = a.digits.size() - b.digits.size();
        std::vector<int> b_shifted(b.digits);
        b_shifted.insert(b_shifted.begin(), shift, 0);
        
        bigint_opt divisor(std::move(b_shifted), false);
        
        for (int i = static_cast<int>(shift); i >= 0; --i)
        {
            // 估算商位
            int q = estimate_quotient_digit(a, divisor);
            
            // 调整商位
            bigint_opt product = divisor * bigint_opt(q);
            while (product > a)
            {
                --q;
                product = divisor * bigint_opt(q);
            }
            
            result.digits[i] = q;
            a = a - product;
            
            // 除数右移一位
            if (i > 0)
            {
                divisor.digits.erase(divisor.digits.begin());
            }
        }
        
        result.trim();
        return result;
    }
    
    bigint_opt& operator/=(const bigint_opt& other)
    {
        *this = *this / other;
        return *this;
    }
    
    bigint_opt operator%(const bigint_opt& other) const
    {
        if (other == bigint_opt(0))
        {
            throw std::runtime_error("Modulo by zero");
        }
        
        bigint_opt quotient = *this / other;
        bigint_opt result = *this - quotient * other;
        
        // 确保余数非负
        if (result.is_negative)
        {
            result = result + other;
        }
        
        return result;
    }
    
    bigint_opt& operator%=(const bigint_opt& other)
    {
        *this = *this % other;
        return *this;
    }
    
    // 快速幂运算（使用移位优化）
    bigint_opt pow(int exponent) const
    {
        if (exponent < 0)
        {
            throw std::runtime_error("Negative exponent not supported for integers");
        }
        
        if (exponent == 0) return bigint_opt(1);
        
        bigint_opt result(1);
        bigint_opt base = *this;
        
        while (exponent > 0)
        {
            if (exponent & 1)
            {
                result *= base;
            }
            base *= base;
            exponent >>= 1;
        }
        
        return result;
    }
    
    // 比较运算符（保持不变，已经足够高效）
    bool operator==(const bigint_opt& other) const noexcept
    {
        return is_negative == other.is_negative && digits == other.digits;
    }
    
    bool operator!=(const bigint_opt& other) const noexcept
    {
        return !(*this == other);
    }
    
    bool operator<(const bigint_opt& other) const noexcept
    {
        if (is_negative != other.is_negative)
        {
            return is_negative;
        }
        
        if (is_negative)
        {
            return compare_abs(other) > 0;
        }
        else
        {
            return compare_abs(other) < 0;
        }
    }
    
    bool operator<=(const bigint_opt& other) const noexcept
    {
        return *this < other || *this == other;
    }
    
    bool operator>(const bigint_opt& other) const noexcept
    {
        return !(*this <= other);
    }
    
    bool operator>=(const bigint_opt& other) const noexcept
    {
        return !(*this < other);
    }
    
    // 前置和后置递增递减
    bigint_opt& operator++()  // 前置++
    {
        *this += bigint_opt(1);
        return *this;
    }
    
    bigint_opt operator++(int)  // 后置++
    {
        bigint_opt temp = *this;
        ++(*this);
        return temp;
    }
    
    bigint_opt& operator--()  // 前置--
    {
        *this -= bigint_opt(1);
        return *this;
    }
    
    bigint_opt operator--(int)  // 后置--
    {
        bigint_opt temp = *this;
        --(*this);
        return temp;
    }
    
    // 位运算运算符（可选实现）
    bigint_opt operator<<(int shift) const  // 左移相当于乘以2^shift
    {
        if (shift < 0) return *this >> (-shift);
        
        bigint_opt result = *this;
        for (int i = 0; i < shift; ++i)
        {
            result *= bigint_opt(2);
        }
        return result;
    }
    
    bigint_opt operator>>(int shift) const  // 右移相当于除以2^shift
    {
        if (shift < 0) return *this << (-shift);
        
        bigint_opt result = *this;
        for (int i = 0; i < shift; ++i)
        {
            result /= bigint_opt(2);
        }
        return result;
    }
    
    // 与整数类型的运算
    bigint_opt operator+(long long n) const
    {
        return *this + bigint_opt(n);
    }
    
    bigint_opt operator-(long long n) const
    {
        return *this - bigint_opt(n);
    }
    
    bigint_opt operator*(long long n) const
    {
        return *this * bigint_opt(n);
    }
    
    bigint_opt operator/(long long n) const 
    {
        return *this / bigint_opt(n);
    }
    
    bigint_opt operator%(long long n) const
    {
        return *this % bigint_opt(n);
    }
    
    // 友元函数，支持整数在左侧的运算
    friend bigint_opt operator+(long long n, const bigint_opt& b)
    {
        return bigint_opt(n) + b;
    }
    
    friend bigint_opt operator-(long long n, const bigint_opt& b)
    {
        return bigint_opt(n) - b;
    }
    
    friend bigint_opt operator*(long long n, const bigint_opt& b)
    {
        return bigint_opt(n) * b;
    }
    
    friend bigint_opt operator/(long long n, const bigint_opt& b)
    {
        return bigint_opt(n) / b;
    }
    
    friend bigint_opt operator%(long long n, const bigint_opt& b)
    {
        return bigint_opt(n) % b;
    }
    
    // 数学函数
    friend bigint_opt abs(const bigint_opt& n)
    {
        bigint_opt result = n;
        result.is_negative = false;
        return result;
    }
    
    friend bigint_opt sqrt(const bigint_opt& n)
    {
        if (n.is_negative)
        {
            throw std::runtime_error("Square root of negative number");
        }
        
        if (n == bigint_opt(0) || n == bigint_opt(1)) return n;
        
        bigint_opt left(1);
        bigint_opt right = n;
        bigint_opt result(1);
        
        while (left <= right)
        {
            bigint_opt mid = (left + right) / 2;
            bigint_opt square = mid * mid;
            
            if (square == n) return mid;
            
            if (square < n)
            {
                left = mid + bigint_opt(1);
                result = mid;
            }
            else
            {
                right = mid - bigint_opt(1);
            }
        }
        
        return result;
    }
    
    // 实用函数
    int get_digit_count() const noexcept
    {
        if (digits.empty()) return 0;
        int count = (digits.size() - 1) * BASE_DIGITS;
        int last = digits.back();
        while (last > 0)
        {
            count++;
            last /= 10;
        }
        return count == 0 ? 1 : count;
    }
    
    bool is_zero() const noexcept
    {
        return digits.size() == 1 && digits[0] == 0;
    }
    
    bool is_odd() const noexcept
    {
        return (digits[0] & 1) == 1;
    }
    
    bool is_even() const noexcept
    {
        return (digits[0] & 1) == 0;
    }
    
    // 交换函数
    void swap(bigint_opt& other) noexcept
    {
        std::swap(digits, other.digits);
        std::swap(is_negative, other.is_negative);
    }
};

// 输出运算符重载
std::ostream& operator<<(std::ostream& os, const bigint_opt& n)
{
    os << n.to_string();
    return os;
}

// 输入运算符重载
std::istream& operator>>(std::istream& is, bigint_opt& n)
{
    std::string s;
    is >> s;
    n = bigint_opt(s);
    return is;
}

#endif
}

namespace maths
{
    high_precision_digit::bigint big_fac(int n) // 大数阶乘
    {
        high_precision_digit::bigint result(1);
        for (int i = 2; i <= n; ++ i)
        {
            result = result * high_precision_digit::bigint(i);
        }
        return result;
    }

    #ifdef IKUN_EXPERIMENTAL
    high_precision_digit::bigint_opt big_fac_opt(int n) // 优化版大数阶乘
    {
        high_precision_digit::bigint_opt result(1);
        for (int i = 2; i <= n; ++ i)
        {
            result = result * high_precision_digit::bigint_opt(i);
        }
        return result;
    }
    #endif

    high_precision_digit::bigint big_pow(high_precision_digit::bigint base, int exp) // 大数幂
    {
        high_precision_digit::bigint result(1);
        for (int i = 0; i < exp; ++ i)
        {
            result = result * base;
        }
        return result;
    }

    #ifdef IKUN_EXPERIMENTAL
    high_precision_digit::bigint_opt big_pow_opt(high_precision_digit::bigint_opt base, int exp) // 优化版大数幂
    {
        high_precision_digit::bigint_opt result(1);
        for (int i = 0; i < exp; ++ i)
        {
            result = result * base;
        }
        return result;
    }
    #endif
}

void test_hpd()
{
    system("color 0a");
    std::cout << "本函数将调用库中自带的测试代码" << std::endl;
    std::cout << "由于库中没有预编译的二进制文件, 此操作需要在PATH环境变量中有可用的编译器" << std::endl;
    std::cout << "请输入编译需要的编译器(GCC:1/Clang:2/MSVC(VS Developer CMD):3):";
    
    int compiler;
    std::cin >> compiler;

    switch (compiler)
    {
        case 1:
            system("g++.exe -std=c++17 -DIKUN_EXPERIMENTAL -Wall -Wextra -pedantic -O2 -o .\\ikun\\test.exe .\\ikun\\test_high_precision_digit.cpp");
            break;
        case 2:
            system("clang++.exe -std=c++17 -DIKUN_EXPERIMENTAL -Wall -Wextra -pedantic -O2 -o .\\ikun\\test.exe .\\ikun\\test_high_precision_digit.cpp");
            break;
        case 3:
            system("cl.exe /EHsc /W4 /O2 /DIKUN_EXPERIMENTAL .\\ikun\\test_high_precision_digit.cpp /std:c++17 /Fe:.\\ikun\\test.exe");
            break;
        default:
            std::cout << "无效的编译器" << std::endl;
        return;
    }

    system("color 07");
    system(".\\ikun\\test.exe");
    system("del .\\ikun\\test.exe");
}
#endif