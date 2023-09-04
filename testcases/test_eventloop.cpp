#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"
#include "tinyrpc/net/fd_event.h"
#include "tinyrpc/net/eventloop.h"


int main(){

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
    eventloop->loop();

    return 0;
}