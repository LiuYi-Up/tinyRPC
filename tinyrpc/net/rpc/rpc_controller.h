#ifndef TINYRPC_NET_RPC_RPC_CONTROLLER_H
#define TINYRPC_NET_RPC_RPC_CONTROLLER_H


#include "google/protobuf/service.h"
#include "google/protobuf/stubs/callback.h"

#include "tinyrpc/net/tcp/net_addr.h"

namespace tinyrpc{


class RpcController : public google::protobuf::RpcController{
public:
    RpcController() {}
    ~RpcController() {}

  
    void Reset();

    bool Failed() const;

    std::string ErrorText() const;

    void StartCancel();

    void SetFailed(const std::string& reason);

    bool IsCanceled() const;

    void NotifyOnCancel(google::protobuf::Closure* callback);

    void SetError(const int32_t error_code, const std::string error_info);

    int32_t GetErrorCode();

    std::string GetErrorInfo();

    void SetMsgId(const std::string msg_id);

    std::string GetMsgId();

    void SetLocalAddr(NetAddr::s_ptr addr);

    NetAddr::s_ptr GetLocalAddr();

    void SetPeerAddr(NetAddr::s_ptr addr);

    NetAddr::s_ptr GetPeerAddr();

    void SetTimeout(int timeout);

    int GetTimeout();


private:
    int32_t m_error_code {0};
    std::string m_error_info;
    std::string m_msg_id;

    bool m_is_failed {false};
    bool m_is_cancled {false};
    bool m_is_finished {false};

    NetAddr::s_ptr m_local_addr {nullptr};
    NetAddr::s_ptr m_peer_addr {nullptr};

    int m_timeout {1000}; // ms
};

}

#endif