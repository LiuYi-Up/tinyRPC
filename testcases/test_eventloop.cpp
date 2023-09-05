#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"
#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/net/eventloop.h"
#include "tinyrpc/net/timer.h"
#include "tinyrpc/net/io_thread.h"
#include "tinyrpc/net/io_thread_group.h"


void test_eventloop(){
    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();

    tinyrpc::EventLoop* eventloop = new tinyrpc::EventLoop();


    // 服务端创建监听链接的套接字
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        ERRORLOG("failed to create listen fd");
        exit(0);
    }

    // 设置监听地址与端口
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_port = htons(12345);
    addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &addr.sin_addr);

    // bind监听端口
    int rt = bind(listenfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if(rt != 0){
        ERRORLOG("bind error.");
        exit(1);
    }

    // listen, args: sockfd, backlog
    rt = listen(listenfd, 100);
    if(rt != 0){
        ERRORLOG("failed to listen.");
        exit(1);
    }

    tinyrpc::FdEvent event(listenfd);
    event.listen(tinyrpc::FdEvent::IN_EVENT, [listenfd](){
        sockaddr_in peer_addr;
        socklen_t addr_len = 0;
        memset(&peer_addr, 0, sizeof(peer_addr));

        int clientfd = accept(listenfd, reinterpret_cast<sockaddr*>(&peer_addr), &addr_len);
        inet_ntoa(peer_addr.sin_addr);
        DEBUGLOG("success get client fd[%d]: [%s:%d]", clientfd, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

    });

    eventloop->addEpollEvent(&event);
    int i = 0;
    tinyrpc::TimerEvent::s_ptr timer_event = std::make_shared<tinyrpc::TimerEvent>(
        1000, true, [&i](){
            INFOLOG("trigger timer event, count=%d", i++);
        }
    );

    eventloop->addTimerEvent(timer_event);

    eventloop->loop();

}


void test_io_thread(){
    

    // 服务端创建监听链接的套接字
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        ERRORLOG("failed to create listen fd");
        exit(0);
    }

    // 设置监听地址与端口
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_port = htons(12345);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    inet_aton("127.0.0.1", &addr.sin_addr);

    // bind监听端口
    int rt = bind(listenfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if(rt != 0){
        ERRORLOG("bind error.");
        exit(1);
    }

    // listen, args: sockfd, backlog
    rt = listen(listenfd, 100);
    if(rt != 0){
        ERRORLOG("failed to listen.");
        exit(1);
    }

    tinyrpc::FdEvent event(listenfd);
    event.listen(tinyrpc::FdEvent::IN_EVENT, [listenfd](){
        sockaddr_in peer_addr;
        socklen_t addr_len = sizeof(peer_addr);
        memset(&peer_addr, 0, sizeof(peer_addr));

        // inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

        int clientfd = accept(listenfd, reinterpret_cast<sockaddr*>(&peer_addr), &addr_len);
        DEBUGLOG("success get client fd[%d]: [%s:%d]", clientfd, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

    });

    int i = 0;
    tinyrpc::TimerEvent::s_ptr timer_event = std::make_shared<tinyrpc::TimerEvent>(
        3000, true, [&i](){
            INFOLOG("trigger timer event, count=%d", i++);
        }
    );

    tinyrpc::IOThread io_thread;
    io_thread.getEventLoop()->addEpollEvent(&event);
    io_thread.getEventLoop()->addTimerEvent(timer_event);

    io_thread.start();
    io_thread.join();
    
}

void test_io_thread_group(){
    

    // 服务端创建监听链接的套接字
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        ERRORLOG("failed to create listen fd");
        exit(0);
    }

    // 设置监听地址与端口
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_port = htons(12345);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    inet_aton("127.0.0.1", &addr.sin_addr);

    // bind监听端口
    int rt = bind(listenfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if(rt != 0){
        ERRORLOG("bind error.");
        exit(1);
    }

    // listen, args: sockfd, backlog
    rt = listen(listenfd, 100);
    if(rt != 0){
        ERRORLOG("failed to listen.");
        exit(1);
    }

    tinyrpc::FdEvent event(listenfd);
    event.listen(tinyrpc::FdEvent::IN_EVENT, [listenfd](){
        sockaddr_in peer_addr;
        socklen_t addr_len = sizeof(peer_addr);
        memset(&peer_addr, 0, sizeof(peer_addr));

        // inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

        int clientfd = accept(listenfd, reinterpret_cast<sockaddr*>(&peer_addr), &addr_len);
        DEBUGLOG("success get client fd[%d]: [%s:%d]", clientfd, inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

    });

    int i = 0;
    tinyrpc::TimerEvent::s_ptr timer_event = std::make_shared<tinyrpc::TimerEvent>(
        3000, true, [&i](){
            INFOLOG("trigger timer event, count=%d", i++);
        }
    );

    tinyrpc::IOThreadGroup io_group(2);
    tinyrpc::IOThread* io_thread1 = io_group.getIOThread();
    io_thread1->getEventLoop()->addEpollEvent(&event);
    io_thread1->getEventLoop()->addTimerEvent(timer_event);

    tinyrpc::IOThread* io_thread2 = io_group.getIOThread();
    io_thread2->getEventLoop()->addTimerEvent(timer_event);

    io_group.start();
    io_group.join();
    
}



int main(){

    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();

    test_io_thread_group();

    return 0;
}