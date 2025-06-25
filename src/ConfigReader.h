#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include <map>

struct ConfigData {
    struct {
        std::string b_black;
        std::string b_brown;
        std::string b_white;

        std::string f_black;

        std::string reset;
    } COLORS;
    struct {
        std::string pawn;
        std::string rook;
        std::string knight;
        std::string bishop;
        std::string queen;
        std::string king;
    } PIECES;
};
extern ConfigData CONFIG;

class ConfigReader {
public:
    // Will check for config file, read from it, and determine used variables set
    ConfigReader();

private:
    // will clean line, and return where '=' sign at if found
    int cleanLine(std::string& line);
};

#endif //CONFIGREADER_H
