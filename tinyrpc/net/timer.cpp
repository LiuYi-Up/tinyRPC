#include <sys/timerfd.h>
#include <string.h>
#include <vector>
#include "tinyrpc/net/timer.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/util.h"

namespace tinyrpc{

Timer::Timer()
: FdEvent() {
    //  CLOCK_MONOTONIC：系统启动后，单调递增时钟
    m_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(m_fd == -1){
        ERRORLOG("failed to create timerfd fd[%d], error=%s", m_fd, strerror(errno));
    }

    DEBUGLOG("success create timer fd[%d]", m_fd);

    // FdEvent 的监听函数，设置 Timer 的监听类型和回调函数
    listen(FdEvent::IN_EVENT, std::bind(&Timer::onTimer, this));
}


Timer::~Timer(){

}


void Timer::addTimerEvent(TimerEvent::s_ptr event){
    bool is_reset_timerfd = false;

    ScoprMutex<Mutex> lk(m_mutex);
    if(m_pending_tasks.empty()){
        // 如果定时任务队列为空，重置 Timer
        is_reset_timerfd = true;
    }
    else{
        // 先判断当前要添加的事件的触发时间是否在Timer之前，
        // 如果是，说明添加进去将会过期，需要重新设置 Timer I/O出发时间
        auto it = m_pending_tasks.begin();
        if(event->getArriveTime() < (*it).second->getArriveTime()){
            is_reset_timerfd = true;
        }
    }
    
    m_pending_tasks.emplace(event->getArriveTime(), event);
    lk.unlock();

    if(is_reset_timerfd){
        resetArriveTime();
    }
}


void Timer::deleteTimerEvent(TimerEvent::s_ptr event){
    event->setCancled(true);

    ScoprMutex<Mutex> lk(m_mutex);

    auto begin = m_pending_tasks.lower_bound(event->getArriveTime());
    auto end = m_pending_tasks.upper_bound(event->getArriveTime());

    auto it = begin;
    for(it = begin; it != end; ++it){
        if(it->second == event){
            break;
        }
    }

    if(it != end){
        m_pending_tasks.erase(it);
    }
    lk.unlock();
    DEBUGLOG("success delete timerevent fron timer.");
}


void Timer::onTimer(){
    
    // 处理缓冲区数据，放置下一次触发事件
    char buf[8];
    while(1){
        if(read(m_fd, buf, 8) == -1 && errno == EAGAIN){
            break;
        }
    }

    // 执行定时任务
    int64_t now = getNowMs();

    std::vector<TimerEvent::s_ptr> event_tmps;
    std::vector<std::function<void()>> tasks_tmps;

    ScoprMutex<Mutex> lk(m_mutex);
    auto it = m_pending_tasks.begin();

    for(it = m_pending_tasks.begin(); it != m_pending_tasks.end(); ++it){
        if(it->first <= now){
            event_tmps.push_back(it->second);
            tasks_tmps.push_back(it->second->handle());
        }
        else{
            break;
        }
    }

    // it 之前的都是需要执行的任务，把他们都删掉
    m_pending_tasks.erase(m_pending_tasks.begin(), it);
    lk.unlock();


    // 将重复的任务再次添加进去
    for(TimerEvent::s_ptr e : event_tmps){
        if(e->isRepeated()){
            e->resetArriveTime();
            addTimerEvent(e);
        }
    }

    // 重置 Timer 触发事件
    resetArriveTime();

    // 执行任务
    for(auto task : tasks_tmps){
        if(task){
            task();
        }
    }

}

void Timer::resetArriveTime(){
    ScoprMutex<Mutex> lk(m_mutex);
    auto tmp = m_pending_tasks;
    lk.unlock();

    if(tmp.empty()){
        return;
    }

    int64_t now = getNowMs();

    auto it = tmp.begin();
    int64_t interval = 0;
    if((*it).second->getArriveTime() > now){
        interval = (*it).second->getArriveTime() - now;
    }
    else{
        interval = 100;
    }

    timespec ts;
    memset(&ts, 0, sizeof(ts));
    ts.tv_sec = interval / 1000;
    ts.tv_nsec = (interval % 1000) * 1000000;

    itimerspec value;
    memset(&value, 0, sizeof(value));
    value.it_value = ts;

    int rt = timerfd_settime(m_fd, 0, &value, NULL);
    if(rt != 0){
        ERRORLOG("failed to set timerfd_arrivetime, errno=%d, error=%s", errno, strerror(errno));
    }
    // DEBUGLOG("timer reset to %lld", now + interval);
}


}