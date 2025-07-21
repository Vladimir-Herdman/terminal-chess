#include <algorithm>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string>

#include "ConfigMap.hpp"
#include "ConfigData.hpp"


namespace {
    // using declarations
    using ptr_configFunction = void (*)(std::string&);
    using ConfigMap = std::map<std::string, ptr_configFunction>;

    // Helper functions
    // Convert hex color code to ANSI escape sequence color code
    void hex_color_code(std::string& value) {
        std::string final = "";
        for (int i = 0; i < 5; i+=2) {
            int color_num = std::stoi(value.substr(i, 2), nullptr, 16);
            final += ";" + std::format("{:03d}", color_num);
        }
        value = final;

        if (CONFIG::OPTIONS.verbose) {
            std::cout << "   Color codes from hex value are r="
                << final.substr(1, 3)
                << ", g=" << final.substr(5, 3)
                << ", b=" << final.substr(9, 3)
                << '\n';
        }
    }

    // Convert rgb color code to ANSI escape sequence color code
    void rgb_color_code(std::string& value) {
        std::string final;
        for (int i = 0; i < 9; i+=3) {
            final += ";" + value.substr(i, 3);
        }
        value = final;

        if (CONFIG::OPTIONS.verbose) {
            std::cout << "   RGB color is " << value << '\n';
        }
    }

    // Determine type of color code, HEX or RGB
    void clean_color(std::string& value) {
        if (value.length() < 8) {
            if (value[0] == '#') {value = value.substr(1);}
            hex_color_code(value);
            return;
        }
        rgb_color_code(value);
    }

    void clean_piece(std::string& value) {
        std::setlocale(LC_ALL, "en_US.UTF-8");
        value.erase(std::remove(value.begin(), value.end(), '\''), value.end());
        value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
        value = " " + value + " ";

        if (CONFIG::OPTIONS.verbose) {
            std::cout << "Piece passed is:" << value << '\n';
        }
    }

    // Mapped functions

    void board_edge(std::string& value) {clean_color(value); CONFIG::COLORS.edge = "\x1B[48;2" + value + 'm';}

    void b_bg(std::string& value) {clean_color(value); CONFIG::COLORS.b_bg = "\x1B[48;2" + value + 'm';}
    void b_fg(std::string& value) {clean_color(value); CONFIG::COLORS.b_fg = "\x1B[38;2" + value + 'm';}

    void b_pawn(std::string& value) {clean_piece(value); CONFIG::PIECES.b_pawn = value;}
    void b_rook(std::string& value) {clean_piece(value); CONFIG::PIECES.b_rook = value;}
    void b_knight(std::string& value) {clean_piece(value); CONFIG::PIECES.b_knight = value;}
    void b_bishop(std::string& value) {clean_piece(value); CONFIG::PIECES.b_bishop = value;}
    void b_queen(std::string& value) {clean_piece(value); CONFIG::PIECES.b_queen = value;}
    void b_king(std::string& value) {clean_piece(value); CONFIG::PIECES.b_king = value;}

    void w_bg(std::string& value) {clean_color(value); CONFIG::COLORS.w_bg = "\x1B[48;2" + value + 'm';}
    void w_fg(std::string& value) {clean_color(value); CONFIG::COLORS.w_fg = "\x1B[38;2" + value + 'm';}

    void w_pawn(std::string& value) {clean_piece(value); CONFIG::PIECES.w_pawn = value;}
    void w_rook(std::string& value) {clean_piece(value); CONFIG::PIECES.w_rook = value;}
    void w_knight(std::string& value) {clean_piece(value); CONFIG::PIECES.w_knight = value;}
    void w_bishop(std::string& value) {clean_piece(value); CONFIG::PIECES.w_bishop = value;}
    void w_queen(std::string& value) {clean_piece(value); CONFIG::PIECES.w_queen = value;}
    void w_king(std::string& value) {clean_piece(value); CONFIG::PIECES.w_king = value;}
}

const ConfigMap config_map = {
    {"board_edge", board_edge},

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
};
