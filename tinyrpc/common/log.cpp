#include <sys/time.h>
#include <cstdio>
#include <sstream>
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/util.h"
#include "tinyrpc/common/config.h"


namespace tinyrpc{

static Logger* g_logger = nullptr;

LogLevel StringToLogLvel(const std::string& log_level_str){
    if(log_level_str == "DEBUG"){
        return Debug;
    }
    else if(log_level_str == "INFO"){
        return Info;
    }
    else if(log_level_str == "ERROR"){
        return Error;
    }
    else{
        return Unknown;
    }

}


std::string LogLevelToString(LogLevel level){
    switch (level)
    {
    case Debug:
        return "DEBUG";
    case Info:
        return "INFO";
    case Error:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}


Logger* Logger::GetGlobalLogger(){
    return g_logger;
}

void Logger::InitGlobalLogger(){
    std::string log_level_str = Config::GetGlobalConfig()->m_log_level;
    LogLevel log_level = StringToLogLvel(Config::GetGlobalConfig()->m_log_level);
    g_logger = new Logger(log_level);
}


Logger::Logger(LogLevel log_level)
:m_set_level(log_level){

}


std::string LogEvent::toString(){
    /*
        将日志内容转换为string
    */
    struct timeval now_time;

    gettimeofday(&now_time, nullptr);

    struct tm now_time_t;  // 获取日期
    localtime_r(&(now_time.tv_sec), &now_time_t);
    
    char buf[128];

    // 将给定格式的日期时间对象转换为字符串。日期时间对象=>字符串，控制输出格式
    strftime(&buf[0], 128, "%y-%m-%d %H:%M:%S", &now_time_t); 
    std::string time_str(buf);
    int ms = now_time.tv_sec / 1000;
    time_str = time_str + "." + std::to_string(ms);

    m_pid = getPid();
    m_thread_id = getThreadId();

    std::stringstream ss;
    ss << "[" << LogLevelToString(m_level) << "]\t"
        << "[" << time_str << "]\t"
        << "[" << m_pid << ":" << m_thread_id << "]\t";
        

    return ss.str();
}

void Logger::pushLog(const std::string& msg){
    ScoprMutex<Mutex> lock(m_mutex);
    m_buffer.push(msg);
    lock.unlock();

}


void Logger::log(){

    ScoprMutex<Mutex> lock(m_mutex);
    std::queue<std::string> tmp;
    m_buffer.swap(tmp);
    lock.unlock();

    while(!tmp.empty()){
        std::string msg = tmp.front();
        tmp.pop();
        printf("%s", msg.c_str());
    } 
}
}