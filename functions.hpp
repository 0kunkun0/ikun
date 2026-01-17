// ikun库functions.hpp
// 包括大量实用函数, 如文件操作, 随机数生成, 数学计算等
// 部分依赖文件操作和系统API的仅支持Windows
// 此版本仅供个人, 开源以及竞赛使用, 商业用途请遵循许可证规定

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