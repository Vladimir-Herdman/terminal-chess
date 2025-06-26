#ifndef TERMINALCHESS_CONFIGDATA_H
#define TERMINALCHESS_CONFIGDATA_H

#include <string>

namespace {
    struct Color {
        std::string edge;

        std::string b_bg;
        std::string b_fg;

        std::string w_bg;
        std::string w_fg;

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

#endif //TERMINALCHESS_CONFIGDATA_H
