#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <string>

namespace {
    struct Color {
        std::string b_black;
        std::string b_brown;
        std::string b_white;

        std::string f_black;

        std::string reset;
    };

    struct Pieces {
        std::string pawn;
        std::string rook;
        std::string knight;
        std::string bishop;
        std::string queen;
        std::string king;
    };
};

struct ConfigData {
    Color COLORS;
    Pieces PIECES;
};
extern ConfigData CONFIG;

#endif //CONFIGDATA_H
