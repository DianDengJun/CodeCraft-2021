# 2021华为软件精英挑战赛

+ 队名：编码吧兄弟

## IDE 要求
可以使用 CLion 或 Visual Studio Code

### CLion
确保已安装 MinGW, 并成功设置环境变量

### VS Code
1. 确保已安装 MinGW, 并成功设置环境变量
2. 安装 CMake, 版本 2.8 以上
3. 安装 VSCode 插件: C/C++, CMake, CMake Tools

## 项目结构
+ [doc](./doc) 文档
+ [include](./include) 头文件包含目录
+ [source](./source) 源文件
+ [training-data](./training-data) 训练数据文件
+ [CMakeList.txt](./CMakeLists.txt) CMake项目配置文件

## 说明
1. 在 [Config.h](./include/Config.h) (公共配置头文件) 中添加项目所需的系统头文件、自定义宏等
2. 如需从训练文件输入, 在 [CodeCraft.cpp](./source/CodeCraft-2021.cpp#L8) 中更改相应内容
   ```c++
   // 训练数据文件
   const char *TestFile = "../training-data/training-1.txt";
   // 输出文件
   const char *OutFile = "../output.txt";
   ```
   此外, 从文件输入, 输出到文件, 仅在 Debug 模式下有效, 请在 IDE 编译时选择 Debug 模式。
3. 可执行文件将在 bin 目录输出

# 初赛
## 提交记录

+ 2021.3.17 13:24
  
  第一个版本
  
  基本框架，包含输入输出，添加请求
  
  成本 33 8359 5193 运行时间 17.326

+ 2021.3.17 18:40

  第二个版本
  
  解决了删除请求
  
  成本 11 9653 2270 运行时间 17.935

+ 2021.3.26

  最终版本

  所有功能全(包括部署与迁移)

  含[本地测试工具(python)](./util/2021HWAutoGrader)

  初赛提交最终成绩 成本 14 9426 5879 迁移次数 6 3301  运行时间 93.607

  为初赛43强