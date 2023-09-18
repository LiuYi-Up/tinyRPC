#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"
#include "tinyrpc/net/tcp/tcp_server.h"
#include "tinyrpc/net/tcp/tcp_client.h"
#include "tinyrpc/net/string_coder.h"
#include "tinyrpc/net/abstract_protocol.h"

void test(){

    // 调用 connect 连接 server
    // write 一个字符串
    // 等待 read 返回结果

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        ERRORLOG("ivalid fd %d", fd);
        exit(0);
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12344);
    inet_aton("127.0.0.1", &addr.sin_addr);

    int rt = connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    std::string msg = "hello server!";

    rt = write(fd, msg.c_str(), msg.length());

    DEBUGLOG("success write %d bytes, [%s]", rt, msg.c_str());

    char buf[100];
    int read_len = read(fd, buf, 100);
    DEBUGLOG("success read %d bytes, [%s]", read_len, std::string(buf, read_len).c_str());

}

void test_client(){
    tinyrpc::IPNetAddr::s_ptr addr = std::make_shared<tinyrpc::IPNetAddr>("127.0.0.1", 12344);
    tinyrpc::TcpClient client(addr);
    client.connection([addr, &client](){
        DEBUGLOG("test success connect [%s]", addr->toString().c_str());
        std::shared_ptr<tinyrpc::StringProtocol> message = std::make_shared<tinyrpc::StringProtocol>();
        message->setReqId("12345");
        message->m_info = "hello service!";
        client.writeMessage(message, [](tinyrpc::AbstractProtocol::s_ptr msg){
            DEBUGLOG("success send req.");
        });

        client.readMessage("12345", [](tinyrpc::AbstractProtocol::s_ptr msg_ptr){
            std::shared_ptr<tinyrpc::StringProtocol> msg = std::dynamic_pointer_cast<tinyrpc::StringProtocol>(msg_ptr);
            DEBUGLOG("success get respose req_id[%s], response message[%s]", msg->getReqId().c_str(), msg->m_info.c_str());
        });

        client.writeMessage(message, [](tinyrpc::AbstractProtocol::s_ptr msg){
            DEBUGLOG("success send222 req.");
        });

        client.readMessage("12345", [](tinyrpc::AbstractProtocol::s_ptr msg_ptr){
            std::shared_ptr<tinyrpc::StringProtocol> msg = std::dynamic_pointer_cast<tinyrpc::StringProtocol>(msg_ptr);
            DEBUGLOG("success get respose req_id[%s], response message[%s]", msg->getReqId().c_str(), msg->m_info.c_str());
        });
    });
}

int main(){
    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();

    test_client();
    return 0;
}