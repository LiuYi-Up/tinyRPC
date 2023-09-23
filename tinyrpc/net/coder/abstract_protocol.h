#ifndef TINYRPC_NET_CODER_ABSTRACT_PROTOCOL_H
#define TINYRPC_NET_CODER_ABSTRACT_PROTOCOL_H

#include <memory>
#include <string>

namespace tinyrpc{

struct AbstractProtocol : public std::enable_shared_from_this<AbstractProtocol>{

public:
    typedef std::shared_ptr<AbstractProtocol> s_ptr;
    virtual ~AbstractProtocol(){}

public:
    std::string m_req_id; // 标识唯一的请求或者响应函数
};


}

#endif