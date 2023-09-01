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

    m_log_level = log_level_str;
    
}

Config::~Config(){

}

}