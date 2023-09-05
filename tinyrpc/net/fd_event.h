#ifndef TINYRPC_NET_FDEVENT_H
#define TINYRPC_NET_FDEVENT_H

#include <functional>
#include <sys/epoll.h>

namespace tinyrpc{

class FdEvent{
public:

    enum TriggerEvent{
        IN_EVENT = EPOLLIN, // 触发读事件
        OUT_EVENT = EPOLLOUT  // 触发写事件
    };

    FdEvent(){};
    FdEvent(int fd);

    ~FdEvent();

    std::function<void()> handler(TriggerEvent event_type);

    void listen(TriggerEvent event_type, std::function<void()> callback);

    int getFd(){
        return m_fd;
    }

    epoll_event getEpollEvent(){
        return m_listen_event;
    }

protected:
    int m_fd {-1};  // 当前事务句柄

    epoll_event m_listen_event;  // 文件句柄构建的epoll事务

    std::function<void()> m_read_callback;
    std::function<void()> m_write_callback;
    
};

}
#endif