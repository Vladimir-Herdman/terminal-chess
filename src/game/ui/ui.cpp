#include "game/ui/ui.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

#define SCI(arg) static_cast<int>(arg)

namespace {
    using u64 = std::uint64_t;
    using namespace CONFIG;

    constexpr UI::pieces board_default[10][10] = {
        {UI::pieces::EDGE_V, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::B_ROOK, UI::pieces::B_KNIGHT, UI::pieces::B_BISHOP, UI::pieces::B_QUEEN, UI::pieces::B_KING, UI::pieces::B_BISHOP, UI::pieces::B_KNIGHT, UI::pieces::B_ROOK, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::B_PAWN, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::SPACE, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::W_PAWN, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::W_ROOK, UI::pieces::W_KNIGHT, UI::pieces::W_BISHOP, UI::pieces::W_QUEEN, UI::pieces::W_KING, UI::pieces::W_BISHOP, UI::pieces::W_KNIGHT, UI::pieces::W_ROOK, UI::pieces::EDGE_V},
        {UI::pieces::EDGE_V, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_H, UI::pieces::EDGE_V}
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
    const int piece_val = SCI(m_board[r][c]);
    std::string square = m_get_bg_color(r, c) + m_get_fg_color(piece_val);
    square += m_get_piece(piece_val);
    
    // get if needed letters and numbers sections
    if (OPTIONS.board_numbers && c == 0 && (r > 0 && r < 9)) {m_get_square_numbers(square, r);}
    if (OPTIONS.board_letters && r == 9 && (c > 0 && c < 9)) {m_get_square_letters(square, c);}
    return square + COLORS.reset;
}
void UI::print_board() const {
    std::string board_text = "";
    board_text.reserve(5000);
    std::string reset_color = COLORS.reset;

    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            board_text += get_square(r, c);
        }
        board_text += reset_color + '\n';
    }
    std::cout << board_text << std::flush;
}
void UI::refresh_board() const {
    //clear line, up ten, clear, start of line
    std::cout << m_ansi.clear_line() << m_ansi.go_lines_up(11)
              << m_ansi.clear_line() << "\r";
    print_board();
    //clear last input line and go back to beginning
    std::cout << m_ansi.clear_line() << "\r" << std::flush;
}

// Privates
// Fill initial board representation with enum values from default boards
void UI::m_fill_initial_board() {
    std::copy(std::begin(board_default[0]), std::end(board_default[9]), m_board[0]);
}
//TODO: Look into setting up string references to CONFIG so less strings just copy returned
//NOTE: Because of pointers so CONFIG can change at runtime, dereference such variables
std::string UI::m_get_bg_color(const int r, const int c) const {
    const bool is_board_edge = (r==0||r==9||c==0||c==9);
    return *(m_lookup_bg[is_board_edge?0:1][(c+r)%2]); //dirty math here, beware: checkered
}
std::string UI::m_get_fg_color(const int piece_val) const {
    if (piece_val < 6) { //pieces enum values 0-5 are white pieces
        return *(m_lookup_fg[SCI(colors::W_FG)]);
    } else if (piece_val < 12) { //pieces enum values 6-11 are black
        return *(m_lookup_fg[SCI(colors::B_FG)]);
    } else if (piece_val == 12) { //values 13-14 are edge pieces
        return *(m_lookup_fg[SCI(colors::LETTER_FG)]);
    } else if (piece_val == 13) { //values 13-14 are edge pieces
        return *(m_lookup_fg[SCI(colors::NUMBER_FG)]);
    }
    return *(m_lookup_fg[SCI(colors::RESET)]);
}
std::string UI::m_get_piece(const int piece_val) const {
    return *(m_lookup_pieces[piece_val]);
}
void UI::m_get_square_letters(std::string& square, const int r) const {
    //std::cout << "\nValue find("   ") is:" << square.find("   ")+1<<"\n"; //DEBUG
    square[SCI(m_piece_index::EDGE_H)] = m_lookup_letter[r-1]; //square.find("   ")+1
}
void UI::m_get_square_numbers(std::string& square, const int c) const {
    //std::cout << "\nValue find("  ") is:" << square.find("  ")+1<<"\n"; //DEBUG
    square[SCI(m_piece_index::EDGE_V)] = m_lookup_number[8-c]; //square.find("  ")+1
}

// Macros
#undef SCI
