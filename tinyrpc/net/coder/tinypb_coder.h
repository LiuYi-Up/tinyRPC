#ifndef TINYRPC_NET_TINYPB_CODER_H
#define TINYRPC_NET_TINYPB_CODER_H

#include "tinyrpc/net/coder/abstract_coder.h"
#include "tinyrpc/net/coder/abstract_protocol.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"

namespace tinyrpc{


class TinyPBCoder : public AbstractCoder{
public:
    TinyPBCoder(){};
    ~TinyPBCoder(){};

    void encoder(std::vector<AbstractProtocol::s_ptr>& message, TCPBuffer::s_ptr write_buffer);

    void decoder(std::vector<AbstractProtocol::s_ptr>& out_message, TCPBuffer::s_ptr buffer);

private:
    const char* encodeTinyPB(std::shared_ptr<TinyPBProtocol> message, int& len);

    void addInt(char*& buf, const int number);

    void addString(char*& buf, const std::string str);

};

}

#endif