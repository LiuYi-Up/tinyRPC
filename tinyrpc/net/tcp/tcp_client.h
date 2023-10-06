#ifndef TINYRPC_NET_TCP_TCP_CLIENT_H
#define TINYRPC_NET_TCP_TCP_CLIENT_H

#include <memory>

#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/net/coder/abstract_protocol.h"
#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/net/tcp/tcp_connection.h"
#include "tinyrpc/net/timer_event.h"

namespace tinyrpc{


class TcpClient{
public:
    typedef std::shared_ptr<TcpClient> s_ptr;

    TcpClient(NetAddr::s_ptr peer_addr);
    ~TcpClient();

    // 异步进行 connection 连接
    // 连接成功，将执行 done
    void connection(std::function<void()> done);

    // 异步发送 message
    // 发送成功，将执行 done 函数，函数入参为 message
    void writeMessage(AbstractProtocol::s_ptr message, std::function<void(AbstractProtocol::s_ptr)> done);

    // 异步读取 message
    // 读取成功，将执行 done 函数，函数入参为 message
    void readMessage(const std::string msg_id, std::function<void(AbstractProtocol::s_ptr)> done);

    void stop();

    NetAddr* getPeerAddr();
    NetAddr* getLocalAddr();

    void initLocalAddr();

    int getConnectErrorCode();

    std::string getConnectErrorInfo();

    void addTimer(TimerEvent::s_ptr timer_event);

private:
    NetAddr::s_ptr m_peer_addr;
    NetAddr::s_ptr m_local_addr;
    EventLoop* m_event_loop {nullptr};

    int m_fd {-1};
    FdEvent* m_fd_event {nullptr};

    TcpConnection::s_ptr m_connection;

    int m_connect_error_code {0};
    std::string m_connect_error_info;


};


}

#endif