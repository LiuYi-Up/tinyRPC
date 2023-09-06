#include <string.h>
#include "tinyrpc/net/tcp/tcp_buffer.h"


namespace tinyrpc{

TCPBuffer::TCPBuffer(int size)
:m_size(size), m_read_index(0), m_write_index(0){
    m_buffer.resize(size);
}


TCPBuffer::~TCPBuffer(){

}


int TCPBuffer::readAble(){
    return m_write_index - m_read_index;
}

int TCPBuffer::writeAble(){
    return m_size - m_write_index;
}

int TCPBuffer::writeIndex(){
    return m_write_index;
}

int TCPBuffer::readIndex(){
    return m_read_index;
}

void TCPBuffer::writeTOBuffer(const char* buf, int size){
    // 如果可写区域不够，先扩容
    if(writeAble() <= size){
        int new_size = (int) 1.5 * (m_write_index + size);
        resizeBuffer(new_size);
    }

    memcpy(&m_buffer[m_write_index], buf, size);
    m_write_index += size;
}


void TCPBuffer::readFromBuffer(std::vector<char>& re, int size){
    if(readAble() == 0){
        return;
    }
    // 最多只能读取readAble()
    int read_size = std::min(readAble(), size);

    std::vector<char> tmp(read_size);  // 为什么有时候需要新建一块内存，再swap，写的时候不需要？？？
    memcpy(&tmp[0], &m_buffer[m_read_index], read_size); 

    re.swap(tmp);

    m_read_index += read_size;

    adjustBuffer();
}

void TCPBuffer::adjustBuffer(){
    /*
    当都不已读取的空间超过总容量的1/3，则将后续没读取的移到头部
    避免数组扩容内存泄漏
    */
    if(m_read_index < m_size / 3){
        return;
    }

    std::vector<char> tmp(m_size);
    int count = readAble();
    memcpy(&tmp[0], &m_buffer[m_read_index], count);

    m_buffer.swap(tmp);  // 和tmp.swap(m_buffer) 等价吗？

    m_read_index = 0;
    m_write_index = m_read_index + count;

    tmp.clear();
}


void TCPBuffer::resizeBuffer(int size){
    /*
    调整缓冲大小
    */
    std::vector<char> tmp(size);

    int count = std::min(size, readAble());  // 计算还没有读取的长度
    memcpy(&tmp[0], &m_buffer[m_read_index], count);
    m_buffer.swap(tmp);

    m_read_index = 0;
    m_write_index = m_read_index + count;

    m_size = size;
}

void TCPBuffer::moveReadIndex(int size){
    int new_index = m_read_index + size;
    if(new_index > m_size){
        ERRORLOG("move read index error, invalid size %d, old index %d, buffer size %d", size, m_read_index, m_size);
        return;
    }
    m_read_index += size;
    adjustBuffer();
}

void TCPBuffer::moveWriteIndex(int size){
    int new_index = m_write_index + size;
    if(new_index > m_size){
        ERRORLOG("move write index error, invalid size %d, old index %d, buffer size %d", size, m_write_index, m_size);
        return;
    }
    m_write_index += size;
    adjustBuffer();
}

}