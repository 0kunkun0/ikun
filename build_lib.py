# ikun库构建用Python脚本
# 本库开源GitHub地址: https://github.com/0kunkun0/ikun
# 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git

# 仅供个人, 非营利性组织, 开源项目以及竞赛使用
# 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

from os import *
from time import sleep

ikun_path = getcwd()

def download_file_from_github_simple(lib_name):
    print(f"正在下载{lib_name}...")
    sleep(0.5)
    # 添加 -k 参数忽略证书检查，或使用 --ssl-no-revoke
    command = f'curl -k -L -o "{lib_name}" "https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/{lib_name}"'
    print(f'当前操作: {command}')
    result = system(command)
    if result != 0:
        # 尝试备用方法
        print(f"curl下载失败，尝试备用方法...")
        # 可以尝试使用 Python 的 urllib 作为备用
        try:
            import urllib.request
            url = f"https://raw.githubusercontent.com/0kunkun0/ikun/refs/heads/main/{lib_name}"
            urllib.request.urlretrieve(url, lib_name)
            print(f"备用方法下载成功: {lib_name}")
        except:
            print(f"所有下载方法都失败: {lib_name}")
            return False
    print("正在检查中...")
    # Windows 应该用 dir 而不是 type
    result = system(f"dir {lib_name} > nul 2>&1" if name == 'nt' else f"ls {lib_name} > /dev/null 2>&1")
    if result != 0:
        print(f"警告: 文件 {lib_name} 可能未正确下载")
    return True

def init_lib_core():
    libs = {
        "ikun_core.cpp",
        "core.hpp",
        "github.hpp",
        "stdc++lib.hpp"
    }
    print("注意: 这只会下载库必须的文件, 其他文件请自行下载")
    for lib in libs:
        download_file_from_github_simple(lib)
    print("下载完成, 请输入2来编译库主程序")

def build():
    # 检查编译器
    print("检查编译器...")
    sleep(0.5)
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
    
    # 询问用户然后编译
    compiler = input("请输入编译器名称(1: GCC, 2: Clang): ")

    compilerargs = {
        "1": "g++ -std=c++26 -O3 -lstdc++exp ikun_core.cpp -o ikun.exe",
        "2": "clang++ -std=c++26 -O3 -lstdc++exp -stdlib=libc++ ikun_core.cpp -o ikun.exe"
    }

    if compiler in compilerargs:
        system(compilerargs[compiler])
    else:
        print("编译器名称错误")
        return 1
    
    return 0

def main():
    print("ikun库构建工具")
    print("通过GitHub仓库下载ikun_core.cpp并编译成ikun.exe")
    print("最后通过ikun.exe来调用ikun库")
    print("请将build_lib.py和build.bat放在工作区下的ikun文件夹下")
    print("")

    while True:
        print("1. 下载ikun库核心")
        print("2. 编译ikun库管理器")
        print("3. 配置环境变量")
        print("4. 退出")

        print("推荐按照顺序执行\n")

        choice = input("请输入选项: ")

        if choice == "1":
            init_lib_core()
        elif choice == "2":
            build()
        elif choice == "3":
            current_path = getcwd()
            print(f"当前路径: {current_path}")
            print("注意: 在 Windows 上需要管理员权限才能设置系统环境变量")
            choice = input("设置为用户环境变量(1)还是系统环境变量(2)? [1/2]: ")
            if choice == "1":
                system(f'setx PATH "%PATH%;{current_path}"')
            elif choice == "2":
                system(f'setx PATH "%PATH%;{current_path}" /M')
            else:
                print("无效选择")
        elif choice == "4":
            return 0
        else:
            print(f"无效选项: {choice}")


if __name__ == "__main__":
    main()