# 假设你的ThreadPool.h实际上需要编译的是模板类或仅包含模板函数
# 如果它包含了cpp文件，请像Logger一样添加

add_library(threadpool INTERFACE)

# 因为是接口库，你只需设置它的包含目录
target_include_directories(threadpool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})