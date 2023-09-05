#include "tinyrpc/net/io_thread_group.h"



namespace tinyrpc{


IOThreadGroup::IOThreadGroup(int size)
:m_size(size){
    for(int i = 0; i < m_size; ++i){
        m_io_thread_groups.push_back(new IOThread());
    }
}



IOThreadGroup::~IOThreadGroup(){


}


void IOThreadGroup::start(){
    for(int i = 0; i < m_size; ++i){
        m_io_thread_groups[i]->start();
    }
}

void IOThreadGroup::join(){
    for(int i = 0; i < m_size; ++i){
        m_io_thread_groups[i]->join();
    }
}


IOThread* IOThreadGroup::getIOThread(){
    if(m_index == (size_t) m_io_thread_groups.size() || m_index == -1){
        m_index = 0;
    }

    return m_io_thread_groups[m_index++];
}


}