/**
 * 公共配置头文件
 *
 * 此头文件包含了常用的头文件，其他文件只需包含这一个头文件就可以
 *
 * 如果要导入某头文件，只需在公共配置头文件中导入
 */
#ifndef CONFIG_H
#define CONFIG_H

// 公共头文件
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <ctime>
#include <pthread.h>

// 引用标准命名空间
using namespace std;

// 当开启测试时...
// 请在CMAKE中填写（使用DEBUG模式时）
#ifdef TEST

#include <cassert> // 断言
#include <fstream> // 文件流

#endif
#endif //CONFIG_H