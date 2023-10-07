#include <tinyxml/tinyxml.h>

#include "tinyrpc/common/config.h"

namespace tinyrpc{

#define READ_XML_NODE(name, parent) \
TiXmlElement* name##_node = parent->FirstChildElement(#name); \
if(!name##_node){ \
    printf("Start tinyrpc server error, failed to read node [%s].\n", #name); \
    exit(0); \
} \

#define READ_STR_FROM_XML_NODE(name, parent) \
TiXmlElement* name##_node = parent->FirstChildElement(#name); \
if(!name##_node || !name##_node->GetText()){ \
    printf("Start tinyrpc server error, failed to read node : %s\n", #name); \
    exit(0); \
} \
std::string name##_str = std::string(name##_node->GetText()); \

static Config* g_config = nullptr;

Config* Config::GetGlobalConfig(){
    return g_config;
}
    
void Config::SetGlobalConfig(const char* xmlfile){
    if(g_config == nullptr){
        if(xmlfile){
            g_config = new Config(xmlfile);
        }
    }
    else{
        g_config = new Config();
    }
}

Config::Config(){
    m_log_level = "DEBUG";
}

Config::Config(const char* xmlfile){
    TiXmlDocument* xml_ducument = new TiXmlDocument();
    bool rt = xml_ducument->LoadFile(xmlfile);

    if(!rt){
        printf("Start tinyrpc server error, failed to read config file: %s\n", xmlfile);
        exit(0);
    }

    READ_XML_NODE(root, xml_ducument);
    READ_XML_NODE(log, root_node);

    READ_STR_FROM_XML_NODE(log_level, log_node);
    READ_STR_FROM_XML_NODE(log_file_name, log_node);
    READ_STR_FROM_XML_NODE(log_file_path, log_node);
    READ_STR_FROM_XML_NODE(log_max_file_size, log_node);
    READ_STR_FROM_XML_NODE(log_sync_inteval, log_node);

    m_log_level = log_level_str;

    m_log_file_name = log_file_name_str;
    m_log_file_path = log_file_path_str;
    m_log_max_file_size = std::atoi(log_max_file_size_str.c_str());
    m_log_sync_inteval = std::atoi(log_sync_inteval_str.c_str());  // 日志时间间隔， ms
    
    printf("LOG -- CONFIG LEVEL[%s], FILE_NAME[%s],FILE_PATH[%s] MAX_FILE_SIZE[%d B], SYNC_INTEVAL[%d ms]\n", 
    m_log_level.c_str(), m_log_file_name.c_str(), m_log_file_path.c_str(), m_log_max_file_size, m_log_sync_inteval);

}

Config::~Config(){

}

}