#ifndef TINYRPC_COMMON_LOG_H
#define TINYRPC_COMMON_LOG_H

#include <string>
#include <memory>
#include <queue>
#include <vector>
#include "tinyrpc/common/mutex.h"


#define DEBUGLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() && tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Debug){ \
        std::string debug_msg = (new tinyrpc::LogEvent(tinyrpc::LogLevel::Debug))->toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        debug_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(debug_msg); \
        tinyrpc::Logger::GetGlobalLogger()->log(); \
    } \

#define INFOLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Info){ \
        std::string info_msg = (new tinyrpc::LogEvent(tinyrpc::LogLevel::Info))->toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        info_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(info_msg); \
        tinyrpc::Logger::GetGlobalLogger()->log(); \
    } \
    

#define ERRORLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Error){ \
        std::string error_msg = (new tinyrpc::LogEvent(tinyrpc::LogLevel::Error))->toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        error_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(error_msg); \
        tinyrpc::Logger::GetGlobalLogger()->log(); \
    } \


namespace tinyrpc{

template<typename... Args>
std::string formatString(const char* str, Args&&... args){
    int size = snprintf(nullptr, 0, str, args...);  //str 格式，args要打印的字符串

    std::string result;
    if(size > 0){
        result.resize(size);
        snprintf(&result[0], size + 1, str, args...);
    }
    return result;

}

enum LogLevel{
    Unknown = 0,
    Debug = 1,
    Info = 2,
    Error = 3
};

class Logger{
public:
    typedef std::shared_ptr<Logger> s_ptr;  // 为啥要用共享指针？

    Logger(LogLevel log_level);

    void pushLog(const std::string& msg);

    // void init();

    void log();

    LogLevel getLogLevel() const{
        return m_set_level;
    }

public:
    static Logger* GetGlobalLogger();
    static void InitGlobalLogger();

private:
    LogLevel m_set_level;
    std::queue<std::string> m_buffer;
    std::vector<std::string> m_app_buffer;

    Mutex m_mutex;
    

};

class LogEvent{

public:
    LogEvent(LogLevel level)
    : m_level(level){}

    std::string getFileName() const{
        return m_file_name;
    }

    LogLevel getLogLevel() const{
        return m_level;
    }

    std::string toString();

private:
    std::string m_file_name; // 文件名
    int32_t m_file_line; // 行号
    int32_t m_pid; // 进程号
    int32_t m_thread_id;  // 线程号

    LogLevel m_level; // 日志级别
};


}


#endif