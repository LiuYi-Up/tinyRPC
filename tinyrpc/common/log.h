#ifndef TINYRPC_COMMON_LOG_H
#define TINYRPC_COMMON_LOG_H

#include <string>
#include <memory>
#include <queue>
#include <vector>
#include <queue>
#include <semaphore.h>

#include "tinyrpc/common/mutex.h"
#include "tinyrpc/net/timer_event.h"

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

#define DEBUGLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() && tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Debug){ \
        std::string debug_msg = tinyrpc::LogEvent(tinyrpc::LogLevel::Debug).toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        debug_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(debug_msg); \
    } \

#define INFOLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Info){ \
        std::string info_msg = tinyrpc::LogEvent(tinyrpc::LogLevel::Info).toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        info_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(info_msg); \
    } \
    

#define ERRORLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Error){ \
        std::string error_msg = tinyrpc::LogEvent(tinyrpc::LogLevel::Error).toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        error_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushLog(error_msg); \
    } \


#define APPDEBUGLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() && tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Debug){ \
        std::string debug_msg = tinyrpc::LogEvent(tinyrpc::LogLevel::Debug).toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        debug_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushAppLog(debug_msg); \
    } \

#define APPINFOLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Info){ \
        std::string info_msg = tinyrpc::LogEvent(tinyrpc::LogLevel::Info).toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        info_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushAppLog(info_msg); \
    } \
    

#define APPERRORLOG(str, ...) \
    if(tinyrpc::Logger::GetGlobalLogger()->getLogLevel() <= tinyrpc::Error){ \
        std::string error_msg = tinyrpc::LogEvent(tinyrpc::LogLevel::Error).toString() + tinyrpc::formatString(str, ##__VA_ARGS__); \
        error_msg += " [" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + "\n"; \
        tinyrpc::Logger::GetGlobalLogger()->pushAppLog(error_msg); \
    } \


enum LogLevel{
    Unknown = 0,
    Debug = 1,
    Info = 2,
    Error = 3
};


class AsyncLogger{
public:
    typedef std::shared_ptr<AsyncLogger> s_ptr;

    AsyncLogger(const std::string& file_name, const std::string& file_path, const int max_size);

    void stop();

    void flush();

    void pushLogBuffer(std::vector<std::string>& vec);

public:
    static void* Loop(void* arg);


public:
    pthread_t m_thread;

private:
    std::queue<std::vector<std::string>> m_buffer;

    std::string m_file_name;
    std::string m_file_path;

    int m_max_file_size {0};

    sem_t m_sempahore;

    pthread_cond_t m_condition;
    Mutex m_mutex;

    std::string m_date;
    FILE* m_file_handler {NULL};

    bool m_reopen_flag {false};
    int m_no {};
    bool m_stop_flag {false};
};


class Logger{
public:
    typedef std::shared_ptr<Logger> s_ptr;  // 为啥要用共享指针？

    Logger(LogLevel log_level, int type=1);

    void init();

    void pushLog(const std::string& msg);

    void pushAppLog(const std::string& msg);

    void log();

    LogLevel getLogLevel() const{
        return m_set_level;
    }

    void syncLoop();

public:
    static Logger* GetGlobalLogger();
    static void InitGlobalLogger(int type = 1);

private:
    LogLevel m_set_level;
    int m_type;  // 0 同步打印到终端，1 异步打印到文件
    std::vector<std::string> m_buffer;
    std::vector<std::string> m_app_buffer;

    Mutex m_mutex;
    Mutex m_app_mutex;
    
    AsyncLogger::s_ptr m_async_logger;
    AsyncLogger::s_ptr m_async_app_logger;

    TimerEvent::s_ptr m_timer_event;
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