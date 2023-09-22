#ifndef TINYRPC_NET_TCP_TCP_CONNECTION_H
#define TINYRPC_NET_TCP_TCP_CONNECTION_H

#include <map>
#include <vector>

#include "tinyrpc/net/tcp/tcp_buffer.h"
#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/net/eventloop.h"
#include "tinyrpc/net/io_thread.h"
#include "tinyrpc/net/coder/abstract_protocol.h"
#include "tinyrpc/net/coder/abstract_coder.h"

namespace tinyrpc{

enum TcpState{
    NotConnected = 1,
    Connected = 2,
    HalfClosing = 3,
    Closed = 4
};

enum TcpConnectionType{
    TcpConnectionByServer = 1,
    TcpConnectionByClient = 2
};

class TcpConnection{
public:
    typedef std::shared_ptr<TcpConnection> s_ptr;

    TcpConnection(EventLoop* event_loop, int fd, int buffer_size, NetAddr::s_ptr peer_addr, TcpConnectionType type = TcpConnectionByServer);
    ~TcpConnection();
    
    void onRead();

    void onWrite();

    void excute();

    void setState(const TcpState& state);

    TcpState getState();

    void clear();

    void shutdown();
    
    void setTcpConnectionType(TcpConnectionType type);

    // 监听可写事件
    void listenWrite();

    // 监听可读事件
    void listenRead();

    void pushSendMessage(AbstractProtocol::s_ptr message, std::function<void(AbstractProtocol::s_ptr)> done);
    
    void pushReadMessage(const std::string req_id, std::function<void(AbstractProtocol::s_ptr)> done);

private:
    // IOThread* m_io_thread {nullptr};  // 代表持有该连接的 IO 线程
    EventLoop* m_event_loop;

    FdEvent* m_fd_event {nullptr};

    int m_fd;
    int m_buffer_size;

    NetAddr::s_ptr m_local_addr;
    NetAddr::s_ptr m_peer_addr;

    TCPBuffer::s_ptr m_in_buffer;
    TCPBuffer::s_ptr m_out_buffer;

    TcpState m_state;


    TcpConnectionType m_connection_type {TcpConnectionByServer};

    std::vector<std::pair<AbstractProtocol::s_ptr, std::function<void(AbstractProtocol::s_ptr)>>> m_write_dones;
    std::map<std::string, std::function<void(AbstractProtocol::s_ptr)>> m_read_dones;

    AbstractCoder* m_coder;
};

}

#endif