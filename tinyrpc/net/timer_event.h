#ifndef TINYRPC_NET_TIMEREVENT_H
#define TINYRPC_NET_TIMEREVENT_H

#include <functional>
#include <memory>

namespace tinyrpc{

class TimerEvent{
public:
    typedef std::shared_ptr<TimerEvent> s_ptr;

    TimerEvent(int interval, bool repeated, std::function<void()> task);

    int64_t getArriveTime(){
        return m_arrive_time;
    }

    void resetArriveTime();

    bool isRepeated(){
        return m_is_repeated;
    }

    bool isCancled(){
        return m_is_cancled;
    }

    std::function<void()> handle(){
        return m_task;
    }

    void setCancled(bool cancled){
        m_is_cancled = cancled;
    }

private:
    int64_t m_arrive_time;  // ms
    int64_t m_interval;  // ms

    bool m_is_repeated {false};
    bool m_is_cancled {false};

    std::function<void()> m_task;

};

}

#endif