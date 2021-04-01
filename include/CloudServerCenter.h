/**
 * 此头文件包含: 云服务器中心类
 */

#ifndef CLOUDSERVERCENTER_H
#define CLOUDSERVERCENTER_H

#include "Config.h"
#include "VM.h"
#include "Server.h"
#include "Request.h"

// 结构体: 对添加请求的回应
struct AddResponse {
    int vmID;                // 虚拟机ID
    int serverID;            // 部署到服务器的ID
    Node serverNode;         // 部署到服务器的节点
    bool isToPurchaseServer; // 是否为将要购买的服务器
};

// 类: 服务器列表
// 该列表具有自动按闲置率降序插入的功能
//class ServerList {
//// 字段
//private:
//    vector<Server> list{};
//
//// 构造函数
//public:
//    // 默认构造函数
//    ServerList() = default;
//
//// 迭代器类型
//private:
//    // 随机访问迭代器
//    using iterator = vector<Server>::iterator;
//    // 常随机访问迭代器
//    using const_iterator = vector<Server>::const_iterator;
//
//public:
//    struct AddResult {
//        bool success{false};
//        int serverID{};
//        Node addNode{};
//
//        AddResult() = default;
//
//        AddResult(bool success, int serverId, Node addNode) {
//            this->success = success;
//            this->serverID = serverId;
//            this->addNode = addNode;
//        }
//    };
//
//// 迭代器
//public:
//    // 起始常迭代器
//    const_iterator begin() { return list.begin(); };
//
//    // 终止常迭代器(为末尾位置的后一个单元)
//    const_iterator end() { return list.end(); };
//
//// 访问器与修改器
//public:
//    // 返回指定 ID 的服务器
//    // 如果不存在该 ID 号的服务器, 将抛出异常
//    Server at(int ID) {
//        for (auto &server:list)
//            if (server.getId() == ID)
//                return server;
//        throw range_error("ServerList::at(int): Can not find this Server.");
//    }
//
//    // 插入某个 Server
//    // 自动按闲置率升序进行合适的插入位置选择
//    void insert(Server &server) {
//        if (list.empty())
//            list.push_back(server);
//        else {
//            bool hasInserted{false};
//            auto it = list.begin();
//            for (; it != list.end(); it++) {
//                if (server.freeRate() < it->freeRate()) {
//                    list.insert(it, server);
//                    hasInserted = true;
//                    break;
//                }
//            }
//            if (!hasInserted) list.push_back(server);
//        }
//    }
//
//    // 插入某个 Server (右值引用)
//    void insert(Server &&server) {
//        insert(server);
//    }
//
//    // 添加虚拟机
//    AddResult addVM(int id, const VMType &vmType) {
//        auto it = list.begin();
//        Server temp;
//        Node addNode;
//        bool success{false};
//        for (; it != list.end(); it++) {
//            if (it->canAddVm(vmType)) {
//                addNode = it->addVM(id, vmType);
//                temp = *it;
//                list.erase(it);
//                success = true;
//                break;
//            }
//        }
//        if (success) {
//            insert(temp);
//#ifdef TEST
//            cerr << "VM: " << id << endl
//                 << "Model: " << vmType.getModel()
//                 << ", CPU: " << vmType.getCpu()
//                 << ", RAM: " << vmType.getRam()
//                 << ", Dual: " << vmType.isDual() << endl;
//            cerr << "Server: " << temp.getId() << endl
//                 << "Model: " << temp.getModel()
//                 << ", CPU: " << temp.getCpu()
//                 << ", RAM: " << temp.getRam()
//                 << endl
//                 << "usedCpu(A): " << temp.usedCpu(Node::A)
//                 << ", usedCpu(B): " << temp.usedCpu(Node::B)
//                 << ", usedRam(A): " << temp.usedRam(Node::A)
//                 << ", usedRam(B): " << temp.usedRam(Node::B)
//                 << endl
//                 << "freeCpu(A): " << temp.freeCpu(Node::A)
//                 << ", freeCpu(B): " << temp.freeCpu(Node::B)
//                 << ", freeRam(A): " << temp.freeRam(Node::A)
//                 << ", freeRam(B): " << temp.freeRam(Node::B) << endl;
//            cerr << endl;
//#endif
//            return AddResult{true, temp.getId(), addNode};
//        } else {
//            return AddResult{false, -1, {}};
//        }
//    }
//
//// 删除虚拟机
//    void delVM(int id) {
//        auto it = list.begin();
//        Server temp;
//        bool find{false};
//        for (; it != list.end(); it++) {
//            if (it->hasVM(id)) {
//                it->delVM(id);
//                temp = *it;
//                list.erase(it);
//                find = true;
//                break;
//            }
//        }
//        if (find)
//            insert(temp);
//    }
//
//};

/**
 * 类: 云服务器调度中心, 综合响应用户请求
 */
class CloudServerCenter {
// 字段
private:
    // 可供采购的服务器信息集合
    // 不需要id和装载的虚拟机, 选用 ServerInfo
    // 插入时自动实现按价格升序功能, 选用 multiset
    multiset<ServerInfo> serverInfoSet;

    // 可供采购的虚拟机信息列表
    // 不需要id, model, 选用 BasicVM
    // 通过 model 快速查找到虚拟机信息, 选用 unordered_map 哈希表
    unordered_map<string, BasicVM> vmMap;

    // 用户每日添加请求列表(二维数组)
    vector<vector<AddRequest>> addRequestList;

    // 对添加请求的回应列表
    vector<AddResponse> addResponseList;

    // 用户每日删除请求列表(二维数组)
    vector<vector<DelRequest>> delRequestList;

    // 已经购买的服务器列表
    vector<Server> serverResource;

    // 已经购买的服务器列表
    // 通过 ID 快速查找到服务器类型, 选用 map 哈希表
//    map<int, ServerType> servers;

    // 已购买的服务器集合
    // 插入时自动实现按闲置率降序功能, 选用 multiset
//    multiset<Server> serverSet;

    // 已购买的服务器列表
    // 插入时自动实现按闲置率降序功能
//    ServerList serverList;

    // 临时存放要购买的各种服务器集合
    // 以model(型号)关键字进行类别区分
    // 每个型号映射了该型号的多个服务器实体
    unordered_map<string, vector<Server>> ToBuyMap;

    // 购买的服务器数量
    int _numOfServer{0};
    // 虚拟机总数
    int _numOfVM{0};
    // 请求天数
    int _numOfReqDays{0};

// 构造函数
public:
    // 构造云服务器调度中心
    CloudServerCenter() {
        // 从输入流读取可供采购的服务器列集合
        inputServerInfoSet();
        // 从输入流读取可供采供的虚拟机列表
        inputVMMap();
        // 从输入读取每一天的请求列表
        inputRequest();
    }

// 开放接口
public:
    // 自动生成结果
    void autoGenerate() {
        for (int day = 0; day < numOfReqDays(); day++) {
            process(day);
        }
#ifdef TEST
        ofstream os;
        os.open("../servers.txt");
        int cnt{0};
        for (auto &server: serverResource) {
            if (server.freeRate() >= 0.5 && server.freeRate() < 1) {
                cnt++;
            }
            os << server.getId() << " "
               << server.getModel() << " "
               << server.usedCpu() << " "
               << server.getCpu() << " "
               << server.usedRam() << " "
               << server.getRam() << " "
               << server.freeRate() * 100 << "%" << " "
               << endl;
        }
        os << ">= 50%: " << cnt;
#endif
    }

// 获取常量值
public:
    // 可供采购的服务器种类
    inline constexpr int typesOfServer() const {
        return serverInfoSet.size();
    }

    // 可供采购的虚拟机种类
    inline constexpr int typesOfVM() const {
        return vmMap.size();
    }

    // 购买的服务器数量
    inline constexpr int numOfServer() const {
        return _numOfServer;
    }

    // 用户请求天数
    inline constexpr int numOfReqDays() const {
        return _numOfReqDays;
    }

    // 虚拟机总数
    inline constexpr int numOfVM() const {
        return _numOfVM;
    }

// 内部函数
private:
    // 输入标识符, 如将"(NV603,"解析为"NV603"
    string inputFlag(string &flag) {
        cin >> flag;
        return flag = flag.substr(1, flag.size() - 2);
    }

    // 输入serverInfoSet
    void inputServerInfoSet() {
        int size{0}; // serverInfoSet的大小
        cin >> size;

        // 临时ServerInfo
        ServerInfo serverInfo;

        for (int i = 0; i < size; i++) {
            cin >> serverInfo;
            serverInfoSet.insert(serverInfo);
        }
    }

    // 输入unordered_map
    template<class T>
    void inputUnorderedMap(unordered_map<string, T> &unorderedMap) {
        int size{0}; // 此unordered_map的大小
        cin >> size;
        string model; // 服务器或虚拟机的型号
        T obj;        // 一个服务器或虚拟机
        for (int i = 0; i < size; i++) {
            inputFlag(model);
            cin >> obj;
            unorderedMap.insert(pair<string, T>(model, obj));
        }
    }

    // 输入vmList
    void inputVMMap() {
        int size{0}; // vmList的大小
        cin >> size;

        string model;       // 虚拟机的型号
        BasicVM basicVM;    // 临时 BasicVM

        for (int i = 0; i < size; i++) {
            inputFlag(model);
            cin >> basicVM;
            vmMap.insert(pair<string, BasicVM>(model, basicVM));
        }
    }

    // 输入向量
    template<typename T>
    void inputVector(vector<T> &obj) {
        T temp;
        int size{0}; // 此 Vector 的大小
        cin >> size;
        for (int i = 0; i < size; i++) {
            cin >> temp;
            obj.push_back(temp);
        }
    }

    // 输入请求列表
    void inputRequest() {
        // 临时AddRequest
        AddRequest addRequestTemp;

        // 临时DelRequest
        DelRequest delRequestTemp;

        // 输入请求天数
        cin >> _numOfReqDays;

        // 标识字符串, 为 "add" 或 "del"
        string flag;

        // 请求次数
        int times{0};

        for (int i = 0; i < _numOfReqDays; i++) {
            // 准备这一天的添加请求列表
            addRequestList.emplace_back();
            // 准备这一天的删除请求列表
            delRequestList.emplace_back();

            // 输入请求次数
            cin >> times;

            // 遍历这一天的请求
            for (int j = 0; j < times; j++) {
                if (inputFlag(flag) == "add") {
                    cin >> addRequestTemp;
                    addRequestList[i].push_back(addRequestTemp);
                } else {
                    cin >> delRequestTemp;
                    delRequestList[i].push_back(delRequestTemp);
                }
            }
        }
    }

    // 处理某一天的请求
    void process(int day) {
        // 先清空要购买的服务器集合
        ToBuyMap.clear();
        // 先清空添加请求回应列表
        addResponseList.clear();

        // 处理所有的添加请求
        for (auto &requestI:addRequestList[day]) {
            // 获取请求添加的虚拟机型号
            const string &model = requestI.getAddModel();

            // 实例化一个虚拟机类型
            VMType vmType{model, vmMap[model]};

            // 添加此虚拟机
            addVM(requestI.getVMId(), vmType);

            // 更新虚拟机数量
            _numOfVM++;
        }

        // 处理所有的删除请求
        for (auto &requestI:delRequestList[day]) {
            // 删除请求对应ID的虚拟机
            delVM(requestI.getVMId());

            // 更新虚拟机数量
            _numOfVM--;
        }

        // 对要购买的服务器进行处理
        for (auto &pair: ToBuyMap) {
            for (auto &server : pair.second) {
                // 按顺序编号
                server.setId(_numOfServer++);
                // 添加到已购买的服务器列表中
                serverResource.push_back(server);
//                servers[server.getId()] = ServerType(server);
//                serverSet.insert(server);
//                serverList.insert(server);
            }
        }

        // 处理该天的添加请求回应
        for (auto &responseI: addResponseList) {
            // 该请求是购买一个新的服务器
            // 则需要将请求中ServerID更改为vmID实际所在的新ServerID
            if (responseI.isToPurchaseServer) {
                for (auto &pair:ToBuyMap) {
                    bool find{false};
                    for (auto &server: pair.second) {
                        if (server.hasVM(responseI.vmID)) {
                            responseI.serverID = server.getId();
                            find = true;
                            break;
                        }
                    }
                    if (find) break;
                }
            }
        }

        // 输出购买服务器
        outputToBuy();

        // 输出上一天迁移结果
        outputMigration(day);

        // 输出创建虚拟机请求的返回
        for (auto &item:addResponseList) {
            switch (item.serverNode) {
                case Node::A:
                    cout << "(" << item.serverID << ", A)" << endl;
                    break;
                case Node::B:
                    cout << "(" << item.serverID << ", B)" << endl;
                    break;
                case Node::Both:
                    cout << "(" << item.serverID << ")" << endl;
                    break;
            }
        }

        sort(serverResource.begin(), serverResource.end());

        // 迁移
        migrate();
    }

    // 添加虚拟机操作
    void addVM(int vmID, const VMType &vmType) {
        // 是否添加成功
        bool isSuccess{false};

        // 尝试在已购买的服务器中添加此虚拟机
        isSuccess = addVMInServerResource(vmID, vmType);
//        isSuccess = addVMInServers(vmID, vmType);
//        isSuccess = addVMInServerSet(vmID, vmType);
//        isSuccess = addVMInServerList(vmID, vmType);

        // 尝试在将要购买的服务器中添加此虚拟机
        if (!isSuccess)
            isSuccess = addVMInToBuyMap(vmID, vmType);

        // 没有在已有、或将购买的服务器中添加成功，则购买一个新的服务器
        // 并添加到临时列表ToBuyMap
        if (!isSuccess) {
            // 在可购买的服务器信息集合中选择合适的服务器信息
            for (auto &serverInfo : serverInfoSet) {
                // 实例化一个服务器，可能要添加到ToBuyMap
                // ID将在执行完这一天的请求后重赋值
                Server newServer{-1, serverInfo};

                if (newServer.canAddVm(vmType)) {
                    // 如果可以添加此虚拟机
                    Node toAddNode = newServer.addVM(vmID, vmType);

                    // 成功添加，将此服务器添加到ToBuy列表中
                    ToBuyMap[serverInfo.getModel()].push_back(newServer);

                    // 记录到添加请求回应列表
                    addResponseList.push_back({vmID, -1, toAddNode, true});
#ifdef TEST
                    cerr << "VM: " << vmID << endl
                         << "Model: " << vmType.getModel()
                         << ", CPU: " << vmType.getCpu()
                         << ", RAM: " << vmType.getRam()
                         << ", Dual: " << vmType.isDual() << endl;
                    cerr << "Server: " << -1 << endl
                         << "Model: " << newServer.getModel()
                         << ", CPU: " << newServer.getCpu()
                         << ", RAM: " << newServer.getRam()
                         << endl
                         << "usedCpu(A): " << newServer.usedCpu(Node::A)
                         << ", usedCpu(B): " << newServer.usedCpu(Node::B)
                         << ", usedRam(A): " << newServer.usedRam(Node::A)
                         << ", usedRam(B): " << newServer.usedRam(Node::B)
                         << endl
                         << "freeCpu(A): " << newServer.freeCpu(Node::A)
                         << ", freeCpu(B): " << newServer.freeCpu(Node::B)
                         << ", freeRam(A): " << newServer.freeRam(Node::A)
                         << ", freeRam(B): " << newServer.freeRam(Node::B) << endl;
                    cerr << endl;
#endif
                    // 成功购买，直接退出
                    break;
                }
            }
        }
    }

    // 在已购买的服务器中尝试添加虚拟机
    // 尝试失败，返回false
    bool addVMInServerResource(int vmID, const VMType &vmType) {
        // 是否添加成功
        bool isSuccess{false};

        // 在 serverSet 中逐个搜索
        for (auto &server: serverResource) {
            // 如果当前服务器可以添加
            if (server.canAddVm(vmType)) {
                // 获取添加到的节点
                Node toAddNode = server.addVM(vmID, vmType);

                // 添加成功
                isSuccess = true;

                // 记录到添加请求回应列表
                addResponseList.push_back({vmID, server.getId(), toAddNode, false});

#ifdef TEST
                cerr << "VM: " << vmID << endl
                     << "Model: " << vmType.getModel()
                     << ", CPU: " << vmType.getCpu()
                     << ", RAM: " << vmType.getRam()
                     << ", Dual: " << vmType.isDual() << endl;
                cerr << "Server: " << server.getId() << endl
                     << "Model: " << server.getModel()
                     << ", CPU: " << server.getCpu()
                     << ", RAM: " << server.getRam()
                     << endl
                     << "usedCpu(A): " << server.usedCpu(Node::A)
                     << ", usedCpu(B): " << server.usedCpu(Node::B)
                     << ", usedRam(A): " << server.usedRam(Node::A)
                     << ", usedRam(B): " << server.usedRam(Node::B)
                     << endl
                     << "freeCpu(A): " << server.freeCpu(Node::A)
                     << ", freeCpu(B): " << server.freeCpu(Node::B)
                     << ", freeRam(A): " << server.freeRam(Node::A)
                     << ", freeRam(B): " << server.freeRam(Node::B) << endl;
                cerr << endl;
#endif
                // 添加成功，直接退出
                break;
            }
        }

        return isSuccess;
    }

    // 在已购买的服务器中尝试添加虚拟机
    // 尝试失败，返回false
//    bool addVMInServers(int vmID, const VMType &vmType) {
//        // 是否添加成功
//        bool isSuccess{false};
//
//        // 在 servers 中逐个搜索
//        for (auto &pair: servers) {
//            // 获取当前服务器ID
//            int currentID = pair.first;
//
//            // 获取当前服务器
//            ServerType &current = pair.second;
//
//            // 如果当前服务器可以添加
//            if (current.canAddVm(vmType)) {
//                // 获取添加到的节点
//                Node toAddNode = pair.second.addVM(vmID, vmType);
//
//                // 添加成功
//                isSuccess = true;
//
//                // 记录到添加请求回应列表
//                addResponseList.push_back({vmID, currentID, toAddNode, false});
//#ifdef TEST
//                cerr << "VM: " << vmID << endl
//                     << "Model: " << vmType.getModel()
//                     << ", CPU: " << vmType.getCpu()
//                     << ", RAM: " << vmType.getRam()
//                     << ", Dual: " << vmType.isDual() << endl;
//                cerr << "Server: " << currentID << endl
//                     << "Model: " << current.getModel()
//                     << ", CPU: " << current.getCpu()
//                     << ", RAM: " << current.getRam()
//                     << endl
//                     << "usedCpu(A): " << current.usedCpu(Node::A)
//                     << ", usedCpu(B): " << current.usedCpu(Node::B)
//                     << ", usedRam(A): " << current.usedRam(Node::A)
//                     << ", usedRam(B): " << current.usedRam(Node::B)
//                     << endl
//                     << "freeCpu(A): " << current.freeCpu(Node::A)
//                     << ", freeCpu(B): " << current.freeCpu(Node::B)
//                     << ", freeRam(A): " << current.freeRam(Node::A)
//                     << ", freeRam(B): " << current.freeRam(Node::B) << endl;
//                cerr << endl;
//#endif
//                // 添加成功，直接退出
//                break;
//            }
//        }
//        return isSuccess;
//    }

    // 在已购买的服务器中尝试添加虚拟机
    // 尝试失败，返回false
//    bool addVMInServerSet(int vmID, const VMType &vmType) {
//        // 是否添加成功
//        bool isSuccess{false};
//
//        // 在 serverSet 中逐个搜索
//        for (auto &server: serverSet) {
//            // 解除const
//            auto ii = const_cast<Server &>(server);
//            // 如果当前服务器可以添加
//            if (server.canAddVm(vmType)) {
//                // 获取添加到的节点
//                Node toAddNode = ii.addVM(vmID, vmType);
//
//                // 添加成功
//                isSuccess = true;
//
//                // 记录到添加请求回应列表
//                addResponseList.push_back({vmID, server.getId(), toAddNode, false});
//
//#ifdef TEST
//                cerr << "VM: " << vmID << endl
//                     << "Model: " << vmType.getModel()
//                     << ", CPU: " << vmType.getCpu()
//                     << ", RAM: " << vmType.getRam()
//                     << ", Dual: " << vmType.isDual() << endl;
//                cerr << "Server: " << server.getId() << endl
//                     << "Model: " << server.getModel()
//                     << ", CPU: " << server.getCpu()
//                     << ", RAM: " << server.getRam()
//                     << endl
//                     << "usedCpu(A): " << server.usedCpu(Node::A)
//                     << ", usedCpu(B): " << server.usedCpu(Node::B)
//                     << ", usedRam(A): " << server.usedRam(Node::A)
//                     << ", usedRam(B): " << server.usedRam(Node::B)
//                     << endl
//                     << "freeCpu(A): " << server.freeCpu(Node::A)
//                     << ", freeCpu(B): " << server.freeCpu(Node::B)
//                     << ", freeRam(A): " << server.freeRam(Node::A)
//                     << ", freeRam(B): " << server.freeRam(Node::B) << endl;
//                cerr << endl;
//#endif
//                // 添加成功，直接退出
//                break;
//            }
//        }
//        return isSuccess;
//    }

    // 在已购买的服务器中尝试添加虚拟机
    // 尝试失败，返回false
//    bool addVMInServerList(int vmID, const VMType &vmType) {
//        // 是否添加成功
//        bool isSuccess{false};
//
//        ServerList::AddResult addResult = serverList.addVM(vmID, vmType);
//
//        isSuccess = addResult.success;
//
//        if (isSuccess) {
//            addResponseList.push_back({vmID, addResult.serverID, addResult.addNode, false});
//        }
//
//        return isSuccess;
//    }

    // 在ToBuyMap表中尝试添加虚拟机
    // 尝试失败, 返回false
    bool addVMInToBuyMap(int vmID, const VMType &vmType) {
        // 是否添加成功
        bool isSuccess{false};

        // 遍历各种型号
        for (auto &pair:ToBuyMap) {
            // 遍历当前型号的所有服务器
            for (auto &server:pair.second) {
                // 如果当前服务器可以添加
                if (server.canAddVm(vmType)) {
                    // 获取添加到的节点
                    Node toAddNode = server.addVM(vmID, vmType);

                    // 添加成功
                    isSuccess = true;

                    // 记录到添加请求回应列表
                    addResponseList.push_back({vmID, server.getId(), toAddNode, true});
#ifdef TEST
                    cerr << "VM: " << vmID << endl
                         << "Model: " << vmType.getModel()
                         << ", CPU: " << vmType.getCpu()
                         << ", RAM: " << vmType.getRam()
                         << ", Dual: " << vmType.isDual() << endl;
                    cerr << "Server: " << server.getId() << endl
                         << "Model: " << server.getModel()
                         << ", CPU: " << server.getCpu()
                         << ", RAM: " << server.getRam()
                         << endl
                         << "usedCpu(A): " << server.usedCpu(Node::A)
                         << ", usedCpu(B): " << server.usedCpu(Node::B)
                         << ", usedRam(A): " << server.usedRam(Node::A)
                         << ", usedRam(B): " << server.usedRam(Node::B)
                         << endl
                         << "freeCpu(A): " << server.freeCpu(Node::A)
                         << ", freeCpu(B): " << server.freeCpu(Node::B)
                         << ", freeRam(A): " << server.freeRam(Node::A)
                         << ", freeRam(B): " << server.freeRam(Node::B) << endl;
                    cerr << endl;
#endif
                    // 添加成功，直接退出
                    break;
                }
            }
            if (isSuccess) break;
        }
        return isSuccess;
    }

    // 删除虚拟机
    void delVM(int id) {
        // 在已购买的服务器列表中查找
        for (auto &server:serverResource) {
            if (server.hasVM(id)) {
                server.delVM(id);
                break;
            }
        }
//        for (auto &pair:servers) {
//            if (pair.second.hasVM(id)) {
//                pair.second.delVM(id);
//                break;
//            }
//        }
//        serverList.delVM(id);

        // 在将购买的服务器列表中查找
        for (auto &pair:ToBuyMap) {
            bool find{false};
            for (auto &server:pair.second) {
                if (server.hasVM(id)) {
                    server.delVM(id);
                    find = true;
                    break;
                }
            }
            if (find) break;
        }
    }

    // 输出购买列表
    void outputToBuy() {
        cout << "(purchase, " << ToBuyMap.size() << ")" << endl;
        for (auto &item: ToBuyMap) {
            cout << "(" << item.first << ", " << item.second.size() << ")" << endl;
        }
    }

    // 输出上一天的迁移结果
    void outputMigration(int day) {
        if (day) {
            if (mTable.empty()) {
                cout << "(migration, " << 0 << ")" << endl;
            } else {
                cout << "(migration, " << mTable.size() << ")" << endl;
                for (auto &item:mTable) {
                    cout << "(" << item.fromVMID << ", " << item.toServerID;
                    switch (item.toNode) {
                        case Node::A:
                            cout << ", A)" << endl;
                            break;
                        case Node::B:
                            cout << ", B)" << endl;
                            break;
                        case Node::Both:
                            cout << ")" << endl;
                            break;
                    }
                }
            }
        } else {
            cout << "(migration, " << 0 << ")" << endl;
        }
    }

    struct Table {
        int fromServerID{};
        int fromVMID{};
        int toServerID{};
        Node toNode{};

        Table() = default;

        Table(int fromServerID, int fromVMID, int toServerID, Node toNode) {
            this->fromServerID = fromServerID;
            this->fromVMID = fromVMID;
            this->toServerID = toServerID;
            this->toNode = toNode;
        }
    };

    // 迁移记录表
    vector<Table> mTable{};

    // 在每一天的最后迁移
    void migrate() {
        mTable.clear();
        // 计算可迁移的虚拟机数量
        size_t canMigrate{static_cast<size_t>(numOfVM() / 460)};
        if (canMigrate > 0) {
            auto firstN100 = serverResource.end() - 1;
            for (; firstN100 != serverResource.begin(); firstN100--)
                if (firstN100->freeRate() < 1)
                    break; // 获取第一个非100%空闲的服务器迭代器
            auto firstN0 = serverResource.begin();
            for (; firstN0 != firstN100; firstN0++)
                if (firstN0->freeRate() > 0)
                    break; // 获取第一个非0%空闲的服务器迭代器
            // 按闲置率逆序遍历
            for (auto fromServer = firstN100; fromServer != firstN0 - 1; fromServer--) {
                if (fromServer->numOfVM() <= canMigrate) {
                    // 拷贝一份fromServer的虚拟机列表
                    unordered_map<int, LoadedVM> copy = fromServer->loadedVMs;
                    // 遍历fromServer的虚拟机列表
                    for (auto &fromPair: copy) {
                        int fromVMID = fromPair.first;
                        VMType fromVMType = VMType(fromPair.second);
                        // 从低闲置率中的服务器中遍历
                        for (auto toServer = firstN0; toServer != fromServer; toServer++) {
                            // toServer可以添加fromVMType
                            if (toServer->canAddVm(fromVMType)) {
                                // 删除fromServer中的fromVMType
                                fromServer->delVM(fromVMID);
                                // 将(fromVMID, fromVMType)添加到toServer中
                                Node toNode = toServer->addVM(fromVMID, fromVMType);
                                // 添加到mTable
                                mTable.emplace_back(fromServer->getId(), fromVMID, toServer->getId(), toNode);
                                // 减少可迁移数
                                canMigrate--;
                                // 该服务器被塞满
                                if (toServer->freeRate() == 1)
                                    firstN0 = toServer + 1;
                                // 找到目的服务器, 停止遍历
                                break;
                            }
                        }
                    }
                }
            }
        }

    }
};


#endif //CLOUDSERVERCENTER_H
