#include "game/ui/ui.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>

#define SCI(arg) static_cast<int>(arg)
#define UI_IS(arg) static_cast<int>(UI::InputStatus::arg)
#define UI_N(arg) static_cast<int>(UI::Notation::arg)

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
    const char ch_one = std::tolower(input[1]);
    const char ch_two = std::tolower(input[2]);

    // r and c are 1 indexed so it matches with our printed grid
    if (std::isalpha(ch_one)){
        r = ch_two-m_normalize.number;
        c = ch_one-m_normalize.lowercase_letter;
    } else {
        r = ch_one-m_normalize.number;
        c = ch_two-m_normalize.lowercase_letter;
    }

    // guard against something like 'hcw' where out of board bounds
    if (m_outOfBoardBounds(r, c)) {
        inputImproper();
        return UI_IS(DONT_REFRESH);
    }

    m_highlight(r, c);
    return UI_IS(DONT_REFRESH);
}
void UI::inputImproper() const {
    std::cout << "Improper move/input";
    std::cout << m_ansi.goLinesUp(1) << m_ansi.clearLine() << "\r";
}
void UI::refreshBoard() const {
    std::cout << m_ansi.goLinesUp(11) << "\r";
    printBoard();
    std::cout << m_ansi.goLinesDown(1) << m_ansi.clearLine() << m_ansi.goLinesUp(1)
              << m_ansi.clearLine() << "\r" << std::flush;
}
int UI::makeChessNotationMove(const std::string input, const int input_length) {
    switch (input_length) {
        case 2: {
            return m_pawnMove(input);
        }
        case 3: {//other piece move
            return m_pieceMove(input);
        }
        case 4: {//taking a piece
            if (input[1] != 'x') { //moving piece where same piece type can move to square, same case 5 but moving
                return m_pieceMoveSpecial(input);
            }
            return m_takePiece(input);
        }
        case 5: {//same piece type can take same, like Nfxe3 with a N also on g4, or N1xe3 if N on same rank on file 3
            return m_takePieceSpecial(input);
        }
    }
    return UI_IS(IMPROPER_INPUT);
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
    if (!m_options.interactive) { //go back to input
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
inline bool UI::m_outOfBoardBounds(const int r, const int c) const {return (c > 8 || c < 1 || r > 8 || r < 1);}

int UI::m_pawnMove(const std::string move) {
    const int c = move[0]-m_normalize.lowercase_letter+1;
    const int r = m_normalize.index[move[1]-m_normalize.number];
    if (m_outOfBoardBounds(r, c)) {std::cout<<"r:"<<r<<",c:"<<c;exit(0);return UI_IS(IMPROPER_INPUT);};

    const MoveResult move_res = m_white.makePawnMove(r-1, c-1);
    if(move_res.legal) { //r,c is to_go cord, move_res.{r,c} is piece to move
        m_board[r][c] = m_board[move_res.r][move_res.c];
        m_board[move_res.r][move_res.c] = Pieces::SPACE;
        //TODO: potentially partial refresh for changed vector for more efficiency, instead of just reprinting whole board each time?
        return UI_IS(FULL_REFRESH);
    }
    //TODO: implement blackside pawnmove. keep a bool of current turn player to decide here
    return UI_IS(IMPROPER_INPUT);
}
int UI::m_pieceMove(const std::string move) {
    //TODO: function - moving a piece on the board
    return UI_IS(IMPROPER_INPUT);
}
int UI::m_pieceMoveSpecial(const std::string move) {
    //TODO: function - moving a piece on the board, where a same type could also move
        // notation like Nd(file)e3 if different file's same attack, or R1(rank)a4 if on same file
    return UI_IS(IMPROPER_INPUT);
}
int UI::m_takePiece(const std::string move) {
    const int piece = move[0]-m_normalize.uppercase_letter;
    if (piece >= 31) { //pawn attack, becuase this means lowercase passed in with x so 'exd4' and ansi lowercase larger than ansi upper

    }
    //subtractions taken from ANSI value table characters to normalize and reduce table size
    const int r = m_normalize.index[move[3]-m_normalize.number];
    const int c = move[2]-m_normalize.lowercase_letter+1;
    if (!m_validAttackNotation(piece, c, r)) {return UI_IS(IMPROPER_INPUT);}

    const MoveResult move_res = m_white.makePieceAttack(piece, r-1, c-1);
    //TODO: return based on move result and move pieces
    if (move_res.legal) {
        std::cout<<"move returned - r:"<<(move_res.legal?"NO ENEMY TO TAKE":"LEGAL");exit(0);
        return UI_IS(FULL_REFRESH);
    }
    return UI_IS(IMPROPER_INPUT);
}
int UI::m_takePieceSpecial(const std::string move) {
    //TODO: function - taking piece but the special notation for when same piece type could also take it
        // notation like Nd(file)xe3 if different file's same attack, or R1(rank)xa4 if on same file
    return UI_IS(IMPROPER_INPUT);
}
bool UI::m_validAttackNotation(const int piece, const int file_to, const int rank_to) {
    if (piece < 0 || piece > 17 || m_notation.lookup_piece[piece] == UI_N(INVALID)) {return false;}
    if (file_to < 1 || file_to > 8) {return false;}
    if (rank_to < 1 || rank_to > 8) {return false;}
    return true;
}

// Macros
#undef SCI
#undef UI_IS
#undef UI_N
