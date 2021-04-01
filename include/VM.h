/**
 * 此头文件包含了: 虚拟机类
 */
#ifndef VM_H
#define VM_H

#include "Config.h" // 包含公共配置头文件

// 类: 基本虚拟机(不含model, id)
class BasicVM {
// 属性
protected:
    int cpu{0};    // CPU核心数
    int ram{0};    // 内存大小
    bool dual{false};  // 是否双节点部署

// 构造函数
public:
    // 默认构造函数
    BasicVM() = default;

    // 有参构造函数
    BasicVM(int CPU, int RAM, bool isDouble) {
        setCpu(CPU);
        setRam(RAM);
        setDual(isDouble);
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

    inline bool isDual() const {
        return dual;
    }

    inline void setDual(bool isDouble) {
        this->dual = isDouble;
    }
};

// 从输入流中输入BasicVM
istream &operator>>(istream &is, BasicVM &basicVM) {
    // 临时字符串数组
    string temp[3];

    // 输入三个字符串
    for (auto &i : temp) {
        is >> i;
    }

    // 解析为对应信息
    basicVM.setCpu(atoi(temp[0].c_str()));
    basicVM.setRam(atoi(temp[1].c_str()));
    basicVM.setDual(atoi(temp[2].c_str()));

    // 返回输入流
    return is;
}

// 类: 虚拟机类型(不含id)
class VMType : public BasicVM {
// 属性
protected:
    string model; // 虚拟机型号

// 构造函数
public:
    // 默认构造函数
    VMType() = default;

    // 有参构造函数
    VMType(const string &Model, int CPU, int RAM, bool isDouble)
            : BasicVM(CPU, RAM, isDouble) {
        setModel(Model);
    }

    // 通过 model 和 BasicVM 构造
    VMType(const string &Model, const BasicVM &basicVM)
            : BasicVM(basicVM) {
        setModel(Model);
    }

// 访问器与设置器
public:
    inline const string &getModel() const {
        return model;
    }

    inline void setModel(const string &Model) {
        this->model = Model;
    }

    // 设置 BasicVM
    void setBasicVM(const BasicVM &basicVM) {
        setCpu(basicVM.getCpu());
        setRam(basicVM.getRam());
        setDual(basicVM.isDual());
    }
};

// 从输入流中输入VMType
istream &operator>>(istream &is, VMType &vmType) {
    // 临时字符串
    string temp;

    // 输入并解析虚拟机型号
    is >> temp;
    vmType.setModel(temp.substr(1, temp.size() - 2));

    // 临时BasicVM
    BasicVM basicVM;

    // 输入并解析BasicVM
    is >> basicVM;
    vmType.setBasicVM(basicVM);

    // 返回输入流
    return is;
}

// 类: 虚拟机实体
class VM final : public VMType {
// 属性
private:
    int id{0}; // 编号

// 构造函数
public:
    // 默认构造函数
    VM() = default;

    // 有参构造函数
    VM(int ID, const string &Model, int CPU, int RAM, bool isDouble)
            : VMType(Model, CPU, RAM, isDouble) {
        setId(ID);
    }

    // 通过 ID 和 VMType 构造
    VM(int ID, const VMType &vmType)
            : VMType(vmType) {
        setId(ID);
    }

    // 通过ID, model, BasicVM 构造
    VM(int ID, const string &Model, const BasicVM &basicVM)
            : VMType(Model, basicVM) {
        setId(ID);
    }

// 访问器与设置器
public:
    inline int getId() const {
        return id;
    }

    inline void setId(int ID) {
        this->id = ID;
    }

    // 设置 VMType
    void setVMType(const VMType& vmType) {
        setCpu(vmType.getCpu());
        setRam(vmType.getRam());
        setModel(vmType.getModel());
        setDual(vmType.isDual());
    }
};

#endif // VM_H
