#include "Config.h"         // 公共配置头文件
#include "Request.h"        // 请求类
#include "Server.h"         // 服务器类
#include "VirtualMachine.h" // 虚拟机类

#ifdef TEST
// 训练数据文件
const char *TestFile = "../training-data/training-1.txt";
// 输出文件
const char *OutFile = "../output.txt";
#endif

// 对添加请求的回应
struct AddResponse {
    int vmID;                // 虚拟机ID
    int serverID;            // 部署到服务器的ID
    Node serverNode;         // 部署到服务器的节点
    bool isToPurchaseServer; // 是否为将要购买的服务器
};

// 可供采购的服务器信息列表
vector<ServerType> serverList;
// 可供采购的虚拟机信息列表
vector<VMType> vmList;
// 用户每天请求列表
vector<vector<Request>> requestList;
// 已经购买的服务器列表
vector<Server> serverResource;

// 要购买的服务器集合，以model(型号)关键字进行类别区分
map<string, vector<Server>> ToBuy;
// 对添加请求的回应列表
vector<AddResponse> addResponseList;

int typesOfServer{0};   // 可供采购的服务器种类
int typesOfVM{0};       // 可供采购的虚拟机种类
int numOfServer{0};     // 购买的服务器数量
int numOfReqDays{0};    // 用户请求天数
long long costOfServer; // 服务器成本
long long costOfEnergy; // 耗能成本

// 输入向量
template<typename T>
void input(istream &is, vector<T> &obj, int &num);

// 输入请求
void inputRequest(istream &is, int &days);

// 虚拟机迁移策略
void migrate() {
    cout << "(migration, 0)" << endl;
}

// 在可供采购的虚拟机种类中查找对应的虚拟机类型
VMType &findVM(const string &model);

// 输出购买列表
void outputPurchase();

// 输出创建请求的回应
void outputAddResponseList();

// 处理某一天的请求
void process(int day);

// 添加虚拟机操作
void addVM(VictualMachine &vm);

// 在某个虚拟机列表中尝试添加虚拟机
// 如果添加失败，将返回false
bool addVM(VictualMachine &vm, vector<Server> &serverV);

// 删除虚拟机
void delVM(int id);

int main() {

#ifdef TEST
    cout << "Input from "
         << "\"" << TestFile << "\"" << endl;
    cout << "Writing to "
         << "\"" << OutFile << "\" ..." << endl;
    cout << "Auto running, don't exit, please wait ..." << endl;
    ifstream fin{TestFile};
    ofstream fout{OutFile};
    assert(fin.is_open());
    assert(fout.is_open());
    // 重定向输入，并保存原输入流缓冲区指针
    streambuf *oldCin = cin.rdbuf(fin.rdbuf());
    // 重定向输出，并保存原输出流缓冲区指针
    streambuf *oldCout = cout.rdbuf(fout.rdbuf());
#endif

    // 输入可供采购的服务器列表
    input(cin, serverList, typesOfServer);
    // 输入可供采购的虚拟机列表
    input(cin, vmList, typesOfVM);
    // 输入所有请求
    inputRequest(cin, numOfReqDays);

    // 对所有服务器类型进行重排序
    sort(serverList.begin(), serverList.end());
    // 对所有虚拟机类型进行重排序
    sort(vmList.begin(), vmList.end());

    // 处理每一天的请求
    for (int i = 0; i < numOfReqDays; i++)
        process(i);

#ifdef TEST
    cin.rdbuf(oldCin);   // 恢复标准输入
    cout.rdbuf(oldCout); // 恢复标准输出
    fin.close();         // 关闭输入文件
    fout.close();        // 关闭输出文件
    // 输出程序时间
    cout << "Program Run Time: " << (double) clock() / CLOCKS_PER_SEC << "s" << endl;
#endif
    return 0;
}

// 输入向量
template<typename T>
void input(istream &is, vector<T> &obj, int &num) {
    T temp;
    is >> num;
    is.get();
    for (int i = 0; i < num; i++) {
        is >> temp;
        obj.push_back(temp);
    }
}

// 输入用户请求
void inputRequest(istream &is, int &days) {
    Request temp;
    is >> days; // 输入请求天数
    int R{0};
    for (int i = 0; i < days; i++) {
        requestList.emplace_back(); // 添加新向量
        is >> R;                    // 输入该天的请求次数
        is.get();                   // 将换行符从缓冲区中删去
        for (int j = 0; j < R; j++) {
            is >> temp;
            requestList[i].push_back(temp);
        }
    }
}

// 在可供采购的虚拟机种类中查找对应的虚拟机类型
VMType &findVM(const string &model) {
    auto it = lower_bound(vmList.begin(), vmList.end(), VMType(model));
    return *it;
}

// 输出购买列表
void outputPurchase() {
    cout << "(purchase, " << ToBuy.size() << ")" << endl;
    for (auto &item : ToBuy) {
        cout << "(" << item.first << ", " << item.second.size() << ")" << endl;
    }
}

// 输出创建请求的回应
void outputAddResponseList() {
    for (auto &item : addResponseList) {
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
}

// 处理某一天的请求
void process(int day) {
    // 先清空要购买的服务器集合
    ToBuy.clear();
    // 先清空添加请求回应列表
    addResponseList.clear();

    // 处理该天的每一个请求
    for (auto &requestI : requestList[day]) {
        // 获取该请求的请求标识
        RequestFlag flag = requestI.select;
        if (flag == RequestFlag::add) {
            // 获取该请求的虚拟机类型
            VMType &vmType = findVM(requestI.addModel);
            // 为该请求实例化一个虚拟机
            VictualMachine vm{requestI.vmID, vmType};
            // 执行添加虚拟机操作
            addVM(vm);
        } else {
            // todo:执行删除虚拟机操作
            delVM(requestI.vmID);
        }
    }

    // 处理要购买的服务器
    for (auto &item : ToBuy) {
        for (auto &serverI : item.second) {
            // 为购买服务器重编号
            serverI.ID = numOfServer++;
            // 将购买的服务器添加到现有服务器列表
            serverResource.push_back(serverI);
        }
    }

    // 处理该天的添加请求回应
    for (auto &responseI : addResponseList) {
        // 该请求是购买一个新的服务器
        // 则需要将请求中ServerID更改为vmID实际所在的新ServerID
        if (responseI.isToPurchaseServer) {
            for (auto &item : ToBuy) {
                bool find{false};
                for (auto &serverI : item.second)
                    if (serverI.hasVM(responseI.vmID)) {
                        responseI.serverID = serverI.ID;
                        find = true;
                        break;
                    }
                if (find)
                    break;
            }
        }
    }

    // todo:计算开销


    // 输出购买服务器
    outputPurchase();

    // todo:迁移
    migrate();

    // 输出创建虚拟机请求的返回
    outputAddResponseList();
}

// 添加虚拟机操作
void addVM(VictualMachine &vm) {
    // 是否成功
    bool isSuccess;
    // 尝试在已购买的服务器中添加此虚拟机
    isSuccess = addVM(vm, serverResource);
    // 尝试在将要购买的服务器中添加此虚拟机
    if (!isSuccess)
        for (auto &item : ToBuy) {
            isSuccess = addVM(vm, item.second);
            if (isSuccess)
                break;
        }

    // 没有在已有、或将购买的服务器中添加成功，则购买一个新的服务器
    if (!isSuccess) {
        // 在可购买的服务器列表中选择合适的服务器类型
        for (auto &type : serverList) {
            // 实例化一个虚拟机，可能要购买
            // ID将在之后重赋值
            Server server{-1, type};
            if (server.canAddVm(vm)) {
                // 如果可以添加此虚拟机
                Node toAddNode = server.addVM(vm);
                // 成功添加，将此虚拟机添加到ToBuy列表中
                ToBuy[type.model].push_back(server);
                // 记录到添加请求回应列表
                addResponseList.push_back({vm.ID, server.ID, toAddNode, true});
                // 记录硬件成本
                costOfServer += type.hardwareCost;
                // 成功购买，直接退出
                break;
            }
        }
    }
}

// 在某个虚拟机列表中尝试添加虚拟机
// 如果添加失败，将返回false
bool addVM(VictualMachine &vm, vector<Server> &serverV) {
    // 是否添加成功
    bool isSuccess{false};
    // 在该列表中逐个搜索
    for (auto &serverI : serverV) {
        if (serverI.canAddVm(vm)) {
            // 如果可以添加此虚拟机
            Node toAddNode = serverI.addVM(vm);
            // 添加成功
            isSuccess = true;
            // 检测是否为将要购买的虚拟机
            bool isToPurchaseServer = (serverI.ID == -1);
            // 记录到添加请求回应列表
            addResponseList.push_back({vm.ID, serverI.ID, toAddNode, isToPurchaseServer});
            // 添加成功，直接退出
            break;
        }
    }
    return isSuccess;
}

// 删除虚拟机
void delVM(int id) {
    // 在已购买的服务器列表中查找
    for (auto &serverI:serverResource) {
        if (serverI.hasVM(id)) {
            serverI.eraseVM(id);
            return;
        }
    }
    // 在将购买的服务器列表中查找
    for (auto &item:ToBuy) {
        for (auto &serverI:item.second) {
            if (serverI.hasVM(id)) {
                serverI.eraseVM(id);
                return;
            }
        }
    }
}