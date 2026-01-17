# ikun库构建用Python脚本
# 本库开源GitHub地址: https://github.com/0kunkun0/ikun
# 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git

# 仅供个人, 非营利性组织, 开源项目以及竞赛使用
# 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

from os import system
from time import sleep

def main():
    # 检查编译器
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

    print("MSVC:(版本要>= 19)")
    sleep(0.1)
    system("cl") # MSVC
    print("")

    sleep(0.1)
    # 询问用户然后编译
    compiler = input("请输入编译器名称(g++, clang++, cl): ")

    compilerargs = {
        "g++": "g++ -std=c++26 -O3 -Wall -Wextra -Werror -o ikun.exe ikun_core.cpp -lstdc++exp",
        "clang++": "clang++ -std=c++26 -O3 -Wall -Wextra -Werror -o ikun.exe ikun_core.cpp -lstdc++exp -stdlib=libc++",
        "cl": "cl /EHsc /O2 /W4 /nologo /std:c++latest ikun_core.cpp /Fe:ikun.exe",
    }

    if compiler in compilerargs:
        system(compilerargs[compiler])
    else:
        print(f"无效的编译器名称{compiler}")
        return -1
    
    print("编译完成(当然也可能失败)")
    return 0

if __name__ == "__main__":
    main()