#ifndef TINYRPC_NET_TCP_TCP_ACCEPTOR_H
#define TINYRPC_NET_TCP_TCP_ACCEPTOR_H

#include <memory>
#include "tinyrpc/net/tcp/net_addr.h"

namespace tinyrpc{


class TcpAcceptor{
public:
    typedef std::shared_ptr<TcpAcceptor> s_ptr;

    TcpAcceptor(NetAddr::s_ptr addr);
    ~TcpAcceptor();

    std::pair<int, NetAddr::s_ptr> accept();

    int getListenFd();

private:
    NetAddr::s_ptr m_local_addr;  // 服务端监听的地址

    int m_listen_fd {-1};  // 监听套接字

    int m_family {-1};  


};


}


#endif