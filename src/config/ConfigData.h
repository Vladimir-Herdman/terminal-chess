#ifndef TERMINALCHESS_CONFIGDATA_H
#define TERMINALCHESS_CONFIGDATA_H

#include <string>

namespace {
    struct Color {
        std::string edge;

        std::string b_bg; //black_background
        std::string b_fg;

        std::string w_bg; //white_background
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

    struct Options {
        bool verbose;
    };
};

struct ConfigData {
    Color COLORS;
    Pieces PIECES;
    Options OPTIONS;
};
extern ConfigData CONFIG;

#endif //TERMINALCHESS_CONFIGDATA_H
