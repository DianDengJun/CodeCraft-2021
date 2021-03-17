/**
 * 此头文件包含了: 请求类
 */

#ifndef REQUEST_H
#define REQUEST_H

#include "Config.h" // 包含公共配置头文件

// 请求操作标识
enum class RequestFlag {
    add, // 添加请求
    del  // 删除请求
};

// 类: 请求
struct Request {
    RequestFlag select; // 请求标识
    string addModel;    // 创建(add)时的虚拟机型号
    int vmID;           // 虚拟机ID
};

// 从流中输入请求
istream &operator>>(istream &is, Request &request) {
    string str;

    is.get();              // 读左括号
    getline(is, str, ','); // 输入请求标识
    is.get();              // 读空格

    // 解析请求标识
    if (str == "add") {
        request.select = RequestFlag::add;
        getline(is, request.addModel, ','); // 输入创建型号
        is.get();                           // 读空格
    } else
        request.select = RequestFlag::del;

    getline(is, str, ')');            // 读右括号
    is.get();                         // 读换行
    request.vmID = atoi(str.c_str()); // 转换为虚拟机ID

    // 返回输入流
    return is;
}

#ifdef TEST
// 输出请求到流中
ostream &operator<<(ostream &os, const Request &request) {
    if (request.select == RequestFlag::add)
        os << "add "
           << request.addModel << " ";
    else
        os << "del ";

    os << request.vmID;

    return os;
}
#endif

#endif // REQUEST_H
