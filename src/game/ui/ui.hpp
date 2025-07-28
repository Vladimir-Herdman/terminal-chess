#ifndef TERMINALCHESS_GAME_UI_H
#define TERMINALCHESS_GAME_UI_H

#include <string>

#include "config/ConfigData.hpp"

void initializeWhiteBoard();

class UI {
public:
    UI();
    std::string get_square(const int r, const int c) const;
    void print_board() const;

    enum class pieces {
        W_PAWN = 0, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
        B_PAWN = 6, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
        EDGE_H = 12, EDGE_V, SPACE,
    };
    enum class colors : int {
        W_BG = 1, W_FG,
        B_BG = 0, B_FG,
        LETTER_FG = 3, NUMBER_FG,
        RESET = 2,
    };

private:
    void m_fill_initial_board();
    std::string m_get_bg_color(const int r, const int c) const;
    std::string m_get_fg_color(const int piece_val) const;
    std::string m_get_piece(const int piece_val) const;
    void m_get_square_letters(std::string& square, const int r) const;
    void m_get_square_numbers(std::string& square, const int c) const;

    //TODO: Use references and pointers for config options to later allow deamon
        //thread to update based on config file change live-time
    const std::string* m_pieces_lookup[15] = {
        &CONFIG::PIECES.w_pawn, &CONFIG::PIECES.w_knight,
        &CONFIG::PIECES.w_bishop, &CONFIG::PIECES.w_rook,
        &CONFIG::PIECES.w_queen, &CONFIG::PIECES.w_king,
        &CONFIG::PIECES.b_pawn, &CONFIG::PIECES.b_knight,
        &CONFIG::PIECES.b_bishop, &CONFIG::PIECES.b_rook,
        &CONFIG::PIECES.b_queen, &CONFIG::PIECES.b_king,
        &CONFIG::PIECES.edge_h, &CONFIG::PIECES.edge_v,
        &CONFIG::PIECES.space
    };
    //TODO: Should I be naming with lookup first to better organize lookup tables?
    const std::string* m_bg_lookup[2][2] = {
        {&CONFIG::COLORS.edge, &CONFIG::COLORS.edge},
        {&CONFIG::COLORS.b_bg, &CONFIG::COLORS.w_bg},
    };
    const std::string* m_fg_lookup[5] = {
        &CONFIG::COLORS.reset, &CONFIG::COLORS.b_fg, &CONFIG::COLORS.w_fg,
        &CONFIG::COLORS.letter, &CONFIG::COLORS.number,
    };
    const char m_letter_lookup[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    const char m_number_lookup[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

    const struct {
        bool letters = CONFIG::OPTIONS.board_letters;
        bool numbers = CONFIG::OPTIONS.board_numbers;
    } m_options;

    enum class m_piece_index {
        EDGE_H = 39, EDGE_V = 39
    };

    pieces m_board[10][10];

    friend class Game;
};

#endif //TERMINALCHESS_GAME_UI_H
