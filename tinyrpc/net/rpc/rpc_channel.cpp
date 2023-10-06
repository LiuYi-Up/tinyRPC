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
    m_client = std::make_shared<TcpClient>(m_peer_addr);
}

RpcChannel::~RpcChannel(){
    INFOLOG("~RpcChannel");
}

void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                        google::protobuf::Message* response, google::protobuf::Closure* done){

    std::shared_ptr<tinyrpc::TinyPBProtocol> req_protocol = std:: make_shared<tinyrpc::TinyPBProtocol>();
    
    RpcController* my_controller = dynamic_cast<RpcController*>(controller);
    if(my_controller == nullptr){
        ERRORLOG("failed callmethod, RpcController convert error");
        return;
    }

    if(my_controller->GetMsgId().empty()){
        req_protocol->m_msg_id = MsgIdUtil::GetMsgId();
        my_controller->SetMsgId(req_protocol->m_msg_id);
    }
    else{
        req_protocol->m_msg_id = my_controller->GetMsgId();
    }

    req_protocol->m_method_name = method->full_name();
    INFOLOG("%s | call method name [%s]", req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str());

    if(!m_is_init){
        std::string err_info = "RpcChannel not init";
        my_controller->SetError(ERROR_RPC_CHANNEL_INIT, err_info);
        ERRORLOG("%s | %s", req_protocol->m_msg_id.c_str(), err_info.c_str());
        return;
    }

    // req 序列化
    if(!request->SerializeToString(&(req_protocol->m_pb_data))){
        std::string err_info = "failed to serialize";
        my_controller->SetError(ERROR_FAILED_SERIALIZE, err_info);
        ERRORLOG("%s | %s, origin request [%s]", req_protocol->m_msg_id.c_str(), err_info.c_str(), request->ShortDebugString().c_str());
        return;
    }

    s_ptr channel = shared_from_this();

    m_timer_event = std::make_shared<TimerEvent>(my_controller->GetTimeout(), false, [my_controller, channel]() mutable{
        my_controller->StartCancel();
        my_controller->SetError(ERROR_RPC_CALL_TIMEOUT, "rpc call timeout" + std::to_string(my_controller->GetTimeout()));

        if(channel->getClosure()){
            channel->getClosure()->Run();
        }
        channel.reset();
    });

    m_client->addTimer(m_timer_event);

    m_client->connection([req_protocol, channel]() mutable{
        
        RpcController* my_controller = dynamic_cast<RpcController*>(channel->getController());

        if(channel->getClient()->getConnectErrorCode() != 0){
            my_controller->SetError(channel->getClient()->getConnectErrorCode(), channel->getClient()->getConnectErrorInfo());
            ERRORLOG("%s | connect error, error code[%d], error info[%s], peer addr[%s]",
            req_protocol->m_msg_id.c_str(), my_controller->GetErrorCode(), 
            my_controller->GetErrorInfo().c_str(), channel->getClient()->getPeerAddr()->toString().c_str());

            return;
        }

        INFOLOG("%s | connect success, peer addr[%s], local addr[%s]",
            req_protocol->m_msg_id.c_str(), 
            channel->getClient()->getPeerAddr()->toString().c_str(), 
            channel->getClient()->getLocalAddr()->toString().c_str()); 


        channel->getClient()->writeMessage(req_protocol, [req_protocol, channel, my_controller](AbstractProtocol::s_ptr msg) mutable{
            INFOLOG("%s | send rpc request success, call method name[%s], peer addr[%s], local addr[%s]",
            req_protocol->m_msg_id.c_str(), req_protocol->m_method_name.c_str(),
            channel->getClient()->getPeerAddr()->toString().c_str(), channel->getClient()->getLocalAddr()->toString().c_str());
        });

        channel->getClient()->readMessage(req_protocol->m_msg_id, [channel, my_controller](AbstractProtocol::s_ptr msg) mutable{
            std::shared_ptr<tinyrpc::TinyPBProtocol> rsp_protocol = std::dynamic_pointer_cast<tinyrpc::TinyPBProtocol>(msg);

            INFOLOG("%s | success get rpc response, call method name[%s], peer addr[%s], local addr[%s]",
            rsp_protocol->m_msg_id.c_str(), rsp_protocol->m_method_name.c_str(),
            channel->getClient()->getPeerAddr()->toString().c_str(), channel->getClient()->getLocalAddr()->toString().c_str());

            channel->getTimerEvent()->setCancled(true);

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

TcpClient* RpcChannel::getClient(){
    return m_client.get();
}

TimerEvent* RpcChannel::getTimerEvent(){
    return m_timer_event.get();
}


}