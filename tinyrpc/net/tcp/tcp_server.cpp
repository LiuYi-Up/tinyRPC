#include "tinyrpc/net/io_thread_group.h"
#include "tinyrpc/net/tcp/tcp_connection.h"
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
    if(m_main_event_loop){
        delete m_main_event_loop;
        m_main_event_loop = nullptr;
    }

    if(m_io_thread_groups){
        delete m_io_thread_groups;
        m_io_thread_groups = nullptr;
    }

    if(m_listen_fd_event){
        delete m_listen_fd_event;
        m_listen_fd_event = nullptr;
    }
}

void TcpServer::init(){

    m_acceptor = std::make_shared<TcpAcceptor>(m_local_addr);
    m_main_event_loop = EventLoop::getCurEventLoop();
    m_io_thread_groups = new IOThreadGroup(2);

    m_listen_fd_event = new FdEvent(m_acceptor->getListenFd());
    m_listen_fd_event->listen(FdEvent::IN_EVENT, std::bind(&TcpServer::onAccept, this));

    DEBUGLOG("Tcp server fd [%d].", m_listen_fd_event->getFd());
    m_main_event_loop->addEpollEvent(m_listen_fd_event);

    INFOLOG("rocket TcpServer success on [%s]", m_local_addr->toString().c_str());

}

void TcpServer::start(){
    m_io_thread_groups->start();
    m_main_event_loop->loop();
}

void TcpServer::onAccept(){
    auto re = m_acceptor->accept();
    int client_fd = re.first;
    NetAddr::s_ptr peer_addr = re.second;
    m_client_count++;

    // 把 client fd 添加到任意 IO 线程里
    IOThread* io_thread = m_io_thread_groups->getIOThread();
    TcpConnection::s_ptr connection = std::make_shared<TcpConnection>(io_thread->getEventLoop(), client_fd, 128, m_local_addr, peer_addr);
    connection->setState(Connected);
    m_client.insert(connection);

    INFOLOG("TcpServer succ get client fd[%d]", client_fd);
}

}