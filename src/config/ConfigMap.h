#ifndef TERMINALCHESS_CONFIGMAP_G
#define TERMINALCHESS_CONFIGMAP_G

#include <string>
#include <map>

extern std::map<std::string, void (*)(std::string&)> config_map;

#endif //TERMINALCHESS_CONFIGMAP_G
