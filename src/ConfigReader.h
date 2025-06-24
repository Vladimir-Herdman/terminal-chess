#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>

// Define defaults here in case of no config file
struct CONFIGS {
    struct {
        std::string b_black = "\x1B[48;2;000;000;000m";
        std::string b_brown = "\x1B[48;2;082;031;000m";
        std::string b_white = "\x1B[48;2;234;233;230m";

        std::string f_black = "\x1B[38;2;000;000;000m";

        std::string reset = "\x1B[0m";
    } COLORS;
    struct {
        std::string pawn = " \u2659 ";
        std::string rook = " \u2656 ";
        std::string knight = " \u2658 ";
        std::string bishop = " \u2657 ";
        std::string queen = " \u2655 ";
        std::string king = " \u2654 ";
    } PIECES;
};

class ConfigReader {
public:
    // Will check for config file, read from it, and determine used variables set
    ConfigReader(CONFIGS& CONFIGS);

private:
    // will clean line, and return where '=' sign at if found
    int cleanLine(std::string& line);
};

#endif //CONFIGREADER_H
