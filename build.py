# ikun库构建用Python脚本
# 本库开源GitHub地址: https://github.com/0kunkun0/ikun
# 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
# 仅供个人, 非营利性组织, 开源项目以及竞赛使用
# 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途
from os import system

def main():
    # 检查编译器
    system("where g++.exe") # GCC
    system("where clang++.exe") # Clang
    system("where cl.exe") # MSVC

    # 询问用户然后编译
    compiler = input("请输入编译器名称(g++, clang++, cl): ")

    compilerargs = {
        "g++": "g++ -std=c++23 -O2 -Wall -Wextra -Werror -o ikun.exe ikun_core.cpp -lstdc++exp",
        "clang++": "clang++ -std=c++23 -O2 -Wall -Wextra -Werror -o ikun.exe ikun_core.cpp -lstdc++exp",
        "cl": "cl /EHsc /O2 /W4 /nologo ikun_core.cpp /Fe:ikun.exe",
    }

    if compiler in compilerargs:
        system(compilerargs[compiler])
    else:
        print(f"无效的编译器名称{compiler}")

if __name__ == "__main__":
    main()