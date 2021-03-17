# CMAKE 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目名
project("CodeCraft-2021")

# include 路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 生成 debug 版本
# set(CMAKE_BUILD_TYPE "Debug")
# 生成 release 版本
#set(CMAKE_BUILD_TYPE "Release")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DTEST)
    add_definitions(-DDEBUG)
endif ()

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# debug 模式下编译选项
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

# release 模式下编译选项
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(source DIR_SRCS)

# 指定生成目标
add_executable(CodeCraft-2021 ${DIR_SRCS})