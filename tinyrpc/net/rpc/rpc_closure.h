#ifndef TINYRPC_NET_RPC_RPC_CLOSURE_H
#define TINYRPC_NET_RPC_RPC_CLOSURE_H

#include <functional>

#include "google/protobuf/stubs/callback.h"

namespace tinyrpc{

class RpcClosure: public google::protobuf::Closure{

public:
    void Run(){
        if(m_cb != nullptr){
            m_cb();
        }
    }

private:
    std::function<void()> m_cb {nullptr};
};

}

#endif