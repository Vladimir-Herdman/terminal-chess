#ifndef TERMINAL_CONFIG_CHESS_CONFIGREADER_H
#define TERMINAL_CONFIG_CHESS_CONFIGREADER_H

#include <string>

class ConfigReader {
public:
    // Will check for config file, read from it, and determine used variables set
    ConfigReader();

private:
    // will clean line, and return where '=' sign at if found
    int m_cleanLine(std::string& line);
    std::string m_getConfigFile();
};

#endif //TERMINAL_CONFIG_CHESS_CONFIGREADER_H
