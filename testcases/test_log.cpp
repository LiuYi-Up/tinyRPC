#include <thread>
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"


void func(){
    DEBUGLOG("test debug log %s", "func");
    INFOLOG("test info log %s", "func");
    return;
}

int main(){

    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();
    std::thread t1(func);
    // std::thread t2(func);

    t1.join();
    // t2.join();
    DEBUGLOG("test debug log %s", "main");
    INFOLOG("test info log %s", "main");
    return 0;
}