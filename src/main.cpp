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

#include "config/ConfigReader.h"
#include "config/ConfigData.h"

//ESC[38;2;{r};{g};{b}m - \x1B[38;2;255;255;255m(white) - Set foreground color as RGB
//ESC[48;2;{r};{g};{b}m - \1xB[48;2;255;255;255m(white) - Set background color as RGB
//ESC[0m - \x1B[0m - reset all modes styles and colors
//(165, 42, 42

auto& PIECES = CONFIG.PIECES;
auto& COLORS = CONFIG.COLORS;
auto& OPTIONS = CONFIG.OPTIONS;

std::string PIECE = "   ";
enum class enum_pieces { PAWN = 1, KNIGHT, BISHOP, ROOK, QUEEN, KING };

//ANSI rgb colors: b_=background, f_=foreground
// Keep all actual colors same width for later indexing ease

void setPiece(enum enum_pieces piece) {
    switch (piece) {
        case enum_pieces::PAWN: PIECE = PIECES.pawn; break;
        case enum_pieces::ROOK: PIECE = PIECES.rook; break;
        case enum_pieces::KNIGHT: PIECE = PIECES.knight; break;
        case enum_pieces::BISHOP: PIECE = PIECES.bishop; break;
        case enum_pieces::QUEEN: PIECE = PIECES.queen; break;
        case enum_pieces::KING: PIECE = PIECES.king; break;
    }
}

void initializeWhiteBoard() {
    int i, j;
    std::string edge_h = COLORS.edge + "   " + COLORS.reset;
    std::string edge_v = COLORS.edge + "  " + COLORS.reset;
    std::string background;
    std::string foreground = COLORS.b_fg;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {

            if (j == 0) { // Edges
                if (i > 0 && i < 9) { edge_v[19] = '9'-i; }
                std::cout << edge_v;
                continue;
            }
            if (j == 9) { edge_v[19] = ' '; std::cout << edge_v; continue; }

            if (i == 0) { edge_h[20] = ' '; std::cout << edge_h; continue; } 
            if (i == 9) {
                if (j > 0 && j < 9) { edge_h[20] = '`'+j; }
                std::cout << edge_h; 
                continue;
            }

            background = ((j+i)%2 == 0) ? COLORS.w_bg : COLORS.b_bg;

            if (i == 2 || i == 7) {setPiece(enum_pieces::PAWN);}

            if (i == 1 || i == 8) {
                if (j == 2 || j == 6) {setPiece(enum_pieces::KNIGHT);}
                else if (j == 1 || j == 7) {setPiece(enum_pieces::ROOK);}
                else if (j == 3 || j == 5) {setPiece(enum_pieces::BISHOP);}
                else if (j == 4) {setPiece(enum_pieces::QUEEN);}
                else {setPiece(enum_pieces::KING);}
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
    // Read in config file
    ConfigReader();

    // Show boards
    initializeWhiteBoard();
}
