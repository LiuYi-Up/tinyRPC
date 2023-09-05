#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <string.h>
#include "tinyrpc/net/eventloop.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/util.h"

// 若已经在监听集合里，则修改epoll event的操作类型
#define ADD_TO_EPOLL() \
    auto it = m_listen_fds.find(event->getFd()); \
    int op = EPOLL_CTL_ADD; \
    if(it != m_listen_fds.end()){ \
        op = EPOLL_CTL_MOD;   \
    } \
    epoll_event tmp = event->getEpollEvent(); \
    int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), &tmp); \
    if(rt == -1){ \
        ERRORLOG("failed epoll_ctl when add fd, errno=%d, error=%s", errno, strerror(errno)); \
    } \
    DEBUGLOG("add event success, fd[%d]", event->getFd()); \

#define DELETE_FROM_EPOLL() \
    auto it = m_listen_fds.find(event->getFd()); \
    if(it == m_listen_fds.end()){ \
        return; \
    } \
    int op = EPOLL_CTL_DEL; \
    epoll_event tmp = event->getEpollEvent(); \
    int rt = epoll_ctl(m_epoll_fd, op, event->getFd(), &tmp); \
    if(rt == -1){ \
        ERRORLOG("failed epoll_ctl when delete fd, errno=%d, error=%s", errno, strerror(errno)); \
    } \
    DEBUGLOG("delete event success, fd[%d]", event->getFd()); \


namespace tinyrpc{


static thread_local EventLoop* t_current_eventloop = nullptr;
static int g_epoll_max_timeout = 10000;
static int g_epoll_max_events = 10;  // 最大监听数量

EventLoop::EventLoop(){
    if(t_current_eventloop != nullptr){
        ERRORLOG("failed to event loop, tgis thread has created event loop.");
        exit(0);
    }

    // 创建 epoll fd
    m_pid = getThreadId();
    m_epoll_fd = epoll_create(10);
    if(m_epoll_fd == -1){
        ERRORLOG("failed to creat event loop, epoll_create error, error info :%s", strerror(errno));
        exit(0);
    }

    // 创建并初始化 wakeup fd && wakeup fd event
    // 将 wakeup fd 添加到 epoll fd 中
    initWakeupFdEvent();
    initTimer();
    
    INFOLOG("succ create event loop in thread %d", m_pid);
    t_current_eventloop = this;
}

void EventLoop::initWakeupFdEvent(){
    m_wakeup_fd = eventfd(0, EFD_NONBLOCK);
    if(m_wakeup_fd < 0){
        ERRORLOG("failed to creat event loop, eventfd error, error info :%s", strerror(errno));
        exit(0);
    }

    m_wakeup_fd_event = new WakeupFdEvent(m_wakeup_fd);
    m_wakeup_fd_event->listen(FdEvent::IN_EVENT, [this](){
        char buf[8];
        while(read(m_wakeup_fd, buf, 8) != -1 && errno != EAGAIN){
        }
        DEBUGLOG("read full bytes from wakeup fd[%d].", m_wakeup_fd);

    });
    addEpollEvent(m_wakeup_fd_event);
    
}

void EventLoop::initTimer(){
    m_timer = new Timer();

    DEBUGLOG("m_timer fd [%d]", m_timer->getFd());

    addEpollEvent(m_timer);

    DEBUGLOG("success init Timer.");
}

void EventLoop::loop(){
    while(!m_is_stop){
        ScoprMutex<Mutex> lock(m_mutex);
        std::queue<std::function<void()>> tmp_tasks;
        m_pending_tasks.swap(tmp_tasks);
        lock.unlock();

        while(!tmp_tasks.empty()){
            std::function<void()> call_back = tmp_tasks.front();  // 执行任务
            tmp_tasks.pop();
            if(call_back != nullptr){
                call_back();
            }
        }
        
        int timeout  = g_epoll_max_timeout;
        epoll_event result_events[g_epoll_max_events];
        int rt = epoll_wait(m_epoll_fd, result_events, g_epoll_max_events, timeout);

        if(rt < 0){
            ERRORLOG("epoll_wait error, errno=%s", strerror(errno));
        }
        else{
            for(int i = 0; i < rt; i++){
                epoll_event trigger_event = result_events[i];

                // 构建句柄事件
                FdEvent* fd_event = static_cast<FdEvent*>(trigger_event.data.ptr);
                if(fd_event == nullptr){
                    continue;
                }

                if(trigger_event.events & EPOLLIN){
                    DEBUGLOG("fd %d trigger in event", fd_event->getFd());
                    addTask(fd_event->handler(FdEvent::IN_EVENT));
                }

                if(trigger_event.events & EPOLLOUT){
                    DEBUGLOG("fd %d trigger in event", fd_event->getFd());
                    addTask(fd_event->handler(FdEvent::OUT_EVENT));
                }

            }
        }
    }
}

void EventLoop::wakeup(){
    m_wakeup_fd_event->wakeup();
}

void EventLoop::stop(){


}

EventLoop::~EventLoop(){
    close(m_epoll_fd);
    if(m_wakeup_fd_event){
        delete m_wakeup_fd_event;
        m_wakeup_fd_event = nullptr;
    }

    if(m_timer){
        delete m_timer;
        m_timer = nullptr;
    }
}

void EventLoop::addEpollEvent(FdEvent* event){
    /*
        只有当前线程自己添加事件，如果不在当前线程（主从线程调用），
        那么将添加封装为回调函数，添加到当前线程的任务队列里，
        等待自己的epoll_wait触发时在完成
    */
    if(isInLoopThread()){
        ADD_TO_EPOLL();
    }
    else{
        auto cb = [this, event] (){
            ADD_TO_EPOLL();
        };
        addTask(cb, true);
    }
}

void EventLoop::addTimerEvent(TimerEvent::s_ptr event){
    m_timer->addTimerEvent(event);
}

    
void EventLoop::deleteEpollEvent(FdEvent* event){
    /*
        只有当前线程自己删除事件，如果不在当前线程（主从线程调用），
        那么将添加封装为回调函数，添加到当前线程的任务队列里，
        等待自己的epoll_wait触发时在完成
    */
    if(isInLoopThread()){
        DELETE_FROM_EPOLL();
    }
    else{
        auto cb = [this, event](){
            DELETE_FROM_EPOLL();
        };
        addTask(cb, true);
    }
}


void EventLoop::dealwakeup(){

}

void EventLoop::addTask(std::function<void()> cb, bool is_wakeup /*=false*/){
    ScoprMutex<Mutex> lock(m_mutex);
    m_pending_tasks.push(cb);
    lock.unlock();

    if(is_wakeup){
        wakeup();
    }
}

bool EventLoop::isInLoopThread(){
    return getThreadId() == m_pid;
}

}