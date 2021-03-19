/**
 * 此头文件包含了虚拟机类型类、虚拟机类
 */
#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "Config.h" // 包含公共配置头文件

// 虚拟机类型
struct VMType {
    string model;  // 型号
    int cpu;       // CPU核数
    int ram;       // 内存大小
    bool isDouble; // 是否双节点部署
    VMType() = default;
    VMType(const string &model) {
        this->model = model;
    }
    // 根据型号比较
    bool operator<(const VMType &rhs) const {
        return this->model < rhs.model;
    }
};

// 从流中输入虚拟机信息
istream &operator>>(istream &is, VMType &vmInfo) {
    // 读左括号
    is.get();
    // 输入型号
    getline(is, vmInfo.model, ',');
    // 读空格
    is.get();

    // 输入3个字符串
    string str[3];
    for (int i = 0; i < 3; i++) {
        // 确定分隔符
        char delim = (i == 2 ? ')' : ',');
        // 读字符串
        getline(is, str[i], delim);
        // 读空格或换行
        is.get();
    }
    // 将3个字符串转换为对应信息
    vmInfo.cpu = atoi(str[0].c_str());
    vmInfo.ram = atoi(str[1].c_str());
    vmInfo.isDouble = atoi(str[2].c_str());

    // 返回输入流
    return is;
}

#ifdef TEST

// 输出虚拟机类型到流中
ostream &operator<<(ostream &os, const VMType &vmInfo) {
    os << vmInfo.model << " "
       << vmInfo.cpu << " "
       << vmInfo.ram << " "
       << vmInfo.isDouble;
    return os;
}

#endif

// 类: 虚拟机
class VictualMachine {
public:
    int ID;      // 虚拟机编号
    VMType type; // 虚拟机类型
    VictualMachine() = default;
    // 以ID构造
    explicit VictualMachine(int id) {
        this->ID = id;
    }
    // 以ID和类型构造
    VictualMachine(int id, const VMType &type) {
        this->ID = id;
        this->type = type;
    }
    // 获取当前虚拟机的型号
    inline string model() const {
        return type.model;
    }
    // 获取当前虚拟机的CPU核心数
    inline int cpu() const {
        return type.cpu;
    }
    // 获取当前虚拟机的内存大小
    inline int ram() const {
        return type.ram;
    }
    // 获取当前虚拟机的部署类型
    inline bool isDouble() const {
        return type.isDouble;
    }
    // 以ID号进行比较
    bool operator<(const VictualMachine &rhs) const {
        return this->ID < rhs.ID;
    }
};


#endif // VIRTUALMACHINE_H
