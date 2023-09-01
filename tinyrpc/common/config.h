#ifndef TINYRPC_COMMON_CONFIG
#define TINYRPC_COMMON_CONFIG

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