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
};


}


#endif