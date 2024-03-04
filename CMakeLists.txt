cmake_minimum_required(VERSION 3.10)
project(MyProject) # 修改为你的项目名称

# 设置C++版本
find_package(Threads REQUIRED) # 查找线程库
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加Logger和threadpool子目录
add_subdirectory(Logger)
add_subdirectory(ThreadPool)

# 如果你有主函数的话，在这里添加你的可执行文件
add_executable(my_executable main.cpp)
target_link_libraries(my_executable PRIVATE Threads::Threads) # 链接到你的目标上
target_link_libraries(my_executable PRIVATE logger threadpool)
