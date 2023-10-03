#ifndef TINYRPC_NET_TINYPB_PROTOCOL_H
#define TINYRPC_NET_TINYPB_PROTOCOL_H

#include <string>

#include "tinyrpc/net/coder/abstract_protocol.h"

namespace tinyrpc{


struct TinyPBProtocol : public AbstractProtocol{
public:
    static char PB_START; // why static ？？？
    static char PB_END;

public:
    int32_t m_pk_len {0}; // 整个包长度，包括前后分隔符
    int32_t m_msg_id_len {0}; // 请求号的长度
    // msg_id 继承自父类

    int32_t m_method_name_len {0};  // 请求方法名长度
    std::string m_method_name;  // 请求方法名

    int32_t m_err_code {0};  // 错误码
    int32_t m_err_info_len {0};  // 错误信息长度
    std::string m_err_info;  // 错误信息

    std::string m_pb_data;  // 请求对象参数等信息

    int32_t m_check_num {0};  // 校验码

    bool m_is_parse_success {false};  // 是否解析成功
};

}

#endif