#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace Logger{
#define LOG(level, msg) Logger::GetInstance().log(level, msg)

class Logger{
public:
    static Logger& GetInstance(){
        static Logger instance;
        return instance;
    }
    void Log(const std::string &level, const std::string &msg);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream log_file;

    std::string get_time();
    std::string get_level(const std::string &level);
    std::string get_file_name(const std::string &file_name);
    std::string get_line_num(const int line_num);
    std::string get_formatted_log_entry(const std::string &level, const std::string &msg);
};

}