#ifndef TINYRPC_NET_TIMER_H
#define TINYRPC_NET_TIMER_H

#include <map>

#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/net/timer_event.h"
#include "tinyrpc/common/mutex.h"


namespace tinyrpc{

class Timer : public FdEvent{
public:

    Timer();
    ~Timer();

    void addTimerEvent(TimerEvent::s_ptr event);
    void deleteTimerEvent(TimerEvent::s_ptr event);
    void onTimer();

    void resetArriveTime();

private:
    
    Mutex m_mutex;

    std::multimap<int64_t, TimerEvent::s_ptr> m_pending_tasks;  // key有序


}; 


}


#endif