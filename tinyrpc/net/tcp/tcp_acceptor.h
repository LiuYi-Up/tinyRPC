#ifndef TINYRPC_NET_TCP_TCP_ACCEPTOR_H
#define TINYRPC_NET_TCP_TCP_ACCEPTOR_H


#include "tinyrpc/net/tcp/net_addr.h"

namespace tinyrpc{


class TcpAcceptor{
public:
    TcpAcceptor(NetAddr::s_ptr addr);
    ~TcpAcceptor();

    int accept();

private:
    NetAddr::s_ptr m_local_addr;  // 服务端监听的地址

    int m_listen_fd {-1};  // 监听套接字

    int m_family {-1};  


};


}


#endif