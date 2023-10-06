#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <google/protobuf/service.h>

#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"
#include "tinyrpc/net/tcp/tcp_server.h"
#include "tinyrpc/net/tcp/tcp_client.h"
#include "tinyrpc/net/coder/string_coder.h"
#include "tinyrpc/net/coder/abstract_protocol.h"
#include "tinyrpc/net/coder/tinypb_coder.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"
#include "tinyrpc/net/rpc/rpc_dispatcher.h"
#include "tinyrpc/net/rpc/rpc_controller.h"
#include "tinyrpc/net/rpc/rpc_closure.h"
#include "tinyrpc/net/rpc/rpc_channel.h"

#include "order.pb.h"

void test_client(){
    tinyrpc::IPNetAddr::s_ptr addr = std::make_shared<tinyrpc::IPNetAddr>("127.0.0.1", 12344);
    tinyrpc::TcpClient client(addr);
    client.connection([addr, &client](){
        DEBUGLOG("test success connect [%s]", addr->toString().c_str());
        std::shared_ptr<tinyrpc::TinyPBProtocol> message = std::make_shared<tinyrpc::TinyPBProtocol>();
        message->m_msg_id = "666666";

        makeOrderRequest request;
        request.set_price(100);
        request.set_good("apple");

        if(!request.SerializeToString(&(message->m_pb_data))){
            ERRORLOG("serialize failed.");
            return;
        }
        message->m_method_name = "Order.makeOrder";

        client.writeMessage(message, [request](tinyrpc::AbstractProtocol::s_ptr msg){
            DEBUGLOG("success send message, request[%s].", request.ShortDebugString().c_str());
        });

        client.readMessage("666666", [](tinyrpc::AbstractProtocol::s_ptr msg_ptr){
            std::shared_ptr<tinyrpc::TinyPBProtocol> msg = std::dynamic_pointer_cast<tinyrpc::TinyPBProtocol>(msg_ptr);
            DEBUGLOG("success get respose msg_id[%s]", msg->m_msg_id.c_str());
            makeOrderResponse response;
            if(!response.ParseFromString(msg->m_pb_data)){
                ERRORLOG("deserialize failed.");
                return;
            }
            DEBUGLOG("get response success, response[%s]", response.ShortDebugString().c_str());

        });

        // client.writeMessage(message, [](tinyrpc::AbstractProtocol::s_ptr msg){
        //     DEBUGLOG("success send222 req.");
        // });

        // client.readMessage("666666", [](tinyrpc::AbstractProtocol::s_ptr msg_ptr){
        //     std::shared_ptr<tinyrpc::TinyPBProtocol> msg = std::dynamic_pointer_cast<tinyrpc::TinyPBProtocol>(msg_ptr);
        //     DEBUGLOG("success get respose msg_id[%s], response message[%s]", msg->m_msg_id.c_str(), msg->m_pb_data.c_str());
        // });
    });
}

void test_rpc_channel(){
    tinyrpc::IPNetAddr::s_ptr addr = std::make_shared<tinyrpc::IPNetAddr>("127.0.0.1", 12344);
    std::shared_ptr<tinyrpc::RpcChannel> channel = std::make_shared<tinyrpc::RpcChannel>(addr);

    std::shared_ptr<makeOrderRequest> request = std::make_shared<makeOrderRequest>();
    request->set_price(100);
    request->set_good("apple");
    
    std::shared_ptr<makeOrderResponse> response = std::make_shared<makeOrderResponse>();

    std::shared_ptr<tinyrpc::RpcController> controller = std::make_shared<tinyrpc::RpcController>();
    controller->SetMsgId("99998888");

    std::shared_ptr<tinyrpc::RpcClosure> closure = std::make_shared<tinyrpc::RpcClosure>([request, response, channel]() mutable{
        INFOLOG("call rpc success, request[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
        channel->getClient()->stop();
        channel.reset();
    });

    channel->Init(controller, request, response, closure);

    Order_Stub stub(channel.get());

    stub.makeOrder(controller.get(), request.get(), response.get(), closure.get());

}


int main(){
    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();

    test_rpc_channel();

    DEBUGLOG("end rpc client");
    return 0;
}