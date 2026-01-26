// ikun库时间相关函数times.hpp - 兼容Linux/macOS/Windows版本
// 版本7.1.2 Preview Build 2026.1.24

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#ifndef IKUN_TIMES_HPP
#define IKUN_TIMES_HPP

#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <print>
#include <format>

// 平台检测宏
#if defined(_WIN32) || defined(_WIN64)
    #define IKUN_WINDOWS 1
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    #define IKUN_UNIX 1
    #include <sys/time.h>
#else
    #error "Unsupported platform"
#endif

namespace times
{
    using std::this_thread::sleep_for;
    using std::ostringstream;
    using std::setw;
    using std::setfill;
    using std::string;
    using std::tm;
    using std::print;
    using std::format;
    using std::println;
    using namespace std::chrono;

    void sleep_ms(int ms) // 休眠指定毫秒
    {
        sleep_for(milliseconds(ms));
    }

    void sleep_us(int us) // 休眠指定微秒
    {
        sleep_for(microseconds(us));
    }

    string get_time() // 获取当前时间字符串
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        const char* weekdays[] = {"日", "一", "二", "三", "四", "五", "六"};
        
        ostringstream oss;
        print("{:04d}年{:02d}月{:02d}日 星期{} {:02d}:{:02d}:{:02d}",
            tm_info.tm_year + 1900,
            tm_info.tm_mon + 1,
            tm_info.tm_mday,
            weekdays[tm_info.tm_wday],
            tm_info.tm_hour,
            tm_info.tm_min,
            tm_info.tm_sec);
        
        return oss.str();
    }

    int get_weekday() // 获取当前星期几 (0-6，0=周日)
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_wday;
    }

    int get_hour() // 获取当前小时
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_hour;
    }

    int get_minute() // 获取当前分钟
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_min;
    }

    int get_second() // 获取当前秒
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_sec;
    }

    int get_year() // 获取当前年份
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_year + 1900;
    }

    int get_month() // 获取当前月份
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_mon + 1;
    }

    int get_day() // 获取当前日期
    {
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &now_time);
#else
        localtime_r(&now_time, &tm_info);
#endif
        
        return tm_info.tm_mday;
    }

    time_t get_ts() // 获取时间戳
    {
        return system_clock::to_time_t(system_clock::now());
    }

    long long get_ts_ms() // 获取毫秒级时间戳
    {
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count();
    }

    long long get_ts_us() // 获取微秒级时间戳（Unix平台）
    {
#if IKUN_UNIX
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return static_cast<long long>(tv.tv_sec) * 1000000LL + static_cast<long long>(tv.tv_usec);
#else
        return duration_cast<microseconds>(
            high_resolution_clock::now().time_since_epoch()).count();
#endif
    }

    string fmt_ts(time_t timestamp, const string& format = "%Y-%m-%d %H:%M:%S") // 格式化时间戳
    {
        tm tm_info;
        
#if IKUN_WINDOWS
        localtime_s(&tm_info, &timestamp);
#else
        localtime_r(&timestamp, &tm_info);
#endif
        
        char buffer[80];
        strftime(buffer, sizeof(buffer), format.c_str(), &tm_info);
        return string(buffer);
    }

    // 时间间隔计算类
    class Duration
    {
    private:
        long long milliseconds_;
    public:
        Duration(long long ms = 0) : milliseconds_(ms) {}
        
        long long hours() const { return milliseconds_ / (1000 * 3600); }
        long long minutes() const { return milliseconds_ / (1000 * 60) % 60; }
        long long seconds() const { return milliseconds_ / 1000 % 60; }
        long long milliseconds() const { return milliseconds_ % 1000; }
        
        string format(const string& fmt = "%H:%M:%S.%ms") const
        {
            string result;
            for (size_t i = 0; i < fmt.size(); ++ i)
            {
                if (fmt[i] == '%' && i + 1 < fmt.size())
                {
                    ++ i;
                    switch (fmt[i])
                    {
                        case 'H': result += std::format("{:02d}", hours()); break;
                        case 'M': result += std::format("{:02d}", minutes()); break;
                        case 'S': result += std::format("{:02d}", seconds()); break;
                        case 'm': 
                            if (i + 1 < fmt.size() && fmt[i + 1] == 's')
                            {
                                ++ i;
                                result += std::format("{:03d}", milliseconds());
                            }
                            break;
                        default: result += std::format("%{}", fmt[i]); break;
                    }
                }
                else
                {
                    result += fmt[i];
                }
            }
            return result;
        }
        
        void print(const string& fmt = "%H:%M:%S.%ms") const
        {
            std::print("{}", format(fmt));
        }
    };

    // 高精度计时器
    class Timer
    {
    private:
        high_resolution_clock::time_point start;
        high_resolution_clock::time_point end;
        bool running = false;
        
    public:
        void start_timer()
        {
            start = high_resolution_clock::now();
            running = true;
        }
        
        void stop_timer()
        {
            if (running)
            {
                end = high_resolution_clock::now();
                running = false;
            }
        }
        
        void reset()
        {
            running = false;
        }
        
        bool is_running() const { return running; }
        
        long long elapsed_ms() const
        {
            if (running)
            {
                auto now = high_resolution_clock::now();
                return duration_cast<milliseconds>(now - start).count();
            }
            return duration_cast<milliseconds>(end - start).count();
        }
        
        long long elapsed_us() const
        {
            if (running)
            {
                auto now = high_resolution_clock::now();
                return duration_cast<microseconds>(now - start).count();
            }
            return duration_cast<microseconds>(end - start).count();
        }
        
        long long elapsed_ns() const
        {
            if (running)
            {
                auto now = high_resolution_clock::now();
                return duration_cast<nanoseconds>(now - start).count();
            }
            return duration_cast<nanoseconds>(end - start).count();
        }
        
        Duration elapsed() const
        {
            return Duration(elapsed_ms());
        }
        
        // 向后兼容的接口
        auto return_ms() { stop_timer(); return elapsed_ms(); }
        auto return_us() { stop_timer(); return elapsed_us(); }
        auto return_ns() { stop_timer(); return elapsed_ns(); }
    };
}

#endif // IKUN_TIMES_HPP