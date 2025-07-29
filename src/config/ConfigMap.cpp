#include "ConfigMap.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ConfigData.hpp"

namespace {
    // using declarations
    using ptr_configFunction = void (*)(std::string&);
    using ConfigMap = std::map<std::string, ptr_configFunction>;

    constexpr auto& COLORS = CONFIG::COLORS;
    constexpr auto& PIECES = CONFIG::PIECES;
    constexpr auto& OPTIONS = CONFIG::OPTIONS;

    // Helper functions
    void to_hex_color_code(std::string& value) { //ANSI
        std::string final = "";
        for (int i = 0; i < 5; i+=2) {
            const int color_num = std::stoi(value.substr(i, 2), nullptr, 16);
            final += ";" + std::format("{:03d}", color_num);
        }
        value = final;

        if (OPTIONS.verbose) {
            std::cout << "   Color codes from hex value are r="
                << final.substr(1, 3)
                << ", g=" << final.substr(5, 3)
                << ", b=" << final.substr(9, 3)
                << '\n';
        }
    }
    void to_rgb_color_code(std::string& value) { //ANSI
        std::string final;
        for (int i = 0; i < 9; i+=3) {
            final += ";" + value.substr(i, 3);
        }
        value = final;

        if (OPTIONS.verbose) {
            std::cout << "   RGB color is " << value << '\n';
        }
    }

    // Determine type of color code, HEX or RGB
    void clean_color(std::string& value) {
        if (value.length() < 8) {
            if (value[0] == '#') {value = value.substr(1);}
            to_hex_color_code(value);
            return;
        }
        to_rgb_color_code(value);
    }
    void clean_piece(std::string& value) {
        std::setlocale(LC_ALL, "en_US.UTF-8");
        value.erase(std::remove(value.begin(), value.end(), '\''), value.end());
        value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
        value = " " + value + " ";

        if (OPTIONS.verbose) {
            std::cout << "Piece passed is:" << value << '\n';
        }
    }

    // Determine if true or false bool option
    void clean_bool(std::string& value) {
        if (value.length() > 5) {throw std::runtime_error("Value:("+value+") must be true or false");}
        for (char& ch : value) {ch = std::tolower(ch);}
        if (value != "true" && value != "false") {
            throw std::runtime_error("Value:("+value+") must be true or false");
        }
    }

    // Inline organizers for map below - improper option should crash/error
    inline void color_bg(std::string& target, std::string& value) {
        clean_color(value);
        target = "\x1B[48;2" + value + 'm';
    }
    inline void color_fg(std::string& target, std::string& value) {
        clean_color(value);
        target = "\x1B[38;2" + value + 'm';
    }
    inline void piece(std::string& target, std::string& value) {
        clean_piece(value);
        target = value;
    }
    inline void boolean(bool& target, std::string& value) {
        clean_bool(value);
        target = (value[0] == 't' ? true : false);
    }
    inline void integer(int& target, std::string& value, const int minimum) {
        const int initial_value = std::stoi(value);
        if (initial_value < minimum) { throw std::runtime_error("Value:("+value+") must be 50+");}
        target = initial_value;
    }

    // Mapped functions
    // TODO: Confirm putting in no value doesn't break it

    //void board_edge(std::string& value) {clean_color(value); COLORS.edge = "\x1B[48;2" + value + 'm';}
    void board_edge(std::string& value) {color_bg(COLORS.edge, value );}
    void board_letter_foreground(std::string& value) {color_fg(COLORS.letter, value);}
    void board_number_foreground(std::string& value) {color_fg(COLORS.number, value);}

    void b_bg(std::string& value) {color_bg(COLORS.b_bg, value);}
    void b_fg(std::string& value) {color_fg(COLORS.b_fg, value);}

    void b_pawn(std::string& value) {piece(PIECES.b_pawn, value);}
    void b_rook(std::string& value) {piece(PIECES.b_rook, value);}
    void b_knight(std::string& value) {piece(PIECES.b_knight, value);}
    void b_bishop(std::string& value) {piece(PIECES.b_bishop, value);}
    void b_queen(std::string& value) {piece(PIECES.b_queen, value);}
    void b_king(std::string& value) {piece(PIECES.b_king, value);}

    void w_bg(std::string& value) {color_bg(COLORS.w_bg, value);}
    void w_fg(std::string& value) {color_fg(COLORS.w_fg, value);}

    void w_pawn(std::string& value) {piece(PIECES.w_pawn, value);}
    void w_rook(std::string& value) {piece(PIECES.w_rook, value);}
    void w_knight(std::string& value) {piece(PIECES.w_knight, value);}
    void w_bishop(std::string& value) {piece(PIECES.w_bishop, value);}
    void w_queen(std::string& value) {piece(PIECES.w_queen, value);}
    void w_king(std::string& value) {piece(PIECES.w_king, value);}

    void options_board_letters(std::string& value) {boolean(OPTIONS.board_letters, value);}
    void options_board_numbers(std::string& value) {boolean(OPTIONS.board_numbers, value);}

    void daemon_config_run(std::string& value) {boolean(CONFIG::run_daemon, value);}
    void daemon_config_speed(std::string& value) {integer(CONFIG::daemon_sleep_milliseconds, value, 50);} //Stay above 50
}

const ConfigMap config_map = {
    //Board coloring and piece text representation
    {"board_edge", board_edge},
    {"board_letter_foreground", board_letter_foreground},
    {"board_number_foreground", board_number_foreground},

    {"black_square_background", b_bg},
    {"black_pieces_foreground", b_fg},

    {"blackside_pawn", b_pawn},
    {"blackside_rook", b_rook},
    {"blackside_knight", b_knight},
    {"blackside_bishop", b_bishop},
    {"blackside_queen", b_queen},
    {"blackside_king", b_king},

    {"white_square_background", w_bg},
    {"white_pieces_foreground", w_fg},

    {"whiteside_pawn", w_pawn},
    {"whiteside_rook", w_rook},
    {"whiteside_knight", w_knight},
    {"whiteside_bishop", w_bishop},
    {"whiteside_queen", w_queen},
    {"whiteside_king", w_king},

    //Options
    {"options_board_letters", options_board_letters},
    {"options_board_numbers", options_board_numbers},

    //Daemons
    {"daemon_config_run", daemon_config_run},
    {"daemon_config_speed", daemon_config_speed},
};
