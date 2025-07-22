#ifndef TERMINALCHESS_GAME_UI_H
#define TERMINALCHESS_GAME_UI_H

#include <string>

#include "config/ConfigData.hpp"

void initializeWhiteBoard();

class UI {
public:
    UI();
    const std::string pieces_lookup[17] = {
        CONFIG::PIECES.w_pawn, CONFIG::PIECES.w_knight,
        CONFIG::PIECES.w_bishop, CONFIG::PIECES.w_rook,
        CONFIG::PIECES.w_queen, CONFIG::PIECES.w_king,
        CONFIG::PIECES.b_pawn, CONFIG::PIECES.b_knight,
        CONFIG::PIECES.b_bishop, CONFIG::PIECES.b_rook,
        CONFIG::PIECES.b_queen, CONFIG::PIECES.b_king,
        CONFIG::COLORS.edge+"   ", CONFIG::COLORS.edge+"  ", //horiz_edge, vert_edge
        CONFIG::COLORS.edge+"  ", CONFIG::COLORS.edge+"  ", //letter_edge, number_edge
        "   " //space
    };
    enum class pieces {
        W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
        B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
        EDGE_HORIZONTAL, EDGE_VERTICAL, EDGE_LETTER, EDGE_NUMBER,
        SPACE
    };
    pieces board[10][10];

private:
    void m_assign_board();

    friend class Game;
};

#endif //TERMINALCHESS_GAME_UI_H
