#include <pthread.h>
#include <assert.h>

#include "tinyrpc/net/io_thread.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/util.h"


namespace tinyrpc{

IOThread::IOThread(){
    int rt = sem_init(&m_init_semaphore, 0, 0);  // 0表示多线程共享，最后0表示初始化值
    assert(rt == 0);

    rt = sem_init(&m_start_semaphore, 0, 0);  // 0表示多线程共享，最后0表示初始化值
    assert(rt == 0);

    pthread_create(&m_thread, nullptr, &IOThread::Main, this);

    sem_wait(&m_init_semaphore);

    DEBUGLOG("IOThread [%d] creat success.", m_thread_id);
}

IOThread::~IOThread(){
    m_event_loop->stop();
    sem_destroy(&m_init_semaphore);
    sem_destroy(&m_start_semaphore);

    pthread_join(m_thread, NULL);

    if(m_event_loop){
        delete m_event_loop;
        m_event_loop = nullptr;
    }
}

void* IOThread::Main(void* arg){
    IOThread* thread = static_cast<IOThread*> (arg);

    thread->m_event_loop = new EventLoop();
    thread->m_thread_id = getPid();

    // 初始化完成，唤醒等待的线程（父线程）
    sem_post(&thread->m_init_semaphore);

    // 使用信号量，主动唤醒进行loop
    sem_wait(&thread->m_start_semaphore);
    thread->m_event_loop->loop();

    DEBUGLOG("IOThread %d end loop", thread->m_thread_id);
}

void IOThread::start(){
    sem_post(&m_start_semaphore);
}


void IOThread::join(){
    pthread_join(m_thread, NULL);
}

}