#ifndef TINYRPC_NET_ABSTRACT_CODER_H
#define TINYRPC_NET_ABSTRACT_CODER_H

#include <vector>

#include "tinyrpc/net/abstract_protocol.h"
#include "tinyrpc/net/tcp/tcp_buffer.h"

namespace tinyrpc{

class AbstractCoder{
public:

    virtual void encoder(std::vector<AbstractProtocol::s_ptr>& message, TCPBuffer::s_ptr write_buffer) = 0;

    virtual void decoder(std::vector<AbstractProtocol::s_ptr>& out_message, TCPBuffer::s_ptr buffer) = 0;

    virtual ~AbstractCoder(){}
};


}

#endif