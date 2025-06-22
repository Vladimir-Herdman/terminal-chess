// TODO
    // Configuration file
        // Should be able to change all colors and symbols used throughout
        // Turn on / off a lot of settings, like highlight move, abc123 row/cols
    // Break apart some sections so more readable
    // Seperate code into other header and source files for logic based main.cpp
    // Learn cmake for better make file setup for others and deviceas
    // Command line arguments
        // Start from black side
        // Show both sides at once
#include <stdbool.h>
#include <iostream>
#include <string>

//ESC[38;2;{r};{g};{b}m - \x1B[38;2;255;255;255m(white) - Set foreground color as RGB
//ESC[48;2;{r};{g};{b}m - \1xB[48;2;255;255;255m(white) - Set background color as RGB
//ESC[0m - \x1B[0m - reset all modes styles and colors
//(165, 42, 42

std::string PIECE = "   ";
enum class Pieces { PAWN = 1, KNIGHT, BISHOP, ROOK, QUEEN, KING };

//ANSI rgb colors: b_=background, f_=foreground
// Keep all actual colors same width for later indexing ease
const struct {
    std::string b_black = "\x1B[48;2;000;000;000m";
    std::string b_brown = "\x1B[48;2;082;031;000m";
    std::string b_white = "\x1B[48;2;234;233;230m";

    std::string f_black = "\x1B[38;2;000;000;000m";

    std::string reset = "\x1B[0m";
} COLORS;

void setPiece(enum Pieces piece) {
    switch (piece) {
        case Pieces::PAWN: PIECE = " \u2659 "; break;
        case Pieces::ROOK: PIECE = " \u2656 "; break;
        case Pieces::KNIGHT: PIECE = " \u2658 "; break;
        case Pieces::BISHOP: PIECE = " \u2657 "; break;
        case Pieces::QUEEN: PIECE = " \u2655 "; break;
        case Pieces::KING: PIECE = " \u2654 "; break;
    }
}

void initializeWhiteBoard() {
    int i, j;
    std::string edge_h = COLORS.b_brown + "   " + COLORS.reset;
    std::string edge_v = COLORS.b_brown + "  " + COLORS.reset;
    std::string background;
    std::string foreground = COLORS.f_black;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {

            if (j == 0) { // Edges
                if (i > 0 and i < 9) { edge_v[19] = '9'-i; }
                std::cout << edge_v;
                continue;
            }
            if (j == 9) { edge_v[19] = ' '; std::cout << edge_v; continue; }

            if (i == 0) { edge_h[20] = ' '; std::cout << edge_h; continue; } 
            if (i == 9) {
                if (j > 0 and j < 9) { edge_h[20] = '`'+j; }
                std::cout << edge_h; 
                continue;
            }

            background = ((j+i)%2 == 0) ? COLORS.b_white : COLORS.b_black;

            if (i == 2 || i == 7) {setPiece(Pieces::PAWN);}

            if (i == 1 || i == 8) {
                if (j == 2 || j == 6) {setPiece(Pieces::KNIGHT);}
                else if (j == 1 || j == 7) {setPiece(Pieces::ROOK);}
                else if (j == 3 || j == 5) {setPiece(Pieces::BISHOP);}
                else if (j == 4) {setPiece(Pieces::QUEEN);}
                else {setPiece(Pieces::KING);}
            }

            // piece[0] = map(j);

            //printf("%s%s%s\x1B[0m", background, foreground, PIECE);
            std::cout << background << foreground << PIECE;
            PIECE = "   ";
        }
        std::cout << "\n";
    }
}

int main(int argc, char *argv[]) {
    initializeWhiteBoard();
}
