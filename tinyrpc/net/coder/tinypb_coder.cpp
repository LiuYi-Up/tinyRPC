#include <string.h>
#include<arpa/inet.h>

#include "tinyrpc/net/coder/tinypb_coder.h"
#include "tinyrpc/net/coder/tinypb_protocol.h"
#include "tinyrpc/common/util.h"

namespace tinyrpc{


void TinyPBCoder::encoder(std::vector<AbstractProtocol::s_ptr>& message, TCPBuffer::s_ptr write_buffer){
    for(auto& i : message){
        std::shared_ptr<TinyPBProtocol> msg = std::dynamic_pointer_cast<TinyPBProtocol>(i);
        int len = 0;
        const char* buf = encodeTinyPB(msg, len);
        if(buf != NULL && len != 0){
            write_buffer->writeTOBuffer(buf, len);
        }

        if(buf){
            free((void*)buf);
            buf = NULL;
        }
    }
}

void TinyPBCoder::decoder(std::vector<AbstractProtocol::s_ptr>& out_message, TCPBuffer::s_ptr buffer){
    // 遍历 buffer ，找到 PB_START，解析出整包的长度。然后得到结束符，判断是否为 PB_END.
    while(1){
        std::vector<char> tmp = buffer->m_buffer;

        int start_idx = buffer->readIndex();
        int end_idx = -1;
        int pk_len = 0;
        bool parse_success = false;
        int i = 0;
        for(i = start_idx; i < buffer->writeIndex(); ++i){
            // 每次 for 只是为了获取起始和结束的索引， parse_success 后一步一步解析
            if(tmp[i] == TinyPBProtocol::PB_START){
                // 接着读后面的四个字节，网络字节序转为主机字节序
                if(i + 1 < buffer->writeIndex()){
                    pk_len = getInt32FromNetByte(&tmp[i + 1]);
                    DEBUGLOG("get pk_len=%d", pk_len);
                }

                // 获取结束符索引
                int j = i + pk_len - 1;
                if(j >= buffer->writeIndex()){
                    // buffer 还没有接收到完整的协议包
                    continue;
                }
                if(tmp[j] == TinyPBProtocol::PB_END){
                    start_idx = i;
                    end_idx = j;
                    parse_success = true;
                    break;
                }
            }
        }

        if(i >= buffer->writeIndex()){
            DEBUGLOG("decode end, read all buffer data.");
            return;
        }

        if(parse_success){
            // 开始解析
            buffer->moveReadIndex(end_idx - start_idx + 1);
            std::shared_ptr<TinyPBProtocol> messgage = std::make_shared<TinyPBProtocol>();
            // 包长
            messgage->m_pk_len = pk_len;

            // 请求号长度
            int msg_id_len_index = start_idx + sizeof(char) + sizeof(messgage->m_pk_len);
            if(msg_id_len_index >= end_idx){
                messgage->m_is_parse_success = false;
                ERRORLOG("parse error, msg_id_len_index[%d] >= end_idx[%d]", msg_id_len_index, end_idx);
                continue;
            }
            messgage->m_msg_id_len = getInt32FromNetByte(&tmp[msg_id_len_index]);
            DEBUGLOG("parse msg_id_len=%d", messgage->m_msg_id_len);

            // 请求号
            int msg_id_index = msg_id_len_index + sizeof(messgage->m_msg_id_len);
            char msg_id[100] = {0};
            memcpy(&msg_id, &tmp[msg_id_index], messgage->m_msg_id_len);
            messgage->m_msg_id = std::string(msg_id);
            DEBUGLOG("parse msg_id=%s", messgage->m_msg_id.c_str());

            // 解析请求方法名的长度
            int method_name_len_index = msg_id_index + messgage->m_msg_id_len;
            if(method_name_len_index >= end_idx){
                messgage->m_is_parse_success = false;
                ERRORLOG("parse error, method_name_len_index[%d] >= end_idx[%d]", method_name_len_index, end_idx);
                continue;
            }
            messgage->m_method_name_len = getInt32FromNetByte(&tmp[method_name_len_index]);

            // 解析请求方法名
            int method_name_index = method_name_len_index + sizeof(messgage->m_method_name_len);
            char method_name[100] = {0};
            memcpy(&method_name, &tmp[method_name_index], messgage->m_method_name_len);
            messgage->m_method_name = std::string(method_name);
            DEBUGLOG("parse method_name=%s", messgage->m_method_name.c_str());

            // 解析错误码
            int err_code_index = method_name_index + messgage->m_method_name_len;
            if(err_code_index >= end_idx){
                messgage->m_is_parse_success = false;
                ERRORLOG("parse error, err_code_index[%d] >= end_idx[%d]", err_code_index, end_idx);
                continue;
            }
            messgage->m_err_code = getInt32FromNetByte(&tmp[err_code_index]);

            // 解析错误信息长度
            int error_info_len_index = err_code_index + sizeof(messgage->m_err_code);
            if(error_info_len_index >= end_idx){
                messgage->m_is_parse_success = false;
                ERRORLOG("parse error, error_info_len_index[%d] >= end_idx[%d]", error_info_len_index, end_idx);
                continue;
            }
            messgage->m_err_info_len = getInt32FromNetByte(&tmp[error_info_len_index]);

            // 解析错误信息
            int error_info_index = error_info_len_index + sizeof(messgage->m_err_info_len);
            char error_info[100] = {0};
            memcpy(&error_info, &tmp[error_info_index], messgage->m_err_info_len);
            messgage->m_err_info = std::string(error_info);
            DEBUGLOG("parse error_info=%s", messgage->m_err_info.c_str());

            // 解析协议内容
            int pb_data_len = messgage->m_pk_len - messgage->m_msg_id_len - messgage->m_err_info_len - messgage->m_method_name_len - 2 - 24;
            int pb_data_index = error_info_index + messgage->m_err_info_len;
            messgage->m_pb_data = std::string(&tmp[pb_data_index], pb_data_len);

            //校验和解析
            messgage->m_is_parse_success = true;

            out_message.push_back(messgage);

        }

    }
}

const char* TinyPBCoder::encodeTinyPB(std::shared_ptr<TinyPBProtocol> message, int& len){
    if(message->m_msg_id.empty()){
        message->m_msg_id = "123456";
    }
    DEBUGLOG("msg_id=%s", message->m_msg_id.c_str());

    int pk_len = 2 + 24 + message->m_msg_id.length() + message->m_method_name.length() + message->m_err_info.length() + message->m_pb_data.length();
    DEBUGLOG("pk_len=%d", pk_len);

    char* buf = reinterpret_cast<char*>(malloc(pk_len));
    char* cur = buf;

    *cur = TinyPBProtocol::PB_START;
    cur++;

    addInt(cur, pk_len);
    
    addInt(cur, message->m_msg_id.length());
    addString(cur, message->m_msg_id);

    addInt(cur, message->m_method_name.length());
    addString(cur, message->m_method_name);

    addInt(cur, message->m_err_code);

    addInt(cur, message->m_err_info.length());
    addString(cur, message->m_err_info);

    addString(cur, message->m_pb_data);

    if(message->m_check_num == 0){
        message->m_check_num = 1;
    }
    addInt(cur, message->m_check_num);

    *cur = TinyPBProtocol::PB_END;

    message->m_pk_len = pk_len;
    message->m_msg_id_len = message->m_msg_id.length();
    message->m_err_info_len = message->m_err_info.length();
    message->m_method_name_len = message->m_method_name.length();

    len = pk_len;

    DEBUGLOG("success encode msg[%s].", message->m_msg_id.c_str());
    return buf;
}

void TinyPBCoder::addInt(char*& buf, const int number){
    int32_t net_num = htonl(number);
    memcpy(buf, &net_num, sizeof(net_num));
    buf += sizeof(net_num);
}

void TinyPBCoder::addString(char*& buf, const std::string str){
    if(!str.empty()){
        memcpy(buf, &str[0], str.length());
        buf += str.length();
    }
}

}