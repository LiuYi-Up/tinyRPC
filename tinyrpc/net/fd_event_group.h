#ifndef TINYRPC_NET_FD_EVENT_GROUP_H
#define TINYRPC_NET_FD_EVENT_GROUP_H

#include <vector>

#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/common/mutex.h"

namespace tinyrpc{

class FdEventGroup{
public:
    FdEventGroup(int size);
    ~FdEventGroup();


    FdEvent* getFdEvent(int fd);

public:
    static FdEventGroup* GetFdEventGroup();

private:
    int m_size;
    std::vector<FdEvent*> m_fd_event_group;

    Mutex m_mutex;


};


}

#endif