#ifndef TINYRPC_COMMON_ERROR_CODE_H
#define TINYRPC_COMMON_ERROR_CODE_H


#ifndef SYS_ERROR_PREFIX
#define SYS_ERROR_PREFIX(xx) 1000##xx
#endif

const int ERROR_PEER_CLOSED = SYS_ERROR_PREFIX(0000);  // 连接时对端关闭
const int ERROR_FAILED_CONNECT = SYS_ERROR_PREFIX(0001);  // 连接失败
const int ERROR_FAILED_GET_REPLY = SYS_ERROR_PREFIX(0002);  // 获取回包失败
const int ERROR_FAILED_DESERIALIZE = SYS_ERROR_PREFIX(0003);  // 反序列化失败
const int ERROR_FAILED_SERIALIZE = SYS_ERROR_PREFIX(0004);  // 序列化失败

const int ERROR_FAILED_ENCODE = SYS_ERROR_PREFIX(0005);  // 编码失败
const int ERROR_FAILED_DECODE = SYS_ERROR_PREFIX(0006);  // 解码失败

const int ERROR_RPC_CALL_TIMEOUT = SYS_ERROR_PREFIX(0007);  // RPC 调用超时

const int ERROR_SERVICE_NOT_FOUND = SYS_ERROR_PREFIX(0008);  // service 不存在
const int ERROR_METHOD_NOT_FOUND = SYS_ERROR_PREFIX(0009);  // method 不存在
const int ERROR_PARSE_SERVICE_NAME = SYS_ERROR_PREFIX(0010);  // 解析 service name 失败
#endif