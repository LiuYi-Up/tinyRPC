#ifndef TINYRPC_NET_RPC_RPC_DISPATCHER_H
#define TINYRPC_NET_RPC_RPC_DISPATCHER_H

#include <map>
#include <string>

#include "google/protobuf/service.h"
#include "tinyrpc/net/coder/abstract_coder.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"

namespace tinyrpc{

class TcpConnection;

class RpcDispatcher{
public:
    static RpcDispatcher* GetGlobalRpcDispatcher();

public:
    typedef std::shared_ptr<google::protobuf::Service> service_s_ptr; 

    void dispatch(AbstractProtocol::s_ptr request, AbstractProtocol::s_ptr respose, TcpConnection* connection);

    void registerService(service_s_ptr service);


private:
    bool parseServiceFullName(const std::string& full_name, std::string& service_name, std::string& method_name);

    void setErrorCode(std::shared_ptr<TinyPBProtocol> msg, const int32_t& err_code, const std::string& err_info);

private:
    std::map<std::string, service_s_ptr> m_service_map;


};


}

#endif