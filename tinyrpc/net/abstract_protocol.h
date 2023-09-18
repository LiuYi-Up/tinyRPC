#ifndef TINYRPC_NET_ABSTRACT_PROTOCOL_H
#define TINYRPC_NET_ABSTRACT_PROTOCOL_H

#include <memory>
#include <string>

namespace tinyrpc{

class AbstractProtocol : public std::enable_shared_from_this<AbstractProtocol>{

public:
    typedef std::shared_ptr<AbstractProtocol> s_ptr;

    std::string getReqId(){
        return m_req_id;
    }

    void setReqId(const std::string& req_id){
        m_req_id = req_id;
    }

    virtual ~AbstractProtocol(){}
protected:
    std::string m_req_id; // 标识唯一的请求或者响应函数
};


}

#endif