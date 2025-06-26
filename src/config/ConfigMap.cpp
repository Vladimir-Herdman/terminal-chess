#include <iostream>
#include <string>
#include "ConfigMap.h"
#include "ConfigData.h"

namespace {
    // Helper functions

    // Convert hex color code to ANSI escape sequence color code
    void hex_color_code(std::string& value) {
        int r = std::stoi(value.substr(0, 2), nullptr, 16);
        int g = std::stoi(value.substr(2, 2), nullptr, 16);
        int b = std::stoi(value.substr(4, 2), nullptr, 16);
        std::cout << "color codes from hex value are r=" << r << ", g=" << g << ", b=" << b << '\n';
    }

    // Convert rgb color code to ANSI escape sequence color code
    void rgb_color_code(std::string& value) {

    }

    // detect color code style and set up to be inserted to config
    void clean_color(std::string& value) {
        if (value.length() < 8) {
            if (value[0] == '#') {value = value.substr(1);}
            hex_color_code(value);
        }
    }

    // Mapped functions

    void board_edge(std::string& value) {std::cout << "Setting board edge color" << std::endl;}

    void b_bg(std::string& value) {clean_color(value); CONFIG.COLORS.b_bg = value;}
    void b_fg(std::string& value) {clean_color(value); CONFIG.COLORS.b_fg = value;}

    void b_pawn(std::string& value) {std::cout << "Test worked" << '\n';}
    void b_rook(std::string& value) {std::cout << "Test worked" << '\n';}
    void b_knight(std::string& value) {std::cout << "Test worked" << '\n';}
    void b_bishop(std::string& value) {std::cout << "Test worked" << '\n';}
    void b_queen(std::string& value) {std::cout << "Test worked" << '\n';}
    void b_king(std::string& value) {std::cout << "Test worked" << '\n';}

    void w_bg(std::string& value) {clean_color(value); CONFIG.COLORS.w_bg = value;}
    void w_fg(std::string& value) {clean_color(value); CONFIG.COLORS.w_fg = value;}

    void w_pawn(std::string& value) {std::cout << "Test worked" << '\n';}
    void w_rook(std::string& value) {std::cout << "Test worked" << '\n';}
    void w_knight(std::string& value) {std::cout << "Test worked" << '\n';}
    void w_bishop(std::string& value) {std::cout << "Test worked" << '\n';}
    void w_queen(std::string& value) {std::cout << "Test worked" << '\n';}
    void w_king(std::string& value) {std::cout << "Test worked" << '\n';}
}

ConfigMap config_map = {
    {"board_edge", board_edge},

    {"blackside_background", b_bg},
    {"blackside_foreground", b_fg},

    {"blackside_pawn", b_pawn},
    {"blackside_rook", b_rook},
    {"blackside_knight", b_knight},
    {"blackside_bishop", b_bishop},
    {"blackside_queen", b_queen},
    {"blackside_king", b_king},

    {"whiteside_background", w_bg},
    {"whiteside_foreground", w_fg},

    {"whiteside_pawn", w_pawn},
    {"whiteside_rook", w_rook},
    {"whiteside_knight", w_knight},
    {"whiteside_bishop", w_bishop},
    {"whiteside_queen", w_queen},
    {"whiteside_king", w_king},
};
