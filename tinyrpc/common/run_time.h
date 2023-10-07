#ifndef TINYRPC_COMMON_RUN_TIME_H
#define TINYRPC_COMMON_RUN_TIME_H

#include <string>

namespace tinyrpc{

class RpcInterface;

class RunTime{
public:
    RpcInterface* getRpcInterface();

public:
    static RunTime* GetRunTime();

public:
    std::string m_msg_id;
    std::string m_method_name;
    RpcInterface* m_rpc_interface {NULL};

};

}


#endif