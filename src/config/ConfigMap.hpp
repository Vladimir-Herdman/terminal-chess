#ifndef TERMINALCHESS_CONFIG_CONFIGMAP_G
#define TERMINALCHESS_CONFIG_CONFIGMAP_G

#include <string>
#include <map>

extern const std::map<std::string, void (*)(std::string&)> config_map;

#endif //TERMINALCHESS_CONFIG_CONFIGMAP_G
