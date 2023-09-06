#include "tinyrpc/net/tcp/tcp_server.h"
#include "tinyrpc/common/log.h"


#include <memory>

namespace tinyrpc{


TcpServer::TcpServer(NetAddr::s_ptr addr)
:m_local_addr(addr){

    init();

    DEBUGLOG("success init tcp server.");
}

TcpServer::~TcpServer(){

}

void TcpServer::init(){

    m_acceptor = std::make_shared<TcpAcceptor>(m_local_addr);
    m_main_event_loop = EventLoop::getCurEventLoop();
    m_io_thread_groups = new IOThreadGroup(2);

    m_listen_fd_event = new FdEvent(m_acceptor->getListenFd());
    m_listen_fd_event->listen(FdEvent::IN_EVENT, std::bind(&TcpServer::onAccept, this));

    m_main_event_loop->addEpollEvent(m_listen_fd_event);

    INFOLOG("rocket TcpServer success on [%s]", m_local_addr->toString().c_str());

}

void TcpServer::start(){
    m_io_thread_groups->start();
    m_main_event_loop->loop();
}

void TcpServer::onAccept(){
    int client_fd = m_acceptor->accept();
    m_client_count++;

    DEBUGLOG("TcpServer succ get client fd[%d]", client_fd);
}

}