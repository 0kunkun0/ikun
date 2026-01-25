// ikun库有关控制台颜色输出的头文件
// 可直接使用命名空间color中的常量字符串来控制输出颜色

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途
#ifndef IKUN_CONSOLE_COLOR_HPP
#define IKUN_CONSOLE_COLOR_HPP

namespace color
{
    constexpr const char* reset = "\033[0m";
    constexpr const char* red = "\033[31m";
    constexpr const char* green = "\033[32m";
    constexpr const char* blue = "\033[34m";
    constexpr const char* bold = "\033[1m";
    constexpr const char* yellow = "\033[33m";
    constexpr const char* purple = "\033[35m";
    constexpr const char* cyan = "\033[36m";
    constexpr const char* white = "\033[37m";
    constexpr const char* black = "\033[30m";
}

// 别想多了整个库就这么点
#endif // IKUN_CONSOLE_COLOR_HPP