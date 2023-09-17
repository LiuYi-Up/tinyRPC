#ifndef TINYRPC_NET_TCP_TCP_CONNECTION_H
#define TINYRPC_NET_TCP_TCP_CONNECTION_H

#include "tinyrpc/net/tcp/tcp_buffer.h"
#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/net/eventloop.h"
#include "tinyrpc/net/io_thread.h"

namespace tinyrpc{

enum TcpState{
        NotConnected = 1,
        Connected = 2,
        HalfClosing = 3,
        Closed = 4
};

class TcpConnection{
public:
    typedef std::shared_ptr<TcpConnection> s_ptr;

    TcpConnection(IOThread* io_thread, int fd, int buffer_size, NetAddr::s_ptr peer_addr);
    ~TcpConnection();
    
    void onRead();

    void onWrite();

    void excute();

    void setState(const TcpState& state);

    TcpState getState();

    void clear();

    void shutdown();
    
private:
    IOThread* m_io_thread {nullptr};  // 代表持有该连接的 IO 线程

    FdEvent* m_fd_event {nullptr};

    int m_fd;
    int m_buffer_size;

    NetAddr::s_ptr m_local_addr;
    NetAddr::s_ptr m_peer_addr;

    TCPBuffer::s_ptr m_in_buffer;
    TCPBuffer::s_ptr m_out_buffer;

    TcpState m_state;

};

}

#endif