#ifndef TINYRPC_COMMON_CONFIG_H
#define TINYRPC_COMMON_CONFIG_H

#include <map>
#include <string>

namespace tinyrpc{

class Config{
public:
    Config();
    Config(const char* xmlfile);

    ~Config();

    static Config* GetGlobalConfig();
    static void SetGlobalConfig(const char* xmlfile);
public:
    std::string m_log_level;

    std::string m_log_file_name;
    std::string m_log_file_path;
    int m_log_max_file_size;
    int m_log_sync_inteval {0};  // 日志时间间隔， ms

};


}


#endif