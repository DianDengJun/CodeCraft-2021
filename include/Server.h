/**
 * 此头文件包含了: 服务器类型类, 服务器类
 */
#ifndef SERVER_H
#define SERVER_H

#include "Config.h" // 包含公共配置头文件
#include "VirtualMachine.h"

// 服务器类型
struct ServerType {
    string model;     // 型号
    int cpu;          // CPU核数
    int ram;          // 内存大小
    int hardwareCost; // 硬件成本
    int dailyCost;    // 每日耗能成本
    // 根据硬件成本排序
    bool operator<(ServerType &rhs) const {
        return this->hardwareCost < rhs.hardwareCost;
    }
};

// 从流中输入服务器信息
istream &operator>>(istream &is, ServerType &serverInfo) {
    // 读左括号
    is.get();
    // 输入型号
    getline(is, serverInfo.model, ',');
    // 读空格
    is.get();

    // 输入4个字符串
    string str[4];
    for (int i = 0; i < 4; i++) {
        // 确定分隔符
        char delim = (i == 3 ? ')' : ',');
        // 读字符串
        getline(is, str[i], delim);
        // 读空格或换行
        is.get();
    }

    // 将4个字符串转换为对应信息
    serverInfo.cpu = atoi(str[0].c_str());
    serverInfo.ram = atoi(str[1].c_str());
    serverInfo.hardwareCost = atoi(str[2].c_str());
    serverInfo.dailyCost = atoi(str[3].c_str());

    // 返回输入流
    return is;
}

#ifdef TEST

// 输出服务器类型到流中
ostream &operator<<(ostream &os, const ServerType &serverInfo) {
    os << serverInfo.model << " "
       << serverInfo.cpu << " "
       << serverInfo.ram << " "
       << serverInfo.hardwareCost << " "
       << serverInfo.dailyCost;
    return os;
}

#endif

// 节点枚举
enum class Node {
    A,   // 表示A节点
    B,   // 表示B节点
    Both // 表示A与B节点
};
// 使用的格式: Node::A, Node::B, Node::Both

// 类: 服务器
class Server {
private:
    ServerType type; // 服务器信息
    int usedCpuA{0}; // A节点已用CPU核心数
    int usedCpuB{0}; // B节点已用CPU核心数
    int usedRamA{0}; // A节点已用内存大小
    int usedRamB{0}; // B节点已用内存大小
public:
    int ID;                     // 服务器编号
    vector<VictualMachine> A{}; // A节点装载的虚拟机
    vector<VictualMachine> B{}; // B节点装载的虚拟机

    // 使用ID和服务器类型构造一个服务器
    Server(int ID, const ServerType &type) {
        this->ID = ID;
        this->type = type;
    }

    // 获取CPU核心数
    inline int cpu() const {
        return type.cpu;
    }

    // 获取内存大小
    inline int ram() const {
        return type.ram;
    }

    // 获取单个节点CPU核心数
    inline int halfCpu() const {
        return cpu() / 2;
    }

    // 获取单个节点内存大小
    inline int halfRam() const {
        return ram() / 2;
    }

    // 获取型号
    inline string model() const {
        return type.model;
    }

    // 获取硬件成本
    inline int hardwareCost() const {
        return type.hardwareCost;
    }

    // 获取每日耗能成本
    inline int dailyCost() const {
        return type.dailyCost;
    }

    // 已用CPU核心数
    inline int usedCpu() const {
        return usedCpuA + usedCpuB;
    }

    // 某节点已用CPU核心数
    int usedCpuCores(Node node) const {
        int use{0};
        switch (node) {
            case Node::A:
                use = usedCpuA;
                break;
            case Node::B:
                use = usedCpuB;
                break;
            case Node::Both:
                use = usedCpu();
                break;
        }
        return use;
    }

    // 已用内存大小
    inline int usedRam() const {
        return usedRamA + usedRamB;
    }

    // 某节点已用内存大小
    int usedRam(Node node) const {
        int use{0};
        switch (node) {
            case Node::A:
                use = usedRamA;
                break;
            case Node::B:
                use = usedRamB;
                break;
            case Node::Both:
                use = usedRam();
                break;
        }
        return use;
    }

    // 空闲CPU核心数
    inline int freeCpu() const {
        return cpu() - usedCpu();
    }

    // 某节点空闲CPU核心数
    int freeCpu(Node node) const {
        int cores;
        switch (node) {
            case Node::A:
                cores = halfCpu() - usedCpuA;
                break;
            case Node::B:
                cores = halfCpu() - usedCpuB;
                break;
            case Node::Both:
                cores = freeCpu();
                break;
        }
        return cores;
    }

    // 空闲内存大小
    inline int freeRam() const {
        return ram() - usedRam();
    }

    // 某节点空闲内存大小
    int freeRam(Node node) const {
        int size;
        switch (node) {
            case Node::A:
                size = halfRam() - usedRamA;
                break;
            case Node::B:
                size = halfRam() - usedRamB;
                break;
            case Node::Both:
                size = freeRam();
                break;
        }
        return size;
    }

    // 检测当前服务器所用资源是否已满
    inline bool isFull() const {
        return !(usedCpu() <= cpu() && usedRam() <= ram());
    }

    // 是否可以添加一个双节点虚拟机
    bool canAddDoubleVM(const VictualMachine &vm) const {
        if (!vm.isDouble() || isFull()) {
            return false;
        } else {
            bool hasFreeCpuA = freeCpu(Node::A) >= vm.cpu() / 2;
            bool hasFreeCpuB = freeCpu(Node::B) >= vm.cpu() / 2;
            bool hasFreeRamA = freeRam(Node::A) >= vm.ram() / 2;
            bool hasFreeRamB = freeRam(Node::B) >= vm.ram() / 2;
            return (hasFreeCpuA && hasFreeRamA && hasFreeCpuB && hasFreeRamB);
        }
    }

    // 是否可以添加一个单节点虚拟机
    bool canAddSingleVM(const VictualMachine &vm) const {
        if (vm.isDouble() || isFull()) {
            return false;
        } else {
            bool hasFreeCpuA = freeCpu(Node::A) >= vm.cpu();
            bool hasFreeCpuB = freeCpu(Node::B) >= vm.cpu();
            bool hasFreeRamA = freeRam(Node::A) >= vm.ram();
            bool hasFreeRamB = freeRam(Node::B) >= vm.ram();
            bool canAddToA = hasFreeCpuA && hasFreeRamA;
            bool canAddToB = hasFreeCpuB && hasFreeRamB;
            return canAddToA || canAddToB;
        }
    }

    // 是否可以添加一个单节点虚拟机到某节点
    bool canAddSingleVM(const VictualMachine &vm, Node node) const {
        // node 指定要添加到的节点，如果是Both是检测满足任意一个节点
        if (vm.isDouble() || isFull()) {
            return false;
        } else {
            bool hasFreeCpuA = freeCpu(Node::A) >= vm.cpu();
            bool hasFreeCpuB = freeCpu(Node::B) >= vm.cpu();
            bool hasFreeRamA = freeRam(Node::A) >= vm.ram();
            bool hasFreeRamB = freeRam(Node::B) >= vm.ram();
            bool canAddToA = hasFreeCpuA && hasFreeRamA;
            bool canAddToB = hasFreeCpuB && hasFreeRamB;
            bool canAdd = (node == Node::Both && canAddToA && canAddToB) ||
                          (node == Node::A && canAddToA) ||
                          (node == Node::B && canAddToB);
            return canAdd;
        }
    }

    // 是否可以添加一个虚拟机
    bool canAddVm(const VictualMachine &vm) const {
        return (canAddSingleVM(vm) || canAddDoubleVM(vm));
    }

    // 添加一个双节点部署的虚拟机
    void addDoubleVM(VictualMachine &vm) {
        if (canAddDoubleVM(vm)) {
            A.push_back(vm);
            B.push_back(vm);
            usedCpuA += vm.cpu() / 2;
            usedCpuB += vm.cpu() / 2;
            usedRamA += vm.ram() / 2;
            usedRamB += vm.ram() / 2;
        } else
            throw invalid_argument{"Can not add this Dual VM!"};
    }

    // 添加单节点虚拟机到指定节点
    void addSingleVM(VictualMachine &vm, Node node) {
        if (node == Node::A && canAddSingleVM(vm, Node::A)) {
            A.push_back(vm);
            usedCpuA += vm.cpu();
            usedRamA += vm.ram();
        } else if (node == Node::B && canAddSingleVM(vm, Node::B)) {
            B.push_back(vm);
            usedCpuB += vm.cpu();
            usedRamB += vm.ram();
        }
    }

    // 添加一个单节点部署的虚拟机
    // 返回添加到节点的枚举
    Node addSingleVM(VictualMachine &vm) {
        if (canAddSingleVM(vm, Node::Both)) {
            // 如果A节点明显比B节点空闲
            if (freeCpu(Node::A) >= freeCpu(Node::B) && freeRam(Node::A) >= freeRam(Node::B)) {
                addSingleVM(vm, Node::A);
                return Node::A;
            } else {
                addSingleVM(vm, Node::B);
                return Node::B;
            }
        } else if (canAddSingleVM(vm, Node::A)) {
            addSingleVM(vm, Node::A);
            return Node::A;
        } else if (canAddSingleVM(vm, Node::B)) {
            addSingleVM(vm, Node::B);
            return Node::B;
        } else
            throw invalid_argument{"Can not add this Sig VM!"};
    }

    // 添加一个虚拟机(综合判断)
    // 返回添加到节点的枚举
    // 推荐使用此方法
    Node addVM(VictualMachine &vm) {
        if (canAddSingleVM(vm)) {
            return addSingleVM(vm);
        } else if (canAddDoubleVM(vm)) {
            addDoubleVM(vm);
            return Node::Both;
        } else {
            throw invalid_argument{"Can not add this VM!"};
        }
    }

    // 是否在当前服务器中存在某ID号的虚拟机
    bool hasVM(int id) {
        for (auto &i : A) {
            if (i.ID == id)
                return true;
        }
        for (auto &i : B) {
            if (i.ID == id)
                return true;
        }
        return false;
    }
};


#endif // SERVER_H
