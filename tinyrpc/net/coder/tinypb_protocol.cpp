#include "tinyrpc/net/coder/tinypb_protocol.h"


namespace tinyrpc{


char TinyPBProtocol::PB_START = 0x02; // why static ？？？
char TinyPBProtocol::PB_END = 0x03;

}