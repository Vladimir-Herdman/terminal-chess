#ifndef CONFIGMAP_H
#define CONFIGMAP_H

#include <string>
#include <map>

using ptr_configFunction = void (*)(std::string&);
using ConfigMap = std::map<std::string, ptr_configFunction>;
extern ConfigMap config_map;

#endif //CONFIGMAP_H
