#ifndef TINYRPC_NET_EVENTLOOP_H
#define TINYRPC_NET_EVENTLOOP_H

#include <pthread.h>
#include <set>
#include <functional>
#include <queue>
#include "tinyrpc/common/mutex.h"
#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/net/wakeup_fd_event.h"
#include "tinyrpc/net/timer.h"
#include "tinyrpc/net/timer_event.h"


namespace tinyrpc{

class EventLoop{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void wakeup();

    void stop();

    void addEpollEvent(FdEvent* event);

    void deleteEpollEvent(FdEvent* event);

    bool isInLoopThread();

    void addTask(std::function<void()> cb, bool is_wakeup=false);  // 将任务添加到队列里

    void addTimerEvent(TimerEvent::s_ptr event);

private:
    void dealwakeup();
    void initWakeupFdEvent();
    void initTimer();
    
private:
    
    pid_t m_pid;  // 每个线程都只能有一个loop
    int m_epoll_fd {0};  // epoll fd
    
    int m_wakeup_fd {0};  // 负责监听任务？？？
    WakeupFdEvent* m_wakeup_fd_event {nullptr};

    Timer* m_timer {nullptr};

    bool m_is_stop {false};
    
    std::set<int> m_listen_fds;  // 监听句柄集合

    std::queue<std::function<void()>> m_pending_tasks;
    
    Mutex m_mutex;

};

}


#endif