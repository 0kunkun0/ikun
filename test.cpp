// ikun库测试程序
// 用于测试ikun库的函数
// 版本7.1.2 Preview Build 2026.1.24
// 运行测试程序前请先安装本库的各种头文件, 如没有all_libs.hpp将无法编译

// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 遵循GPL 3.0许可证, 禁止使用本库进行闭源用途

#if __has_include("all_libs.hpp")
#include "all_libs.hpp"
#else
#error "当前目录不存在all_libs.hpp"
#endif

#ifdef IKUN_FILES_HPP
void test_file()
{
    println("\nfiles.hpp测试:");
    wfile("test.ikun", "test");
    println("文件创建wfile()函数测试通过(如果没通过会抛出运行时错误)");

    if (fileexists("test.ikun")) println("文件存在, fileexists()函数测试通过");
    else
    {
        println("文件不存在, fileexists()函数测试失败");
        return;
    }

    string s = rfile("test.ikun");
    if (s == "test") println("文件读取rfile()函数测试通过");
    else
    {
        println("文件读取rfile()函数测试失败");
        return;
    }

    wfile("test2.ikun", "test2");
    vector<string> v = filedir(".", ".ikun");
    if (v == vector<string>{"test.ikun", "test2.ikun"}) println("获取路径下的文件filedir()函数测试通过");
    else
    {
        println("获取路径下的文件filedir()函数测试失败");
        return;
    }

    mkdir("testdir");
    if (fileexists("testdir")) println("创建文件夹mkdir()函数测试通过");
    else
    {
        println("创建文件夹mkdir()函数测试失败");
        return;
    }

    if (check_file_name("test.ikun")) println("检查文件名check_file_name()函数测试1通过");
    else
    {
        println("检查文件名check_file_name()函数测试1失败");
        return;
    }
    if (check_file_name("test.ikun/")) println("检查文件名check_file_name()函数测试2通过");
    else
    {
        println("检查文件名check_file_name()函数测试2失败");
        return;
    }

    if (check_dir_name("testdir")) println("检查文件夹名check_dir_name()函数测试1通过");
    else
    {
        println("检查文件夹名check_dir_name()函数测试1失败");
        return;
    }

    if (check_dir_name("testdir>")) println("检查文件夹名check_dir_name()函数测试2通过");
    else
    {
        println("检查文件夹名check_dir_name()函数测试2失败");
        return;
    }

    println("文件操作函数测试通过");

    #ifdef __linux__
    system("rm *.ikun");
    #elifdef _WIN32
    system("del *.ikun");
    #endif
    system("rmdir testdir");
}
#endif

#ifdef IKUN_TIMES_HPP
void test_time()
{
    println("\ntimes.hpp测试:");
    println("注意: 本测试没有标准答案, 请自行判断是否正确");
    println("当前时间: {}", get_time());
    println("当前星期{}(0~6, 0为周日)", get_weekday());
    println("当前年份: {}", get_year());
    println("当前月份: {}", get_month());
    println("当前日期: {}", get_day());
    println("当前小时: {}", get_hour());
    println("当前分钟: {}", get_minute());
    println("当前秒数: {}", get_second());
    println("当前时间戳: {}", get_ts());

    println("时间操作函数测试通过");
}
#endif

#ifdef IKUN_MATHS_HPP
void test_maths()
{
    println("\nmaths.hpp测试:");
    println("注意: 本测试有运行时计算, 时间复杂度可能较高");
    println("π的值(一般精度到16~18位就开始不准确了): {:.40f}", PI);
    println("此处定义的实际值:                       3.1415926535897932384626433832795028841971");

    println("20的阶乘(模板元编程): {}", Fac<20>::value);
    println("20的阶乘(运行时计算): {}", fac(20));
    println("实际值:               2432902008176640000");

    println("斐波那契数列(模板元编程): {}", Fib<20>::value);
    println("斐波那契数列(运行时计算): {}", fib(20));
    println("实际值:                   6765");

    println("2的20次方(模板元编程): {}", Pow<2, 20>::value);
    println("2的20次方(运行时计算): {}", pow(2, 20));
    println("实际值:                1048576");

    println("5是不是质数:   {}", is_prime(5));
    println("10是不是质数:  {}", is_prime(10));
    println("上面的结果应为:true, false");

    println("78和91的最大公因数: {}", gcd(91, 78));
    println("上面的结果应为:     13");

    println("78和91的最小公倍数: {}", lcm(91, 78));
    println("上面的结果应为:     546");

    println("数学函数测试通过");
}
#endif

#ifdef IKUN_RANDOM_HPP
void test_random()
{
    Random r;
    println("\nrandom.hpp测试:");
    println("注意: 本测试没有标准答案, 请自行判断是否正确");
    println("随机数生成器测试");
    println("1~3随机数:            {}", r.random_int(1, 3));
    println("78~7891随机数:        {}", r.random_int(78, 7891));
    println("随机可显示ASCII字符:   {}", r.random_char());
    println("随机字符串(长度为10):  {}", r.random_string(10));

    println("随机数生成器测试通过");
}
#endif

int main()
{
    println("ikun库测试程序");
    println("C++版本:{}", get_cppversion());
#ifdef IKUN_FILES_HPP
    test_file();
#endif
#ifdef IKUN_TIMES_HPP
    test_time();
#endif
#ifdef IKUN_MATHS_HPP
    test_maths();
#endif
#ifdef IKUN_RANDOM_HPP
    test_random();
#endif
    return 0;
}