#include "tinyrpc/net/fd_event_group.h"



namespace tinyrpc{
static FdEventGroup* g_fd_event_group = nullptr;

FdEventGroup* FdEventGroup::GetFdEventGroup(){
    if(g_fd_event_group){
        return g_fd_event_group;
    }

    g_fd_event_group = new FdEventGroup(128);
    return g_fd_event_group;
}

FdEventGroup::FdEventGroup(int size)
:m_size(size){
    for(int i = 0; i < size; ++i){
        m_fd_event_group.push_back(new FdEvent(i));
    }
}


FdEventGroup::~FdEventGroup(){
    for(size_t i = 0; i < m_fd_event_group.size(); ++i){
        if(m_fd_event_group[i]){
            delete m_fd_event_group[i];
            m_fd_event_group[i] = nullptr;
        }
    }
}


FdEvent* FdEventGroup::getFdEvent(int fd){
    ScoprMutex<Mutex> lk(m_mutex);

    if((size_t) fd < m_fd_event_group.size()){
        return m_fd_event_group[fd];
    }

    int new_size = m_fd_event_group.size() * 1.5;
    for(int i = m_fd_event_group.size(); i < new_size; ++i){
        m_fd_event_group.push_back(new FdEvent(i));
    }

    return m_fd_event_group[fd];
}




}