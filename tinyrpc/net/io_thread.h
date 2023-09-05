#ifndef TINYRPRC_NET_IOTHREAD_H
#define TINYRPRC_NET_IOTHREAD_H

#include <pthread.h>
#include <semaphore.h>
#include "tinyrpc/net/eventloop.h"

namespace tinyrpc{


class IOThread{
public:
    IOThread();
    ~IOThread();

    void start();
    void join();

    EventLoop* getEventLoop(){
        return m_event_loop;
    }

public:
    static void* Main(void* arg);

private:
    pid_t m_thread_id {-1};
    pthread_t m_thread;

    EventLoop* m_event_loop;

    sem_t m_init_semaphore;
    sem_t m_start_semaphore;
};


}

#endif