#include <sys/syscall.h>
#include <sys/time.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>

#include "tinyrpc/common/util.h"


namespace tinyrpc{

static int g_pid = 0;
static thread_local int t_thread_id = 0;

pid_t getPid(){
    if(g_pid != 0) return g_pid;
    return getpid();
}

pid_t getThreadId(){
    if(t_thread_id != 0) return t_thread_id;
    return syscall(SYS_gettid);
}

int64_t getNowMs(){
    timeval val;
    gettimeofday(&val, NULL);

    return val.tv_sec * 1000 + val.tv_sec / 1000;
}

int getInt32FromNetByte(const char* buf){
    int re;
    memcpy(&re, buf, sizeof(re));
    return ntohl(re);
}

}