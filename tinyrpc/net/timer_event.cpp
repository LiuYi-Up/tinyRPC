#include "tinyrpc/net/timer_event.h"
#include "tinyrpc/common/util.h"
#include "tinyrpc/common/log.h"



namespace tinyrpc{

TimerEvent::TimerEvent (int interval, bool repeated, std::function<void()> callback)
:m_interval(interval), m_is_repeated(repeated), m_task(callback){
    resetArriveTime();
    DEBUGLOG("success to create Timer Event.");    
}

void TimerEvent::resetArriveTime(){
    m_arrive_time = getNowMs() + m_interval;
}

}