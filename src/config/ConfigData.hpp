#ifndef TERMINALCHESS_CONFIG_CONFIGDATA_H
#define TERMINALCHESS_CONFIG_CONFIGDATA_H

#include <string>

namespace CONFIG {
    struct Color {
        std::string edge;

        std::string b_bg; //black_square_background
        std::string b_fg; //black_pieces_foreground

        std::string w_bg; //white_square_background
        std::string w_fg; //white_pieces_foreground

        const std::string reset;
    };

    struct Pieces {
        std::string b_pawn;
        std::string b_rook;
        std::string b_knight;
        std::string b_bishop;
        std::string b_queen;
        std::string b_king;

        std::string w_pawn;
        std::string w_rook;
        std::string w_knight;
        std::string w_bishop;
        std::string w_queen;
        std::string w_king;

        std::string edge_h;
        std::string edge_v;
        std::string space;
    };

    struct Options {
        bool verbose;
        bool dry_run;
        bool board_letters;
        bool board_numbers;
    };

    extern Color COLORS;
    extern Pieces PIECES;
    extern Options OPTIONS;
};

#endif //TERMINALCHESS_CONFIG_CONFIGDATA_H
