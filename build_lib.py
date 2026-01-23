#!/usr/bin/env python3
# ikun库构建用Python脚本
# 本库开源GitHub地址: https://github.com/0kunkun0/ikun
# 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git

# 仅供个人, 非营利性组织, 开源项目以及竞赛使用
# 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

from os import *
from time import sleep
import platform

ikun_path = getcwd()
compilerargs_on_windows = {
    1: "g++ -std=c++26 -O3 -lstdc++exp ikun_core.cpp -o ikun.exe",
    2: "clang++ -std=c++26 -O3 -lstdc++exp -stdlib=libc++ ikun_core.cpp -o ikun.exe",
    3: "cl /EHsc /std:c++latest /O2 /MT /Zc:__cplusplus ikun_core.cpp /Fe:ikun.exe",
    4: "g++ -std=c++26 -O3 -lstdc++exp ikun_error_analyzer.cpp -o ikun_error_analyzer.exe",
    5: "clang++ -std=c++26 -O3 -lstdc++exp -stdlib=libc++ ikun_error_analyzer.cpp -o ikun_error_analyzer.exe",
    6: "cl /EHsc /std:c++latest /O2 /MT /Zc:__cplusplus ikun_error_analyzer.cpp /Fe:ikun_error_analyzer.exe"
}
compilerargs_on_linux = {
    1: "g++ -std=c++26 -O3 -lstdc++exp ikun_core.cpp -o ikun",
    2: "clang++ -std=c++26 -O3 -lstdc++exp -stdlib=libc++ ikun_core.cpp -o ikun",
    3: "g++ -std=c++26 -O3 -lstdc++exp ikun_error_analyzer.cpp -o ikun_error_analyzer",
    4: "clang++ -std=c++26 -O3 -lstdc++exp -stdlib=libc++ ikun_error_analyzer.cpp -o ikun_error_analyzer"
}

system_env = platform.system()
system_arch = platform.architecture()[0]
system_status = ""
match system_env:
    case "Windows":
        system_status = "Windows "
        if system_arch.endswith("64"):
            system_status += "x86_64"
        elif "x86" in system_arch:
            system_status += "x86"
        else:
            system_status += "Unknown"
    case "Linux":
        system_status = "Linux "
        if system_arch.endswith("64"):
            system_status += "x86_64"
        elif "x86" in system_arch:
            system_status += "x86"
        else:
            system_status += "Unknown"
    case "Darwin":
        system_status = "Apple macOS "
        if system_arch.endswith("64"):
            system_status += "x86_64"
        elif "arm" in system_arch:
            system_status += "ARM64E"
        else:
            system_status += "Unknown"
    case "Java":
        system_status = "Java JVM (Unsupported)"
    case _:
        system_status = "Unknown " + system_arch

def download_file_from_github_simple(lib_name):
    global system_status
    print(f"正在下载{lib_name}...")
    sleep(0.5)
    #参数忽略证书检查
    command = f'curl -k -L -o "{lib_name}" "https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/{lib_name}"'
    print(f'当前操作: {command}')
    result = system(command)
    if result != 0:
        # 尝试备用方法
        print(f"curl下载失败，尝试备用方法...")
        # 尝试使用 Python 的 urllib 作为备用
        try:
            import urllib.request
            url = f"https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/{lib_name}"
            urllib.request.urlretrieve(url, lib_name)
            print(f"备用方法下载成功: {lib_name}")
        except:
            print(f"所有下载方法都失败: {lib_name}")
            return False
    print("正在检查中...")
    if system_status.startswith("Windows"):
        result = system(f"dir {lib_name} > nul 2>&1" if name == 'nt' else f"ls {lib_name} > /dev/null 2>&1")
    else:
        result = system(f"ls {lib_name} > /dev/null 2>&1")
    if result != 0:
        print(f"警告: 文件 {lib_name} 可能未正确下载")
    return True

def init_lib_core():
    libs = {
        "ikun_core.cpp",
        "stdc++lib.hpp",
        "ikun_error_analyzer.cpp",
        "ikun_stderr.hpp"
    }
    print("注意: 这只会下载库必须的文件, 其他文件请自行通过库管理器下载")
    for lib in libs:
        download_file_from_github_simple(lib)
    print("下载完成, 请输入2来编译库管理器和错误分析器")

def build_core():
    global compilerargs_on_windows, compilerargs_on_linux, system_status
    # 检查编译器
    print("检查编译器...")
    system("pause")
    print("GCC(版本要>= 11):")
    sleep(0.1)
    system("g++ --version") # GCC
    sleep(0.1)
    print("")

    print("Clang(版本要>= 14):")
    sleep(0.1)
    system("clang++ --version") # Clang
    sleep(0.1)
    print("")
    sleep(0.1)

    print("MSVC(版本要>= 19.30):")
    if system_status.startswith("Windows"):
        sleep(0.1)
        system("cl") # MSVC
        sleep(0.1)
        print("")
    else:
        print(f"{system_status}不支持MSVC")
    print("将会编译ikun(.exe)(库核心管理器)和ikun_error_analyzer(.exe)(错误分析器)")
    system("pause")
    
    # 询问用户然后编译
    compiler = int(input("请输入编译器编号(1: GCC, 2: Clang, 3:MSVC): "))

    if compiler in compilerargs_on_windows or compiler in compilerargs_on_linux:
        if compiler == 3 and not system_status.startswith("Windows"):
            print(f"{system_status}不支持MSVC")
            return 1
        if system_status.startswith("Windows"):
            system(compilerargs_on_windows[compiler])
            system(compilerargs_on_windows[compiler + 3])
        else:
            system(compilerargs_on_linux[compiler])
            system(compilerargs_on_linux[compiler + 2])
        if compiler == 3:
            # MSVC编译后需要清理未链接可执行文件和中间文件
            system("del *.obj")
            system("del *.ilk")
    else:
        print("编译器名称错误")
        return 1
    
    return 0

def main():
    global system_status
    print("ikun库构建工具")
    print("通过GitHub仓库下载ikun_core.cpp并编译成ikun(.exe)")
    print("最后通过ikun(.exe)来调用ikun库版本管理和相关项目编译")
    print("请将build_lib.py和build.bat/.sh放在工作区下的ikun文件夹下")
    print("")

    while True:
        print("1. 下载ikun库核心")
        print("2. 编译ikun库管理器")
        print("3. 配置环境变量")
        print("4. 退出")

        choice = input("请输入选项: ")

        if choice == "1":
            init_lib_core()
        elif choice == "2":
            build_core()
        elif choice == "3":
            current_path = getcwd()
            if system_status.startswith("Windows"):
                print(f"当前路径: {current_path}")
                print("注意: 在 Windows 上需要管理员权限才能设置系统环境变量")
                choice = input("设置为用户环境变量(1)还是系统环境变量(2)? [1/2]: ")
                if choice == "1":
                    system(f'setx PATH "%PATH%;{current_path}"')
                elif choice == "2":
                    system(f'setx PATH "%PATH%;{current_path}" /M')
                else:
                    print("无效选择")
            else:
                print(f"当前路径: {current_path}")
                print("注意: 在 macOS 或 Linux 上需要root权限才能设置系统环境变量")
                choice = input("设置为用户环境变量(1)还是系统环境变量(2)? [1/2]: ")
                if choice == "1":
                    system(f'export PATH="$PATH:{current_path}"')
                elif choice == "2":
                    system(f'sudo export PATH="$PATH:{current_path}"')
                else:
                    print("无效选择")
        elif choice == "4":
            return 0
        else:
            print(f"无效选项: {choice}")


if __name__ == "__main__":
    main()