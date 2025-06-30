#include "ConfigData.h"

// Define defaults here in case of no config file
namespace CONFIG {
    Color COLORS {
        .edge = "\x1B[48;2;048;046;043m",

        .b_bg = "\x1B[48;2;119;149;086m",
        .b_fg = "\x1B[38;2;000;000;000m",

        .w_bg = "\x1B[48;2;235;236;208m",
        .w_fg = "\x1B[48;2;234;233;230m", // TODO: Figure out white foregrouds

        .reset = "\x1B[0m",
    };

    Pieces PIECES { //TODO: add white or black unicode pieces
        .b_pawn = " p ",
        .b_rook = " R ",
        .b_knight = " N ",
        .b_bishop = " B ",
        .b_queen = " Q ",
        .b_king = " K ",

        .w_pawn = " p ",
        .w_rook = " R ",
        .w_knight = " N ",
        .w_bishop = " B ",
        .w_queen = " Q ",
        .w_king = " K ",
    };

    Options OPTIONS {
        .verbose = false,
        .dry_run = false,
    };
};
