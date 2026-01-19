// ikun库C++ print库实现

#ifndef IKUN_PRINT_HPP
#define IKUN_PRINT_HPP

#ifndef WIN32_LEAD_AND_MEAN
#define WIN32_LEAD_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <sstream>
#include <type_traits>
#include <utility>
#include <tuple>

namespace ikun_print
{
    // 内部辅助函数和类型定义
    namespace detail
    {
        // 设置控制台输出代码页为UTF-8
        inline void initConsole()
        {
            static bool initialized = false;
            if (!initialized)
            {
                SetConsoleOutputCP(CP_UTF8);
                initialized = true;
            }
        }

        // 检查类型是否可流输出
        template <typename T>
        struct is_streamable
        {
            template <typename U>
            static auto test(int) -> decltype(
                std::declval<std::ostream&>() << std::declval<U>(),
                std::true_type{}
            );

            template <typename>
            static std::false_type test(...);

            static constexpr bool value = decltype(test<T>(0))::value;
        };

        // 打印单个参数的实现
        template <typename T>
        void printImpl(std::ostream& os, T&& arg)
        {
            os << std::forward<T>(arg);
        }

        // 递归展开参数包
        template <typename T, typename... Args>
        void printImpl(std::ostream& os, T&& first, Args&&... args)
        {
            os << std::forward<T>(first);
            if constexpr (sizeof...(args) > 0)
            {
                os << ' ';
                printImpl(os, std::forward<Args>(args)...);
            }
        }

        // 字符串视图优化 - 避免拷贝
        class StringView
        {
        private:
            const char* data_;
            size_t size_;

        public:
            template<size_t N>
            constexpr StringView(const char (&str)[N])
                : data_(str)
                , size_(N - 1) // 去掉null终止符
            {}

            StringView(const std::string& str)
                : data_(str.c_str())
                , size_(str.size())
            {}

            const char* data() const
            {
                return data_;
            }

            size_t size() const
            {
                return size_;
            }
        };
    }

    // 主打印函数 - 可变参数模板版本
    template <typename... Args>
    void print(Args&&... args)
    {
        detail::initConsole();
        detail::printImpl(std::cout, std::forward<Args>(args)...);
    }

    // 带换行的打印函数
    template <typename... Args>
    void println(Args&&... args)
    {
        print(std::forward<Args>(args)..., '\n');
    }

    // 格式化字符串输出 - 类似printf但类型安全
    template <typename... Args>
    void printfmt(const std::string& format, Args&&... args)
    {
        detail::initConsole();
        
        // 使用stringstream避免多次分配
        thread_local std::stringstream ss;
        ss.str("");
        ss.clear();
        
        detail::printImpl(ss, std::forward<Args>(args)...);
        
        std::cout << format << " " << ss.str() << '\n';
    }

    // 高性能批量输出
    class BatchPrinter
    {
    private:
        std::string buffer_;
        bool enabled_;

    public:
        BatchPrinter(bool enableBuffering = true)
            : enabled_(enableBuffering)
        {
            if (enabled_)
            {
                buffer_.reserve(4096); // 预分配4KB缓冲区
            }
        }

        template<typename T>
        BatchPrinter& operator<<(T&& value)
        {
            if (enabled_)
            {
                thread_local std::stringstream ss;
                ss.str("");
                ss.clear();
                ss << std::forward<T>(value);
                buffer_.append(ss.str());
            }
            else
            {
                std::cout << std::forward<T>(value);
            }
            return *this;
        }

        // 自增运算符换行
        BatchPrinter& operator++()
        {
            if (enabled_)
            {
                buffer_.push_back('\n');
            }
            else
            {
                std::cout << '\n';
            }
            return *this;
        }

        // 自减运算符换行
        BatchPrinter& operator--()
        {
            if (enabled_)
            {
                buffer_.push_back('\n');
            }
            else
            {
                std::cout << '\n';
            }
            return *this;
        }

        void flush()
        {
            if (enabled_ && !buffer_.empty())
            {
                std::cout.write(buffer_.data(), buffer_.size());
                buffer_.clear();
            }
            std::cout.flush();
        }

        ~BatchPrinter()
        {
            flush();
        }
    };

    // 带颜色的输出
    namespace color
    {
        enum ColorCode
        {
            RESET = 0,
            RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
            GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
        };

        inline void setColor(ColorCode color)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, color);
        }

        template <typename... Args>
        void print(ColorCode color, Args&&... args)
        {
            setColor(color);
            OptimisedPrint::print(std::forward<Args>(args)...);
            setColor(RESET);
        }

        template <typename... Args>
        void println(ColorCode color, Args&&... args)
        {
            setColor(color);
            OptimisedPrint::println(std::forward<Args>(args)...);
            setColor(RESET);
        }
    }

    // 进度条显示
    class ProgressBar
    {
    private:
        int width_;
        float progress_;
        std::string title_;

    public:
        ProgressBar(int width = 50, std::string title = "")
            : width_(width)
            , progress_(0.0f)
            , title_(std::move(title))
        {}

        void update(float progress)
        {
            progress_ = std::clamp(progress, 0.0f, 1.0f);
            display();
        }

        void display() const
        {
            int pos = static_cast<int>(width_ * progress_);
            
            std::cout << "\r" << title_ << " [";
            for (int i = 0; i < width_; ++i)
            {
                if (i < pos)
                {
                    std::cout << "=";
                }
                else if (i == pos)
                {
                    std::cout << ">";
                }
                else
                {
                    std::cout << " ";
                }
            }
            std::cout << "] " << int(progress_ * 100.0) << " %";
            std::cout.flush();
        }

        void complete()
        {
            progress_ = 1.0f;
            display();
            std::cout << std::endl;
        }
    };
}

#endif // IKUN_PRINT_HPP