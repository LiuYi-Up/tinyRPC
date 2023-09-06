#include <sys/socket.h>
#include <string.h>
#include "tinyrpc/net/tcp/tcp_acceptor.h"
#include "tinyrpc/common/log.h"


namespace tinyrpc{

TcpAcceptor::TcpAcceptor(NetAddr::s_ptr addr)
:m_local_addr(addr){
    if(!addr->checkValid()){
        ERRORLOG("invalid local addr %s", addr->toString().c_str());
        exit(0);
    }

    // 创建 socket 套接字
    m_family = m_local_addr->getFamily();
    m_listen_fd = socket(m_family, SOCK_STREAM, 0); // 地址描述iPv4，socket类型,协议0为不指定
    if(m_listen_fd < 0){
        ERRORLOG("create listen fd failed %d", m_listen_fd);
        exit(0);
    }

    // 设置端口复用
    int val = 1;
    if(setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) != 0){
        ERRORLOG("setsockopt error, errno=%d, error=%s", errno, strerror(errno));
        exit(0);
    }

    // bind
    socklen_t len = m_local_addr->getSockLen();
    if(bind(m_listen_fd, m_local_addr->getSockAddr(), len) != 0){
        ERRORLOG("bind error, errno=%d, error=%s", errno, strerror(errno));
        exit(0);
    }

    // listen
    if(listen(m_listen_fd, 1000) != 0){
        ERRORLOG("listen error, errno=%d, error=%s", errno, strerror(errno));
        exit(0);
    }

}


TcpAcceptor::~TcpAcceptor(){

}

int TcpAcceptor::accept(){
    if(m_family == AF_INET){
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));

        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = ::accept(m_listen_fd, reinterpret_cast<sockaddr*>(&client_addr, client_addr_len));
        if(client_fd < 0){
            ERRORLOG("accept error, errno=%d, error=%s", errno, strerror(errno));
        }

        IPNetAddr peer_addr(client_addr);
        INFOLOG("a client have accept succ, peer addr [%s]", peer_addr.toString());

        return client_fd;
    }
    else{
        // ...
    }
}


}