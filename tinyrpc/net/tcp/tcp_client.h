#ifndef TINYRPC_NET_TCP_TCP_CLIENT_H
#define TINYRPC_NET_TCP_TCP_CLIENT_H



#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/net/abstract_protocol.h"
#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/net/tcp/tcp_connection.h"

namespace tinyrpc{


class TcpClient{
public:
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
    void readMessage(AbstractProtocol::s_ptr, std::function<void(AbstractProtocol::s_ptr)> done);

private:
    NetAddr::s_ptr m_peer_addr;
    EventLoop* m_event_loop {nullptr};

    int m_fd {-1};
    FdEvent* m_fd_event {nullptr};

    TcpConnection::s_ptr m_connection;

};


}

#endif