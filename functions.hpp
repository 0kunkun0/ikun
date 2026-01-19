// ikun库functions.hpp
// 包括如文件操作, 随机数生成, 数学计算等实用函数
// 某些库依赖Windows API
// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_FUNCTIONS_HPP
#define IKUN_FUNCTIONS_HPP

#include <string>
#include <fstream>
#include <print>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <random>

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

namespace libs
{
    bool fileexists(const std::string& filename) // 检查文件是否存在
    {
        return std::filesystem::exists(filename); 
    }

    /** @brief 获取目录下所有文件名
     *  @param path 路径
     *  @param fileextname 文件扩展名, 默认为.
     *  @return std::vector<std::string> 文件名列表
     */
    std::vector<std::string> filedir(std::string path = ".", std::string fileextname = ".")
    {
        std::vector<std::string> files;

        // 检查目录是否存在
        if (!std::filesystem::exists(path))
        {
            std::cerr << "错误: 目录 '" << path << "' 不存在" << std::endl;
            return files; // 返回空向量
        }

        // 检查是否是目录
        if (!std::filesystem::is_directory(path))
        {
            std::cerr << "错误: '" << path << "' 不是目录" << std::endl;
            return files;
        }

        // 确保文件扩展名以点开头
        if (!fileextname.empty() && fileextname != ".")
        {
            if (fileextname[0] != '.')
            {
                fileextname = "." + fileextname; // 自动添加点
            }
        }

        try
        {
            // 遍历目录
            for (const auto &entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    std::string filename = entry.path().filename().string();
                    
                    // 根据 fileextname 参数进行过滤
                    if (fileextname == ".") // 默认值，匹配所有文件
                    {
                        files.push_back(filename);
                    }
                    else if (fileextname.empty()) // 空字符串也匹配所有文件
                    {
                        files.push_back(filename);
                    }
                    else
                    {
                        // 6. 检查文件扩展名
                        // 获取文件扩展名
                        std::string ext = entry.path().extension().string();
                        
                        // 7. 比较扩展名(不区分大小写)
                        if (!ext.empty() && 
                            std::equal(ext.begin(), ext.end(), 
                                    fileextname.begin(), fileextname.end(),
                                    [](char a, char b) {
                                        return std::tolower(a) == std::tolower(b);
                                    }))
                        {
                            files.push_back(filename);
                        }
                    }
                }
            }
            std::sort(files.begin(), files.end());
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "文件系统错误: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "错误: " << e.what() << std::endl;
        }

        return files;
    }

    /**
     * @brief 读取文件全部内容
     * @param filename 文件名
     * @return std::string 文件内容
     */
    std::string rfile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("无法打开文件: " + filename);
        }
        
        std::string content;
        file.seekg(0, std::ios::end);
        content.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&content[0], content.size());
        file.close();
        
        return content;
    }

    /**
     * @brief 写入文件
     * @param filename 文件名
     * @param content 内容
     */
    void wfile(const std::string& filename, const std::string& content)
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("无法创建文件: " + filename);
        }
        file.write(content.c_str(), content.size());
        file.close();
    }

    void mkdir(const std::string& path) // 创建目录
    {
        std::filesystem::create_directory(path);
    }
}

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

namespace random
{
    int random_int(int min, int max) // 区间随机数生成器
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    char random_char() // 随机可显示ASCII字符生成器
    {
        return static_cast<char>(random_int(32, 126));
    }

    /**
     * @brief 随机字符串生成器
     * @brief 仅支持可显示ASCII字符
     * @param length 字符串长度
     * @return std::string 字符串内容    
    */
    std::string random_string(int length)
    {
        std::string str;
        for (int i = 0; i < length; i ++)
        {
            str += random_char();
        }

        return str;
    }
}

namespace times
{
    void sleep_ms(int ms) // 休眠指定毫秒
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

#ifdef _WINDOWS_
    std::string get_time() // 获取当前时间字符串
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        
        const char* weekdays[] = {"日", "一", "二", "三", "四", "五", "六"};
        
        char buffer[100];
        snprintf(buffer, sizeof(buffer), 
                "%d年%02d月%02d日 星期%s %02d:%02d:%02d",
                st.wYear, st.wMonth, st.wDay, 
                weekdays[st.wDayOfWeek],
                st.wHour, st.wMinute, st.wSecond);
        
        return std::string(buffer);
    }

    int get_weekday() // 获取当前星期几 (0-6，0=周日)
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wDayOfWeek);
    }

    int get_hour() // 获取当前小时
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wHour);
    }

    int get_minute() // 获取当前分钟
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wMinute);
    }

    int get_second() // 获取当前秒
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wSecond);
    }

    int get_year() // 获取当前年份
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wYear);
    }

    int get_month() // 获取当前月份
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wMonth);
    }

    int get_day() // 获取当前日期
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        return static_cast<int>(st.wDay);
    }
#endif

    time_t get_ts() // 获取时间戳
    {
        return time(nullptr);
    }

    long long get_ts_ms() // 获取毫秒级时间戳
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    std::string fmt_ts(time_t timestamp, const std::string& format = "%Y-%m-%d %H:%M:%S") // 格式化时间戳
    {
        tm timeinfo;
        localtime_s(&timeinfo, &timestamp);  // 使用安全的 localtime_s
        char buffer[80];
        strftime(buffer, sizeof(buffer), format.c_str(), &timeinfo);
        return std::string(buffer);
    }

    // 高精度计时器
    class Timer
    {
        private:
            std::chrono::high_resolution_clock::time_point start;
            std::chrono::high_resolution_clock::time_point end;
        public:
            void start_timer()
            {
                start = std::chrono::high_resolution_clock::now();
            }
            void stop_timer()
            {
                end = std::chrono::high_resolution_clock::now();
            }
            int return_ms()
            {
                return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            }
            int return_us()
            {
                return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            }
            int return_ns()
            {
                return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            }
    };
}

#endif // IKUN_FUNCTIONS_HPP