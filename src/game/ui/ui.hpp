#ifndef TERMINALCHESS_GAME_UI_H
#define TERMINALCHESS_GAME_UI_H

#include <string>

#include "config/ConfigData.hpp"

void initializeWhiteBoard();

class UI {
public:
    UI();
    std::string get_square(const int r, const int c) const;
    const std::string pieces_lookup[17] = {
        CONFIG::PIECES.w_pawn, CONFIG::PIECES.w_knight,
        CONFIG::PIECES.w_bishop, CONFIG::PIECES.w_rook,
        CONFIG::PIECES.w_queen, CONFIG::PIECES.w_king,
        CONFIG::PIECES.b_pawn, CONFIG::PIECES.b_knight,
        CONFIG::PIECES.b_bishop, CONFIG::PIECES.b_rook,
        CONFIG::PIECES.b_queen, CONFIG::PIECES.b_king,
        CONFIG::COLORS.edge+"   ", CONFIG::COLORS.edge+"  ", //edge_h, edge_v
        CONFIG::COLORS.edge+"  ", CONFIG::COLORS.edge+"   ", //edge_letter, edge_num
        "   " //space
    };
    //TODO: Should I be naming with lookup first to better organize lookup tables?
    const std::string bg_lookup[2][2] = {
        {CONFIG::COLORS.edge, CONFIG::COLORS.edge},
        {CONFIG::COLORS.b_bg, CONFIG::COLORS.w_bg},
    };
    const std::string fg_lookup[3] = {
        CONFIG::COLORS.reset, CONFIG::COLORS.b_fg, CONFIG::COLORS.w_fg
    };
    enum class pieces {
        W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
        B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
        EDGE_H, EDGE_V, EDGE_LETTER, EDGE_NUM,
        SPACE
    };
    enum class colors : int {
        W_BG = 1, W_FG,
        B_BG = 0, B_FG,
        RESET
    };
    pieces board[10][10];

private:
    void m_fill_initial_board();
    std::string m_get_bg_color(const int r, const int c) const;
    std::string m_get_fg_color(const int piece_val) const;
    std::string m_get_piece(const int piece_val) const;

    friend class Game;
};

#endif //TERMINALCHESS_GAME_UI_H
