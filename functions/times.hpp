// ikun库functions模块times.hpp库
// 提供时间相关函数和计时器类, 基于chrono和thread库实现
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据许可证和用户协议规定, 禁止商业用途, 违者依法追究法律责任

#ifndef IKUN_FUNCTIONS_TIMES_HPP
#define IKUN_FUNCTIONS_TIMES_HPP

#ifndef IKUN_CORE
    #error "请先使用#include "ikun/core.hpp"包含ikun库核心"
    #error "否则无法使用版本管理功能"
#endif // IKUN_CORE

#include <chrono>
#include <thread>
#include <string>

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

#endif // IKUN_FUNCTIONS_TIMES_HPP