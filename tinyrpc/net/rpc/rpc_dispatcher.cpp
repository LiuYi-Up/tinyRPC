#include "tinyrpc/net/rpc/rpc_dispatcher.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/error_code.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"
#include "tinyrpc/net/rpc/rpc_controller.h"
#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/net/tcp/tcp_connection.h"

#include "google/protobuf/service.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"


namespace tinyrpc{

static RpcDispatcher* g_rpc_dispatcher = nullptr;

RpcDispatcher* RpcDispatcher::GetGlobalRpcDispatcher(){
    if(g_rpc_dispatcher != nullptr){
        return g_rpc_dispatcher;
    }
    g_rpc_dispatcher = new RpcDispatcher();
    return g_rpc_dispatcher;
}

void RpcDispatcher::dispatch(AbstractProtocol::s_ptr request, AbstractProtocol::s_ptr respose, TcpConnection* connection){
    std::shared_ptr<TinyPBProtocol> req_protocol = std::dynamic_pointer_cast<TinyPBProtocol>(request);
    std::shared_ptr<TinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<TinyPBProtocol>(respose);
    
    rsp_protocol->m_msg_id = req_protocol->m_msg_id;
    rsp_protocol->m_method_name = req_protocol->m_method_name;

    // Service.method
    std::string method_full_name = req_protocol->m_method_name;
    std::string service_name;
    std::string method_name;
    if(!parseServiceFullName(method_full_name, service_name, method_name)){
        ERRORLOG("%s | failed to parse method_full_name[%s].", req_protocol->m_msg_id.c_str(), method_full_name.c_str());
        setErrorCode(rsp_protocol, ERROR_PARSE_SERVICE_NAME, "parse service name failed.");
        return;
    }

    auto it = m_service_map.find(service_name);
    if(it == m_service_map.end()){
        ERRORLOG("%s | not found service[%s].", req_protocol->m_msg_id.c_str(), service_name.c_str());
        setErrorCode(rsp_protocol, ERROR_SERVICE_NOT_FOUND, "service not found.");
        return;
    }

    service_s_ptr service = (*it).second;

    const google::protobuf::MethodDescriptor* method = service->GetDescriptor()->FindMethodByName(method_name);
    if(method == NULL){
        ERRORLOG("%s | not found method[%s] in service[%s].", req_protocol->m_msg_id.c_str(), method_name.c_str(), service_name.c_str());
        setErrorCode(rsp_protocol, ERROR_METHOD_NOT_FOUND, "method not found.");
        return;
    }

    // 获取 request type,并实例化 request type 对象
    google::protobuf::Message* req_msg = service->GetRequestPrototype(method).New();
    
    // 反序列化 pb_data 为 req_smg
    bool rt = req_msg->ParseFromString(req_protocol->m_pb_data);
    if(!rt){
        ERRORLOG("%s | deserialize error.", req_protocol->m_msg_id.c_str());
        setErrorCode(rsp_protocol, ERROR_FAILED_DESERIALIZE, "deserialize error.");
        if(req_msg != nullptr){
            delete req_msg;
            req_msg = nullptr;
        }
        return;
    }
    INFOLOG("%s | get rpc request[%s]", req_protocol->m_msg_id.c_str(), req_msg->ShortDebugString().c_str());

    RpcController rpc_controller;
    rpc_controller.SetLocalAddr(connection->getLocalAddr());
    rpc_controller.SetPeerAddr(connection->getPeerAddr());
    rpc_controller.SetMsgId(req_protocol->m_msg_id);

    // 实例化 response type 对象
    google::protobuf::Message* rps_msg = service->GetResponsePrototype(method).New();

    // 执行 func
    service->CallMethod(method, &rpc_controller, req_msg, rps_msg, NULL);

    // 将结果序列化，并写入 TinyPBProtocol 对象中
    rt = rps_msg->SerializeToString(&(rsp_protocol->m_pb_data));
    if(!rt){
        ERRORLOG("%s | serialize error.", req_protocol->m_msg_id.c_str());
        setErrorCode(rsp_protocol, ERROR_FAILED_SERIALIZE, "serialize error.");
        if(req_msg != nullptr){
            delete req_msg;
            req_msg = nullptr;
        }
        if(rps_msg != nullptr){
            delete rps_msg;
            rps_msg = nullptr;
        }
        return;
    }

    rsp_protocol->m_err_code = 0;
    INFOLOG("%s | dispatch success, request[%s], response[%s].", req_protocol->m_msg_id.c_str(), req_msg->ShortDebugString().c_str(), rps_msg->ShortDebugString().c_str());
    
    if(req_msg != nullptr){
        delete req_msg;
        req_msg = nullptr;
    }
    if(rps_msg != nullptr){
        delete rps_msg;
        rps_msg = nullptr;
    }
}

void RpcDispatcher::registerService(service_s_ptr service){
    std::string service_name = service->GetDescriptor()->full_name();
    m_service_map[service_name] = service;
}


bool RpcDispatcher::parseServiceFullName(const std::string& full_name, std::string& service_name, std::string& method_name){
    if(full_name.empty()){
        ERRORLOG("service full name is empty.");
        return false;
    }
    
    size_t i = full_name.find_first_of(".");
    if(i == full_name.npos){
        ERRORLOG("not find . in full name[%s].", full_name.c_str());
        return false;
    }

    service_name = full_name.substr(0, i);
    method_name = full_name.substr(i + 1, full_name.size() - i - 1);

    INFOLOG("parse full name[%s] with service name[%s] and method name[%s].", full_name.c_str(), service_name.c_str(), method_name.c_str());

    return true;
}

void RpcDispatcher::setErrorCode(std::shared_ptr<TinyPBProtocol> msg, const int32_t& err_code, const std::string& err_info){
    msg->m_err_code = err_code;
    msg->m_err_info = err_info;
    msg->m_err_info_len = err_info.length();
}

}