#include <unistd.h>
#include "tinyrpc/net/tcp/tcp_connection.h"
#include "tinyrpc/net/fd_event_group.h"
#include "tinyrpc/net/coder/string_coder.h"
#include "tinyrpc/net/coder/tinypb_coder.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"

namespace tinyrpc{


TcpConnection::TcpConnection(EventLoop* event_loop, int fd, int buffer_size, NetAddr::s_ptr peer_addr, TcpConnectionType type /*=TcpConnectionByServer*/)
:m_event_loop(event_loop), m_fd(fd), m_buffer_size(buffer_size) , m_peer_addr(peer_addr), m_state(NotConnected), m_connection_type(type){
    m_in_buffer = std::make_shared<TCPBuffer>(m_buffer_size);
    m_out_buffer = std::make_shared<TCPBuffer>(m_buffer_size);

    m_fd_event = FdEventGroup::GetFdEventGroup()->getFdEvent(m_fd);
    m_fd_event->setNonblock();
    
    if(m_connection_type == TcpConnectionByServer){
        listenRead();
    }
    DEBUGLOG("tcp connection creator fd [%d]", m_fd_event->getFd());
    
    m_coder = new TinyPBCoder();
}



TcpConnection::~TcpConnection(){
    DEBUGLOG("~TcpConnection.");
}

void TcpConnection::onRead(){
    // 1.从 socket 缓冲区，调用系统调用 read 函数读取字节到 in buffer 里
    if(m_state != Connected){
        ERRORLOG("read error, client has already disconnected, addr[%s], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd);
        return;
    }

    bool is_read_all = false;
    bool is_close = false;
    while(!is_read_all){
        if(m_in_buffer->writeAble() == 0){
            m_in_buffer->resizeBuffer(2 * m_in_buffer->m_buffer.size());
        }
        int read_count = m_in_buffer->writeAble();
        int write_index = m_in_buffer->writeIndex();
    
        int rt = read(m_fd, &(m_in_buffer->m_buffer[write_index]), read_count);
        DEBUGLOG("success read %d bytes from addr[%s], client fd[%d]", rt, m_peer_addr->toString().c_str(), m_fd);

        if(rt > 0){
            m_in_buffer->moveWriteIndex(rt);
            // 说明有可能还有数据没读完
            if(rt == read_count){
                continue;
            }
            else if(rt < read_count){
                // 已经读完了
                is_read_all = true;
                break;
            }
        }
        else if(rt == 0){
            // 客户端关闭连接，触发可读事件
            is_close = true;
            break;
        }
        else if(rt == -1 && errno == EAGAIN){
            // 表示已经读完数据
            is_read_all = true;
            break;
        }
    }
    if(is_close){
        // TODO: 处理关闭连接
        clear();
        INFOLOG("peer closed, peer addr [%s], client fd [%d]", m_peer_addr->toString().c_str(), m_fd);
        return;
    }
    if(!is_read_all){
        ERRORLOG("not read all data");
    }

    // TODO: 简单的 echo ，后面补充 RPC 协议解析
    excute();

    
}

void TcpConnection::onWrite(){
    /*
    将当前 out buffer 中所有的数据全部返回给 client
    */

    if(m_state != Connected){
        ERRORLOG("read error, client has already disconnected, addr[%s], clientfd[%d]", m_peer_addr->toString().c_str(), m_fd);
        return;
    }

    if(m_connection_type == TcpConnectionByClient){
        // 1.将 message encode 得到字节流
        // 2.将字节流写入 buffer 中，全部发送
        std::vector<AbstractProtocol::s_ptr> messages;
        for(size_t i = 0; i < m_write_dones.size(); ++i){
            messages.push_back(m_write_dones[i].first);
        }
        m_coder->encoder(messages, m_out_buffer);
    }

    bool is_write_all = false;
    while (true){
        if(m_out_buffer->readAble() == 0){
            DEBUGLOG("no data need to send to client [%s]", m_peer_addr->toString().c_str());
            is_write_all = true;
            break;
        }

        int write_count = m_out_buffer->readAble();
        int read_index = m_out_buffer->readIndex();

        int rt = write(m_fd, &m_out_buffer->m_buffer[read_index], write_count);
        if(rt >= write_count){
            DEBUGLOG("no data need to send to client [%s]", m_peer_addr->toString().c_str());
            is_write_all = true;
            break;
        }

        if(rt == -1 && errno == EAGAIN){
            // 发送缓冲区已满
            // 下次 fd 可写时再将其发送
            ERRORLOG("write date error, errno=EAGAIN and rt == -1");
            break;
        }
    }
    if(is_write_all){
        m_fd_event->cancle(FdEvent::OUT_EVENT);
        DEBUGLOG("tcp connection execute fd [%d]", m_fd_event->getFd());
        m_event_loop->addEpollEvent(m_fd_event);
    }
    
    if(m_connection_type == TcpConnectionByClient){
        // 执行回调函数
        for(size_t i = 0; i < m_write_dones.size(); ++i){
            m_write_dones[i].second(m_write_dones[i].first);
        }
        m_write_dones.clear();
    }
}

void TcpConnection::excute(){
    if(m_connection_type == TcpConnectionByServer){
        // 将 rpc 请求执行业务逻辑，获取 rpc 响应， 再把 rpc 响应发送出去
        std::vector<AbstractProtocol::s_ptr> result;
        std::vector<AbstractProtocol::s_ptr> reply_result;

        m_coder->decoder(result, m_in_buffer);
        for(size_t i = 0; i < result.size(); ++i){
            INFOLOG("success request [%s] from client[%s]", result[i]->m_req_id.c_str(), m_peer_addr->toString().c_str());
            std::shared_ptr<TinyPBProtocol> respones = std::make_shared<TinyPBProtocol>();
            respones->m_req_id = result[i]->m_req_id;
            respones->m_pb_data = "hello. this is tinyrpc response.";
            reply_result.emplace_back(respones);
        }

        m_coder->encoder(reply_result, m_out_buffer);

        listenWrite();
        DEBUGLOG("tcp connection execute fd [%d]", m_fd_event->getFd());
    }
    else{
        // 从 buffer 中读取字节流构建 message 对象
        std::vector<AbstractProtocol::s_ptr> results;
        m_coder->decoder(results, m_in_buffer);

        for(size_t i = 0; i < results.size(); ++i){
            auto it = m_read_dones.find(results[i]->m_req_id);
            if(it != m_read_dones.end()){
                it->second(results[i]);
            }
        }
    }
    
}

void TcpConnection::setState(const TcpState& state){
    m_state = state;
}

TcpState TcpConnection::getState(){
    return m_state;
}

void TcpConnection::clear(){
    // 处理一些关闭连接后的清理动作
    if(m_state == Closed){
        return;
    }

    m_fd_event->cancle(FdEvent::IN_EVENT);
    m_fd_event->cancle(FdEvent::OUT_EVENT);
    m_event_loop->deleteEpollEvent(m_fd_event);
    m_state = Closed;

}


void TcpConnection::shutdown(){
    if(m_state == Closed || m_state == NotConnected){
        return;
    }

    m_state = HalfClosing;

    // 调用shutdown关闭读写，意味着服务器不会再对这个 fd 进行独写操作
    // 发送 FIN 包文， 触发第一次挥手
    // 当 fd 发生可读事件，即可读数据为0，对端关闭连接，对端也发出了 FIN 报文

    ::shutdown(m_fd, SHUT_RDWR);
}

void TcpConnection::setTcpConnectionType(TcpConnectionType type){
    m_connection_type = type;
}


void TcpConnection::listenWrite(){
    m_fd_event->listen(FdEvent::OUT_EVENT, std::bind(&TcpConnection::onWrite, this));
    m_event_loop->addEpollEvent(m_fd_event);
}

// 监听可读事件
void TcpConnection::listenRead(){
    m_fd_event->listen(FdEvent::IN_EVENT, std::bind(&TcpConnection::onRead, this));
    m_event_loop->addEpollEvent(m_fd_event);
}

void TcpConnection::pushSendMessage(AbstractProtocol::s_ptr message, std::function<void(AbstractProtocol::s_ptr)> done){
    m_write_dones.push_back({message, done});
}

void TcpConnection::pushReadMessage(const std::string req_id, std::function<void(AbstractProtocol::s_ptr)> done){
    m_read_dones.insert({req_id, done});
}


}