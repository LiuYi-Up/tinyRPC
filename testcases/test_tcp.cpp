#include <string>

#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"

int main(){

    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();
    std::string ip = "127.0.0.1:12345";
    tinyrpc::IPNetAddr ipaddr(ip);
    
    DEBUGLOG(ipaddr.toString().c_str());

}