#include "game/ui/ui.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>

#define SCI(arg) static_cast<int>(arg)

namespace {
    using u64 = std::uint64_t;
    using namespace CONFIG;

    constexpr UI::Pieces board_default[10][10] = {
        {UI::Pieces::EDGE_V, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::B_ROOK, UI::Pieces::B_KNIGHT, UI::Pieces::B_BISHOP, UI::Pieces::B_QUEEN, UI::Pieces::B_KING, UI::Pieces::B_BISHOP, UI::Pieces::B_KNIGHT, UI::Pieces::B_ROOK, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::B_PAWN, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::SPACE, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::W_PAWN, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::W_ROOK, UI::Pieces::W_KNIGHT, UI::Pieces::W_BISHOP, UI::Pieces::W_QUEEN, UI::Pieces::W_KING, UI::Pieces::W_BISHOP, UI::Pieces::W_KNIGHT, UI::Pieces::W_ROOK, UI::Pieces::EDGE_V},
        {UI::Pieces::EDGE_V, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_H, UI::Pieces::EDGE_V}
    };
}

// Publics
// Check options and configure ui based off such configurations
// For example, check for the no letter or numbers option to turn off
// those pieces from board
UI::UI() {
    m_fillInitialBoard();
}
std::string UI::getSquare(const int r, const int c) const {
    const int piece_val = SCI(m_board[r][c]);
    std::string square = m_getBgColor(r, c) + m_getFgColor(piece_val);
    square += m_getPiece(piece_val);
    
    // get if needed letters and numbers sections
    if (m_options.numbers && c == 0 && (r > 0 && r < 9)) {m_getSquareNumbers(square, r);}
    if (m_options.letters && r == 9 && (c > 0 && c < 9)) {m_getSquareLetters(square, c);}
    return square + m_colors_fg.reset;
}
void UI::printBoard() const {
    std::string board_text = "";
    board_text.reserve(5000);
    std::string reset_color = m_colors_fg.reset;

    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            board_text += getSquare(r, c);
        }
        board_text += reset_color + '\n';
    }
    std::cout << board_text << std::flush;
}
int UI::highlight(const std::string input) const {
    int r, c;
    const char ch_one = input[1];
    const char ch_two = input[2];

    // r and c are 1 indexed so it matches with our printed grid
    if (std::isalpha(ch_one)){
        r = SCI(ch_two)-48;
        c = SCI(ch_one)-96;
    } else {
        r = SCI(ch_one)-48;
        c = SCI(ch_two)-96;
    }

    // guard against something like 'hcw' where out of board bounds
    if (c > 8 || c < 1 || r > 8 || r < 1) {
        inputImproper();
        return SCI(InputStatus::DONT_REFRESH);
    }

    m_highlight(r, c);
    return SCI(InputStatus::DONT_REFRESH);
}
void UI::inputImproper() const {std::cout << m_ansi.goLinesUp(1) << m_ansi.clearLine() << "\r";}
void UI::refreshBoard() const {
    //clear line, up ten, clear, start of line
    std::cout << m_ansi.goLinesUp(11);
    printBoard();
    std::cout << m_ansi.clearLine() << "\r" << std::flush;
}

// Privates
// Fill initial board representation with enum values from default boards
void UI::m_fillInitialBoard() {
    std::copy(std::begin(board_default[0]), std::end(board_default[9]), m_board[0]);
}
void UI::m_highlight(const int r, const int c) const {
    const int piece_val = SCI(m_board[r][c]);
    const std::string highlight = m_colors_bg.highlight_main + m_getFgColor(piece_val) + m_getPiece(piece_val);

    //TODO: make it so if piece there, highlight possible moves/takes available
    std::cout << m_ansi.goLinesUp(r+2) << m_ansi.goToColumn(c*3) //in square
              << highlight << m_colors_fg.reset;
    if (! m_options.interactive) { //go back to input
        std::cout << m_ansi.goLinesDown(r+1) << m_ansi.clearLine() //highlight
                  << "\r" << std::flush;
    }
}
//TODO: Look into setting up string references to CONFIG so less strings just copy returned
//NOTE: Because of pointers so CONFIG can change at runtime, dereference such variables
std::string UI::m_getBgColor(const int r, const int c) const {
    const bool is_board_edge = (r==0||r==9||c==0||c==9);
    return *(m_lookup_bg[is_board_edge?0:1][(c+r)%2]); //dirty math here, beware: checkered
}
std::string UI::m_getFgColor(const int piece_val) const {
    if (piece_val < 6) { //pieces enum values 0-5 are white pieces
        return m_colors_fg.white;
    } else if (piece_val < 12) { //pieces enum values 6-11 are black
        return m_colors_fg.black;
    } else if (piece_val == 12) { //values 13-14 are edge pieces
        return m_colors_fg.letter;
    } else if (piece_val == 13) { //values 13-14 are edge pieces
        return m_colors_fg.number;
    }
    return "";
}
std::string UI::m_getPiece(const int piece_val) const {
    return *(m_lookup_pieces[piece_val]);
}
void UI::m_getSquareLetters(std::string& square, const int r) const {
    //std::cout << "\nValue find("   ") is:" << square.find("   ")+1<<"\n"; //DEBUG
    square[SCI(m_PieceIndex::EDGE_H)] = m_lookup_letter[r-1]; //square.find("   ")+1
}
void UI::m_getSquareNumbers(std::string& square, const int c) const {
    //std::cout << "\nValue find("  ") is:" << square.find("  ")+1<<"\n"; //DEBUG
    square[SCI(m_PieceIndex::EDGE_V)] = m_lookup_number[8-c]; //square.find("  ")+1
}

// Macros
#undef SCI
