#include "ConfigData.hpp"

#include <ctime>

// Define defaults here in case of no config file
// NOTE: Treat this as the main place to always get such data, use pointers so
    // if it changes it is reflected throughout
namespace CONFIG {
    Color COLORS {
        .edge = "\x1B[48;2;048;046;043m",
        .letter = "\x1B[38;2;255;255;255m",
        .number = "\x1B[38;2;255;255;255m",

        .b_bg = "\x1B[48;2;119;149;086m",
        .b_fg = "\x1B[38;2;000;000;000m",

        .w_bg = "\x1B[48;2;235;236;208m",
        .w_fg = "\x1B[38;2;100;100;100m", // TODO: Figure out white foregrouds

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

        .edge_h = "   ",
        .edge_v = "  ",
        .space = "   ",
    };

    //TODO: add the options to the config file as changeable
    Options OPTIONS {
        .verbose = false,
        .dry_run = false,

        .board_letters = true,
        .board_numbers = true,

        .input_interactive = false,
    };

    Daemon DAEMON {
        .last_write = std::chrono::file_clock::now(),
        .config_path = "",
        //TODO: update_daemon settings
        .run_daemon = false,
        .daemon_sleep_milliseconds = 500,
    };
};
