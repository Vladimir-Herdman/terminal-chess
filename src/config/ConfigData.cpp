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
    .PIECES {
        .pawn = " \u2659 ",
        .rook = " \u2656 ",
        .knight = " \u2658 ",
        .bishop = " \u2657 ",
        .queen = " \u2655 ",
        .king = " \u2654 ",
    },
    .OPTIONS {
        .verbose = false,
    },
};
