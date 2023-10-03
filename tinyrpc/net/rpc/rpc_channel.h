#ifndef TINYRPC_NET_RPC_RPC_CHANNEL_H
#define TINYRPC_NET_RPC_RPC_CHANNEL_H

#include <memory>

#include "google/protobuf/service.h"

#include "tinyrpc/net/tcp/net_addr.h"

namespace tinyrpc{

class RpcChannel : public google::protobuf::RpcChannel, public std::enable_shared_from_this<RpcChannel>{
public:
    typedef std::shared_ptr<RpcChannel> s_ptr;
    typedef std::shared_ptr<google::protobuf::RpcController> controller_s_ptr;
    typedef std::shared_ptr<google::protobuf::Message> msg_s_ptr;
    typedef std::shared_ptr<google::protobuf::Closure> closure_s_ptr;

public:
    RpcChannel(NetAddr::s_ptr peer_addr);
    ~RpcChannel();

    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done);

    void Init(controller_s_ptr contrller, msg_s_ptr req, msg_s_ptr res, closure_s_ptr closure);

    google::protobuf::RpcController* getController();

    google::protobuf::Message* getRequest();

    google::protobuf::Message* getResponse();

    google::protobuf::Closure* getClosure();

private:

    NetAddr::s_ptr m_peer_addr {nullptr};
    NetAddr::s_ptr m_local_addr {nullptr};

    controller_s_ptr m_controller {nullptr};
    msg_s_ptr m_request {nullptr};
    msg_s_ptr m_response {nullptr};
    closure_s_ptr m_closure {nullptr};

    bool m_is_init {false};
};

}

#endif