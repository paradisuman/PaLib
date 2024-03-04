#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "log.h"

// TODO log拓展类
namespace Logger{
Logger::Logger() {
    // 打开日志文件, 如果需要每天一个日志文件，可以在文件名中加上日期喵
    log_file.open("log.txt", std::ios::app);
}

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close(); // 确保关闭日志文件喵
    }
}

void Logger::Log(const std::string &level, const std::string &msg) {
    std::string log_entry = get_formatted_log_entry(level, msg);
    if (log_file.is_open()) {
        log_file << log_entry << std::endl; // 写入到日志文件喵
    }
}

std::string Logger::get_time() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X"); // 格式化时间喵
    return ss.str();
}

std::string Logger::get_level(const std::string &level) {
    return "[" + level + "]";
}

std::string Logger::get_formatted_log_entry(const std::string &level, const std::string &msg) {
    std::stringstream log_entry_stream;
    log_entry_stream << Logger::get_time() << " "
                     << Logger::get_level(level) << " "
                     << msg;
    return log_entry_stream.str();
}
}