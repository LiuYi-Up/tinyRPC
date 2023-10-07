#include <sys/time.h>
#include <cstdio>
#include <assert.h>
#include <sstream>
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/util.h"
#include "tinyrpc/common/config.h"
#include "tinyrpc/common/run_time.h"
#include "tinyrpc/net/eventloop.h"

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

void Logger::InitGlobalLogger(int type /*=1*/){
    std::string log_level_str = Config::GetGlobalConfig()->m_log_level;
    LogLevel log_level = StringToLogLvel(Config::GetGlobalConfig()->m_log_level);
    g_logger = new Logger(log_level, type);
    g_logger->init();
}


Logger::Logger(LogLevel log_level, int type /*=1*/)
:m_set_level(log_level), m_type(type){
    if(m_type == 0){
        return;
    }
    m_async_logger = std::make_shared<AsyncLogger>(
        Config::GetGlobalConfig()->m_log_file_name + "_rpc",
        Config::GetGlobalConfig()->m_log_file_path,
        Config::GetGlobalConfig()->m_log_max_file_size
    );

     m_async_app_logger = std::make_shared<AsyncLogger>(
        Config::GetGlobalConfig()->m_log_file_name + "_app",
        Config::GetGlobalConfig()->m_log_file_path,
        Config::GetGlobalConfig()->m_log_max_file_size
    );
}

void Logger::init(){
    if(m_type == 0){
        return;
    }

    m_timer_event = std::make_shared<TimerEvent>(Config::GetGlobalConfig()->m_log_sync_interval, true, std::bind(&Logger::syncLoop, this));
    EventLoop::getCurEventLoop()->addTimerEvent(m_timer_event);
}

void Logger::syncLoop(){
  // 同步 m_buffer 到 async_logger 的buffer队尾
    std::vector<std::string> tmp_vec;
    ScopeMutex<Mutex> lock(m_mutex);
    tmp_vec.swap(m_buffer);
    lock.unlock();

    if(!tmp_vec.empty()){
        m_async_logger->pushLogBuffer(tmp_vec);
    }
    tmp_vec.clear();

    std::vector<std::string> tmp_vec2;
    ScopeMutex<Mutex> lock2(m_app_mutex);
    tmp_vec2.swap(m_app_buffer);
    lock2.unlock();

    if(!tmp_vec2.empty()){
        m_async_app_logger->pushLogBuffer(tmp_vec2);
    }
    tmp_vec2.clear();
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
        
    std::string msg_id = RunTime::GetRunTime()->m_msg_id;
    std::string method_name = RunTime::GetRunTime()->m_method_name;
    if(!msg_id.empty()){
        ss << "[" << msg_id << "]\t";
    }
    if(!method_name.empty()){
        ss << "[" << method_name << "]\t";
    }

    return ss.str();
}

void Logger::pushLog(const std::string& msg){
    if(m_type == 0){
        printf((msg + "\n").c_str());
        return;
    }
    ScopeMutex<Mutex> lock(m_mutex);
    m_buffer.push_back(msg);
    lock.unlock();
}


void Logger::pushAppLog(const std::string& msg){
    if(m_type == 0){
        printf((msg + "\n").c_str());
        return;
    }
    ScopeMutex<Mutex> lock(m_app_mutex);
    m_app_buffer.push_back(msg);
    lock.unlock();
}


void Logger::log(){

}

AsyncLogger::AsyncLogger(const std::string& file_name, const std::string& file_path, const int max_size)
:m_file_name(file_name), m_file_path(file_path), m_max_file_size(max_size) {
    sem_init(&m_sempahore, 0, 0);
    
    assert(pthread_create(&m_thread, NULL, &AsyncLogger::Loop, this) == 0);

    sem_wait(&m_sempahore);
}

void* AsyncLogger::Loop(void* arg){
    AsyncLogger* logger = reinterpret_cast<AsyncLogger*>(arg);

    assert(pthread_cond_init(&logger->m_condition, NULL) == 0);

    sem_post(&logger->m_sempahore);

    while(1){
        ScopeMutex<Mutex> lock(logger->m_mutex);
        while(logger->m_buffer.empty()){
            pthread_cond_wait(&(logger->m_condition), logger->m_mutex.getMutex());
        }

        std::vector<std::string> tmp;
        tmp.swap(logger->m_buffer.front());
        logger->m_buffer.pop();

        lock.unlock();

        timeval now;
        gettimeofday(&now, NULL);
        struct tm now_time;
        localtime_r(&(now.tv_sec), &now_time);

        const char* format = "%Y%m%d";
        char date[32];
        strftime(date, sizeof(date), format, &now_time);

        // 日志日期更新了，到了新的一天
        if(std::string(date) != logger->m_date){
            logger->m_no = 0;
            logger->m_reopen_flag = true;
            logger->m_date = std::string(date);
        }

        // 没有打开的日志文件句柄
        if(logger->m_file_handler == NULL){
            logger->m_reopen_flag = true;
        }

        std::stringstream ss;
        ss << logger->m_file_path << logger->m_file_name << "_"
            << std::string(date) << "_log.";
        std::string log_file_name = ss.str() + std::to_string(logger->m_no);

        if(logger->m_reopen_flag){
            // 如果已经有打开的，先关闭原来的文件句柄
            if(logger->m_file_handler){
                fclose(logger->m_file_handler);
            }
            logger->m_file_handler = fopen(log_file_name.c_str(), "a");  // "a"追加的模式
            logger->m_reopen_flag = false;
        }

        if(ftell(logger->m_file_handler) > logger->m_max_file_size){
            fclose(logger->m_file_handler);

            log_file_name = ss.str() + std::to_string(logger->m_no++);
            logger->m_file_handler = fopen(log_file_name.c_str(), "a");
            logger->m_reopen_flag = false;
        }

        for(auto& i : tmp){
            if(!i.empty()){
                fwrite(i.c_str(), 1, i.length(), logger->m_file_handler);
            }
        }
        fflush(logger->m_file_handler);

        if(logger->m_stop_flag){
            return NULL;
        }
    }
    return NULL;
}

void AsyncLogger::stop(){
    m_stop_flag = true;
}

void AsyncLogger::flush(){
    if(m_file_handler){
        fflush(m_file_handler);
    }
}

void AsyncLogger::pushLogBuffer(std::vector<std::string>& vec){
    // 异步唤醒
    ScopeMutex<Mutex> lock(m_mutex);
    m_buffer.push(vec);
    pthread_cond_signal(&m_condition);
    lock.unlock();
}


}