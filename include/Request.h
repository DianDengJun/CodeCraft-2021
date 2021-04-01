/**
 * 此头文件包含了: 请求类
 */
#ifndef REQUEST_H
#define REQUEST_H

#include "Config.h" // 包含公共配置头文件

// 类: 基本请求
class BasicRequest {
// 属性
protected:
    int vmID{0}; // 请求操作虚拟机的ID

// 构造函数
public:
    // 默认构造函数
    BasicRequest() = default;

    // 显式构造函数
    explicit BasicRequest(int vmId) : vmID{vmId} {}

// 访问器和设置器
public:
    inline int getVMId() const {
        return vmID;
    }

    inline void setVMId(int ID) {
        vmID = ID;
    }
};

// 类: 添加请求
class AddRequest : public BasicRequest {
// 属性
private:
    string addModel; // 创建(add)的虚拟机型号

// 构造函数
public:
    // 默认构造函数
    AddRequest() = default;

    // 有参构造函数
    AddRequest(int vmId, string model)
            : BasicRequest(vmId), addModel(std::move(model)) {}

// 访问器和设置器
public:
    inline const string &getAddModel() const {
        return addModel;
    }

    inline void setAddModel(const string &model) {
        this->addModel = model;
    }
};

// 从流中输入 AddRequest
istream &operator>>(istream &is, AddRequest &addRequest) {
    // 临时字符串
    string temp;

    // 输入并解析请求添加的虚拟机型号
    is >> temp;
    addRequest.setAddModel(temp.substr(0, temp.size() - 1));

    // 输入并解析请求添加的虚拟机ID
    is >> temp;
    addRequest.setVMId(atoi(temp.c_str()));

    // 返回输入流
    return is;
}

// 类: 删除请求
class DelRequest : public BasicRequest {
};

// 从流中输入DelRequest
istream &operator>>(istream &is, DelRequest &delRequest) {
    // 临时字符串
    string temp;

    // 输入并解析请求删除的虚拟机ID
    is >> temp;
    delRequest.setVMId(atoi(temp.c_str()));

    // 返回输入流
    return is;
}

#endif //REQUEST_H
