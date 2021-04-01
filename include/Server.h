/**
 * 此头文件包含了: 服务器类
 */
#ifndef SERVER_H
#define SERVER_H

#include "Config.h" // 包含公共配置头文件
#include "VM.h"

// 类: 基本服务器(不含model,id,装载的VM)
class BasicServer {
// 属性
protected:
    int cpu{0};            // CPU核心数
    int ram{0};            // 内存大小
    int hardwareCost{0};   // 硬件成本
    int dailyCost{0};      // 每日耗能成本

// 构造函数
public:
    // 默认构造函数
    BasicServer() = default;

    // 有参构造函数
    BasicServer(int CPU, int RAM, int HardwareCost, int DailyCost) {
        setCpu(CPU);
        setRam(RAM);
        setHardwareCost(HardwareCost);
        setDailyCost(DailyCost);
    }

// 访问器与设置器
public:
    inline int getCpu() const {
        return cpu;
    }

    inline void setCpu(int CPU) {
        this->cpu = CPU;
    }

    inline int getRam() const {
        return ram;
    }

    inline void setRam(int RAM) {
        this->ram = RAM;
    }

    inline int getHardwareCost() const {
        return hardwareCost;
    }

    inline void setHardwareCost(int HardwareCost) {
        this->hardwareCost = HardwareCost;
    }

    inline int getDailyCost() const {
        return dailyCost;
    }

    inline void setDailyCost(int DailyCost) {
        this->dailyCost = DailyCost;
    }

// 操作符
public:
    // 重载小于运算符，用于排序
    bool operator<(const BasicServer &rhs) const {
        return this->hardwareCost > rhs.hardwareCost;
    }
};

// 从流中输入BasicServer
istream &operator>>(istream &is, BasicServer &basicServer) {
    // 临时字符串数组
    string temp[4];

    // 输入4个字符串
    for (auto &i:temp)
        is >> i;

    // 解析为对应信息
    basicServer.setCpu(atoi(temp[0].c_str()));
    basicServer.setRam(atoi(temp[1].c_str()));
    basicServer.setHardwareCost(atoi(temp[2].c_str()));
    basicServer.setDailyCost(atoi(temp[3].c_str()));

    // 返回输入流
    return is;
}

// 枚举: 节点
enum class Node {
    A, // A节点
    B, // B节点
    Both, // A与B节点
};

// 类: 虚拟机信息(不含ID,装载的VM)
class ServerInfo : public BasicServer {
// 属性
protected:
    string model; // 型号

// 构造函数
public:
    // 默认构造函数
    ServerInfo() = default;

    // 有参构造函数
    ServerInfo(const string &Model, int CPU, int RAM, int HardwareCost, int DailyCost)
            : BasicServer(CPU, RAM, HardwareCost, DailyCost) {
        setModel(Model);
    }

    // 以 model 和 BasicServer 构造
    ServerInfo(const string &model, const BasicServer &basicServer)
            : BasicServer(basicServer) {
        setModel(model);
    }

// 访问器和设置器
public:
    // 设置BasicServer
    void setBasicServer(const BasicServer &basicServer) {
        setCpu(basicServer.getCpu());
        setRam(basicServer.getRam());
        setHardwareCost(basicServer.getHardwareCost());
        setDailyCost(basicServer.getDailyCost());
    }

    const string &getModel() const {
        return model;
    }

    void setModel(const string &Model) {
        this->model = Model;
    }
};

// 重载小于运算符，用以排序ServerInfo
bool operator<(const ServerInfo& lhs, const ServerInfo &rhs) {
    return lhs.getHardwareCost() < rhs.getHardwareCost();
}

// 从流中输入服务器信息
istream &operator>>(istream &is, ServerInfo &serverInfo) {
    // 临时字符串
    string temp;

    // 输入并解析服务器型号
    is >> temp;
    serverInfo.setModel(temp.substr(1, temp.size() - 2));

    // 临时 BasicServer
    BasicServer basicServer;

    // 输入并解析 BasicServer
    is >> basicServer;
    serverInfo.setBasicServer(basicServer);

    // 返回输入流
    return is;
}

// 类: 装载的虚拟机
class LoadedVM final : public VMType {
// 属性
private:
    Node loadedNode{}; // 装载的节点

// 构造函数
public:
    // 默认构造函数
    LoadedVM() = default;

    // 有参构造函数
    LoadedVM(Node LoadedNode, const VMType &vmType)
            : VMType(vmType) {
        setLoadedNode(LoadedNode);
    }

// 访问器与设置器
public:
    inline Node getLoadedNode() const {
        return loadedNode;
    }

    void setLoadedNode(Node LoadedNode) {
        if (this->isDual()) {
            if (LoadedNode == Node::Both)
                this->loadedNode = Node::Both;
            else
                throw invalid_argument(
                        "LoadedVM::setLoadedNode(Node): Node is dual, but this->isDual() is not dual.");
        } else {
            if (LoadedNode != Node::Both)
                this->loadedNode = LoadedNode;
            else
                throw invalid_argument(
                        "LoadedVM::setLoadedNode(Node): Node is not dual, but this->isDual() is dual.");
        }
    }
};

// 类: 服务器类型(不含ID)
class ServerType : public ServerInfo {
// 属性
private:
    int usedCpuA{0}; // A节点已用CPU核心数
    int usedCpuB{0}; // B节点已用CPU核心数
    int usedRamA{0}; // A节点已用内存大小
    int usedRamB{0}; // B节点已用内存大小

public:
    unordered_map<int, LoadedVM> loadedVMs{}; // 装载的虚拟机, 以ID号为索引值

// 构造函数
public:
    // 默认构造函数
    ServerType() = default;

    // 有参构造函数
    ServerType(const string &model, int cpu, int ram, int hardwareCost, int dailyCost)
            : ServerInfo(model, cpu, ram, hardwareCost, dailyCost) {}

    // 通过 model 和  BasicServer 构造
    ServerType(const string &model, const BasicServer &basicServer)
            : ServerInfo(model, basicServer) {}

    // 通过 ServerInfo 构造 (显式构造)
    explicit ServerType(const ServerInfo &serverInfo)
            : ServerInfo(serverInfo) {}

// 访问器与设置器
public:
    // 设置服务器信息
    void setServerInfo(const ServerInfo &serverInfo) {
        setCpu(serverInfo.getCpu());
        setRam(serverInfo.getRam());
        setDailyCost(serverInfo.getDailyCost());
        setHardwareCost(serverInfo.getHardwareCost());
    }

    // 获取虚拟机个数
    inline size_t numOfVM() const {
        return loadedVMs.size();
    }

    // 获取CPU核心数的一半大小
    inline int halfCpu() const {
        return cpu / 2;
    }

    // 获取某个节点的CPU大小
    inline int getCpu(Node node = Node::Both) const {
        return node == Node::Both ? cpu : halfCpu();
    }

    // 获取内存大小的一半
    inline int halfRam() const {
        return ram / 2;
    }

    // 获取某个节点的内存大小
    inline int getRam(Node node = Node::Both) const {
        return node == Node::Both ? ram : halfRam();
    }

// 容量
public:
    // 某节点已用CPU核心数
    int usedCpu(Node node = Node::Both) const {
        int use{0};
        switch (node) {
            case Node::A:
                use = usedCpuA;
                break;
            case Node::B:
                use = usedCpuB;
                break;
            case Node::Both:
                use = usedCpuA + usedCpuB;
                break;
        }
        return use;
    }

    // 某节点空闲CPU核心数
    inline int freeCpu(Node node = Node::Both) const {
        return getCpu(node) - usedCpu(node);
    }

    // 某节点CPU闲置率
    inline double freeRateCpu(Node node = Node::Both) const {
        return double(freeCpu(node)) / getCpu(node);
    }

    // 某节点已用内存大小
    int usedRam(Node node = Node::Both) const {
        int use{0};
        switch (node) {
            case Node::A:
                use = usedRamA;
                break;
            case Node::B:
                use = usedRamB;
                break;
            case Node::Both:
                use = usedRamA + usedRamB;
                break;
        }
        return use;
    }

    // 某节点空闲内存大小
    inline int freeRam(Node node = Node::Both) const {
        return getRam(node) - usedRam(node);
    }

    // 某节点内存闲置率
    inline double freeRateRam(Node node = Node::Both) const {
        return double(freeRam(node)) / getRam(node);
    }

    // 某节点的平均闲置率
    inline double freeRate(Node node = Node::Both) const {
        return 0.5 * (freeRateCpu(node) + freeRateRam(node));
    }

    // 检测当前服务器所用资源是否已满
    inline bool isFull() const {
        return !(usedCpu() <= cpu && usedRam() <= ram);
    }

    // 检测某节点所用资源是否已满
    inline bool isFull(Node node) const {
        if (node == Node::Both) {
            return isFull(Node::A) || isFull(Node::B);
        } else {
            bool isCpuFull = (freeCpu(node) == 0);
            bool isRamFull = (freeRam(node) == 0);
            return isCpuFull || isRamFull;
        }
    }

// 修改
public:
    // 是否可以添加一个双节点虚拟机
    bool canAddDualVM(const VMType &vmType) const {
        if (!vmType.isDual() || isFull()) {
            return false;
        } else {
            bool hasFreeCpuA = freeCpu(Node::A) >= vmType.getCpu() / 2;
            bool hasFreeCpuB = freeCpu(Node::B) >= vmType.getCpu() / 2;
            bool hasFreeRamA = freeRam(Node::A) >= vmType.getRam() / 2;
            bool hasFreeRamB = freeRam(Node::B) >= vmType.getRam() / 2;
            bool canAddToA = hasFreeCpuA && hasFreeRamA;
            bool canAddToB = hasFreeCpuB && hasFreeRamB;
            return canAddToA && canAddToB;
        }
    }

    // 是否可以添加一个单节点虚拟机
    bool canAddSingleVM(const VMType &vmType) const {
        if (vmType.isDual() || isFull()) {
            return false;
        } else {
            bool hasFreeCpuA = freeCpu(Node::A) >= vmType.getCpu();
            bool hasFreeCpuB = freeCpu(Node::B) >= vmType.getCpu();
            bool hasFreeRamA = freeRam(Node::A) >= vmType.getRam();
            bool hasFreeRamB = freeRam(Node::B) >= vmType.getRam();
            bool canAddToA = hasFreeCpuA && hasFreeRamA;
            bool canAddToB = hasFreeCpuB && hasFreeRamB;
            return canAddToA || canAddToB;
        }
    }

    // 是否可以添加一个单节点虚拟机到指定节点
    bool canAddSingleVM(const VMType &vmType, Node node) const {
        // node 指定要添加到的节点，如果是Both是检测满足任意一个节点
        if (vmType.isDual() || isFull()) {
            return false;
        } else {
            bool hasFreeCpuA = freeCpu(Node::A) >= vmType.getCpu();
            bool hasFreeCpuB = freeCpu(Node::B) >= vmType.getCpu();
            bool hasFreeRamA = freeRam(Node::A) >= vmType.getRam();
            bool hasFreeRamB = freeRam(Node::B) >= vmType.getRam();
            bool canAddToA = hasFreeCpuA && hasFreeRamA;
            bool canAddToB = hasFreeCpuB && hasFreeRamB;
            bool canAdd = (node == Node::Both && canAddToA && canAddToB) ||
                          (node == Node::A && canAddToA) ||
                          (node == Node::B && canAddToB);
            return canAdd;
        }
    }

    // 是否可以添加一个虚拟机
    bool canAddVm(const VMType &vmType) const {
        return (canAddSingleVM(vmType) || canAddDualVM(vmType));
    }

    // 添加一个双节点部署的虚拟机
    void addDualVM(int id, const VMType &vmType) {
        if (canAddDualVM(vmType)) {
            loadedVMs.insert(pair<int, LoadedVM>(id, {Node::Both, vmType}));
            usedCpuA += vmType.getCpu() / 2;
            usedCpuB += vmType.getCpu() / 2;
            usedRamA += vmType.getRam() / 2;
            usedRamB += vmType.getRam() / 2;
        } else
            throw invalid_argument{"Server::addDualVM(int, VMType&): Can not add this Dual VM."};
    }

    // 添加单节点虚拟机到指定节点
    void addSingleVM(int id, const VMType &vmType, Node node) {
        if (node == Node::A && canAddSingleVM(vmType, Node::A)) {
            loadedVMs.insert(pair<int, LoadedVM>(id, {Node::A, vmType}));
            usedCpuA += vmType.getCpu();
            usedRamA += vmType.getRam();
        } else if (node == Node::B && canAddSingleVM(vmType, Node::B)) {
            loadedVMs.insert(pair<int, LoadedVM>(id, {Node::B, vmType}));
            usedCpuB += vmType.getCpu();
            usedRamB += vmType.getRam();
        }
    }

    // 添加一个单节点部署的虚拟机
    // 返回添加到节点的枚举
    Node addSingleVM(int id, const VMType &vmType) {
        if (canAddSingleVM(vmType, Node::Both)) {
            // 如果A节点明显比B节点空闲
            if (freeCpu(Node::A) >= freeCpu(Node::B) && freeRam(Node::A) >= freeRam(Node::B)) {
                addSingleVM(id, vmType, Node::A);
                return Node::A;
            } else {
                addSingleVM(id, vmType, Node::B);
                return Node::B;
            }
        } else if (canAddSingleVM(vmType, Node::A)) {
            addSingleVM(id, vmType, Node::A);
            return Node::A;
        } else if (canAddSingleVM(vmType, Node::B)) {
            addSingleVM(id, vmType, Node::B);
            return Node::B;
        } else
            throw invalid_argument{"Server::addSingleVM(int, VMType&): Can not add this Single VM."};
    }

    // 添加一个虚拟机(综合判断)
    // 返回添加到节点的枚举
    // 推荐使用此方法
    Node addVM(int id, const VMType &vmType) {
        if (canAddSingleVM(vmType)) {
            return addSingleVM(id, vmType);
        } else if (canAddDualVM(vmType)) {
            addDualVM(id, vmType);
            return Node::Both;
        } else {
            throw invalid_argument{"Server::addVM(int, VMType&): Can not add this VM."};
        }
    }

    // 是否在当前服务器中存在ID号的虚拟机
    bool hasVM(int id) {
        for (const auto &item:loadedVMs)
            if (id == item.first)
                return true;
        return false;
    }

    // 在当前服务器中删除某ID号虚拟机
    void delVM(int id) {
        for (const auto &item:loadedVMs)
            if (id == item.first) {
                switch (item.second.getLoadedNode()) {
                    case Node::A:
                        usedCpuA -= item.second.getCpu();
                        usedRamA -= item.second.getRam();
                        break;
                    case Node::B:
                        usedCpuB -= item.second.getCpu();
                        usedRamB -= item.second.getRam();
                        break;
                    case Node::Both:
                        usedCpuA -= item.second.getCpu() / 2;
                        usedRamA -= item.second.getRam() / 2;
                        usedCpuB -= item.second.getCpu() / 2;
                        usedRamB -= item.second.getRam() / 2;
                        break;
                }
                loadedVMs.erase(id);
                break;
            }
    }
};

// 类: 服务器
class Server final : public ServerType {
// 属性
private:
    int id{0}; // 编号

// 构造函数
public:
    // 默认构造函数
    Server() = default;

    // 有参构造函数
    Server(int ID, const string &Model, int CPU, int RAM, int HardwareCost, int DailyCost)
            : ServerType(Model, CPU, RAM, HardwareCost, DailyCost) {
        setId(ID);
    }

    // 有参构造函数
    Server(int ID, const string &Model, const BasicServer &BasicServer)
            : ServerType(Model, BasicServer) {
        setId(ID);
    }

    // 有参构造函数
    Server(int ID, const ServerInfo &serverInfo)
            : ServerType(serverInfo) {
        setId(ID);
    }

    // 有参构造函数
    Server(int id, const ServerType &serverType)
            : ServerType(serverType) {
        setId(id);
    }

// 访问器与设置器
public:
    int getId() const {
        return id;
    }

    void setId(int ID) {
        this->id = ID;
    }
};

// 重载小于运算符，按闲置率排序
bool operator<(const Server& lhs, const Server& rhs) {
    return lhs.freeRate() < rhs.freeRate();
}

#endif // SERVER_H
