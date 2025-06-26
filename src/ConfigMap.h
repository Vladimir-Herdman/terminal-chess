#ifndef TERMINALCHESS_CONFIGMAP_G
#define TERMINALCHESS_CONFIGMAP_G

#include <string>
#include <map>

using ptr_configFunction = void (*)(std::string&);
using ConfigMap = std::map<std::string, ptr_configFunction>;
extern ConfigMap config_map;

#endif //TERMINALCHESS_CONFIGMAP_G
