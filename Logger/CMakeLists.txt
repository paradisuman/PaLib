# 把源文件定义为一个变量，以便它们可以被多处引用
set(LOGGER_SOURCES
    log.cpp
)

add_library(logger ${LOGGER_SOURCES})

# 指定库的公有头文件
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})