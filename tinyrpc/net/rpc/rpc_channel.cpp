#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

#include "tinyrpc/net/rpc/rpc_channel.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"
#include "tinyrpc/net/coder/abstract_protocol.h"
#include "tinyrpc/net/rpc/rpc_controller.h"
#include "tinyrpc/net/tcp/tcp_client.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/msg_id_util.h"
#include "tinyrpc/common/error_code.h"


namespace tinyrpc{


RpcChannel::RpcChannel(NetAddr::s_ptr peer_addr)
:m_peer_addr(peer_addr){
    
}

RpcChannel::~RpcChannel(){

}

void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                        google::protobuf::Message* response, google::protobuf::Closure* done){

    std::shared_ptr<tinyrpc::TinyPBProtocol> req_protocol = std:: make_shared<tinyrpc::TinyPBProtocol>();
    
    RpcController* my_controler = dynamic_cast<RpcController*>(controller);
    if(my_controler == nullptr){
        ERRORLOG("failed callmethod, RpcController convert error");
        return;
    }

    if(my_controler->GetMsgId().empty()){
        req_protocol->m_msg_id = MsgIdUtil::GetMsgId();
        my_controler->SetMsgId(req_protocol->m_msg_id);
    }
    else{
        req_protocol->m_msg_id = my_controler->GetMsgId();
    }

    req_protocol->m_method_name = method->full_name();
    INFOLOG("%s | call method name [%s]", req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str());

    if(!m_is_init){
        std::string err_info = "RpcChannel not init";
        my_controler->SetError(ERROR_RPC_CHANNEL_INIT, err_info);
        ERRORLOG("%s | %s", req_protocol->m_msg_id.c_str(), err_info.c_str());
        return;
    }

    // req 序列化
    if(!request->SerializeToString(&(req_protocol->m_pb_data))){
        std::string err_info = "failed to serialize";
        my_controler->SetError(ERROR_FAILED_SERIALIZE, err_info);
        ERRORLOG("%s | %s, origin request [%s]", req_protocol->m_msg_id.c_str(), err_info.c_str(), request->ShortDebugString().c_str());
        return;
    }

    s_ptr channel = shared_from_this();

    TcpClient client(m_peer_addr);

    client.connection([&client, req_protocol, channel]() mutable{

        client.writeMessage(req_protocol, [&client, req_protocol, channel](AbstractProtocol::s_ptr msg) mutable{
            INFOLOG("%s | send rpc request success, call method name[%s]",
            req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str());
        });

        client.readMessage(req_protocol->m_msg_id, [channel](AbstractProtocol::s_ptr msg) mutable{
            std::shared_ptr<tinyrpc::TinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<tinyrpc::TinyPBProtocol>(msg);
            INFOLOG("%s | success get rpc response, call method name[%s]",
            rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str());

            RpcController* my_controller = dynamic_cast<RpcController*>(channel->getController());
            if(!channel->getResponse()->ParseFromString(rsp_protocol->m_pb_data)){
                ERRORLOG("%s | deserialize failed.", rsp_protocol->m_msg_id.c_str());
                my_controller->SetError(ERROR_PARSE_SERVICE_NAME, "parse error");
                return;
            }

            if(rsp_protocol->m_err_code != 0){
                ERRORLOG("%s | call rpc method[%s] failed, error code[%d], error ifo[%s]",
                rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str(),
                rsp_protocol->m_err_code, rsp_protocol->m_err_info.c_str());
                return;
            }

            if(channel->getClosure()){
                channel->getClosure()->Run();
            }
            channel.reset();
        });
    });
}

void RpcChannel::Init(controller_s_ptr contrller, msg_s_ptr req, msg_s_ptr res, closure_s_ptr closure){
    if(m_is_init){
        return;
    }

    m_controller = contrller;
    m_request = req;
    m_response = res;
    m_closure = closure;
    m_is_init = true;
}

google::protobuf::RpcController* RpcChannel::getController(){
    return m_controller.get();
}

google::protobuf::Message* RpcChannel::getRequest(){
    return m_request.get();
}

google::protobuf::Message* RpcChannel::getResponse(){
    return m_response.get();
}

google::protobuf::Closure* RpcChannel::getClosure(){
    return m_closure.get();
}

}