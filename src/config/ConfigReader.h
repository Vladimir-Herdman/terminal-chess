#ifndef TERMINAL_CHESS_CONFIGREADER_H
#define TERMINAL_CHESS_CONFIGREADER_H

#include <string>

class ConfigReader {
public:
    // Will check for config file, read from it, and determine used variables set
    ConfigReader();

private:
    // will clean line, and return where '=' sign at if found
    int cleanLine(std::string& line);
};

#endif //TERMINAL_CHESS_CONFIGREADER_H
