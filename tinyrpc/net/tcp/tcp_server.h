#ifndef TINYRPC_NET_TCP_SERVER_H
#define TINYRPC_NET_TCP_SERVER_H

#include <set>

#include "tinyrpc/net/io_thread_group.h"
#include "tinyrpc/net/tcp/tcp_acceptor.h"
#include "tinyrpc/net/tcp/tcp_connection.h"
#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/net/eventloop.h"

namespace tinyrpc{

class TcpServer{
public:
    TcpServer(NetAddr::s_ptr addr);
    ~TcpServer();

    void start();

private:
    void init();


    void onAccept();

private:

    TcpAcceptor::s_ptr m_acceptor;  // 用于建立连接

    NetAddr::s_ptr m_local_addr;  // 本地监听的地址

    EventLoop* m_main_event_loop {nullptr};  // main Reactor
    IOThreadGroup* m_io_thread_groups {nullptr};  // subReactor
    FdEvent* m_listen_fd_event {nullptr};  // 用main Reactor的监听事件，添加到m_main_event_loop

    int m_client_count {0};

    std::set<TcpConnection::s_ptr> m_client;
};


}

#endif
