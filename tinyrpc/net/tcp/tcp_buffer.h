#ifndef TINYRPC_NET_TCP_TCP_BUFFER_H
#define TINYRPC_NET_TCP_TCP_BUFFER_H

#include <vector>
#include "tinyrpc/common/log.h"

namespace tinyrpc{

class TCPBuffer{
public:

    typedef std::shared_ptr<TCPBuffer> s_ptr;
    
    TCPBuffer(int size);
    ~TCPBuffer();


    int readAble();

    int writeAble();

    int writeIndex();

    int readIndex();

    void writeTOBuffer(const char* buf, int size);

    void readFromBuffer(std::vector<char>& re, int size);

    void adjustBuffer();

    void resizeBuffer(int size);

    void moveReadIndex(int size);

    void moveWriteIndex(int size);


private:
    int m_read_index {0};
    int m_write_index {0};
    int m_size {0};

public:

    std::vector<char> m_buffer;

};


}

#endif