#ifndef TINYRPC_COMMON_RUN_TIME_H
#define TINYRPC_COMMON_RUN_TIME_H

#include <string>

namespace tinyrpc{


class RunTime{
public:
    static RunTime* GetRunTime();

public:
    std::string m_msg_id;
    std::string m_method_name;

};

}


#endif