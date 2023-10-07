#include <string>
#include <unistd.h>
#include "google/protobuf/service.h"

#include "tinyrpc/net/tcp/net_addr.h"
#include "tinyrpc/common/log.h"
#include "tinyrpc/common/config.h"
#include "tinyrpc/net/tcp/tcp_server.h"
#include "tinyrpc/net/rpc/rpc_dispatcher.h"


#include "order.pb.h"

class OrderImpl : public Order{
public:

    void makeOrder(google::protobuf::RpcController* controller,
                       const ::makeOrderRequest* request,
                       ::makeOrderResponse* response,
                       ::google::protobuf::Closure* done){

        APPDEBUGLOG("start sleep 5s");
        sleep(3);
        APPDEBUGLOG("end sleep 5s");

        if(request->price() < 10){
            response->set_res_code(-1);
            response->set_res_info("short balance");
            return;
        }
        response->set_order_id("123321");
        APPDEBUGLOG("call rpc success");

        return;
    }
};


void test_tcp_server(){
    tinyrpc::IPNetAddr::s_ptr addr = std::make_shared<tinyrpc::IPNetAddr>("127.0.0.1", 12344);

    DEBUGLOG("creat addr %s", addr->toString().c_str());

    tinyrpc::TcpServer tcp_server(addr);

    tcp_server.start();

}

int main(){

    tinyrpc::Config::SetGlobalConfig("./conf/tinyrpc.xml");
    tinyrpc::Logger::InitGlobalLogger();
    tinyrpc::RpcDispatcher::GetGlobalRpcDispatcher();
    
    std::shared_ptr<OrderImpl> service = std::make_shared<OrderImpl>();
    tinyrpc::RpcDispatcher::GetGlobalRpcDispatcher()->registerService(service);

    tinyrpc::IPNetAddr::s_ptr addr = std::make_shared<tinyrpc::IPNetAddr>("127.0.0.1", 12344);

    DEBUGLOG("creat addr %s", addr->toString().c_str());

    tinyrpc::TcpServer tcp_server(addr);

    tcp_server.start();

}