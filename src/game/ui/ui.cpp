#include "game/ui/ui.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream> //REMOVE
#include <limits>
#include <string>

#define SCI(arg) static_cast<int>(arg)

namespace {
    using u64 = std::uint64_t;

    constexpr UI::pieces board_default[10][10] = {
        {UI::pieces::EDGE_V, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::B_ROOK, UI::pieces::B_KNIGHT, UI::pieces::B_BISHOP, UI::pieces::B_QUEEN, UI::pieces::B_KING, UI::pieces::B_BISHOP, UI::pieces::B_KNIGHT, UI::pieces::B_ROOK, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_LETTER, UI::pieces::W_ROOK, UI::pieces::W_KNIGHT, UI::pieces::W_BISHOP, UI::pieces::W_QUEEN, UI::pieces::W_KING, UI::pieces::W_BISHOP, UI::pieces::W_KNIGHT, UI::pieces::W_ROOK, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_NUM, UI::pieces::EDGE_V}
    };
}

// Publics
// Check options and configure ui based off such configurations
// For example, check for the no letter or numbers option to turn off
// those pieces from board
UI::UI() {
    m_fill_initial_board();
}
std::string UI::get_square(const int r, const int c) const {
    const int piece_val = SCI(board[r][c]);
    std::string square = m_get_bg_color(r, c) + m_get_fg_color(piece_val);
    square += m_get_piece(piece_val);
    
    // get if needed letters and numbers sections
    if (c == 0 && (r > 0 && r < 9)) {m_get_square_numbers(square, r);}
    if (r == 9 && (c > 0 && c < 9)) {m_get_square_letters(square, c);}
    return square;
}

// Privates
// Fill initial board representation with enum values from default boards
void UI::m_fill_initial_board() {
    std::copy(std::begin(board_default[0]), std::end(board_default[9]), board[0]);
}
//TODO: Look into setting up string references to CONFIG so less strings just copy returned
std::string UI::m_get_bg_color(const int r, const int c) const {
    return bg_lookup[(r==0||r==9)?0:1][(c+r)%2]; //dirty math here, beware: checkered
}
std::string UI::m_get_fg_color(const int piece_val) const {
    if (piece_val < 6) { //pieces enum values 0-5 are white pieces
        return fg_lookup[SCI(colors::W_FG)];
    } else if (piece_val < 12) { //pieces enum values 6-11 are black
        return fg_lookup[SCI(colors::B_FG)];
    }
    return fg_lookup[SCI(colors::RESET)];
}
std::string UI::m_get_piece(const int piece_val) const {
    return pieces_lookup[piece_val];
}
void UI::m_get_square_letters(std::string& square, const int r) const {
    square[square.find("   ")+1] = m_letter_lookup[r-1];
}
void UI::m_get_square_numbers(std::string& square, const int c) const {
    square[square.find("  ")+1] = m_number_lookup[8-c];
}

// Macros
#undef SCI
