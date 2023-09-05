#ifndef TINYRPC_NET_IOTHREAD_GROUP_H
#define TINYRPC_NET_IOTHREAD_GROUP_H

#include <vector>
#include "tinyrpc/common/log.h"
#include "tinyrpc/net/io_thread.h"

namespace tinyrpc{

class IOThreadGroup{
public:
    IOThreadGroup(int size);
    ~IOThreadGroup();


    void start();

    void join();

    IOThread* getIOThread();

private:
    int m_size {0};
    std::vector<IOThread*> m_io_thread_groups;

    int m_index {-1};
};


}

#endif