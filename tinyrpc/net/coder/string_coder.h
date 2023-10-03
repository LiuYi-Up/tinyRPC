#ifndef TINYRPC_NET_CODER_STRING_CODER_H
#define TINYRPC_NET_CODER_STRING_CODER_H

#include "tinyrpc/net/coder/abstract_coder.h"
#include "tinyrpc/net/coder/abstract_protocol.h"

namespace tinyrpc{

struct StringProtocol: public AbstractProtocol{
public:
    std::string m_info;

};

class StringCoder: public AbstractCoder{
public:
    void encoder(std::vector<AbstractProtocol::s_ptr>& message, TCPBuffer::s_ptr write_buffer){
        for(size_t i = 0; i < message.size(); ++i){
            std::shared_ptr<StringProtocol> msg = std::dynamic_pointer_cast<StringProtocol>(message[i]);
            write_buffer->writeTOBuffer(msg->m_info.c_str(), msg->m_info.length());
        }
        
    }

    void decoder(std::vector<AbstractProtocol::s_ptr>& out_message, TCPBuffer::s_ptr buffer){
        std::vector<char> buf;
        buffer->readFromBuffer(buf, buffer->readAble());

        std::string info;
        for(size_t i = 0; i < buf.size(); ++i){
            info += buf[i];
        }

        std::shared_ptr<StringProtocol> msg = std::make_shared<StringProtocol>();
        msg->m_msg_id = "12345";
        msg->m_info = info;
        out_message.push_back(msg);
        
    }

};


}

#endif