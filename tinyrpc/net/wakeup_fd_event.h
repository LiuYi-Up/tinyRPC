#ifndef TINYRPC_WAKEUPFD_EVENT_H
#define TINYRPC_WAKEUPFD_EVENT_H

#include "tinyrpc/net/fd_event.h"

namespace tinyrpc{

class WakeupFdEvent : public FdEvent{
public:
    WakeupFdEvent(int fd);

    ~WakeupFdEvent();

    void wakeup();

};


}


#endif