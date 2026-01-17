// ikun库functions.hpp
// 包括大量实用函数, 如文件操作, 随机数生成, 数学计算等
// 部分依赖文件操作和系统API的仅支持Windows
// 本库开源GitHub地址: https://github.com/0kunkun0/ikun
// 下载本库开源完整版: git clone https://github.com/0kunkun0/ikun.git
// 仅供个人, 非营利性组织, 开源项目以及竞赛使用
// 根据GPL 3.0许可证规定, 禁止使用本库进行闭源用途

#ifndef IKUN_FUNCTIONS_HPP
#define IKUN_FUNCTIONS_HPP

#ifndef IKUN_CORE
    #error "请先使用#include "ikun/core.hpp"包含ikun库核心"
    #error "否则无法使用版本管理功能"
#endif // IKUN_CORE

#include "functions/libs.hpp"
#include "functions/mathematics.hpp"
#include "functions/times.hpp"
#include "functions/random.hpp"

#endif // IKUN_FUNCTIONS_HPP