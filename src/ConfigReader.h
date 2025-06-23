#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>

class ConfigReader {
public:
    // Will check for config file, read from it, and determine used variables set
    ConfigReader();

private:
    void cleanLine(std::string& line);
};

#endif //CONFIGREADER_H
