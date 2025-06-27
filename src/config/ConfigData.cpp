#include "ConfigData.h"

// Define defaults here in case of no config file
ConfigData CONFIG = {
    .COLORS {
        .edge = "\x1B[48;2;082;031;000m",

        .b_bg = "\x1B[48;2;000;000;000m",
        .b_fg = "\x1B[38;2;000;000;000m",

        .w_bg = "\x1B[48;2;234;233;230m",
        .w_fg = "\x1B[48;2;234;233;230m", // TODO: Figure out white foregrouds

        .reset = "\x1B[0m",
    },
    .PIECES { //TODO: add white or black unicode pieces
        .b_pawn = " \u2659 ",
        .b_rook = " \u2656 ",
        .b_knight = " \u2658 ",
        .b_bishop = " \u2657 ",
        .b_queen = " \u2655 ",
        .b_king = " \u2654 ",

        .w_pawn = " \u2659 ",
        .w_rook = " \u2656 ",
        .w_knight = " \u2658 ",
        .w_bishop = " \u2657 ",
        .w_queen = " \u2655 ",
        .w_king = " \u2654 ",
    },
    .OPTIONS {
        .verbose = false,
        .dry_run = false,
    },
};
