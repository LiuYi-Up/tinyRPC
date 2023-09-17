#include <sys/unistd.h>
#include <string.h>

#include "tinyrpc/common/log.h"
#include "tinyrpc/net/tcp/tcp_client.h"
#include "tinyrpc/net/fd_event_group.h"


namespace tinyrpc{

TcpClient::TcpClient(NetAddr::s_ptr peer_addr)
:m_peer_addr(peer_addr){
    m_event_loop = EventLoop::getCurEventLoop();
    
    m_fd = socket(m_peer_addr->getFamily(), SOCK_STREAM, 0);
    if(m_fd == -1){
        ERRORLOG("TcpClient::TcpClient failed to create fd.");
        return;
    }

    m_fd_event = FdEventGroup::GetFdEventGroup()->getFdEvent(m_fd);
    m_fd_event->setNonblock();

    m_connection = std::make_shared<TcpConnection>(m_event_loop, m_fd, 128, m_peer_addr);
    m_connection->setTcpConnectionType(TcpConnectionByClient);
    
}

TcpClient::~TcpClient(){
    if(m_fd > 0){
        close(m_fd);
    }
}

// 异步进行 connection 连接
// 连接成功，将执行 done
void TcpClient::connection(std::function<void()> done){
    int rt = ::connect(m_fd, m_peer_addr->getSockAddr(), m_peer_addr->getSockLen());
    if(rt == 0){
        DEBUGLOG("connect [%s] success", m_peer_addr->toString());
        if(done){
            done();
        }
    }
    else{
        if(errno == EINPROGRESS){
            // epoll 监听可写事件， 然后判断错误码
            m_fd_event->listen(FdEvent::OUT_EVENT, [this, done](){
                int error = 0;
                socklen_t error_len = sizeof(error);
                getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &error, &error_len);
                if(error == 0){
                    DEBUGLOG("connect [%s] success", m_peer_addr->toString().c_str());
                    if(done){
                        done();
                    }
                }
                else{
                    ERRORLOG("connect failed, errno=%d, error=%s", errno, strerror(errno));
                }
                // 连接完成后需要去掉可写事件的监听
                m_fd_event->cancle(FdEvent::OUT_EVENT);
                m_event_loop->addEpollEvent(m_fd_event);
            });
            m_event_loop->addEpollEvent(m_fd_event);
            if(!m_event_loop->isLooping()){
                m_event_loop->loop();
            }
        }
        else{
            ERRORLOG("connect failed, errno=%d, error=%s", errno, strerror(errno));
        }
    }
}


// 异步发送 message
// 发送成功，将执行 done 函数，函数入参为 message
void TcpClient::writeMessage(AbstractProtocol::s_ptr message, std::function<void(AbstractProtocol::s_ptr)> done){

}


// 异步读取 message
// 读取成功，将执行 done 函数，函数入参为 message
void TcpClient::readMessage(AbstractProtocol::s_ptr, std::function<void(AbstractProtocol::s_ptr)> done){


}

}