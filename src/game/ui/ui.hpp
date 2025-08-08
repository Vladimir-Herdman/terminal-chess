#ifndef TERMINALCHESS_GAME_UI_H
#define TERMINALCHESS_GAME_UI_H

#include <iostream> //REMOVE
#include <string>
#include <map>

#include "config/ConfigData.hpp"
#include "types/classes.hpp"

void initializeWhiteBoard();

class UI {
public:
    UI();
    std::string getSquare(const int r, const int c) const;
    void printBoard() const;
    int highlight(const std::string input) const;
    void inputImproper() const;
    void refreshBoard() const;

    enum class Pieces {
        W_PAWN = 0, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
        B_PAWN = 6, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
        EDGE_H = 12, EDGE_V, SPACE,
    };
    enum class InputStatus {
        DONT_REFRESH = 0, FULL_REFRESH ,
        END_GAME,
        IMPROPER_INPUT,
    };

private:
    friend class Game;

    void m_fillInitialBoard();
    void m_highlight(const int r, const int c) const;
    std::string m_getBgColor(const int r, const int c) const;
    std::string m_getFgColor(const int piece_val) const;
    std::string m_getPiece(const int piece_val) const;
    void m_getSquareLetters(std::string& square, const int r) const;
    void m_getSquareNumbers(std::string& square, const int c) const;

    //TODO: Use references and pointers for config options to later allow deamon
        //thread to update based on config file change live-time
    const std::string* m_lookup_pieces[15] = {
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
    //REMOVE BEGIN
    void m_test1() {std::cout << "test1 func called";}
    void m_test2() {std::cout << "test2 func called";}
    void m_test3() {std::cout << "test3 func called";}
    void m_test4() {std::cout << "test4 func called";}
    //REMOVE END
    //NOTE: Need this-> when calling as needs object
    //(this->*m_lookup_movement_functions[0])();
    const VoidMemberFuncPtr<UI> m_lookup_movement_functions[4] = {
        &UI::m_test1, &UI::m_test2, &UI::m_test3, &UI::m_test4
    };
    const std::string* m_lookup_bg[2][2] = {
        {&CONFIG::COLORS.edge, &CONFIG::COLORS.edge},
        {&CONFIG::COLORS.b_bg, &CONFIG::COLORS.w_bg},
    };
    const struct {
        const std::string& reset = CONFIG::COLORS.reset;
        const std::string& black = CONFIG::COLORS.b_fg;
        const std::string& white = CONFIG::COLORS.w_fg;
        const std::string& letter = CONFIG::COLORS.letter;
        const std::string& number = CONFIG::COLORS.number;
    } m_colors_fg;
    const struct {
        const std::string& highlight_main = CONFIG::COLORS.highlight_main;
        const std::string& highlight_around = CONFIG::COLORS.highlight_around;
    } m_colors_bg;
    const char m_lookup_letter[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    const char m_lookup_number[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

    struct {
        inline std::string clearLine() const {return "\x1B[2K";};
        inline std::string goLinesUp(const int lines) const {return "\033["+std::to_string(lines)+"A";};
        inline std::string goLinesDown(const int lines) const {return "\033["+std::to_string(lines)+"B";};
        inline std::string goToColumn(const int c) const {return "\033["+std::to_string(c)+"G";};
    } m_ansi;
    const struct {
        const bool& letters = CONFIG::OPTIONS.board_letters;
        const bool& numbers = CONFIG::OPTIONS.board_numbers;
        const bool& interactive = CONFIG::OPTIONS.input_interactive;
    } m_options;
    const struct {
        int row = 0;
        int column = 0;
    } m_cursor;

    enum class m_PieceIndex {
        EDGE_H = 39, EDGE_V = 39
    };

    Pieces m_board[10][10];
};

#endif //TERMINALCHESS_GAME_UI_H
